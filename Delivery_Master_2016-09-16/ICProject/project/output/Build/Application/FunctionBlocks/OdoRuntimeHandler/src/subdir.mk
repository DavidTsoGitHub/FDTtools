################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/OdoRuntimeHandler/src/OdoRuntimeHandler.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/OdoRuntimeHandler/src/OdoRuntimeHandler.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/OdoRuntimeHandler/src/OdoRuntimeHandler.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/OdoRuntimeHandler/src/OdoRuntimeHandler.obj" \

C_DEPS += \
Application/FunctionBlocks/OdoRuntimeHandler/src/OdoRuntimeHandler.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/OdoRuntimeHandler/src/OdoRuntimeHandler.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/OdoRuntimeHandler/src/OdoRuntimeHandler.obj: ../Source/Application/FunctionBlocks/OdoRuntimeHandler/src/OdoRuntimeHandler.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/OdoRuntimeHandler/src/OdoRuntimeHandler.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/OdoRuntimeHandler/src/%.d: ../Source/Application/FunctionBlocks/OdoRuntimeHandler/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
