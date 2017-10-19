################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/BrakePressureLowTellTale/src/BrakePressureLowTellTale.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/BrakePressureLowTellTale/src/BrakePressureLowTellTale.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/BrakePressureLowTellTale/src/BrakePressureLowTellTale.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/BrakePressureLowTellTale/src/BrakePressureLowTellTale.obj" \

C_DEPS += \
Application/FunctionBlocks/BrakePressureLowTellTale/src/BrakePressureLowTellTale.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/BrakePressureLowTellTale/src/BrakePressureLowTellTale.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/BrakePressureLowTellTale/src/BrakePressureLowTellTale.obj: ../Source/Application/FunctionBlocks/BrakePressureLowTellTale/src/BrakePressureLowTellTale.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/BrakePressureLowTellTale/src/BrakePressureLowTellTale.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/BrakePressureLowTellTale/src/%.d: ../Source/Application/FunctionBlocks/BrakePressureLowTellTale/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
