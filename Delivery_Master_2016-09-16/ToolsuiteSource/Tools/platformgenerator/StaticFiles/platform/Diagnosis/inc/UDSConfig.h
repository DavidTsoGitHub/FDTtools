/*****************************************************************/
/*                                                               */
/* FILE        : UDSConfig.h                                     */
/* DATE        : 2015-10-09                                      */
/* AUTHOR      : Cao Wei                                         */
/* DESCRIPTION : configurations  of the UDS resolution.          */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*               Last Modified By Cao Wei on 2015-10-09          */
/*                                                               */
/*****************************************************************/
#ifndef UDS_CONFIG_H
#define UDS_CONFIG_H


/* Diagnostic and communication management functional unit */
#define DIAGNOSTIC_SESSION_CONTROL_ENABLE       1   /* DiagnosticSessionControl(0x10) serive enable or not */
#define ECU_RESET_ENABLE                        1   /* ECUReset(0x11) service enabled or not */
#define SECURITY_ACCESS_ENABLE                  1   /* SecurityAccess(0x27) service enabled or not */
#define COMMUNICATION_CONTROL_ENABLE            0   /* CommunicationControl(0x28) service enable or not */
#define TESTER_PRESENT_ENABLE                   1   /* TesterPresent(0x3E) service enable or not */
#define ACCESS_TIMING_PARAMETER_ENABLE          0   /* AccessTimingParameter(0x83) service enable or not */
#define SECURED_DATA_TRANSMISSION_ENABLE        0   /* SecuredDataTransmission(0x84) service enable or not */
#define CONTROL_DTC_SETTING_ENABLE              0   /* ControlDTCSetting (85 hex) service enable or not */
#define RESPONSE_ON_EVENT_ENABLE                0   /* ResponseOnEvent (86 hex) service enable or not */
#define LINK_CONTROL_ENABLE                     0   /* LinkControl (87 hex) service enable or not */

/* Data transmission functional unit */
#define READ_DATA_BY_ID_ENABLE                  1   /* ReadDataByIdentifier (22 hex) service enable or not */
#define READ_MEMORY_BY_ADDRESS_ENABLE           0   /* ReadMemoryByAddress (23 hex) service enable or not */
#define READ_SCALING_DATA_BY_IDR_ENABLE         0   /* ReadScalingDataByIdentifier (24 hex) service enable or not */
#define READ_DATA_BY_PERIODIC_ID_ENABLE         0   /* ReadDataByPeriodicIdentifier (2A hex) service enable or not */
#define DYNAMICALLY_DEFINE_DATA_ID_ENABLE       0   /* DynamicallyDefineDataIdentifier (2C hex) service enable or not */
#define WRITE_DATA_BY_ID_ENABLE                 1   /* WriteDataByIdentifier (2E hex) service enable or not */
#define WRITE_MEMORY_BY_ADDRESS_ENABLE          0   /* WriteMemoryByAddress (3D hex) service enable or not */

/* Stored data transmission functional unit */
#define CLEAR_DIAGNOSTIC_INFO_ENABLE            1   /* ClearDiagnosticInformation (14 hex) service enable or not */
#define READ_DTC_INFO_ENABLE                    1   /* ReadDTCInformation (19 hex) service enable or not */

/* InputOutput control functional unit */
#define INPUT_OUTPUT_CONTROL_BY_ID_ENABLE       0   /* InputOutputControlByIdentifier (2F hex) service enable or not */

/* Remote activation of routine functional unit */
#define ROUTINE_CONTROL_ENABLE                  0   /* RoutineControl (31 hex) service enable or not */

/* Upload download functional unit */
#define REQUEST_DOWNLOAD_ENABLE                 0  /* RequestDownload (34 hex) service enable or not */
#define REQUEST_UPLOAD_ENABLE                   0  /* RequestUpload (35 hex) service enable or not */
#define TRANSFER_DATA_ENABLE                    0  /* TransferData (36 hex) service enable or not */
#define REQUEST_TRANSFER_EXIT_ENABLE            0  /* RequestTransferExit (37 hex) service enable or not */

#endif
