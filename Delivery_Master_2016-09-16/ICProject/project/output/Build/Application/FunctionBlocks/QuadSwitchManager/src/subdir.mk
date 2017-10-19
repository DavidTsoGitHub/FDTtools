################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/QuadSwitchManager/src/QuadSwitchManager.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/QuadSwitchManager/src/QuadSwitchManager.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/QuadSwitchManager/src/QuadSwitchManager.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/QuadSwitchManager/src/QuadSwitchManager.obj" \

C_DEPS += \
Application/FunctionBlocks/QuadSwitchManager/src/QuadSwitchManager.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/QuadSwitchManager/src/QuadSwitchManager.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/QuadSwitchManager/src/QuadSwitchManager.obj: ../Source/Application/FunctionBlocks/QuadSwitchManager/src/QuadSwitchManager.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/QuadSwitchManager/src/QuadSwitchManager.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/QuadSwitchManager/src/%.d: ../Source/Application/FunctionBlocks/QuadSwitchManager/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
