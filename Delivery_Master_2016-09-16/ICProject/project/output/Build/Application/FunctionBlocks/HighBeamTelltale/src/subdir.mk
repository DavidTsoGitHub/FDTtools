################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/HighBeamTelltale/src/HighBeamTelltale.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/HighBeamTelltale/src/HighBeamTelltale.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/HighBeamTelltale/src/HighBeamTelltale.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/HighBeamTelltale/src/HighBeamTelltale.obj" \

C_DEPS += \
Application/FunctionBlocks/HighBeamTelltale/src/HighBeamTelltale.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/HighBeamTelltale/src/HighBeamTelltale.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/HighBeamTelltale/src/HighBeamTelltale.obj: ../Source/Application/FunctionBlocks/HighBeamTelltale/src/HighBeamTelltale.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/HighBeamTelltale/src/HighBeamTelltale.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/HighBeamTelltale/src/%.d: ../Source/Application/FunctionBlocks/HighBeamTelltale/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
