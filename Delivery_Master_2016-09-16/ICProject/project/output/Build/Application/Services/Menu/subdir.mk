################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/Services/Menu/MenuController.c" \
"../Source/Application/Services/Menu/MenuRenderer.c" \
"../Source/Application/Services/Menu/Menu_cfg.c" \

C_SRCS += \
../Source/Application/Services/Menu/MenuController.c \
../Source/Application/Services/Menu/MenuRenderer.c \
../Source/Application/Services/Menu/Menu_cfg.c \

S_SRCS += \

OBJS += \
Application/Services/Menu/MenuController.obj \
Application/Services/Menu/MenuRenderer.obj \
Application/Services/Menu/Menu_cfg.obj \

OBJS_QUOTED += \
"Application/Services/Menu/MenuController.obj" \
"Application/Services/Menu/MenuRenderer.obj" \
"Application/Services/Menu/Menu_cfg.obj" \

C_DEPS += \
Application/Services/Menu/MenuController.d \
Application/Services/Menu/MenuRenderer.d \
Application/Services/Menu/Menu_cfg.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/Services/Menu/MenuController.d" \
"Application/Services/Menu/MenuRenderer.d" \
"Application/Services/Menu/Menu_cfg.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/Services/Menu/MenuController.obj: ../Source/Application/Services/Menu/MenuController.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/Menu/MenuController.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/Menu/MenuRenderer.obj: ../Source/Application/Services/Menu/MenuRenderer.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/Menu/MenuRenderer.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/Menu/Menu_cfg.obj: ../Source/Application/Services/Menu/Menu_cfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/Menu/Menu_cfg.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/Menu/%.d: ../Source/Application/Services/Menu/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
