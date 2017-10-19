################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/ChargingTellTale/src/ChargingTellTale.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/ChargingTellTale/src/ChargingTellTale.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/ChargingTellTale/src/ChargingTellTale.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/ChargingTellTale/src/ChargingTellTale.obj" \

C_DEPS += \
Application/FunctionBlocks/ChargingTellTale/src/ChargingTellTale.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/ChargingTellTale/src/ChargingTellTale.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/ChargingTellTale/src/ChargingTellTale.obj: ../Source/Application/FunctionBlocks/ChargingTellTale/src/ChargingTellTale.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/ChargingTellTale/src/ChargingTellTale.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/ChargingTellTale/src/%.d: ../Source/Application/FunctionBlocks/ChargingTellTale/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
