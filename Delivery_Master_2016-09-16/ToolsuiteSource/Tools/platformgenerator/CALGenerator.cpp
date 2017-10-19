#include "CALGenerator.h"

#include "CalibrationParameter.h"
#include "FunctionBlock.h"
#include "Logger.h"

//Defines the size of the variable used to store checksums
#define CAL_STATUS_STORAGE_SIZE 4

CALGenerator::CALGenerator(Project* project, bool generateForSimulation) :
    m_targetSettings(project->GetTargetSettings()),
    m_functionBlocks(project->GetFunctionBlocks().List()),
    m_bspSpecification(project->GetBSPSpecification()),
    m_generateForSimulation(generateForSimulation)
{
    const qint8 numberOfODOstorageSlots = 5;
    qint8 i;
    m_CALConfigSourcePath = QString(PlatformPaths::CreatePlatformComponentPath("CAL")).append("/src/CAL_cfg.c");
    m_CALConfigHeaderPath = QString(PlatformPaths::CreatePlatformComponentPath("CAL")).append("/inc/CAL_cfg.h");

    // Static ODO calibrations

    CalibrationParameter* odoStatus = new CalibrationParameter();
    CalibrationParameter* odoPulses = new CalibrationParameter();

    odoStatus->SetDescriptiveName(QString("ODO Status byte"));
    odoStatus->SetParameterName(QString("ODO_STATUS"));
    odoStatus->SetDescription(QString(""));
    odoStatus->SetDataType(QString("uint32"));
    odoStatus->SetArrayLength(1);

    m_ODOParameterList.append(odoStatus);

    for (i=0;i<numberOfODOstorageSlots;i++)
    {
        CalibrationParameter* param = new CalibrationParameter();
        QString calindex = QString::number(i+1);

        param->SetDescriptiveName(QString("ODO Storage " + calindex));
        param->SetParameterName(QString("ODO_VALUE" + calindex));
        param->SetDescription(QString(""));
        param->SetDataType(QString("uint32"));
        param->SetArrayLength(1);
        m_ODOParameterList.append(param);
    }

    odoPulses->SetDescriptiveName(QString("ODO Rest Pulse"));
    odoPulses->SetParameterName(QString("ODO_KM_REST_PULSES"));
    odoPulses->SetDescription(QString(""));
    odoPulses->SetDataType(QString("uint32"));
    odoPulses->SetArrayLength(1);
    m_ODOParameterList.append(odoPulses);

    m_FBCalStart = 0;
    m_u32TotalParameterLength = 0;
}

void CALGenerator::OnGenerateFiles()
{
    LOG_DEBUG("Entering");

    generateCALConfigSource();      //Must be run before headers to get lengths..
    generateCALConfigHeader();
}

