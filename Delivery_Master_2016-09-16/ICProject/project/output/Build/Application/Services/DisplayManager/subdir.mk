################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/Services/DisplayManager/DisplayManager_cfg.c" \
"../Source/Application/Services/DisplayManager/MonochromeDisplayManager.c" \

C_SRCS += \
../Source/Application/Services/DisplayManager/DisplayManager_cfg.c \
../Source/Application/Services/DisplayManager/MonochromeDisplayManager.c \

S_SRCS += \

OBJS += \
Application/Services/DisplayManager/DisplayManager_cfg.obj \
Application/Services/DisplayManager/MonochromeDisplayManager.obj \

OBJS_QUOTED += \
"Application/Services/DisplayManager/DisplayManager_cfg.obj" \
"Application/Services/DisplayManager/MonochromeDisplayManager.obj" \

C_DEPS += \
Application/Services/DisplayManager/DisplayManager_cfg.d \
Application/Services/DisplayManager/MonochromeDisplayManager.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/Services/DisplayManager/DisplayManager_cfg.d" \
"Application/Services/DisplayManager/MonochromeDisplayManager.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/Services/DisplayManager/DisplayManager_cfg.obj: ../Source/Application/Services/DisplayManager/DisplayManager_cfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/DisplayManager/DisplayManager_cfg.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/DisplayManager/MonochromeDisplayManager.obj: ../Source/Application/Services/DisplayManager/MonochromeDisplayManager.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/DisplayManager/MonochromeDisplayManager.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/DisplayManager/%.d: ../Source/Application/Services/DisplayManager/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
