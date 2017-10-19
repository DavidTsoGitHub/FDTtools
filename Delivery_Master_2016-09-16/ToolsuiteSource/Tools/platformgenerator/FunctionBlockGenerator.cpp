#include "FunctionBlockGenerator.h"

#include "Exceptions/ProjectException.h"
#include "FunctionBlock.h"
#include "Logger.h"

#include <QFileInfo>
#include <QTextStream>

FunctionBlockFileCopier::FunctionBlockFileCopier(Project* project, bool generateForSimulation) :
    m_project(project),
    m_generateForSimulation(generateForSimulation)
{
}

FunctionBlockFileCopier::~FunctionBlockFileCopier()
{
}

void FunctionBlockFileCopier::OnGenerateFiles()
{
    LOG_DEBUG("Entering");

    copyFunctionBlockFiles();
    generateFunctionBlockConfigurationSourceFiles();
    generateFunctionBlockConfigurationHeaderFiles();
}


void FunctionBlockFileCopier::copyFunctionBlockFiles()
{
    foreach (FunctionBlock* block, m_project->GetFunctionBlocks().List())
    {
        QFileInfo functionDefinitionFile(m_project->GetFunctionLibraryRoot().absoluteFilePath(block->GetPathInFunctionLibrary()));
        m_currentFunctionBlockRoot = functionDefinitionFile.absolutePath();
        m_currentFunctionBlockName = block->GetFunctionDefinition().GetName();
        bool libFound = false;
        if (m_generateForSimulation)
        {
            QDir pcLibDir(m_currentFunctionBlockRoot + "/libs/PCLib");
            if (getFiles(pcLibDir) > 0)
            {
                libFound = true;
            }
        }
        else
        {
            QDir cwLibDir(m_currentFunctionBlockRoot + "/libs/CWLib");
            if (getFiles(cwLibDir) > 0)
            {
                libFound = true;
            }
        }

        if (!libFound)
        {
            int nbrOfFiles = 0;
            QDir srcDir(m_currentFunctionBlockRoot + "/src");
            nbrOfFiles += getFiles(srcDir);
            QDir incDir(m_currentFunctionBlockRoot + "/inc");
            nbrOfFiles += getFiles(incDir);

            if (nbrOfFiles == 0)
            {
                QString errorString("FunctionBlock \"" + m_currentFunctionBlockName + "\"has no library or source code for this platform");
                LOG_ERROR(errorString);
            }
        }
    }
}

