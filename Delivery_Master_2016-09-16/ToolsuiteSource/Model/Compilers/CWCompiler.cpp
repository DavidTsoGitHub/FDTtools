#include "CWCompiler.h"
#include "Logger.h"
#include "BSPSpecification.h"

#include <QProcess>
#include <QDir>
#include <QDirIterator>
#include <QThread>
#include <QObjectList>

//Attributes defines
#define MAKEEXEKEY              "MakeExeKey"
#define COMPILERPATHKEY         "CompilerRootPath"
#define COMPILEREXEKEY          "CompilerExeKey"
#define ASSEMBLEREXEKEY         "AssemblerExeKey"
#define LINKEREXEKEY            "LinkerExeKey"
#define LINKERFILEKEY           "LinkerFileKey"
#define GENERATES19KEY          "GenerateS19Key"
#define EXTERNALMEMORYADDRESS   "ExternalMemoryAddress"

//Filename defines
#define OBJ_ARGS_FILE   "MPC5606S.obj.args"
#define ASM_ARGS_FILE   "MPC5606S.asm.args"
#define ELF_ARGS_FILE   "MPC5606S.elf.args"
#define ELF_FILE        "FreescaleMPC5606S.elf"

#define EXTERNAL_FLASH_ADDRESS     0x80000000u

static QStringList m_paddedS19File;

CWCompiler::CWCompiler() : CompilerBase("CodeWarrior_MPC5606S")
{
    addCustomAttributeDefinition(MAKEEXEKEY, "Make executable path", CompilerBase::File);
    addCustomAttributeDefinition(COMPILERPATHKEY, "CodeWarrior root path", CompilerBase::Directory);
    addCustomAttributeDefinition(COMPILEREXEKEY, "Compiler executable filename", CompilerBase::Text);
    addCustomAttributeDefinition(ASSEMBLEREXEKEY, "Assembler executable filename", CompilerBase::Text);
    addCustomAttributeDefinition(LINKEREXEKEY, "Linker executable filename", CompilerBase::Text);
    addCustomAttributeDefinition(LINKERFILEKEY, "Linker file", CompilerBase::File);
    addCustomAttributeDefinition(GENERATES19KEY, "Generate Padded S19 File", CompilerBase::Checkbox);
    addCustomAttributeDefinition(EXTERNALMEMORYADDRESS, "", CompilerBase::BSPPropertyExternalMemoryAddress);
}

CWCompiler::~CWCompiler()
{

}

bool CWCompiler::GenerateBuildEnvironment(QString outputRootPath)
{
    LOG_INFO("[CWCompiler] Generating build environment... ");
    bool predicate = true;

    m_buildPath = QString(outputRootPath).append("/Build");
    m_sourcePath = QString(outputRootPath).append("/Source");

    m_tree = parseSourceTree(m_sourcePath);

    predicate &= generateMPC5606SMakeFiles();
    predicate &= generateMPC2606SMakeFileLocal();
    predicate &= generateSourceArgsFile();
    predicate &= generateAsmArgsFile();
    predicate &= generateBinaryArgsFile();

    QFile prefixFile(m_buildPath + "/MPC5606S.prefix");
    if(quickBuild && prefixFile.exists())
    {
        ; /* do nothing */
    }
    else
    {
        predicate &= generatePrefixFile();
    }

    predicate &= copyLinkerFile();

    LOG_INFO("Generating subdir.mk in: ");
    foreach (QString subDir, m_tree.SourceDirectories)
    {
        predicate &= generateSubDirMakefile(subDir);
    }
    
    foreach (QString subDir, m_tree.LibraryDirectories)
    {
        predicate &= generateSubDirMakefile(subDir);
    }

    return predicate;
}

bool CWCompiler::Build(QString projectOutputPath)
{
    m_buildPath = QString(projectOutputPath).append("/Build");
    QString buildCommand = GetCustomAttribute(MAKEEXEKEY);

    QProcess buildPlatformProcess;
    buildPlatformProcess.setProgram(buildCommand);
    buildPlatformProcess.setWorkingDirectory(m_buildPath);
    buildPlatformProcess.setProcessChannelMode(QProcess::ForwardedChannels);
    buildPlatformProcess.start();
    if(!buildPlatformProcess.waitForFinished(-1) || buildPlatformProcess.exitCode() != 0)
    {
        // If error occured during execution
        LOG_ERROR("Error while building: " + buildPlatformProcess.errorString());
        return false;
    }

    if (GetCustomAttribute(GENERATES19KEY) == "1")
    {
        GeneratePaddedS19File();
    }

    return true;
}

