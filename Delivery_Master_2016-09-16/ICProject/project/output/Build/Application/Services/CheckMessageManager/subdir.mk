################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/Services/CheckMessageManager/CheckMessageManager.c" \
"../Source/Application/Services/CheckMessageManager/CheckMessageManager_cfg.c" \

C_SRCS += \
../Source/Application/Services/CheckMessageManager/CheckMessageManager.c \
../Source/Application/Services/CheckMessageManager/CheckMessageManager_cfg.c \

S_SRCS += \

OBJS += \
Application/Services/CheckMessageManager/CheckMessageManager.obj \
Application/Services/CheckMessageManager/CheckMessageManager_cfg.obj \

OBJS_QUOTED += \
"Application/Services/CheckMessageManager/CheckMessageManager.obj" \
"Application/Services/CheckMessageManager/CheckMessageManager_cfg.obj" \

C_DEPS += \
Application/Services/CheckMessageManager/CheckMessageManager.d \
Application/Services/CheckMessageManager/CheckMessageManager_cfg.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/Services/CheckMessageManager/CheckMessageManager.d" \
"Application/Services/CheckMessageManager/CheckMessageManager_cfg.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/Services/CheckMessageManager/CheckMessageManager.obj: ../Source/Application/Services/CheckMessageManager/CheckMessageManager.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/CheckMessageManager/CheckMessageManager.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/CheckMessageManager/CheckMessageManager_cfg.obj: ../Source/Application/Services/CheckMessageManager/CheckMessageManager_cfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/CheckMessageManager/CheckMessageManager_cfg.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/CheckMessageManager/%.d: ../Source/Application/Services/CheckMessageManager/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
