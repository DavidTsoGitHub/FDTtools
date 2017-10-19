################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/platform/Logger/Logger.c" \

C_SRCS += \
../Source/platform/Logger/Logger.c \

S_SRCS += \

OBJS += \
platform/Logger/Logger.obj \

OBJS_QUOTED += \
"platform/Logger/Logger.obj" \

C_DEPS += \
platform/Logger/Logger.d \

S_DEPS += \

C_DEPS_QUOTED += \
"platform/Logger/Logger.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
platform/Logger/Logger.obj: ../Source/platform/Logger/Logger.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "platform/Logger/Logger.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

platform/Logger/%.d: ../Source/platform/Logger/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
