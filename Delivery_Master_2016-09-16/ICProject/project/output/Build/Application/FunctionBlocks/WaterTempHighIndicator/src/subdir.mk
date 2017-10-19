################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/WaterTempHighIndicator/src/WaterTempHighIndicator.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/WaterTempHighIndicator/src/WaterTempHighIndicator.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/WaterTempHighIndicator/src/WaterTempHighIndicator.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/WaterTempHighIndicator/src/WaterTempHighIndicator.obj" \

C_DEPS += \
Application/FunctionBlocks/WaterTempHighIndicator/src/WaterTempHighIndicator.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/WaterTempHighIndicator/src/WaterTempHighIndicator.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/WaterTempHighIndicator/src/WaterTempHighIndicator.obj: ../Source/Application/FunctionBlocks/WaterTempHighIndicator/src/WaterTempHighIndicator.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/WaterTempHighIndicator/src/WaterTempHighIndicator.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/WaterTempHighIndicator/src/%.d: ../Source/Application/FunctionBlocks/WaterTempHighIndicator/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