void CALGenerator::generateCALConfigHeader()
{

    QString fileNameAsDefine("CAL_CFG_H");
    QStringList fileLines;
    fileLines.append("#ifndef " + fileNameAsDefine);
    fileLines.append("#define " + fileNameAsDefine);
    fileLines.append("");
    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"Platform_Types.h\"");
    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");
    fileLines.append("/* This enum is generated based on the ");
    fileLines.append("   calibration parameters defined in ");
    fileLines.append("   the function blocks and the BSP */");
    fileLines.append("typedef enum");
    fileLines.append("{");
    fileLines.append("    //Static ODO configuration");


    foreach (const CalibrationParameter* odoParam, m_ODOParameterList)
    {
        m_u32TotalParameterLength += getParameterByteLength(odoParam);
        fileLines.append("    " + odoParam->GetParameterName() + ",");
    }

    fileLines.append("    //Function block calibrations");

    int longestCalParameterLength = 4;
    //Status byte for application and BSP calibrations
    m_u32TotalParameterLength +=CAL_STATUS_STORAGE_SIZE;
    fileLines.append("    CAL_CHECKSUM,  //CALIBRATION CHECKSUM");
    foreach (FunctionBlock* fb, m_functionBlocks)
    {
        if (fb->GetFunctionDefinition().CalibrationParameters().count() == 0) continue;

        fileLines.append("    /* " + fb->GetComponentName() + " */");
        foreach (const CalibrationParameter* cal, fb->GetFunctionDefinition().CalibrationParameters())
        {
            //If the parameters is not part of the ODOParameterList add it to the enum.
            if ( !isODOParameter(cal->GetParameterName()) )
            {
                int calParameterLength = addCalibrationParameterEnum(fileLines, cal);
                m_u32TotalParameterLength += calParameterLength;
                if (calParameterLength > longestCalParameterLength)
                {
                    longestCalParameterLength = calParameterLength;
                }
            }
        }
    }
    fileLines.append("    /* BSP Calibration parameters */");
    foreach (const CalibrationParameter* cal, m_bspSpecification.CalibrationParameters())
    {
        int calParameterLength = addCalibrationParameterEnum(fileLines, cal);
        m_u32TotalParameterLength += calParameterLength;
        if (calParameterLength > longestCalParameterLength)
        {
            longestCalParameterLength = calParameterLength;
        }
    }

    fileLines.append("    CALIBRATIONPARAMETERCOUNT");
    fileLines.append("} calibration_parameter_id_t;");
    fileLines.append("");
    fileLines.append("typedef struct");
    fileLines.append("{");
    fileLines.append("    uint16 Offset;");
    fileLines.append("    uint8 Length;");
    fileLines.append("} calibration_parameter_t;");
    fileLines.append("");
    fileLines.append("#define CAL_TOTALPARAMETERLENGTH  0x" + QString::number(m_u32TotalParameterLength, 16) + "u  // " + QString::number(m_u32TotalParameterLength));
    fileLines.append("#define CAL_START_ADDR 0x" + QString("%1").arg(m_FBCalStart, 4, 16, QChar('0')) + "u  // " + QString::number(m_FBCalStart));
    fileLines.append("#define MAX_CAL_PARAMETER_LENGTH " + QString("0x%1u // %2").arg(longestCalParameterLength, 4, 16, QChar('0')).arg(longestCalParameterLength));
    fileLines.append("");
    fileLines.append("extern const uint32 g_u32CALCheckSum;");
    fileLines.append("extern const calibration_parameter_t CAL_CalibrationParameters[CALIBRATIONPARAMETERCOUNT];");
    fileLines.append("extern uint8 CAL_DefaultCalibrationParameters[CAL_TOTALPARAMETERLENGTH];");
    fileLines.append("");

    fileLines.append("#endif // " + fileNameAsDefine);
    fileLines.append("");

    AddGeneratedFile(m_CALConfigHeaderPath, fileLines.join("\n"));

}

