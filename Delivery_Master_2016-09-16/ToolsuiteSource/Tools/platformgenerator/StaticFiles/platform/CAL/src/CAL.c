/********************************* Includes **********************************/
#include "CAL.h"
#include "BSP.h"
#include "CAL_cfg.h"
#include "Logger.h"

/**************************** Constants and Types ****************************/
static CAL_STATUS u8CalMemoryInitStatus = CAL_NOK;

/********************************* Variables *********************************/
static uint8 CALMemoryCache[(CAL_TOTALPARAMETERLENGTH - CAL_START_ADDR)];

/************************* Local Function Prototypes *************************/
static void PopulateCacheFromDefaultValues(void);
static CAL_STATUS PopulateCacheFromPersistentStorage(void);
static CAL_STATUS WriteDefaultValuesToPersistentStorage(void);
static void ReadValuesFromCache(uint8* value, uint16 offset, uint16 length);
static void WriteValuesToCache(const uint8* value, uint16 offset, uint16 length);

/***************************** Exported Functions ****************************/
void CAL_Init(void)
{
    Logger_Info("CAL", "CAL_Init");

    uint32 u32storedChecksum;
    BSP_StdReturnType ret;
    const calibration_parameter_t* calibrationParameter = &CAL_CalibrationParameters[CAL_CHECKSUM];
    ret = BSP_Read_Memory((uint8*)&u32storedChecksum, calibrationParameter->Length, calibrationParameter->Offset);

    if (ret == BSP_INVALID_RANGE) //Check if BSP memory has been initialized
    {
        if (WriteDefaultValuesToPersistentStorage() == CAL_OK)
        {
            PopulateCacheFromDefaultValues();
            u8CalMemoryInitStatus = CAL_OK;
            Logger_Info("CAL", "CAL initialized");
        }
    }
    else
    {
        if (ret == BSP_OK)
        {
            if (u32storedChecksum != g_u32CALCheckSum)
            {
                Logger_Info("CAL", "CAL persistent storage not initialized, initializing with default values");
                if (WriteDefaultValuesToPersistentStorage() == CAL_OK)
                {
                    PopulateCacheFromDefaultValues();
                    u8CalMemoryInitStatus = CAL_OK;
                    Logger_Info("CAL", "CAL initialized");
                }
                else
                {
                    //Faild to write default values from NVRAM.Deny read access.
                    u8CalMemoryInitStatus = CAL_NOK;
                    Logger_Info("CAL", "CAL FAILED to initialize");
                }
            }
            else
            {
                Logger_Info("CAL", "CAL persistent storage was initialized");
                CAL_Refresh();

            }
        }
    }
}

void CAL_Refresh(void)
{
    Logger_Info("CAL", "%s", "CAL_Refresh");
    if (PopulateCacheFromPersistentStorage() != CAL_OK)
    {
        //Failed to fetch data from NVRAM
        u8CalMemoryInitStatus = CAL_NOK;
        Logger_Info("CAL", "CAL FAILED to initialize");
    }
    else
    {
        u8CalMemoryInitStatus = CAL_OK;
    }
}

CAL_STATUS CAL_Read8(calibration_parameter_id_t u16ParameterId, uint8* value)
{
    CAL_STATUS returnValue = CAL_NOK;
    if (u8CalMemoryInitStatus == CAL_OK)
    {
        const calibration_parameter_t* calibrationParameter = &CAL_CalibrationParameters[u16ParameterId];

        ReadValuesFromCache(value, (calibrationParameter->Offset - CAL_START_ADDR), calibrationParameter->Length);
        returnValue = CAL_OK;
     }

    return returnValue;
}

CAL_STATUS CAL_Read16(calibration_parameter_id_t u16ParameterId, uint16* value)
{
    CAL_STATUS returnValue = CAL_NOK;
    if (u8CalMemoryInitStatus == CAL_OK)
    {
        const calibration_parameter_t* calibrationParameter = &CAL_CalibrationParameters[u16ParameterId];

        ReadValuesFromCache((uint8*)value, (calibrationParameter->Offset - CAL_START_ADDR), calibrationParameter->Length);
        returnValue = CAL_OK;
    }

    return returnValue;
}

CAL_STATUS CAL_Read32(calibration_parameter_id_t u16ParameterId, uint32* value)
{
    CAL_STATUS returnValue = CAL_NOK;

    if (u8CalMemoryInitStatus == CAL_OK)
    {
        const calibration_parameter_t* calibrationParameter = &CAL_CalibrationParameters[u16ParameterId];

        ReadValuesFromCache((uint8*)value, (calibrationParameter->Offset - CAL_START_ADDR), calibrationParameter->Length);
        returnValue = CAL_OK;
    }

    return returnValue;
}

CAL_STATUS CAL_Write(calibration_parameter_id_t calID, const uint8* pData, uint16 u16ExpectedLength)
{
    CAL_STATUS returnValue = CAL_OK;
    if (u8CalMemoryInitStatus == CAL_OK)
    {
        const calibration_parameter_t* calibrationParameter = &CAL_CalibrationParameters[calID];

        if (u16ExpectedLength != calibrationParameter->Length)
        {
            returnValue = CAL_NOK;
        }
        else
        {
            // Since BSP_Write_Memory expects a non-const data pointer we have to copy the data to a non-const array before passing it on
            uint8 dataCopy[MAX_CAL_PARAMETER_LENGTH] = { 0 };
            for (uint16 i = 0; i<u16ExpectedLength; ++i)
            {
                dataCopy[i] = pData[i];
            }

            WriteValuesToCache(dataCopy, calibrationParameter->Offset - CAL_START_ADDR, calibrationParameter->Length);
            if (BSP_Write_Memory(dataCopy, calibrationParameter->Length,  calibrationParameter->Offset) == BSP_OK )
            {
                returnValue = CAL_OK;
            }
            else
            {
                returnValue = CAL_NOK;
            }
        }
    }

    return returnValue;
}

