################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/OutputShaftRotationSpeed/src/OutputShaftRotationSpeed.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/OutputShaftRotationSpeed/src/OutputShaftRotationSpeed.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/OutputShaftRotationSpeed/src/OutputShaftRotationSpeed.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/OutputShaftRotationSpeed/src/OutputShaftRotationSpeed.obj" \

C_DEPS += \
Application/FunctionBlocks/OutputShaftRotationSpeed/src/OutputShaftRotationSpeed.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/OutputShaftRotationSpeed/src/OutputShaftRotationSpeed.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/OutputShaftRotationSpeed/src/OutputShaftRotationSpeed.obj: ../Source/Application/FunctionBlocks/OutputShaftRotationSpeed/src/OutputShaftRotationSpeed.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/OutputShaftRotationSpeed/src/OutputShaftRotationSpeed.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/OutputShaftRotationSpeed/src/%.d: ../Source/Application/FunctionBlocks/OutputShaftRotationSpeed/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
