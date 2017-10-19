################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/FunctionBlocks/ODOTrip/src/ODOTrip.c" \

C_SRCS += \
../Source/Application/FunctionBlocks/ODOTrip/src/ODOTrip.c \

S_SRCS += \

OBJS += \
Application/FunctionBlocks/ODOTrip/src/ODOTrip.obj \

OBJS_QUOTED += \
"Application/FunctionBlocks/ODOTrip/src/ODOTrip.obj" \

C_DEPS += \
Application/FunctionBlocks/ODOTrip/src/ODOTrip.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/FunctionBlocks/ODOTrip/src/ODOTrip.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/FunctionBlocks/ODOTrip/src/ODOTrip.obj: ../Source/Application/FunctionBlocks/ODOTrip/src/ODOTrip.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/FunctionBlocks/ODOTrip/src/ODOTrip.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/FunctionBlocks/ODOTrip/src/%.d: ../Source/Application/FunctionBlocks/ODOTrip/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