void CWCompiler::GeneratePaddedS19File()
{
    LOG_INFO("[CWCompiler] Generating Padded S19 file... ");

    QStringList filter("*.mot");
    QDir directory(m_buildPath);
    QStringList motFiles = directory.entryList(filter);

    if (motFiles.count() != 1)
    {
        LOG_ERROR("[CWCompiler] The number of found mot files was " + QString::number(motFiles.count()));
    }

    QString motFileName = m_buildPath + "/" + motFiles.first();
    QFile motFile(motFileName);

    if (motFile.open(QIODevice::ReadOnly))
    {
        quint32 lastAddress = 0;
        bool status;
        quint32 externalMemoryAddress = GetCustomAttribute(EXTERNALMEMORYADDRESS).toULong();

        QTextStream source(&motFile);
        QString line;

        line = source.readLine();
        while (line != NULL)
        {

            if (line.mid(0,2) != "S3")
            {
                // The first and last row doesn't start with S3 and is not in the same format as the data part of the file
                m_paddedS19File.append(line);
                line = source.readLine();
            }
            else
            {
                quint8 length = line.mid(2, 2).toUInt(&status, 16);
                quint32 address = line.mid(4, 8).toUInt(&status, 16);

                if (length == 0x19)
                {
                    if (lastAddress == 0x00 || (address == (lastAddress + 0x14)))
                    {
                        // This part is run for the lines from the source file that doesn't need to be updated in anyway
                        m_paddedS19File.append(line);
                        lastAddress = address;
                    }
                    else
                    {
                        if (address != externalMemoryAddress)
                        {
                            // Write lines containing 0xFF until we get to the next line with data in the source file
                            AddEmptyLines(lastAddress + 0x14, address);
                        }
                        m_paddedS19File.append(line);
                    }
                    line = source.readLine();
                }
                else
                {
                    // If the line isn't full size the data has to be padded with 0xFF
                    QString currentLine = line;
                    line = source.readLine();
                    quint32 nextAddress = line.mid(4, 8).toUInt(&status, 16);

                    if (nextAddress != externalMemoryAddress)
                    {
                        PadAndAppendLine(currentLine, nextAddress - address);
                    }
                    else
                    {
                        m_paddedS19File.append(currentLine);
                    }
                }
                lastAddress = address;
            }
        }

        QString s19FileName = m_buildPath + "/" + motFiles.first();
        s19FileName.remove(s19FileName.length() - 3, 3);
        s19FileName = s19FileName + "s19";

        writeToFile(s19FileName, m_paddedS19File.join('\n'));
    }
    else
    {
        LOG_ERROR("[CWCompiler] ERROR: Couldn't open file " + motFiles.first());
    }

    LOG_INFO("[CWCompiler] Generating Padded S19 file...Done!");
}

void CWCompiler::PadAndAppendLine(QString line, quint32 rowLength)
{
    quint8 length = (rowLength < 20 ? rowLength : 20);

    line.remove(0, 4);
    line = "S3" + QString("%1").arg(length + 5, 2, 16, QChar('0')).toUpper() + line;
    line.remove(line.length() - 2, 2);

    while (line.length() < ((2 * length) + 4 + 8))
    {
        line.append('F');
    }

    quint8 checksum = GetChecksum(line);
    line = line + QString("%1").arg(checksum, 2, 16, QChar('0')).toUpper();

    m_paddedS19File.append(line);

}

