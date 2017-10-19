################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/WarningIconFields/src/WarningIconFields.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/WarningIconFields/src/WarningIconFields.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/WarningIconFields/src/WarningIconFields.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/WarningIconFields/src/WarningIconFields.obj" \

C_DEPS += \
Application/FunctionBlocks/WarningIconFields/src/WarningIconFields.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/WarningIconFields/src/WarningIconFields.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/WarningIconFields/src/WarningIconFields.obj: ../Source/Application/FunctionBlocks/WarningIconFields/src/WarningIconFields.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/WarningIconFields/src/WarningIconFields.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/WarningIconFields/src/%.d: ../Source/Application/FunctionBlocks/WarningIconFields/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
