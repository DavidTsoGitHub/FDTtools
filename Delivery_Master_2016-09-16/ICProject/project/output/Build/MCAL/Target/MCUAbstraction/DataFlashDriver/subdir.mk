################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/MCAL/Target/MCUAbstraction/DataFlashDriver/eed_highlevel.c" \
"../Source/MCAL/Target/MCUAbstraction/DataFlashDriver/eed_lowlevel.c" \
"../Source/MCAL/Target/MCUAbstraction/DataFlashDriver/eed_middlelevel.c" \
"../Source/MCAL/Target/MCUAbstraction/DataFlashDriver/ee_C90_utils.c" \

C_SRCS += \
../Source/MCAL/Target/MCUAbstraction/DataFlashDriver/eed_highlevel.c \
../Source/MCAL/Target/MCUAbstraction/DataFlashDriver/eed_lowlevel.c \
../Source/MCAL/Target/MCUAbstraction/DataFlashDriver/eed_middlelevel.c \
../Source/MCAL/Target/MCUAbstraction/DataFlashDriver/ee_C90_utils.c \

S_SRCS += \

OBJS += \
MCAL/Target/MCUAbstraction/DataFlashDriver/eed_highlevel.obj \
MCAL/Target/MCUAbstraction/DataFlashDriver/eed_lowlevel.obj \
MCAL/Target/MCUAbstraction/DataFlashDriver/eed_middlelevel.obj \
MCAL/Target/MCUAbstraction/DataFlashDriver/ee_C90_utils.obj \

OBJS_QUOTED += \
"MCAL/Target/MCUAbstraction/DataFlashDriver/eed_highlevel.obj" \
"MCAL/Target/MCUAbstraction/DataFlashDriver/eed_lowlevel.obj" \
"MCAL/Target/MCUAbstraction/DataFlashDriver/eed_middlelevel.obj" \
"MCAL/Target/MCUAbstraction/DataFlashDriver/ee_C90_utils.obj" \

C_DEPS += \
MCAL/Target/MCUAbstraction/DataFlashDriver/eed_highlevel.d \
MCAL/Target/MCUAbstraction/DataFlashDriver/eed_lowlevel.d \
MCAL/Target/MCUAbstraction/DataFlashDriver/eed_middlelevel.d \
MCAL/Target/MCUAbstraction/DataFlashDriver/ee_C90_utils.d \

S_DEPS += \

C_DEPS_QUOTED += \
"MCAL/Target/MCUAbstraction/DataFlashDriver/eed_highlevel.d" \
"MCAL/Target/MCUAbstraction/DataFlashDriver/eed_lowlevel.d" \
"MCAL/Target/MCUAbstraction/DataFlashDriver/eed_middlelevel.d" \
"MCAL/Target/MCUAbstraction/DataFlashDriver/ee_C90_utils.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
MCAL/Target/MCUAbstraction/DataFlashDriver/eed_highlevel.obj: ../Source/MCAL/Target/MCUAbstraction/DataFlashDriver/eed_highlevel.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/MCUAbstraction/DataFlashDriver/eed_highlevel.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/MCUAbstraction/DataFlashDriver/eed_lowlevel.obj: ../Source/MCAL/Target/MCUAbstraction/DataFlashDriver/eed_lowlevel.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/MCUAbstraction/DataFlashDriver/eed_lowlevel.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/MCUAbstraction/DataFlashDriver/eed_middlelevel.obj: ../Source/MCAL/Target/MCUAbstraction/DataFlashDriver/eed_middlelevel.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/MCUAbstraction/DataFlashDriver/eed_middlelevel.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/MCUAbstraction/DataFlashDriver/ee_C90_utils.obj: ../Source/MCAL/Target/MCUAbstraction/DataFlashDriver/ee_C90_utils.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/MCUAbstraction/DataFlashDriver/ee_C90_utils.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/MCUAbstraction/DataFlashDriver/%.d: ../Source/MCAL/Target/MCUAbstraction/DataFlashDriver/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
