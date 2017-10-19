################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/ParkBrakeIndicator/src/ParkBrakeIndicator.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/ParkBrakeIndicator/src/ParkBrakeIndicator.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/ParkBrakeIndicator/src/ParkBrakeIndicator.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/ParkBrakeIndicator/src/ParkBrakeIndicator.obj" \

C_DEPS += \
Application/FunctionBlocks/ParkBrakeIndicator/src/ParkBrakeIndicator.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/ParkBrakeIndicator/src/ParkBrakeIndicator.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/ParkBrakeIndicator/src/ParkBrakeIndicator.obj: ../Source/Application/FunctionBlocks/ParkBrakeIndicator/src/ParkBrakeIndicator.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/ParkBrakeIndicator/src/ParkBrakeIndicator.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/ParkBrakeIndicator/src/%.d: ../Source/Application/FunctionBlocks/ParkBrakeIndicator/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