void CWCompiler::AddEmptyLines(quint32 startAddress, quint32 endAddress)
{
    quint32 currentAddress = startAddress;
    QString emptyData = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";

    while (currentAddress < (endAddress - 0x14))
    {
        quint32 checksum = 0x19 + ((0xFF000000 & currentAddress) >> 24) + ((0x00FF0000 & currentAddress) >> 16) + ((0x0000FF00 & currentAddress) >> 8) + (0x000000FF & currentAddress) + 0x13EC;
        checksum = (~checksum) & 0x00FF;
        QString line = "S319" + QString("%1").arg(currentAddress, 8, 16, QChar('0')).toUpper() + emptyData + QString("%1").arg(checksum, 2, 16, QChar('0')).toUpper();

        m_paddedS19File.append(line);
        currentAddress += 0x14;
    }

    if ( currentAddress < endAddress)
    {
        quint8 dataLength = endAddress - currentAddress;
        emptyData = "";
        quint32 sumOfData = 0;

        for (quint8 i = 0; i < dataLength; ++i)
        {
            emptyData.append("FF");
            sumOfData += 0xFF;
        }

        quint8 length = dataLength + 5;
        quint32 checksum = length + ((0xFF000000 & currentAddress) >> 24) + ((0x00FF0000 & currentAddress) >> 16) + ((0x0000FF00 & currentAddress) >> 8) + (0x000000FF & currentAddress) + sumOfData;
        checksum = (~checksum) & 0x00FF;
        QString line = "S3" + QString("%1").arg(length, 2, 16, QChar('0')).toUpper() + QString("%1").arg(currentAddress, 8, 16, QChar('0')).toUpper() + emptyData + QString("%1").arg(checksum, 2, 16, QChar('0')).toUpper();

        m_paddedS19File.append(line);
    }

}

quint8 CWCompiler::GetChecksum(QString s19Line)
{
    quint8 retVal = 0;
    bool convertionOK;

    QString checksumData = s19Line.mid(2, s19Line.length() - 2);

    quint32 sum = 0;

    for (quint8 i = 0; i < checksumData.length(); i = i + 2)
    {
        quint8 val = checksumData.mid(i, 2).toUInt(&convertionOK, 16);
        if (convertionOK)
        {
            sum += val;
        }
        else
        {
            LOG_ERROR("[CWCompiler] The source for calculating checksum is in an incorrect format!");
            retVal = 0;
            break;
        }
    }

    if (convertionOK)
    {
        retVal = (~sum) & 0x000000FF;
    }

    return retVal;
}

bool CWCompiler::HasMandatoryCompilerSpecificData()
{
    bool hasMandatoryData = true;

    hasMandatoryData &= CompilerBase::CompilerAttributeExist(MAKEEXEKEY, "Make exe path empty or file missing");
    hasMandatoryData &= CompilerBase::CompilerAttributeExist(COMPILERPATHKEY, "Compiler path empty or missing");
    hasMandatoryData &= CompilerBase::CompilerAttributeExist(COMPILEREXEKEY, "Compiler exe path empty or file missing");
    hasMandatoryData &= CompilerBase::CompilerAttributeExist(ASSEMBLEREXEKEY, "Assembler exe path empty or file missing");
    hasMandatoryData &= CompilerBase::CompilerAttributeExist(LINKEREXEKEY, "Linker exe path empty or file missing");
    hasMandatoryData &= CompilerBase::CompilerAttributeExist(LINKERFILEKEY, "Linker file path empty or file missing");

    return hasMandatoryData;
}

