################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/EngineOilPressureTellTale/src/EngineOilPressureTellTale.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/EngineOilPressureTellTale/src/EngineOilPressureTellTale.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/EngineOilPressureTellTale/src/EngineOilPressureTellTale.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/EngineOilPressureTellTale/src/EngineOilPressureTellTale.obj" \

C_DEPS += \
Application/FunctionBlocks/EngineOilPressureTellTale/src/EngineOilPressureTellTale.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/EngineOilPressureTellTale/src/EngineOilPressureTellTale.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/EngineOilPressureTellTale/src/EngineOilPressureTellTale.obj: ../Source/Application/FunctionBlocks/EngineOilPressureTellTale/src/EngineOilPressureTellTale.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/EngineOilPressureTellTale/src/EngineOilPressureTellTale.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/EngineOilPressureTellTale/src/%.d: ../Source/Application/FunctionBlocks/EngineOilPressureTellTale/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
