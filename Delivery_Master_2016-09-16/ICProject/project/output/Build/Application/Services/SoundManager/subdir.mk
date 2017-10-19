################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/Services/SoundManager/SoundManager.c" \

C_SRCS += \
../Source/Application/Services/SoundManager/SoundManager.c \

S_SRCS += \

OBJS += \
Application/Services/SoundManager/SoundManager.obj \

OBJS_QUOTED += \
"Application/Services/SoundManager/SoundManager.obj" \

C_DEPS += \
Application/Services/SoundManager/SoundManager.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/Services/SoundManager/SoundManager.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/Services/SoundManager/SoundManager.obj: ../Source/Application/Services/SoundManager/SoundManager.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/SoundManager/SoundManager.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/SoundManager/%.d: ../Source/Application/Services/SoundManager/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