bool CWCompiler::generateMPC5606SMakeFiles()
{
    LOG_INFO("Generating MPC5606S makefiles...");
    QStringList fileLines;

    fileLines.append("################################################################################");
    fileLines.append("# Automatically-generated file. Do not edit! Unless you really want to.");
    fileLines.append("################################################################################");

    fileLines.append("#Progress monitor hint: 76");
    fileLines.append("first : all");
    fileLines.append("-include ../makefile.init");

    fileLines.append("# This file contains definitions of environment variables used in the makefiles and .args files if exist.");
    fileLines.append("-include makefile.local");

    fileLines.append("RM := \"$(GNU_Make_Install_DirEnv)/rm\" -f");

    fileLines.append("# All of the sources participating in the build are defined here");
    fileLines.append("SUBDIRS := \\");

    foreach (QString path, m_tree.SourceDirectories)
    {
        fileLines.append("../Source/" + path + " \\");
    }

    fileLines.append("");

    foreach (QString path, m_tree.SourceDirectories)
    {
        fileLines.append("-include " + path + "/subdir.mk");
    }

    foreach (QString path, m_tree.LibraryDirectories)
    {
        fileLines.append("-include " + path + "/subdir.mk");
    }

    fileLines.append("");
    fileLines.append("ifneq ($(MAKECMDGOALS),clean)");
    fileLines.append("ifneq ($(strip $(C++_DEPS)),)");
    fileLines.append("-include $(C++_DEPS)");
    fileLines.append("endif");
    fileLines.append("ifneq ($(strip $(ASM_DEPS)),)");
    fileLines.append("-include $(ASM_DEPS)");
    fileLines.append("endif");
    fileLines.append("ifneq ($(strip $(C_DEPS)),)");
    fileLines.append("-include $(C_DEPS)");
    fileLines.append("endif");
    fileLines.append("ifneq ($(strip $(CC_DEPS)),)");
    fileLines.append("-include $(CC_DEPS)");
    fileLines.append("endif");
    fileLines.append("ifneq ($(strip $(ASM_UPPER_DEPS)),)");
    fileLines.append("-include $(ASM_UPPER_DEPS)");
    fileLines.append("endif");
    fileLines.append("ifneq ($(strip $(CPP_DEPS)),)");
    fileLines.append("-include $(CPP_DEPS)");
    fileLines.append("endif");
    fileLines.append("ifneq ($(strip $(S_DEPS)),)");
    fileLines.append("-include $(S_DEPS)");
    fileLines.append("endif");
    fileLines.append("ifneq ($(strip $(CXX_DEPS)),)");
    fileLines.append("-include $(CXX_DEPS)");
    fileLines.append("endif");
    fileLines.append("ifneq ($(strip $(C_UPPER_DEPS)),)");
    fileLines.append("-include $(C_UPPER_DEPS)");
    fileLines.append("endif");
    fileLines.append("ifneq ($(strip $(S_UPPER_DEPS)),)");
    fileLines.append("-include $(S_UPPER_DEPS)");
    fileLines.append("endif");
    fileLines.append("endif");
    fileLines.append("");
    fileLines.append("# All Target");
    fileLines.append("all: warning-messages " ELF_FILE);
    fileLines.append("");
    fileLines.append("# Tool invocations");
    fileLines.append(ELF_FILE ": $(OBJS)");
    fileLines.append("\t@echo 'Building target: $@'");
    fileLines.append("\t@echo 'Invoking: PowerPC Linker'");
    fileLines.append("\t\"$(PAToolsDirEnv)/" + GetCustomAttribute(LINKEREXEKEY) + "\" -o \""  ELF_FILE  "\" @@\"" + ELF_ARGS_FILE + "\" $(LIBS_QUOTED)");
    fileLines.append("\t@echo 'Finished building target: $@'");
    fileLines.append("\t@echo ' '");
    fileLines.append("");
    fileLines.append("# Other Targets");
    fileLines.append("clean:");
    // TODO: Doesn't remove properly, fix the clean routine, you mean machine!
    fileLines.append("\t-$(RM) \"./*/*/*.d\"  $(STATICLIB_QUOTED) \"./*/*/*.obj\" \"./*/*/*/*/*/*/*.obj\" \"./*/*/*/*/*/*.obj\" \"./*/*/*/*/*.obj\" \"./*/*.obj\"  $(CPP_DEPS_QUOTED) $(CC_DEPS_QUOTED) $(S_UPPER_DEPS_QUOTED) $(CXX_DEPS_QUOTED) $(ASM_DEPS_QUOTED) $(ASM_UPPER_DEPS_QUOTED) $(C_UPPER_DEPS_QUOTED) $(C++_DEPS_QUOTED) $(EXECUTABLES_QUOTED) \"./*/*/*.d\" \"./*/*/*/*/*/*/*.d\" \"./*/*/*/*/*/*.d\" \"./*/*/*/*/*.d\" \"./*/*.d\"  Build/MPC5606S/bin/FreescaleMPC5606S.elf");
    fileLines.append("\t-@echo ' '");
    fileLines.append("");
    fileLines.append(".PHONY: all clean dependents warning-messages");
    fileLines.append(".SECONDARY:");
    fileLines.append("");
    fileLines.append("-include ../makefile.targets");
    fileLines.append("");
    //fileLines.append("explicit-dependencies: ../Source/TargetBSP/Linker_Files/default560S.lcf");
    //fileLines.append("");
    fileLines.append("warning-messages:");

    QString fileContent = fileLines.join("\n");

    return writeToFile(m_buildPath + "/Makefile", fileContent);
}

