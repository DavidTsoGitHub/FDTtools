#include <QFileInfo>
#include "MakefileCreator.h"
#include "Logger.h"

    QStringList m_fileLines;

QString MakefileCreator::GenerateMakefile(const QList<GeneratedFile>& generatedFiles, const QList<QString> staticFiles)
{
    LOG_DEBUG("Entering");
    ParseGeneratedFiles(generatedFiles);

    if (0) //target == PC)
    {
        GeneratePCMakeFile();
    }
    else //(target == MPC5606S)
    {
        ParseStaticFiles(staticFiles);
        //GenerateMPC5606SMakeFiles();
    }
    return m_fileLines.join("\n");
}

void MakefileCreator::GeneratePCMakeFile()
{
    m_fileLines.append("# This is an automatically created Makefile for PC.");
    m_fileLines.append("# Any changes will be overwritten upon subsequent runs of the code generator tool");
    m_fileLines.append("");
    m_fileLines.append("# Environment setup");
    m_fileLines.append("ifeq ($(MSYSTEM), MINGW32)");
    m_fileLines.append("\tMD:= mkdir -p");
    m_fileLines.append("\tRM:= rm -rf");
    m_fileLines.append("\tPATHSEP2:=/");
    m_fileLines.append("else");
    m_fileLines.append("\tMD:= mkdir");
    m_fileLines.append("\tRM:= del /s /q");
    m_fileLines.append("\tPATHSEP2:=\\\\");
    m_fileLines.append("endif");
    m_fileLines.append("");
    m_fileLines.append("PATHSEP:=$(strip $(PATHSEP2))");
    m_fileLines.append("");
    m_fileLines.append("CC:=gcc");
    m_fileLines.append("LD:=gcc");
    m_fileLines.append("SRC_DIR:=./src");
#ifdef _WIN32
    m_fileLines.append("OUTPUT_FILE_PATH:=PCIC.exe");
#else
    m_fileLines.append("OUTPUT_FILE_PATH:=PCIC");
#endif
    m_fileLines.append("");

    QString includes;
    foreach (QString includeDirectory, m_includeDirectories)
    {
        includes.append("\\\n -I" + includeDirectory);
    }
    m_fileLines.append("INCLUDES:= " + includes);

    QString libs;
    QString libIncludes;
    foreach (QString libraryFile, m_libraryFiles)
    {
        QFileInfo libFile(libraryFile);
        libs.append("\\\n  -l" + libFile.completeBaseName());
        libIncludes.append("\\\n  -L" + libFile.path());
    }
    /* -lws2_32 is simulator only!! */
    m_fileLines.append("");
    m_fileLines.append("LIBS:= " + libs + "\\\n  -lws2_32");
    m_fileLines.append("");
    m_fileLines.append("LIB_INCLUDES:= " + libIncludes);
    m_fileLines.append("");
    m_fileLines.append("CFLAGS+= -Wall -c -std=c99");
    m_fileLines.append("");

    QString srcs;
    foreach (QString sourceFile, m_sourceFiles)
    {
        srcs.append(" \\\n " + sourceFile.replace("/", "$(PATHSEP)") );
    }

    m_fileLines.append("SRCS:=" + srcs);
    m_fileLines.append("");
    m_fileLines.append("OBJS:=$(SRCS:.c=.o)");
    m_fileLines.append("");
    m_fileLines.append("all: $(OUTPUT_FILE_PATH)");
    m_fileLines.append("");
    m_fileLines.append("$(OUTPUT_FILE_PATH): $(OBJS)");
    m_fileLines.append("\t$(LD) $(OBJS) $(LIB_INCLUDES) $(LIBS) -o $@");
    m_fileLines.append("");
    m_fileLines.append("%.o: %.c");
    m_fileLines.append("\t$(CC) $(CFLAGS) $(INCLUDES) -o \"$@\" \"$<\"");
    m_fileLines.append("");
    m_fileLines.append("clean:");
    m_fileLines.append("\t-$(RM) $(OBJS)");
    m_fileLines.append("\t-$(RM) $(OUTPUT_FILE_PATH)");
}

void MakefileCreator::ParseGeneratedFiles(const QList<GeneratedFile>& files)
{
    m_sourceFiles.clear();
    m_includeDirectories.clear();
    m_libraryFiles.clear();
    m_libraryDirectories.clear();

    foreach (GeneratedFile file, files)
    {
        QFileInfo fileInfo(file.RelativePath());

        if (fileInfo.fileName().endsWith(".c"))
        {
            m_sourceFiles.append(fileInfo.filePath());
        }
        else if (fileInfo.fileName().endsWith(".h"))
        {
            if (m_includeDirectories.contains(fileInfo.path())) continue;
            m_includeDirectories.append(fileInfo.path());
        }
        else if (fileInfo.fileName().endsWith(".lib"))
        {
            m_libraryFiles.append(fileInfo.filePath());
            if (m_libraryDirectories.contains(fileInfo.path())) continue;
            m_libraryDirectories.append(fileInfo.path());
        }
    }
}

