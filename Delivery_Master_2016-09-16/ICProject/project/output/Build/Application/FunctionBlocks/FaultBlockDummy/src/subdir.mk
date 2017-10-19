################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/FaultBlockDummy/src/FaultBlockDummy.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/FaultBlockDummy/src/FaultBlockDummy.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/FaultBlockDummy/src/FaultBlockDummy.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/FaultBlockDummy/src/FaultBlockDummy.obj" \

C_DEPS += \
Application/FunctionBlocks/FaultBlockDummy/src/FaultBlockDummy.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/FaultBlockDummy/src/FaultBlockDummy.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/FaultBlockDummy/src/FaultBlockDummy.obj: ../Source/Application/FunctionBlocks/FaultBlockDummy/src/FaultBlockDummy.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/FaultBlockDummy/src/FaultBlockDummy.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/FaultBlockDummy/src/%.d: ../Source/Application/FunctionBlocks/FaultBlockDummy/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