bool CWCompiler::generateMPC2606SMakeFileLocal()
{
    LOG_INFO("Generating Makefile.Local...");
    QStringList fileLines;

    fileLines.append("################################################################################");
    fileLines.append("# These macros are used by makefiles and argument files.");
    fileLines.append("# When a space is included in the path and quotes (\") cannot be used,");
    fileLines.append("# a _ESCAPE version of the macro is added that escapes the space with backslash (\\).");
    fileLines.append("################################################################################");
    fileLines.append("");
    fileLines.append("export GNU_Make_Install_DirEnv=$(CWInstallLocationEnv)/gnu/bin");
    fileLines.append("export CWInstallLocationEnv=" + GetCustomAttribute(COMPILERPATHKEY));
    fileLines.append("export MCUToolsBaseDirEnv=$(CWInstallLocationEnv)/MCU");
    fileLines.append("export PAToolsDirEnv=$(CWInstallLocationEnv)/MCU/PA_Tools/Command_Line_Tools");

    QString fileContent = fileLines.join("\n");

    return writeToFile(m_buildPath + "/Makefile.local", fileContent);
}

bool CWCompiler::generateSourceArgsFile()
{
    LOG_INFO("Generating argument file for compiler...");

    QStringList fileLines;

    fileLines.append("-sym dwarf-2");
    fileLines.append("-sym full");
    fileLines.append("-msgstyle parseable");
    fileLines.append("-proc zen");
    fileLines.append("-big");
    fileLines.append("-fp soft");
    fileLines.append("-model absolute");
    fileLines.append("-abi eabi");
    fileLines.append("-sdata 0");
    fileLines.append("-sdata2 0");
    fileLines.append("-vle");
    fileLines.append("-enc ascii");
    fileLines.append("-prefix");
    fileLines.append("\"$(MCUToolsBaseDirEnv)/PA_Support/ewl/EWL_C/include/lib_ewl.prefix\"");
    fileLines.append("-prefix");
    fileLines.append("\"MPC5606S.prefix\"");
    fileLines.append("-c");
    fileLines.append("-nostdinc");
    fileLines.append("-nosyspath");
    fileLines.append("");

    foreach(QString path, m_tree.IncludeDirectories)
    {

        fileLines.append("-i ../Source/" + path);
    }

    fileLines.append("-i ./");
    fileLines.append("");
    fileLines.append("-I\"$(MCUToolsBaseDirEnv)/PA_Support/ewl/EWL_C/include\"");
    fileLines.append("-I\"$(MCUToolsBaseDirEnv)/PA_Support/ewl/EWL_Runtime/Runtime_PA/Include\"");
    fileLines.append("-I\"$(MCUToolsBaseDirEnv)/PA_Support/Initialization_Files\"");
    fileLines.append("");
    fileLines.append("-disable_extensions off");
    fileLines.append("-warn illpragmas");
    fileLines.append("-warn unwanted");
    fileLines.append("-warn hidevirtual");
    fileLines.append("-warn implicitconv");
    fileLines.append("-warn unusedarg");
    fileLines.append("-warn unusedvar");
    fileLines.append("-warn unusedexpr");
    fileLines.append("-warn extracomma");
    fileLines.append("-warn structclass");
    fileLines.append("-opt level=0");
    fileLines.append("-opt speed");
    fileLines.append("-inline off");
    fileLines.append("-inline nobottomup");
    fileLines.append("-align powerpc");
    fileLines.append("-func_align 4");
    fileLines.append("-pooldata off");
    fileLines.append("-common off");
    fileLines.append("-use_lmw_stmw on");
    fileLines.append("-novolatileasm");
    fileLines.append("-schedule off");
    fileLines.append("-opt nopeephole");
    fileLines.append("-use_isel on");
    fileLines.append("-ppc_asm_to_vle");
    fileLines.append("-Cpp_Exceptions off");
    fileLines.append("-bool off");
    fileLines.append("-requireprotos");
    fileLines.append("-lang c99");
    fileLines.append("-char unsigned");
    fileLines.append("-str nopool");
    fileLines.append("-str reuse");
    fileLines.append("-ipa off");

    return writeToFile(m_buildPath + "/" + OBJ_ARGS_FILE, fileLines.join("\n"));
}