void FunctionBlockFileCopier::generateFunctionBlockConfigurationSourceFiles()
{
    foreach (FunctionBlock* functionBlock, m_project->GetFunctionBlocks().List())
    {
        const FunctionDefinition& definition = functionBlock->GetFunctionDefinition();
        QStringList includeLines;
        QStringList fileLines;
        includeLines.append("#include \"StandardTypes.h\"");
        fileLines.append("");

        if ((definition.ConfigurationParameters().count() > 0 || definition.FaultDefinitions().count() > 0))
        {
            includeLines.append("#include \"" + definition.GetName() + "_cfg.h\"");
        }
        if (definition.ConfigurationParameters().count() > 0)
        {

            fileLines.append("/****** Configuration Parameters ******/");
            foreach (const ConfigurationParameter* configurationParameter, definition.ConfigurationParameters())
            {
                if (!functionBlock->HasConfigurationParameter(configurationParameter->GetParameterName()))
                {
                    LOG_WARNING("Configuration parameter missing value! " + definition.GetName() + "." + configurationParameter->GetParameterName() + " (" + configurationParameter->GetDescriptiveName() + ")");
                    continue;
                }

                fileLines.append("/* " + configurationParameter->GetDescriptiveName());
                fileLines.append(" * " +  configurationParameter->GetDescription());
                fileLines.append(" */");

                QString configurationParameterValue = functionBlock->GetConfigurationParameterValue(configurationParameter->GetParameterName());
                QString paramDecl = "const " + configurationParameter->GetDataType() + " " + configurationParameter->GetParameterName();
                if(configurationParameter->GetArrayLength() > 1)
                {
                    paramDecl.append("[" + QString::number(configurationParameter->GetArrayLength()) + "] =\n{\n");
                    QStringList valueList = configurationParameterValue.split('|', QString::SkipEmptyParts);
                    QStringList convertedValues;
                    foreach (QString value, valueList)
                    {
                        convertedValues.append("    " + value);
                    }

                    paramDecl.append(convertedValues.join(",\n"));
                    paramDecl.append("\n};");
                }
                else
                {
                    paramDecl.append(" = " + configurationParameterValue + ";");
                }

                fileLines.append(paramDecl);
                fileLines.append("");
            }
            fileLines.append("");
        }

        if(definition.LabelMappingsParameters().count() > 0)
        {
            includeLines.append("#include \"" + definition.GetName() + "_cfg.h\"");

            foreach (LabelMappings* labelMappingsParameter, definition.LabelMappingsParameters())
            {
                QString name = labelMappingsParameter->GetLabelMappingsName();
                QString count = QString::number(labelMappingsParameter->GetMappedLabels().count());

                if (labelMappingsParameter->GetMappedLabels().count() == 0)
                {
                    LOG_WARNING("LabelMappings parameter is empty! (" + name + ")");
                    continue;
                }
                fileLines.append("LabelMappings " + name + " =");
                fileLines.append("{");
                fileLines.append("    .Count = " + count + ",");
                fileLines.append("    .Mappings = ");

                fileLines.append("        {");
                int index = 0;
                foreach (Label* label, labelMappingsParameter->GetMappedLabels())
                {
                    fileLines.append("            {");
                    fileLines.append("                .LabelID = " + QString::number(label->GetID()) + ",");
                    fileLines.append("                .Value = " + QString::number(index));
                    fileLines.append("            },");
                    index++;
                }
                fileLines.append("        }");

                fileLines.append("};");
                fileLines.append("");
            }
        }

        if (definition.IconResources().count() > 0)
        {
            includeLines.append("#include \"" + definition.GetName() + "_cfg.h\"");
            fileLines.append("// IconIDs");
            foreach (const IconResource iconResource, definition.IconResources())
            {
                fileLines.append("const uint32 IconID_" + iconResource.Name + " = " + QString::number(functionBlock->GetIconResourceID(iconResource.Name)) + "U;");
            }
        }

        if (definition.ImageResources().count() > 0)
        {
            includeLines.append("#include \"" + definition.GetName() + "_cfg.h\"");
            fileLines.append("// ImageIDs");
            foreach (const ImageResource imageResource, definition.ImageResources())
            {
                fileLines.append("const uint32 ImageID_" + imageResource.Name + " = " + QString::number(functionBlock->GetImageResourceID(imageResource.Name)) + "U;");
            }
        }

        if (definition.FaultDefinitions().count() > 0)
        {

            fileLines.append("// DTCs");
            foreach (const FaultDefinition fault, definition.FaultDefinitions())
            {
                if (!functionBlock->HasDTCParameter(fault.Name))
                {
                    LOG_WARNING("Fault Definition missing assigned DTC! (" + definition.GetName() + ")");
                    continue;
                }
                else
                {

                    fileLines.append("const uint32 DTC_" + fault.Name + " = " + QString::number(functionBlock->GetDTCParameterID(fault.Name)) + "U;");
                }
            }
        }

        fileLines.append("");
        if (definition.ConfigurationParameters().count() > 0 || definition.LabelMappingsParameters().count() > 0 || definition.IconResources().count() > 0 || definition.ImageResources().count() > 0 || definition.FaultDefinitions().count() > 0)
        {
            QString configurationFilePath(PlatformPaths::CreateFunctionBlockPath(definition.GetName()));
            configurationFilePath.append("/cfg/" + definition.GetName() + "_cfg.c");
            includeLines.removeDuplicates();
            includeLines.append(fileLines);
            AddGeneratedFile(configurationFilePath, includeLines.join("\n"));
        }
    }
}

