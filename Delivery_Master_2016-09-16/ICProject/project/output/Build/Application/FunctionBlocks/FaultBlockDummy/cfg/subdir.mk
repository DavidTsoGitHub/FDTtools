################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/FaultBlockDummy/cfg/FaultBlockDummy_cfg.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/FaultBlockDummy/cfg/FaultBlockDummy_cfg.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/FaultBlockDummy/cfg/FaultBlockDummy_cfg.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/FaultBlockDummy/cfg/FaultBlockDummy_cfg.obj" \

C_DEPS += \
Application/FunctionBlocks/FaultBlockDummy/cfg/FaultBlockDummy_cfg.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/FaultBlockDummy/cfg/FaultBlockDummy_cfg.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/FaultBlockDummy/cfg/FaultBlockDummy_cfg.obj: ../Source/Application/FunctionBlocks/FaultBlockDummy/cfg/FaultBlockDummy_cfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/FaultBlockDummy/cfg/FaultBlockDummy_cfg.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/FaultBlockDummy/cfg/%.d: ../Source/Application/FunctionBlocks/FaultBlockDummy/cfg/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