bool CWCompiler::generateAsmArgsFile()
{
    LOG_INFO("Generating argument file for assembler...");

    QStringList fileLines;
    fileLines.append("-sym dwarf-2");
    fileLines.append("-sym full");
    fileLines.append("-msgstyle parseable");
    fileLines.append("-proc zen");
    fileLines.append("-big");
    fileLines.append("-vle");
    fileLines.append("-nosyspath");

    foreach (QString dir, m_tree.AsmDirectories)
    {
        fileLines.append("-i \"../Source/" + dir + "\"");
    }

    return writeToFile(m_buildPath + "/" + ASM_ARGS_FILE, fileLines.join("\n"));
}

bool CWCompiler::generateBinaryArgsFile()
{
    LOG_INFO("Generating argument file for linker...");

    QDir basePath(m_sourcePath);
    QStringList fileLines;
    foreach(QString path, m_tree.SourceFiles)
    {
        QString relFile = basePath.relativeFilePath(path);
        fileLines.append("\"" + relFile.remove(relFile.count() - 2, 2) + ".obj\"");
    }

    foreach(QString path, m_tree.AsmFiles)
    {
        QString relFile = basePath.relativeFilePath(path);
        fileLines.append("\"" + relFile.remove(relFile.count() - 2, 2) + ".obj\"");
    }

    QFileInfo* linkerFile = new QFileInfo(GetCustomAttribute(LINKERFILEKEY));

    fileLines.append("-lcf \"" + linkerFile->fileName() + "\"");
    fileLines.append("-sym dwarf-2");
    fileLines.append("-sym full");
    fileLines.append("-msgstyle parseable");
    fileLines.append("-proc zen");
    fileLines.append("-big");
    fileLines.append("-fp soft");
    fileLines.append("-model absolute");
    fileLines.append("-abi eabi");
    fileLines.append("-sdata 0");
    fileLines.append("-sdata2 0");
    fileLines.append("-nostdlib");

    fileLines.append("-m __start");
    fileLines.append("-L\"$(MCUToolsBaseDirEnv)/PA_Support/ewl/lib\"");
    fileLines.append("-char unsigned");
    fileLines.append("-application");

    //fileLines.append("-heapaddr 0x4000A000");
    //fileLines.append("-stackaddr 0x4000C000");
    fileLines.append("-heapsize 32");
    fileLines.append("-stacksize 32");
    fileLines.append("-map");
    fileLines.append("-listclosure");
    fileLines.append("-mapunused");
    fileLines.append("-genbinary none");
    fileLines.append("-srec");
    fileLines.append("-sreclength 26");
    fileLines.append("-sreceol dos");
    fileLines.append("-romaddr 0x00012000");
    fileLines.append("-rambuffer 0x00012000");

    return writeToFile(m_buildPath + "/" + ELF_ARGS_FILE, fileLines.join("\n"));
}

bool CWCompiler::generatePrefixFile()
{
    LOG_INFO("Generating prefix file...");

    QStringList fileLines;
    fileLines.append("#define Z0H_CORE 1");
    fileLines.append("");
    fileLines.append("#define VLE_IS_ON 1");
    fileLines.append("#define ROM_VERSION 1");

    return writeToFile(m_buildPath + "/MPC5606S.prefix", fileLines.join("\n"));
}