void CALGenerator::generateCALConfigSource()
{
    QStringList fileLines;
    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"CAL_cfg.h\"");
    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");
    fileLines.append("const calibration_parameter_t CAL_CalibrationParameters[CALIBRATIONPARAMETERCOUNT] = ");
    fileLines.append("{");
    quint32 offset = 0;
    quint32 u32CheckSum = 0;

    foreach (const CalibrationParameter* odoParam, m_ODOParameterList)
    {
        quint8 length = getParameterByteLength(odoParam);

        fileLines.append("    { .Offset = 0x" + QString("%1").arg(offset, 4, 16, QChar('0')) + ", .Length = 0x" + QString("%1").arg(length, 2, 16, QChar('0')) + " }, // " + odoParam->GetParameterName());
        offset += length;

    }

    //Allocate space for CAL_CHECKSUM directly after all ODO parameters.
    fileLines.append("    { .Offset = 0x" + QString("%1").arg(offset, 4, 16, QChar('0')) + ", .Length = 0x" + QString("%1").arg(CAL_STATUS_STORAGE_SIZE, 2, 16, QChar('0')) + " }, // CAL_CHECKSUM");
    offset += CAL_STATUS_STORAGE_SIZE;
    //set the start of 'normal' calibration parameters
    m_FBCalStart = offset;

    foreach (FunctionBlock* fb, m_functionBlocks)
    {
        foreach (const CalibrationParameter* cal, fb->GetFunctionDefinition().CalibrationParameters())
        {
            if (!isODOParameter(cal->GetParameterName()))
            {
                quint8 length = getParameterByteLength(cal);

                fileLines.append("    { .Offset = 0x" + QString("%1").arg(offset, 4, 16, QChar('0')) + ", .Length = 0x" + QString("%1").arg(length, 2, 16, QChar('0')) + " }, // " + cal->GetParameterName());
                offset += length;
             }
        }
    }

    foreach (const CalibrationParameter* cal, m_bspSpecification.CalibrationParameters())
    {
        quint8 length = getParameterByteLength(cal);

        fileLines.append("    { .Offset = 0x" + QString("%1").arg(offset, 4, 16, QChar('0')) + ", .Length = 0x" + QString("%1").arg(length, 2, 16, QChar('0')) + " }, // " + cal->GetParameterName());
        offset += length;
    }

    fileLines.append("};");
    fileLines.append("");
    bool isLittleEndian = true;
    if (m_generateForSimulation == false)
    {
        qDebug("generating for target");
        if (m_targetSettings.GetTargetEndianess() == TargetSettings::BigEndian)
        {
            qDebug("  BigEndian!");
            isLittleEndian = false;
        }
        else
        {
            qDebug("  LittleEndian!");
            isLittleEndian = true;
        }
    }

    QString endianessString = (isLittleEndian ? "LittleEndian" : "BigEndian");
    fileLines.append("/* " + endianessString + " */");
    fileLines.append("uint8 CAL_DefaultCalibrationParameters[CAL_TOTALPARAMETERLENGTH] =");
    fileLines.append("{");

    foreach (const CalibrationParameter* odoParam, m_ODOParameterList)
    {
        QString byteString = convertToByteString("0", odoParam, isLittleEndian, &u32CheckSum);
        fileLines.append("    " + byteString + ", // " + odoParam->GetParameterName());
    }
    fileLines.append("    0x00, 0x00, 0x00, 0x00,  //CAL_CHECKSUM. Will be managed by CAL manager. Not include in default write.");
    foreach (FunctionBlock* fb, m_functionBlocks)
    {
        foreach (const CalibrationParameter* cal, fb->GetFunctionDefinition().CalibrationParameters())
        {
            if (!isODOParameter(cal->GetParameterName()))
            {
                QString defaultValue = fb->GetCalibrationParameterDefaultValue(cal->GetParameterName());

                QString byteString = convertToByteString(defaultValue, cal, isLittleEndian, &u32CheckSum);
                fileLines.append("    " + byteString + ", // " + cal->GetParameterName());
            }
        }
    }

    foreach (const CalibrationParameter* cal, m_bspSpecification.CalibrationParameters())
    {
        QString defaultValue = m_bspSpecification.GetCalibrationParameterDefaultValue(cal->GetParameterName());

        QString byteString = convertToByteString(defaultValue, cal, isLittleEndian, &u32CheckSum);
        fileLines.append("    " + byteString + ", // " + cal->GetParameterName());
    }
    fileLines.append("};");

    u32CheckSum += m_u32TotalParameterLength;
    fileLines.append("const uint32 g_u32CALCheckSum = "+ QString("0x%1").arg(u32CheckSum, 4, 16, QChar('0')) + ";  //CAL_CHECKSUM");
    fileLines.append("");

    AddGeneratedFile(m_CALConfigSourcePath, fileLines.join("\n"));
}

int CALGenerator::addCalibrationParameterEnum(QStringList& fileLines, const CalibrationParameter* cal)
{
    fileLines.append("    " + cal->GetParameterName() + ",");
    return getParameterByteLength(cal);
}

QString CALGenerator::convertToByteString(QString defaultValue, const CalibrationParameter* calParameter, bool isLittleEndian, quint32* checksum)
{
    QStringList bytes;
    QStringList values = defaultValue.split("|", QString::SkipEmptyParts);
    if(values.count() != calParameter->GetArrayLength())
    {
        LOG_ERROR("Default calibration parameter value count differs from expected array length!");
        return "INVALID (expected: " + QString::number(calParameter->GetArrayLength()) + " elements in array, actual: " + QString::number(values.count()) + ")";
    }

    foreach (QString value, values)
    {
        if (calParameter->GetDataType() == "uint32")
        {
            quint32 val = value.toUInt(0, 0);
            *checksum += val;
            bytes.append(toByteArray(val, isLittleEndian));
        }
        else if (calParameter->GetDataType() == "sint32")
        {
            qint32 val = value.toInt(0, 0);
            *checksum += val;
            bytes.append(toByteArray(val, isLittleEndian));
        }
        else if (calParameter->GetDataType() == "uint16")
        {
            quint16 val = value.toUShort(0, 0);
            *checksum += val;
            bytes.append(toByteArray(val, isLittleEndian));
        }
        else if (calParameter->GetDataType() == "sint16")
        {
            qint16 val = value.toShort(0, 0);
            *checksum += val;
            bytes.append(toByteArray(val, isLittleEndian));
        }
        else if (calParameter->GetDataType() == "uint8")
        {
            quint8 val = (quint8)value.toUShort(0, 0);
            *checksum += val;
            bytes.append(QString("0x%1").arg(val, 2, 16, QChar('0')));
        }
        else if (calParameter->GetDataType() == "sint8")
        {
            qint8 val = (qint8)value.toShort(0, 0);
            *checksum += val;
            bytes.append(QString("0x%1").arg(val, 2, 16, QChar('0')));
        }
        else
        {
            return "UNKNOWN CALIBRATION DATATYPE: " + calParameter->GetDataType();
        }
    }

    return bytes.join(", ");
}

