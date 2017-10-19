################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/UIWidgets/DynamicIcon/DynamicIconWidget.c" \
"../Source/Application/UIWidgets/DynamicIcon/DynamicIconWidgetCfg.c" \

C_SRCS += \
../Source/Application/UIWidgets/DynamicIcon/DynamicIconWidget.c \
../Source/Application/UIWidgets/DynamicIcon/DynamicIconWidgetCfg.c \

S_SRCS += \

OBJS += \
Application/UIWidgets/DynamicIcon/DynamicIconWidget.obj \
Application/UIWidgets/DynamicIcon/DynamicIconWidgetCfg.obj \

OBJS_QUOTED += \
"Application/UIWidgets/DynamicIcon/DynamicIconWidget.obj" \
"Application/UIWidgets/DynamicIcon/DynamicIconWidgetCfg.obj" \

C_DEPS += \
Application/UIWidgets/DynamicIcon/DynamicIconWidget.d \
Application/UIWidgets/DynamicIcon/DynamicIconWidgetCfg.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/UIWidgets/DynamicIcon/DynamicIconWidget.d" \
"Application/UIWidgets/DynamicIcon/DynamicIconWidgetCfg.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/UIWidgets/DynamicIcon/DynamicIconWidget.obj: ../Source/Application/UIWidgets/DynamicIcon/DynamicIconWidget.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/DynamicIcon/DynamicIconWidget.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/DynamicIcon/DynamicIconWidgetCfg.obj: ../Source/Application/UIWidgets/DynamicIcon/DynamicIconWidgetCfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/DynamicIcon/DynamicIconWidgetCfg.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/DynamicIcon/%.d: ../Source/Application/UIWidgets/DynamicIcon/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
