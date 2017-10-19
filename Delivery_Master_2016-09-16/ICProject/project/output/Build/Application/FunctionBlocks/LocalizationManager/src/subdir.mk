################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/LocalizationManager/src/LocalizationManager.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/LocalizationManager/src/LocalizationManager.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/LocalizationManager/src/LocalizationManager.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/LocalizationManager/src/LocalizationManager.obj" \

C_DEPS += \
Application/FunctionBlocks/LocalizationManager/src/LocalizationManager.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/LocalizationManager/src/LocalizationManager.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/LocalizationManager/src/LocalizationManager.obj: ../Source/Application/FunctionBlocks/LocalizationManager/src/LocalizationManager.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/LocalizationManager/src/LocalizationManager.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/LocalizationManager/src/%.d: ../Source/Application/FunctionBlocks/LocalizationManager/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
