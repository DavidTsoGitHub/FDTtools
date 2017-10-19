################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/OdoAndTripDisplayHandler/src/OdoAndTripDisplayHandler.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/OdoAndTripDisplayHandler/src/OdoAndTripDisplayHandler.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/OdoAndTripDisplayHandler/src/OdoAndTripDisplayHandler.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/OdoAndTripDisplayHandler/src/OdoAndTripDisplayHandler.obj" \

C_DEPS += \
Application/FunctionBlocks/OdoAndTripDisplayHandler/src/OdoAndTripDisplayHandler.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/OdoAndTripDisplayHandler/src/OdoAndTripDisplayHandler.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/OdoAndTripDisplayHandler/src/OdoAndTripDisplayHandler.obj: ../Source/Application/FunctionBlocks/OdoAndTripDisplayHandler/src/OdoAndTripDisplayHandler.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/OdoAndTripDisplayHandler/src/OdoAndTripDisplayHandler.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/OdoAndTripDisplayHandler/src/%.d: ../Source/Application/FunctionBlocks/OdoAndTripDisplayHandler/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
