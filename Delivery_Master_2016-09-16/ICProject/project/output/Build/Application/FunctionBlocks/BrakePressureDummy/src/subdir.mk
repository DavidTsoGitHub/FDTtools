################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/BrakePressureDummy/src/BrakePressureDummy.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/BrakePressureDummy/src/BrakePressureDummy.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/BrakePressureDummy/src/BrakePressureDummy.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/BrakePressureDummy/src/BrakePressureDummy.obj" \

C_DEPS += \
Application/FunctionBlocks/BrakePressureDummy/src/BrakePressureDummy.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/BrakePressureDummy/src/BrakePressureDummy.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/BrakePressureDummy/src/BrakePressureDummy.obj: ../Source/Application/FunctionBlocks/BrakePressureDummy/src/BrakePressureDummy.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/BrakePressureDummy/src/BrakePressureDummy.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/BrakePressureDummy/src/%.d: ../Source/Application/FunctionBlocks/BrakePressureDummy/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
