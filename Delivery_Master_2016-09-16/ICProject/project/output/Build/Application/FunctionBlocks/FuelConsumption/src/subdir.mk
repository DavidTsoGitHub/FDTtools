################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/FuelConsumption/src/FuelConsumption.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/FuelConsumption/src/FuelConsumption.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/FuelConsumption/src/FuelConsumption.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/FuelConsumption/src/FuelConsumption.obj" \

C_DEPS += \
Application/FunctionBlocks/FuelConsumption/src/FuelConsumption.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/FuelConsumption/src/FuelConsumption.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/FuelConsumption/src/FuelConsumption.obj: ../Source/Application/FunctionBlocks/FuelConsumption/src/FuelConsumption.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/FuelConsumption/src/FuelConsumption.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/FuelConsumption/src/%.d: ../Source/Application/FunctionBlocks/FuelConsumption/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
