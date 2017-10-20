#ifndef GPT_H_
#define GPT_H_

/*=======[I N C L U D E S]====================================================*/

#include "Std_Types.h"

/*=======[Global Type Definitions]============================================*/

typedef uint8 Gpt_ChannelType;
typedef uint8 Gpt_ModeType;
typedef uint8 Gpt_ClockType;
typedef uint8 Gpt_GtmGlobalChannelNoType;

typedef struct Gpt_ChannelConfig
{
  /* The AssignedHwUnit is packed data containing TOM/ATOM channel used */
  Gpt_GtmGlobalChannelNoType AssignedHwUnit;

  /* Channel mode - GPT_MODE_CONTINUOUS/GPT_MODE_ONESHOT - GPT185:, GPT186: */
  Gpt_ModeType GptChannelMode;

  Gpt_ClockType ClockSelect;  /*Channel Clock Select */

} Gpt_ChannelConfigType;

typedef struct Gpt_ConfigType
{
  /* Pointer to set of channel specific configuration */
  const Gpt_ChannelConfigType* ChannelConfigPtr;

  /* Maximum number of channels in the given set of Configuration */
  Gpt_ChannelType GptMaxChannels;

} Gpt_ConfigType;


/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

void Gpt_Init(const Gpt_ConfigType *ConfigPtr);


void Gpt_Deinit(void);


boolean Gpt_GetFlag(void);

#endif


