################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/VehSpdGauge/src/VehSpdGauge.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/VehSpdGauge/src/VehSpdGauge.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/VehSpdGauge/src/VehSpdGauge.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/VehSpdGauge/src/VehSpdGauge.obj" \

C_DEPS += \
Application/FunctionBlocks/VehSpdGauge/src/VehSpdGauge.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/VehSpdGauge/src/VehSpdGauge.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/VehSpdGauge/src/VehSpdGauge.obj: ../Source/Application/FunctionBlocks/VehSpdGauge/src/VehSpdGauge.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/VehSpdGauge/src/VehSpdGauge.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/VehSpdGauge/src/%.d: ../Source/Application/FunctionBlocks/VehSpdGauge/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