CAL_STATUS CAL_Read(calibration_parameter_id_t calID, uint8* pData, uint16 u16ExpectedLength)
{
    CAL_STATUS returnValue = CAL_NOK;

    if (u8CalMemoryInitStatus == CAL_OK)
    {
        const calibration_parameter_t* calibrationParameter = &CAL_CalibrationParameters[calID];

        if (u16ExpectedLength != calibrationParameter->Length)
        {
            returnValue =  CAL_NOK;
        }
        else
        {
            ReadValuesFromCache(pData, calibrationParameter->Offset - CAL_START_ADDR, calibrationParameter->Length);
            returnValue =  CAL_OK;
        }
    }

    return returnValue;
}

//ODO is not stored in RAM memory expect for the ODO FB. We directly read/write to EEPROM
CAL_STATUS CAL_NoCacheRead32(calibration_parameter_id_t u16ParameterId, uint32* value)
{
    CAL_STATUS ret = CAL_NOK;
    const calibration_parameter_t* calibrationParameter = &CAL_CalibrationParameters[u16ParameterId];
    Logger_Info("CAL", "Reading ODO parameter from persistent storage");
    if (BSP_Read_Memory((uint8*)value,calibrationParameter->Length ,calibrationParameter->Offset) == BSP_OK)
    {
        ret =  CAL_OK;
    }
    return ret;
}

CAL_STATUS CAL_NoCacheWrite32(calibration_parameter_id_t u16ParameterId, const uint32* value)
{
    CAL_STATUS ret = CAL_NOK;
    uint32 valueToWrite = *value; // To please MISRA,
    const calibration_parameter_t* calibrationParameter = &CAL_CalibrationParameters[u16ParameterId];
    Logger_Info("CAL", "Writing ODO values to persistent storage");

    if ( BSP_Write_Memory((uint8*)&valueToWrite,calibrationParameter->Length,calibrationParameter->Offset) == BSP_OK )
    {
        ret = CAL_OK;
    }
    return ret;
}

/****************************** Local Functions ******************************/
static CAL_STATUS WriteDefaultValuesToPersistentStorage(void)
{
    CAL_STATUS ret = CAL_NOK;
    uint32 calCheckSum = g_u32CALCheckSum;
    uint16 index;
    uint16 writePos;

    Logger_Info("CAL", "Writing default CAL values to persistent storage");

    //start from cal start address
    writePos = CAL_START_ADDR;

    Logger_Info("CAL", "CALIBRATIONPARAMETERCOUNT = %d", CALIBRATIONPARAMETERCOUNT);
    Logger_Info("CAL", "CAL_START_ADDR = %d", CAL_START_ADDR);

    if ( BSP_Write_Memory((uint8*)&calCheckSum, CAL_CalibrationParameters[CAL_CHECKSUM].Length,CAL_CalibrationParameters[CAL_CHECKSUM].Offset) == BSP_OK )
    {
        ret = CAL_OK;
    }
    else
    {
        ret = CAL_NOK;
    }

    if(ret == CAL_OK)
    {
        for (index = (uint16)CAL_CHECKSUM + (uint16)1U; index < (uint16)CALIBRATIONPARAMETERCOUNT; index++)
        {

            if ( BSP_Write_Memory((uint8*)&CAL_DefaultCalibrationParameters[writePos],CAL_CalibrationParameters[index].Length,CAL_CalibrationParameters[index].Offset) == BSP_OK )
            {
                ret = CAL_OK;
            }
            else
            {
                Logger_Error("CAL", "Can't write persistent storage");
                ret = CAL_NOK;
                break;
            }
            writePos += CAL_CalibrationParameters[index].Length;
        }
    }

    return ret;
}

static void PopulateCacheFromDefaultValues(void)
{
    Logger_Info("CAL", "Storing default CAL values in RAM cache");
    uint16 index;

    //start from cal start address
    for (index = CAL_START_ADDR; index < CAL_TOTALPARAMETERLENGTH; ++index)
    {
        //Create array without ODO values
        CALMemoryCache[index-CAL_START_ADDR] = CAL_DefaultCalibrationParameters[index];
    }
}

static CAL_STATUS PopulateCacheFromPersistentStorage(void)
{
    sint32 calParameterIndex;
    CAL_STATUS ret = CAL_NOK;

    Logger_Info("CAL", "Populating cache from persistent storage");

    const calibration_parameter_t* calParam;
    for (calParameterIndex = ((sint32)CAL_CHECKSUM + 1); calParameterIndex < (sint32)CALIBRATIONPARAMETERCOUNT; ++calParameterIndex)
    {
        calParam = &CAL_CalibrationParameters[calParameterIndex];

        if (BSP_Read_Memory((uint8*)&CALMemoryCache[calParam->Offset - CAL_START_ADDR], calParam->Length, calParam->Offset) == BSP_OK)
        {
            ret = CAL_OK;
        }
        else
        {
            ret = CAL_NOK;
            Logger_Error("CAL", "Error reading CAL parameter from persistent storage! Index:%d Offset:%d Length:%d", calParameterIndex, calParam->Offset, calParam->Length);
            break;
        }
    }

    return ret;
}

static void ReadValuesFromCache(uint8* value, uint16 offset, uint16 length)
{
    uint8 index;
    for (index = 0; index < length; ++index)
    {
        value[index] = CALMemoryCache[offset+index];
    }
}

static void WriteValuesToCache(const uint8* value, uint16 offset, uint16 length)
{
    for (uint8 index = 0; index < length; ++index)
    {
        CALMemoryCache[offset+index] = value[index];
    }
}
