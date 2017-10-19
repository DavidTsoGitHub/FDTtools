#include "ReviewFunctionBlockWizardPage.h"
#include "ui_ReviewFunctionBlockWizardPage.h"

#include "FunctionDefinitionSerializer.h"

#include "Exceptions/CannotWriteFileException.h"

#include "FunctionDefinition.h"
#include <QDir>
#include <QMessageBox>
#include <QStringList>
#include <QTextStream>
#include <QWizardPage>

ReviewFunctionBlockWizardPage::ReviewFunctionBlockWizardPage(FunctionDefinition& functionDefinition, QString functionLibraryRoot, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ReviewFunctionBlockWizardPage),
    m_functionDefinition(functionDefinition),
    m_functionLibraryRoot(functionLibraryRoot)
{
    ui->setupUi(this);
}

ReviewFunctionBlockWizardPage::~ReviewFunctionBlockWizardPage()
{
    delete ui;
}


void ReviewFunctionBlockWizardPage::initializePage()
{
    QPlainTextEdit* output = ui->functionBlockOutput;

    output->appendHtml("<b>Function Block Name:</b> " + m_functionDefinition.GetName());
    output->appendHtml("<b>Tasks:</b> <i>(Init | Runnable | Delay | Periodicity)</i>");

    foreach (const Task* task, m_functionDefinition.Tasks())
    {
        output->appendPlainText("    " + task->InitFunction + " " + task->Runnable + " " + QString::number(task->StartupDelay) + " ms " + QString::number(task->Periodicity) + " ms");
    }
    output->appendPlainText("");

    output->appendHtml("<b>Input signals:</b>");
    foreach (const GCLSignalSink* sink, m_functionDefinition.Sinks())
    {
        output->appendPlainText("    " + sink->DataType() + " " + sink->SignalName() + " UpdateNotification: " + (sink->HasUpdateNotification() ? "Yes" : "No") + " Mandatory: " + (sink->IsMandatory() ? "Yes" : "No"));
    }
    output->appendPlainText("");

    output->appendHtml("<b>Output signals:</b>");
    foreach (const GCLSignalSource* source, m_functionDefinition.Sources())
    {
        output->appendPlainText("    " + source->DataType() + " " + source->SignalName());
    }
    output->appendPlainText("");

    output->appendHtml("<b>Configuration parameters:</b>");

    foreach (const ConfigurationParameter* cfgParam, m_functionDefinition.ConfigurationParameters())
    {
        if (cfgParam->GetArrayLength() > 1)
        {
            output->appendPlainText(QString("    %1[%3] %2 (%4)").arg(cfgParam->GetDataType()).arg(cfgParam->GetParameterName()).arg(cfgParam->GetArrayLength()).arg(cfgParam->GetDescriptiveName()));
        }
        else
        {
            output->appendPlainText(QString("    %1 %2 (%3)").arg(cfgParam->GetDataType()).arg(cfgParam->GetParameterName()).arg(cfgParam->GetDescriptiveName()));
        }
    }
    output->appendPlainText("");

    output->appendHtml("<b>Calibration parameters:</b>");
    foreach (const CalibrationParameter* calParam, m_functionDefinition.CalibrationParameters())
    {
        if (calParam->GetArrayLength() > 1)
        {
            output->appendPlainText(QString("    %1[%3] %2 (%4)").arg(calParam->GetDataType()).arg(calParam->GetParameterName()).arg(calParam->GetArrayLength()).arg(calParam->GetDescriptiveName()));
        }
        else
        {
            output->appendPlainText(QString("    %1 %2 (%3)").arg(calParam->GetDataType()).arg(calParam->GetParameterName()).arg(calParam->GetDescriptiveName()));
        }
    }
    output->appendPlainText("");

    output->appendHtml("<b>LabelMappings parameters:</b>");
    foreach (LabelMappings* labelMappingsParameter, m_functionDefinition.LabelMappingsParameters())
    {
        output->appendPlainText("    " + labelMappingsParameter->GetLabelMappingsName() + " (" + labelMappingsParameter->GetDescriptiveName() + ")");
    }
    output->appendPlainText("");

    output->appendHtml("<b>Icon Resources:</b>");
    foreach (const IconResource iconResource, m_functionDefinition.IconResources())
    {
        output->appendPlainText("    * " + iconResource.Name);
    }
    output->appendPlainText("");

    output->appendHtml("<b>Image Resources:</b>");
    foreach (const ImageResource imageResource, m_functionDefinition.ImageResources())
    {
        output->appendPlainText("    * " + imageResource.Name);
    }
    output->appendPlainText("");

    output->appendHtml("<b>Fault Resources:</b>");
    foreach (const FaultDefinition faultDefinition, m_functionDefinition.FaultDefinitions())
    {
        output->appendPlainText("    " + faultDefinition.Name + " (" + faultDefinition.Description + ")");
    }

}

