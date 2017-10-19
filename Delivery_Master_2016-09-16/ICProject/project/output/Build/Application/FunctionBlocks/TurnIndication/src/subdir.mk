################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/TurnIndication/src/TurnIndication.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/TurnIndication/src/TurnIndication.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/TurnIndication/src/TurnIndication.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/TurnIndication/src/TurnIndication.obj" \

C_DEPS += \
Application/FunctionBlocks/TurnIndication/src/TurnIndication.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/TurnIndication/src/TurnIndication.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/TurnIndication/src/TurnIndication.obj: ../Source/Application/FunctionBlocks/TurnIndication/src/TurnIndication.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/TurnIndication/src/TurnIndication.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/TurnIndication/src/%.d: ../Source/Application/FunctionBlocks/TurnIndication/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