bool CWCompiler::generateSubDirMakefile(QString subDir)
{
    LOG_INFO("    " + subDir);

    QStringList fileLines;
    QStringList sourceFiles;
    QStringList asmFiles;
    //static quint32 fileCount = 0;
    QDir basePath(m_sourcePath);

    fileLines.append("################################################################################");
    fileLines.append("# Automatically-generated file. Do not edit!");
    fileLines.append("################################################################################");
    fileLines.append("");
    fileLines.append("# Add inputs and outputs from these tool invocations to the build variables");

    fileLines.append("S_SRCS_QUOTED += \\");

    foreach (QString file, m_tree.AsmFiles)
    {
        QString relFile = basePath.relativeFilePath(file);
        QFileInfo qf(relFile);

        QString rel = basePath.relativeFilePath((subDir));

        if (0 == rel.compare(qf.path()))
        {
            fileLines.append("\"../Source/" + relFile + "\" \\");
            asmFiles.append(relFile.remove(relFile.count() - 2, 2));
        }
    }

    fileLines.append("");
    fileLines.append("C_SRCS_QUOTED += \\");

    foreach(QString file, m_tree.SourceFiles)
    {
        QString relFile = basePath.relativeFilePath(file);
        QFileInfo qf(relFile);

        QString rel = basePath.relativeFilePath(subDir);

        if (0 == rel.compare(qf.path()))
        {
            fileLines.append("\"../Source/" + relFile + "\" \\");
            sourceFiles.append(relFile.remove(relFile.count() -2, 2));
        }
    }

    fileLines.append("");
    fileLines.append("C_SRCS += \\");

    foreach(QString file, sourceFiles)
    {
        fileLines.append("../Source/" +file + ".c \\");
    }

    fileLines.append("");
    fileLines.append("S_SRCS += \\");

    foreach(QString file, asmFiles)
    {
        fileLines.append("../Source/" +file + ".s \\");
    }

    fileLines.append("");
    fileLines.append("OBJS += \\");

    foreach(QString file, sourceFiles)
    {
        fileLines.append(file + ".obj \\");
    }

    foreach(QString file, asmFiles)
    {
        fileLines.append(file + ".obj \\");
    }

    fileLines.append("");
    fileLines.append("OBJS_QUOTED += \\");

    foreach(QString file, sourceFiles)
    {
        fileLines.append("\"" +  file + ".obj\" \\");
    }

    foreach(QString file, asmFiles)
    {
        fileLines.append("\"" +  file + ".obj\" \\");
    }

    fileLines.append("");
    fileLines.append("C_DEPS += \\");

    foreach(QString file, sourceFiles)
    {
        fileLines.append(file + ".d \\");
    }

    fileLines.append("");
    fileLines.append("S_DEPS += \\");

    foreach(QString file, asmFiles)
    {
        fileLines.append(file + ".d \\");
    }

    fileLines.append("");
    fileLines.append("C_DEPS_QUOTED += \\");

    foreach(QString file, sourceFiles)
    {
        fileLines.append("\"" + file + ".d\" \\");
    }

    fileLines.append("");
    fileLines.append("S_DEPS_QUOTED += \\");

    foreach(QString file, asmFiles)
    {
        fileLines.append("\"" + file + ".d\" \\");
    }

    fileLines.append("");

    quint16 libFileCounter = 0;
    foreach (QString file, m_tree.LibraryFiles)
    {
        QString relFile = basePath.relativeFilePath(file);
        QFileInfo qf(relFile);

        QString rel = basePath.relativeFilePath(subDir);

        if (0 == rel.compare(qf.path()))
        {
            if ( libFileCounter == 0)
            {
                fileLines.append("LIBS_QUOTED += \\");
            }
            fileLines.append("\"../Source/" + file + "\" \\");
            libFileCounter++;
        }
    }


    fileLines.append("");
    fileLines.append("");
    fileLines.append("# Each subdirectory must supply rules for building sources it contributes");

    foreach(QString file, sourceFiles)
    {
        fileLines.append(file + ".obj: ../Source/" + file + ".c");
        fileLines.append("\t@echo 'Building file: $<'");
        fileLines.append("\t@echo 'Invoking: PowerPC Compiler'");
        //fileLines.append("\t\"$(PAToolsDirEnv)/mwcceppc\" @@\"MPC5606S.obj.args\" -o \"" + file + ".obj\" \"$<\" -MD -gccdep");
        fileLines.append("\t\"$(PAToolsDirEnv)/" + GetCustomAttribute(COMPILEREXEKEY) + "\" @@\"" + OBJ_ARGS_FILE + "\" -o \"" + file + ".obj\" \"$<\" -MD -gccdep");
        fileLines.append("\t@echo 'Finished building: $<'");
        fileLines.append("\t@echo ' '");
        fileLines.append("");
    }

    foreach(QString file, asmFiles)
    {
        fileLines.append(file + ".obj: ../Source/" + file + ".s");
        fileLines.append("\t@echo 'Building file: $<'");
        fileLines.append("\t@echo 'Invoking: PowerPC Compiler'");
        //fileLines.append("\t\"$(PAToolsDirEnv)/mwasmeppc\" @@\"MPC5606S.asm.args\" -o \"" + file + ".obj\" \"$<\" -MD -gccdep");
        fileLines.append("\t\"$(PAToolsDirEnv)/" + GetCustomAttribute(ASSEMBLEREXEKEY) + "\" @@\"" + ASM_ARGS_FILE + "\" -o \"" + file + ".obj\" \"$<\" -MD -gccdep");
        fileLines.append("\t@echo 'Finished building: $<'");
        fileLines.append("\t@echo ' '");
        fileLines.append("");
    }

    if (sourceFiles.count() > 0)
    {
        QString relFile = basePath.relativeFilePath(subDir);
        fileLines.append(relFile + "/%.d: ../Source/" + relFile + "/%.c"); //Sources/Swedspot_Implementation/Application/FunctionBlocks/HighBeamTellTaleNoInd/src/%.d: ../Sources/Swedspot_Implementation/Application/FunctionBlocks/HighBeamTellTaleNoInd/src/%.c
        fileLines.append("\t@echo 'Regenerating dependency file: $@'");
        fileLines.append("");
        fileLines.append("\t@echo ' '");
        fileLines.append("");
    }

    if (asmFiles.count() > 0)
    {
        QString relFile = basePath.relativeFilePath(subDir);
        fileLines.append(relFile + "/%.d: ../Source/" + relFile + "/%.s"); //Sources/Swedspot_Implementation/Application/FunctionBlocks/HighBeamTellTaleNoInd/src/%.d: ../Sources/Swedspot_Implementation/Application/FunctionBlocks/HighBeamTellTaleNoInd/src/%.c
        fileLines.append("\t@echo 'Regenerating dependency file: $@'");
        fileLines.append("");
        fileLines.append("\t@echo ' '");
        fileLines.append("");
    }

    QString fileContent = fileLines.join("\n");

    return writeToFile(m_buildPath + "/" + subDir + "/" + "subdir.mk", fileContent);
}

