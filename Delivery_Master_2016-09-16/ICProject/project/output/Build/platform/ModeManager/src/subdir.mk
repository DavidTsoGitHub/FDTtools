################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/platform/ModeManager/src/ModeManager.c" \

C_SRCS += \
../Source/platform/ModeManager/src/ModeManager.c \

S_SRCS += \

OBJS += \
platform/ModeManager/src/ModeManager.obj \

OBJS_QUOTED += \
"platform/ModeManager/src/ModeManager.obj" \

C_DEPS += \
platform/ModeManager/src/ModeManager.d \

S_DEPS += \

C_DEPS_QUOTED += \
"platform/ModeManager/src/ModeManager.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
platform/ModeManager/src/ModeManager.obj: ../Source/platform/ModeManager/src/ModeManager.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "platform/ModeManager/src/ModeManager.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

platform/ModeManager/src/%.d: ../Source/platform/ModeManager/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