QStringList CALGenerator::toByteArray(quint32 value, bool isLittleEndian)
{
    QStringList result;
    quint8 bytes[4];
    bytes[0] = (quint8)(value & 0xFF);
    bytes[1] = (quint8)((value & 0xFF00) >> 8);
    bytes[2] = (quint8)((value & 0xFF0000) >> 16);
    bytes[3] = (quint8)((value & 0xFF000000) >> 24);

    for (int i=0; i<4; ++i)
    {
        if (isLittleEndian)
        {
            result.append(QString("0x%1").arg(bytes[i], 2, 16, QChar('0')));
        }
        else
        {
            result.append(QString("0x%1").arg(bytes[3-i], 2, 16, QChar('0')));
        }
    }

    return result;
}

QStringList CALGenerator::toByteArray(qint32 value, bool isLittleEndian)
{
    QStringList result;
    quint8 bytes[4];
    bytes[0] = (quint8)(value & 0xFF);
    bytes[1] = (quint8)((value & 0xFF00) >> 8);
    bytes[2] = (quint8)((value & 0xFF0000) >> 16);
    bytes[3] = (quint8)((value & 0xFF000000) >> 24);

    for (int i=0; i<4; ++i)
    {
        if (isLittleEndian)
        {
            result.append(QString("0x%1").arg(bytes[i], 2, 16, QChar('0')));
        }
        else
        {
            result.append(QString("0x%1").arg(bytes[3-i], 2, 16, QChar('0')));
        }
    }

    return result;
}

QStringList CALGenerator::toByteArray(quint16 value, bool isLittleEndian)
{
    QStringList result;
    quint8 bytes[2];
    bytes[0] = (quint8)(value & 0xFF);
    bytes[1] = (quint8)((value & 0xFF00) >> 8);

    for (int i=0; i<2; ++i)
    {
        if (isLittleEndian)
        {
            result.append(QString("0x%1").arg(bytes[i], 2, 16, QChar('0')));
        }
        else
        {
            result.append(QString("0x%1").arg(bytes[1-i], 2, 16, QChar('0')));
        }
    }

    return result;
}

QStringList CALGenerator::toByteArray(qint16 value, bool isLittleEndian)
{
    QStringList result;
    quint8 bytes[2];
    bytes[0] = (quint8)(value & 0xFF);
    bytes[1] = (quint8)((value & 0xFF00) >> 8);

    for (int i=0; i<2; ++i)
    {
        if (isLittleEndian)
        {
            result.append(QString("0x%1").arg(bytes[i], 2, 16, QChar('0')));
        }
        else
        {
            result.append(QString("0x%1").arg(bytes[1-i], 2, 16, QChar('0')));
        }
    }

    return result;
}

//Checks if the parameter is defined as a ODO parameter.
bool CALGenerator::isODOParameter(QString ParameterName)
{
    foreach (const CalibrationParameter* odoParam, m_ODOParameterList)
    {
        if ( 0 == odoParam->GetParameterName().compare(ParameterName,Qt::CaseInsensitive) )
        {
           return true;
        }
    }
    return false;
}

int CALGenerator::getParameterByteLength(const CalibrationParameter* calParam)
{
    quint8 length = calParam->GetArrayLength();
    if (calParam->GetDataType().endsWith("8"))
    {
        return length;
    }
    else if (calParam->GetDataType().endsWith("16"))
    {
        return length * 2;
    }
    else if (calParam->GetDataType().endsWith("32"))
    {
        return length * 4;
    }
    else
    {
        LOG_ERROR("Unknown data type " + calParam->GetDataType() + "for calibration parameter " + calParam->GetParameterName() + ". Only use uint8, sint8, uint16, sint16, uint32 or sint32!");
        return 0;
    }
}

