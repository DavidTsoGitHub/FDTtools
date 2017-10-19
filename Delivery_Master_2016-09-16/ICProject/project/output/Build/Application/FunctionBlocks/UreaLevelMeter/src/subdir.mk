################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/UreaLevelMeter/src/UreaLevelMeter.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/UreaLevelMeter/src/UreaLevelMeter.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/UreaLevelMeter/src/UreaLevelMeter.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/UreaLevelMeter/src/UreaLevelMeter.obj" \

C_DEPS += \
Application/FunctionBlocks/UreaLevelMeter/src/UreaLevelMeter.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/UreaLevelMeter/src/UreaLevelMeter.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/UreaLevelMeter/src/UreaLevelMeter.obj: ../Source/Application/FunctionBlocks/UreaLevelMeter/src/UreaLevelMeter.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/UreaLevelMeter/src/UreaLevelMeter.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/UreaLevelMeter/src/%.d: ../Source/Application/FunctionBlocks/UreaLevelMeter/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