bool CWCompiler::copyLinkerFile()
{
    //Copy linkerfile to target folder
    QFileInfo src(GetCustomAttribute(LINKERFILEKEY));
    QFileInfo dst(m_buildPath + "/" + src.fileName());

    QFile srcFile(src.absoluteFilePath());
    QFile dstFile(dst.absoluteFilePath());

    if (!srcFile.exists())
    {
        LOG_ERROR("[CWCompiler] ERROR: Couldn't open specified linker file! " + src.absoluteFilePath());
        return false;
    }

    if(dst.exists())
    {
        if(!dstFile.remove())
        {
            LOG_ERROR("[CWCompiler] ERROR: Couldn't copy linker file, there is already a file in the destination path that cannot be removed! " + dst.absoluteFilePath());
            return false;
        }
    }

    bool result = srcFile.copy(dst.absoluteFilePath());

    if (!result)
    {
        LOG_ERROR("[CWCompiler] ERROR: Couldn't copy linker file! " + srcFile.errorString());
    }

    return result;
}

bool CWCompiler::ValidateBuildOutput(QString buildOutput, QStringList& errors)
{
    if (buildOutput.contains("Linker|Warning"))
    {
        errors.append("[CWCompiler] Link failed/completed with warning(s), see \"Platform Build Output\" tab for more information!");
        return false;
    }
    else
    {
        return true;
    }
}
