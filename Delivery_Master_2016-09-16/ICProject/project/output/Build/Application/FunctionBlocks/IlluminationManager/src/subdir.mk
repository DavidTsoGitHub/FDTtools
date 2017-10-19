################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/IlluminationManager/src/IlluminationManager.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/IlluminationManager/src/IlluminationManager.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/IlluminationManager/src/IlluminationManager.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/IlluminationManager/src/IlluminationManager.obj" \

C_DEPS += \
Application/FunctionBlocks/IlluminationManager/src/IlluminationManager.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/IlluminationManager/src/IlluminationManager.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/IlluminationManager/src/IlluminationManager.obj: ../Source/Application/FunctionBlocks/IlluminationManager/src/IlluminationManager.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/IlluminationManager/src/IlluminationManager.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/IlluminationManager/src/%.d: ../Source/Application/FunctionBlocks/IlluminationManager/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