void FunctionBlockFileCopier::generateFunctionBlockConfigurationHeaderFiles(void)
{
    foreach (FunctionBlock* functionBlock, m_project->GetFunctionBlocks().List())
    {
        const FunctionDefinition& definition = functionBlock->GetFunctionDefinition();
        QStringList fileLines;
        fileLines.append("#include \"StandardTypes.h\"");
        fileLines.append("");

        if (definition.ConfigurationParameters().count() > 0)
        {
            fileLines.append("/****** Configuration Parameters ******/");
            foreach (const ConfigurationParameter* configurationParameter, definition.ConfigurationParameters())
            {
                if (!functionBlock->HasConfigurationParameter(configurationParameter->GetParameterName()))
                {
                    LOG_WARNING("Configuration parameter missing value! " + definition.GetName() + "." + configurationParameter->GetParameterName() + " (" + configurationParameter->GetDescriptiveName() + ")");
                    continue;
                }

                fileLines.append("/* " + configurationParameter->GetDescriptiveName());
                fileLines.append(" * " +  configurationParameter->GetDescription());
                fileLines.append(" */");

                QString paramDecl = "extern const " + configurationParameter->GetDataType() + " " + configurationParameter->GetParameterName();
                if(configurationParameter->GetArrayLength() > 1)
                {
                    paramDecl.append("[" + QString::number(configurationParameter->GetArrayLength()) + "];");
                }
                else
                {
                    paramDecl.append(";");
                }

                fileLines.append(paramDecl);
                fileLines.append("");
            }
        }

        if(definition.LabelMappingsParameters().count() > 0)
        {
            foreach (LabelMappings* labelMappingsParameter, definition.LabelMappingsParameters())
            {
                QString name = labelMappingsParameter->GetLabelMappingsName();
                fileLines.append("extern LabelMappings " + name + ";");
            }
            fileLines.append("");
        }

        if (definition.IconResources().count() > 0)
        {
            fileLines.append("// IconIDs");
            foreach (const IconResource iconResource, definition.IconResources())
            {
                fileLines.append("extern const uint32 IconID_" + iconResource.Name + ";");
            }
            fileLines.append("");
        }

        if (definition.ImageResources().count() > 0)
        {
            fileLines.append("// ImageIDs");
            foreach (const ImageResource imageResource, definition.ImageResources())
            {
                fileLines.append("extern const uint32 ImageID_" + imageResource.Name + ";");
            }
            fileLines.append("");
        }

        if (definition.FaultDefinitions().count() > 0)
        {

            fileLines.append("// DTCs");
            foreach (const FaultDefinition fault, definition.FaultDefinitions())
            {
                if (!functionBlock->HasDTCParameter(fault.Name))
                {
                    LOG_WARNING("Fault Definition missing assigned DTC! (" + definition.GetName() + ")");
                    continue;
                }
                else
                {

                    fileLines.append("extern const uint32 DTC_" + fault.Name + ";");
                }
            }
            fileLines.append("");
        }

        if (definition.IconResources().count() > 0 || definition.ImageResources().count() > 0 || definition.LabelMappingsParameters().count() > 0 || definition.ConfigurationParameters().count() > 0 || (definition.FaultDefinitions().count() > 0))
        {
            QString configurationFileHeaderPath(PlatformPaths::CreateFunctionBlockPath(definition.GetName()));
            configurationFileHeaderPath.append("/cfg/" + definition.GetName() + "_cfg.h");
            AddGeneratedFile(configurationFileHeaderPath, fileLines.join("\n"));
        }
    }
}

int FunctionBlockFileCopier::getFiles(const QDir& currentDir)
{
    int nbrOfFiles = 0;
    foreach (QFileInfo directory, currentDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        getFiles(directory.absoluteDir());
    }

    foreach (QFileInfo file, currentDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot))
    {
        QFile fileToRead(file.absoluteFilePath());
        if (!fileToRead.exists() || !fileToRead.open(QFile::ReadOnly))
        {
            continue;
        }

        AddCopiedFile(getRelativePath(file), fileToRead.readAll());
        ++nbrOfFiles;
    }

    return nbrOfFiles;
}

QString FunctionBlockFileCopier::getRelativePath(const QFileInfo& functionBlockFile)
{
    QString relativePath(functionBlockFile.absoluteFilePath());
    relativePath.remove(m_currentFunctionBlockRoot);

    return QString(PlatformPaths::CreateFunctionBlockPath(m_currentFunctionBlockName)).append(relativePath);
}
