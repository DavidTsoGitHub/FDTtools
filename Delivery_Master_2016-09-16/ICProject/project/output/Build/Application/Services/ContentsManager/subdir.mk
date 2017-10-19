################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/Services/ContentsManager/ContentsManager_cfg.c" \
"../Source/Application/Services/ContentsManager/MonochromeContentsManager.c" \

C_SRCS += \
../Source/Application/Services/ContentsManager/ContentsManager_cfg.c \
../Source/Application/Services/ContentsManager/MonochromeContentsManager.c \

S_SRCS += \

OBJS += \
Application/Services/ContentsManager/ContentsManager_cfg.obj \
Application/Services/ContentsManager/MonochromeContentsManager.obj \

OBJS_QUOTED += \
"Application/Services/ContentsManager/ContentsManager_cfg.obj" \
"Application/Services/ContentsManager/MonochromeContentsManager.obj" \

C_DEPS += \
Application/Services/ContentsManager/ContentsManager_cfg.d \
Application/Services/ContentsManager/MonochromeContentsManager.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/Services/ContentsManager/ContentsManager_cfg.d" \
"Application/Services/ContentsManager/MonochromeContentsManager.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/Services/ContentsManager/ContentsManager_cfg.obj: ../Source/Application/Services/ContentsManager/ContentsManager_cfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/ContentsManager/ContentsManager_cfg.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/ContentsManager/MonochromeContentsManager.obj: ../Source/Application/Services/ContentsManager/MonochromeContentsManager.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/ContentsManager/MonochromeContentsManager.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/ContentsManager/%.d: ../Source/Application/Services/ContentsManager/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