void MakefileCreator::ParseStaticFiles(const QList<QString> files)
{
    foreach (QString file, files)
    {
        QFileInfo fileInfo(file);

        if (fileInfo.fileName().endsWith(".c"))
        {
            m_sourceFiles.append(fileInfo.filePath());
        }
        else if (fileInfo.fileName().endsWith(".h"))
        {
            if (m_includeDirectories.contains(fileInfo.path())) continue;
            m_includeDirectories.append(fileInfo.path());
        }
        else if (fileInfo.fileName().endsWith(".lib"))
        {
            m_libraryFiles.append(fileInfo.filePath());
            if (m_libraryDirectories.contains(fileInfo.path())) continue;
            m_libraryDirectories.append(fileInfo.path());
        }
    }
}

QString MakefileCreator::GenerateMakefileLocal()
{
    QStringList makefileLocal;
    makefileLocal.append("################################################################################");
    makefileLocal.append("# These macros are used by makefiles and argument files.");
    makefileLocal.append("# When a space is included in the path and quotes (\") cannot be used,");
    makefileLocal.append("# a _ESCAPE version of the macro is added that escapes the space with backslash (\\).");
    makefileLocal.append("################################################################################");
    makefileLocal.append("");
    makefileLocal.append("export GNU_Make_Install_DirEnv=$(CWInstallLocationEnv)/gnu/bin");
    makefileLocal.append("export ProjDirPath=C:/Users/maan/workspace/Fresscale5606");
    makefileLocal.append("export CWInstallLocationEnv=C:/Freescale/CW MCU v10.6");
    makefileLocal.append("export MCUToolsBaseDirEnv=$(CWInstallLocationEnv)/MCU");
    makefileLocal.append("export PAToolsDirEnv=$(CWInstallLocationEnv)/MCU/PA_Tools/Command_Line_Tools");

    return makefileLocal.join("\n");
}

QString MakefileCreator::GenerateSubDirMakefile(QString subDir)
{
    QStringList makefileSubDir;
    QStringList files;
    static quint32 fileCount = 0;

    makefileSubDir.append("################################################################################");
    makefileSubDir.append("# Automatically-generated file. Do not edit!");
    makefileSubDir.append("################################################################################");
    makefileSubDir.append("");
    makefileSubDir.append("# Add inputs and outputs from these tool invocations to the build variables");
    makefileSubDir.append("C_SRCS_QUOTED += \\");

    foreach(QString file, m_sourceFiles)
    {
        QFileInfo qf(file);

        if (0 == qf.path().compare(subDir))
        {
            makefileSubDir.append("\"Source/" + file + "\" \\");
            files.append(file.remove(file.count() -2, 2));
        }
    }

    makefileSubDir.append("");
    makefileSubDir.append("C_SRCS += \\");

    foreach(QString file, files)
    {
        makefileSubDir.append("Source/" +file + ".c \\");
    }

    makefileSubDir.append("");
    makefileSubDir.append("OBJS += \\");

    foreach(QString file, files)
    {
        makefileSubDir.append("Build/MPC5606S/Source/" + file + ".obj \\");
    }

    makefileSubDir.append("");
    makefileSubDir.append("OBJS_QUOTED += \\");

    foreach(QString file, files)
    {
        makefileSubDir.append("\"Build/MPC5606S/Source/" +  file + ".obj\" \\");
    }

    makefileSubDir.append("");
    makefileSubDir.append("C_DEPS += \\");

    foreach(QString file, files)
    {
        makefileSubDir.append("Build/MPC5606S/Source/" + file + ".d \\");
    }

    makefileSubDir.append("");
    makefileSubDir.append("C_DEPS_QUOTED += \\");

    foreach(QString file, files)
    {
        makefileSubDir.append("\"Build/MPC5606S/Source/" + file + ".d\" \\");
    }

    makefileSubDir.append("");
    makefileSubDir.append("");
    makefileSubDir.append("# Each subdirectory must supply rules for building sources it contributes");

    foreach(QString file, files)
    {
        makefileSubDir.append("Build/MCP5606S/Source/" + file + ".obj: Sources/" + file + ".c");
        makefileSubDir.append("@echo 'Building file: $<'");
        makefileSubDir.append("@echo 'Executing target #" + QString::number(++fileCount) + " $<'");
        makefileSubDir.append("    @echo 'Invoking: PowerPC Compiler'");
        makefileSubDir.append("    \"$(PAToolsDirEnv)/mwcceppc\" @@\"Build/MPC5606S/MPC5606S.obj.args\" -o \"Build/MPC5606S/Source/" + file + ".obj\" \"$<\" -MD -gccdep");
        makefileSubDir.append("    @echo 'Finished building: $<'");
        makefileSubDir.append("    @echo ' '");
        makefileSubDir.append("");
    }

    return makefileSubDir.join("\n");
}

QStringList MakefileCreator::GetSourceDirs()
{
    return m_sourceDirectories;
}