bool ReviewFunctionBlockWizardPage::validatePage()
{
    QDir destination(m_functionLibraryRoot.absoluteFilePath(m_functionDefinition.GetName()));
    if (destination.exists())
    {
        QMessageBox::StandardButton result = QMessageBox::question(this, "Overwrite destination folder?", "A function block named " + m_functionDefinition.GetName() + " already exists in the function library, overwrite?");
        if (result == QMessageBox::No)
        {
            return false;
        }
        else
        {
            if (!destination.removeRecursively())
            {
                QMessageBox::warning(this, "Failed to clean destination folder!", "Attempting to clean the destination folder (" + destination.absolutePath() + ") failed, try to remove the files manually and try again.", QMessageBox::Ok);
                return false;
            }
        }
    }

    if (!destination.mkpath(".") || !destination.mkpath("inc") || !destination.mkpath("src"))
    {
        QMessageBox::warning(this, "Error creating destination directory", "An error occured while creating the destination directory structure (" + destination.absolutePath() + ")");
        return false;
    }

    return writeFunctionDefinitionFile(destination) &&
           writeExampleHeaderFile(destination) &&
           writeExampleSourceFile(destination);
}

bool ReviewFunctionBlockWizardPage::writeFunctionDefinitionFile(QDir& rootDir)
{
    QFile functionDefinitionFile(rootDir.absoluteFilePath(m_functionDefinition.GetName() + ".fdf"));
    if (!functionDefinitionFile.open(QFile::WriteOnly))
    {
        QMessageBox::warning(this, "Error writing file!", "An error occured while writing the function definition file (" + functionDefinitionFile.errorString() + ")");
        return false;
    }

    QString contents = FunctionDefinitionSerializer::Serialize(m_functionDefinition);
    QTextStream fileStream(&functionDefinitionFile);
    fileStream << contents;
    fileStream.flush();
    functionDefinitionFile.close();

    return true;
}

bool ReviewFunctionBlockWizardPage::writeExampleSourceFile(QDir& rootDir)
{
    QFile exampleSourceFile(rootDir.absoluteFilePath("src/" + m_functionDefinition.GetName() + ".c"));
    if (!exampleSourceFile.open(QFile::WriteOnly))
    {
        QMessageBox::warning(this, "Error writing file!", "An error occured while writing the example source file (" + exampleSourceFile.errorString() + ")");
        return false;
    }

    QStringList fileLines;   

    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"" + m_functionDefinition.GetName() + ".h\"");
    fileLines.append("#include \"GCL_" + m_functionDefinition.GetName() + ".h\"");
    fileLines.append("#include \"StandardTypes.h\"");
    if (m_functionDefinition.CalibrationParameters().count() > 0)
    {
        fileLines.append("#include \"CAL.h\"");
    }
    if (m_functionDefinition.FaultDefinitions().count() > 0)
    {
        fileLines.append("#include \"DTCManager_cfg.h\"");
        fileLines.append("#include \"Diagnosis.h\"");
    }
    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");
    if (m_functionDefinition.CalibrationParameters().count() > 0)
    {
        fileLines.append("// Array length constants for calibration parameters");
        foreach (const CalibrationParameter* calParam, m_functionDefinition.CalibrationParameters())
        {
            fileLines.append(QString("#define CAL_%1_COUNT %2u").arg(calParam->GetParameterName()).arg(calParam->GetArrayLength()));
        }
        fileLines.append("");
    }
    if (m_functionDefinition.ConfigurationParameters().count() > 0)
    {
        fileLines.append("// Configuration parameters (provided by FDT tool while generating platform code)");
        QStringList externDeclarations;
        foreach (const ConfigurationParameter* cfgParam, m_functionDefinition.ConfigurationParameters())
        {
            fileLines.append(QString("#define %1_COUNT %2u").arg(cfgParam->GetParameterName().toUpper()).arg(cfgParam->GetArrayLength()));

            if (cfgParam->GetArrayLength() > 1)
            {
                externDeclarations.append(QString("extern const %1 %2[]; // %3").arg(cfgParam->GetDataType()).arg(cfgParam->GetParameterName()).arg(cfgParam->GetDescriptiveName()));
            }
            else
            {
                externDeclarations.append(QString("extern const %1 %2; // %3").arg(cfgParam->GetDataType()).arg(cfgParam->GetParameterName()).arg(cfgParam->GetDescriptiveName()));
            }
        }

        fileLines.append(externDeclarations);
        fileLines.append("");
    }

    if (m_functionDefinition.LabelMappingsParameters().count() > 0)
    {
        fileLines.append("// Label Mappings (provided by FDT tool while generating platform code)");
        foreach (LabelMappings* labelMappings, m_functionDefinition.LabelMappingsParameters())
        {
            fileLines.append("extern const LabelMappings " + labelMappings->GetLabelMappingsName() + "; // " + labelMappings->GetDescriptiveName());
        }
        fileLines.append("");
    }

    if (m_functionDefinition.IconResources().count() > 0)
    {
        fileLines.append("// Icon IDs (provided by FDT tool while generating platform code)");
        foreach (const IconResource iconResource, m_functionDefinition.IconResources())
        {
            fileLines.append("extern const uint32 IconID_" + iconResource.Name + ";");
        }
        fileLines.append("");
    }

    if (m_functionDefinition.ImageResources().count() > 0)
    {
        fileLines.append("// Image IDs (provided by FDT tool while generating platform code)");
        foreach (const ImageResource imageResource, m_functionDefinition.ImageResources())
        {
            fileLines.append("extern const uint32 ImageID_" + imageResource.Name + ";");
        }
        fileLines.append("");
    }

    if (m_functionDefinition.FaultDefinitions().count() > 0)
    {
        fileLines.append("// Fault definition DTC representation (provided by FDT tool while generating platform code)");
        foreach (FaultDefinition faultDefinition, m_functionDefinition.FaultDefinitions())
        {
            fileLines.append("extern const uint32 DTC_" + faultDefinition.Name + "; // " + faultDefinition.Description);
        }
        fileLines.append("");
    }

    fileLines.append("/********************************* Variables *********************************/");
    foreach (const GCLSignalSink* sink, m_functionDefinition.Sinks())
    {
        if (sink->HasUpdateNotification())
        {
            fileLines.append("static uint8 u8" + sink->SignalName() + "Indicated;");
        }
    }
    fileLines.append("");
    fileLines.append("/************************* Local Function Prototypes *************************/");
    fileLines.append("");
    fileLines.append("/***************************** Exported Functions ****************************/");
    fileLines.append("/* Sample code is only generated for the function block named task functions");
    fileLines.append("   If you have defined more tasks, they have to be added manually */");
    fileLines.append("void " + m_functionDefinition.GetName() + "_Init(void)");
    fileLines.append("{");
    fileLines.append("    // Add initialization routines here");

