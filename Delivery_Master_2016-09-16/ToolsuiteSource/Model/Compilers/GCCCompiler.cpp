#include "GCCCompiler.h"
#include "Logger.h"

#include <QProcess>

#define MAKEPATHKEY "MakePath"
#define BINPATHKEY "BinPath"
#define MAKEFLAGSKEY "MakeFlags"
#define GCCFLAGSKEY "GCCFlags"

GCCCompiler::GCCCompiler() : CompilerBase("GCC Compiler")
{
    addCustomAttributeDefinition(MAKEPATHKEY, "Make.exe location", CompilerBase::File);
    addCustomAttributeDefinition(BINPATHKEY, "Path to GCC bin", CompilerBase::Directory);
    addCustomAttributeDefinition(MAKEFLAGSKEY, "Make flags", CompilerBase::Text);
    addCustomAttributeDefinition(GCCFLAGSKEY, "GCC flags", CompilerBase::Text);
}

bool GCCCompiler::GenerateBuildEnvironment(QString projectOutputPath)
{
    m_sourcePath = projectOutputPath + "/Source";
    m_buildFolder = projectOutputPath + "/Build";

    m_tree = parseSourceTree(m_sourcePath);

    return generatePCMakeFile();
}

bool GCCCompiler::Build(QString projectOutputPath)
{
    m_sourcePath = projectOutputPath + "/Source";
    m_buildFolder = projectOutputPath + "/Build";

    QString makePath = GetCustomAttribute(MAKEPATHKEY);
    QString arguments = GetCustomAttribute(MAKEFLAGSKEY);

    LOG_INFO("Building for PC target using: " + makePath + " " + arguments);
    LOG_DEBUG("m_buildFolder: " + m_buildFolder);
    QProcess buildProcess;
    buildProcess.setProgram(makePath);

#ifdef _WIN32
    buildProcess.setNativeArguments(arguments);
#else
    buildProcess.setArguments(QStringList({arguments}));
#endif

    buildProcess.setWorkingDirectory(m_buildFolder);
    buildProcess.setProcessChannelMode(QProcess::ForwardedChannels);
    buildProcess.start();

    if (!buildProcess.waitForFinished(-1) || buildProcess.exitCode() != 0)
    {
        LOG_ERROR("Error while building: " + buildProcess.errorString());
        return false;
    }

    return true;
}

bool GCCCompiler::HasMandatoryCompilerSpecificData()
{
    bool hasMandatoryData = true;

    hasMandatoryData &= CompilerBase::CompilerAttributeExist(MAKEPATHKEY, "Make exe path empty or file missing");
    hasMandatoryData &= CompilerBase::CompilerAttributeExist(BINPATHKEY, "Make bin path empty or file missing");

    return hasMandatoryData;
}

bool GCCCompiler::generatePCMakeFile()
{
    QString makeFilePath(m_buildFolder + "/Makefile");
    LOG_INFO("Generating Makefile for PC target to " + makeFilePath);

    QStringList fileLines;
    QString binPath = GetCustomAttribute(BINPATHKEY);

    fileLines.append("# This is an automatically created Makefile for PC.");
    fileLines.append("# Any changes will be overwritten upon subsequent runs of the code generator tool");
    fileLines.append("");
    fileLines.append("# Environment setup");
    fileLines.append("ifeq ($(OS),Windows_NT)");
    fileLines.append("\tSHELL:=C:/Windows/System32/cmd.exe");
    fileLines.append("\tMD:= mkdir");
    fileLines.append("\tRM:= del /s /q");
    fileLines.append("\tPATHSEP2:=\\\\");
    fileLines.append("else");
    fileLines.append("\tMD:= mkdir -p");
    fileLines.append("\tRM:= rm -rf");
    fileLines.append("\tPATHSEP2:=/");
    fileLines.append("endif");
    fileLines.append("");
    fileLines.append("PATHSEP:=$(strip $(PATHSEP2))");
    fileLines.append("");
    fileLines.append("CCPATH:=" + binPath.replace("/", "$(PATHSEP)"));
    fileLines.append("CC:=gcc");
    fileLines.append("LD:=gcc");
#ifdef _WIN32
    fileLines.append("OUTPUT_FILE_PATH:=PCIC.exe");
#else
    fileLines.append("OUTPUT_FILE_PATH:=PCIC");
#endif
    fileLines.append("");

    QString includes;
    foreach (QString includeDirectory, m_tree.IncludeDirectories)
    {
        includes.append("\\\n -I" + m_sourcePath.replace("/", "$(PATHSEP)") + "$(PATHSEP)" + includeDirectory.replace("/", "$(PATHSEP)"));
    }
    fileLines.append("INCLUDES:= " + includes);

    QString libs;
    QString libIncludes;
    foreach (QString libraryFile, m_tree.LibraryFiles)
    {
        QFileInfo libFile(libraryFile);
        if (libFile.suffix() == "a")
        {
            libs.append("\\\n  -l" + libFile.completeBaseName().mid(3));
        }
        else
        {
            libs.append("\\\n  -l" + libFile.completeBaseName());
        }
        libIncludes.append("\\\n  -L" + m_sourcePath.replace("/", "$(PATHSEP)") + "$(PATHSEP)" + libFile.path().replace("/", "$(PATHSEP)"));
    }

    fileLines.append("");
    fileLines.append("LIBS:= " + libs + "\\\n  -lws2_32");
    fileLines.append("");
    fileLines.append("LIB_INCLUDES:= " + libIncludes);
    fileLines.append("");
    fileLines.append("CFLAGS+= -Wall -c -std=c99 " + GetCustomAttribute(GCCFLAGSKEY));
    fileLines.append("");

    QString srcs;
    QString objs;
    foreach (QString sourceFile, m_tree.SourceFiles)
    {
        srcs.append(" \\\n ..$(PATHSEP)Source$(PATHSEP)" + sourceFile.replace("/", "$(PATHSEP)") );
        QString obj(sourceFile);
        obj.replace(obj.length()-1, 1, "o");
        objs.append(" \\\n objs$(PATHSEP)" + obj);
    }

    fileLines.append("SRCS:=" + srcs);
    fileLines.append("");
    fileLines.append("OBJS:=" + objs);
    fileLines.append("");
    fileLines.append("all: $(OUTPUT_FILE_PATH)");
    fileLines.append("");
    fileLines.append("$(OUTPUT_FILE_PATH): $(OBJS)");
    fileLines.append("\t$(LD) $(OBJS) $(LIB_INCLUDES) $(LIBS) -o $@");
    fileLines.append("");
    fileLines.append("objs$(PATHSEP)%.o: ..$(PATHSEP)Source$(PATHSEP)%.c");
    fileLines.append("ifeq ($(OS),Windows_NT)");
    fileLines.append("\tif not exist $(dir $@) mkdir $(dir $@)");
    fileLines.append("else");
    fileLines.append("\tmkdir -p $(dir $@)");
    fileLines.append("endif");
    fileLines.append("\t$(CCPATH)$(PATHSEP)$(CC) $(CFLAGS) $(INCLUDES) -o \"$@\" \"$<\"");
    fileLines.append("");
    fileLines.append("clean:");
    fileLines.append("\t-$(RM) $(OBJS)");
    fileLines.append("\t-$(RM) $(OUTPUT_FILE_PATH)");

    QString fileContent = fileLines.join("\n");

    bool writeResult = writeToFile(makeFilePath, fileContent);
    if (!writeResult)
    {
        LOG_ERROR("Failed to write Makefile!");
        return false;
    }

    LOG_INFO("Done!");

    return true;
}