//    Outputs should be declared explicitly. This won't generate the function prototype in the GCL subsystem.

//    foreach (LabelMappings* labelMappings, m_functionDefinition.LabelMappingsParameters())
//    {
//        fileLines.append("    GCL_Write_" + m_functionDefinition.GetName() + "_" + labelMappings->GetLabelMappingsName().replace(0, 1, labelMappings->GetLabelMappingsName().at(0).toUpper()) + "(&" + labelMappings->GetLabelMappingsName() + "); // Initial write of labelmappings for " + labelMappings->GetLabelMappingsName() + " to GCL" );
//    }

    fileLines.append("}");
    fileLines.append("");
    fileLines.append("void " + m_functionDefinition.GetName() + "_Run(void)");
    fileLines.append("{");
    if (m_functionDefinition.CalibrationParameters().count() > 0)
    {
        fileLines.append("    /** Calibration parameter reading samples **/");
        foreach (const CalibrationParameter* calParam, m_functionDefinition.CalibrationParameters())
        {
            if (calParam->GetArrayLength() > 1)
            {
                fileLines.append(QString("    %1 %2[%3];").arg(calParam->GetDataType()).arg(calParam->GetParameterName().toLower()).arg(calParam->GetArrayLength()));
                quint32 dataLength = calParam->GetArrayLength();
                if (calParam->GetDataType().endsWith("16")) dataLength *= 2;
                else if (calParam->GetDataType().endsWith("32")) dataLength *= 4;

                fileLines.append(QString("    CAL_Read(%1, (uint8*)%2, %3);").arg(calParam->GetParameterName()).arg(calParam->GetParameterName().toLower()).arg(dataLength));
            }
            else
            {
                fileLines.append(QString("    %1 %2;").arg(calParam->GetDataType()).arg(calParam->GetParameterName().toLower()));
                quint32 dataLength = 1;
                if (calParam->GetDataType().endsWith("16")) dataLength = 2;
                else if (calParam->GetDataType().endsWith("32")) dataLength = 4;

                fileLines.append(QString("    CAL_Read(%1, (uint8*)&%2, %3);").arg(calParam->GetParameterName()).arg(calParam->GetParameterName().toLower()).arg(dataLength));
            }

        }
        fileLines.append("");
    }

    fileLines.append("    /** Input signal reading samples **/");
    fileLines.append("    uint8 u8SignalStatus;");
    foreach (const GCLSignalSink* sink, m_functionDefinition.Sinks())
    {
        fileLines.append("    " + sink->DataType() + " " + sink->SignalName() + ";");
    }
    fileLines.append("");
    foreach (const GCLSignalSink* sink, m_functionDefinition.Sinks())
    {
        if (sink->HasUpdateNotification())
        {
            fileLines.append("    if (u8" + sink->SignalName() + "Indicated)");
            fileLines.append("    {");
            fileLines.append("        GCL_Read_" + m_functionDefinition.GetName() + "_" + sink->SignalName() + "(&" + sink->SignalName() + ", &u8SignalStatus);");
            fileLines.append("        u8" + sink->SignalName() + "Indicated = FALSE;");
            fileLines.append("    }");
        }
        else
        {
            fileLines.append("    GCL_Read_" + m_functionDefinition.GetName() + "_" + sink->SignalName() + "(&" + sink->SignalName() + ", &u8SignalStatus);");
        }
    }
    fileLines.append("");
    fileLines.append("    /** Output signal writing samples **/");
    foreach (const GCLSignalSource* source, m_functionDefinition.Sources())
    {
        fileLines.append("    GCL_Write_" + m_functionDefinition.GetName() + "_" + source->SignalName() + "(0);");
    }
    fileLines.append("");
    if (m_functionDefinition.FaultDefinitions().count() > 0)
    {
        fileLines.append("    /** Sample for reporting errors to diagnostics **/ ");
        FaultDefinition fd = m_functionDefinition.FaultDefinitions().first();
        fileLines.append("    tDiagReport errorReport;");
        fileLines.append("    errorReport.u32DTCNumber = DTC_" + fd.Name + ";");
        fileLines.append("    errorReport.Status = DTC_ACTIVE; // or DTC_INACTIVE");
        fileLines.append("    DIAG_ErrorReport(errorReport);");
        fileLines.append("    /** Note that the labelmappings struct needs to be updated programmatically");
        fileLines.append("        when/if the DTCs shall be shown in the display using a SelectableListWidget");
        fileLines.append("        and a DTC indication signal need to be sent to trigger the configured DTC menu item */");
    }

    fileLines.append("}");
    fileLines.append("");
    foreach (const GCLSignalSink* sink, m_functionDefinition.Sinks())
    {
        if (!sink->HasUpdateNotification()) continue;

        fileLines.append("void " + m_functionDefinition.GetName() + "_" + sink->SignalName() + "_Indication()");
        fileLines.append("{");
        fileLines.append("    u8" + sink->SignalName() + "Indicated = TRUE;");
        fileLines.append("}");
        fileLines.append("");
    }
    fileLines.append("/****************************** Local Functions ******************************/");
    fileLines.append("");

    QString contents = fileLines.join('\n').toLocal8Bit();
    QTextStream fileStream(&exampleSourceFile);
    fileStream << contents;
    fileStream.flush();
    exampleSourceFile.close();

    return true;
}

bool ReviewFunctionBlockWizardPage::writeExampleHeaderFile(QDir& rootDir)
{
    QFile exampleHeaderFile(rootDir.absoluteFilePath("inc/" + m_functionDefinition.GetName() + ".h"));
    if (!exampleHeaderFile.open(QFile::WriteOnly))
    {
        QMessageBox::warning(this, "Error writing file!", "An error occured while writing the example header file (" + exampleHeaderFile.errorString() + ")");
        return false;
    }

    QString headerGuard("__" + m_functionDefinition.GetName().toUpper() + "_H");

    QStringList fileLines;
    fileLines.append("#ifndef " + headerGuard);
    fileLines.append("#define " + headerGuard);
    fileLines.append("");
    foreach (const Task* task, m_functionDefinition.Tasks())
    {
        fileLines.append("void " + task->Runnable + "(void);");
        fileLines.append("void " + task->InitFunction + "(void);");
        fileLines.append("");
    }
    foreach (const GCLSignalSink* sink, m_functionDefinition.Sinks())
    {
        if (!sink->HasUpdateNotification()) continue;

        fileLines.append("void " + m_functionDefinition.GetName() + "_" + sink->SignalName() + "_Indication(void);");

    }

    fileLines.append("#endif // " + headerGuard);

    QString contents = fileLines.join('\n').toLocal8Bit();
    QTextStream fileStream(&exampleHeaderFile);
    fileStream << contents;
    fileStream.flush();
    exampleHeaderFile.close();

    return true;
}
