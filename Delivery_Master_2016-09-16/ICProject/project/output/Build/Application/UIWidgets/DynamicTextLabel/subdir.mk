################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/UIWidgets/DynamicTextLabel/DynamicTextLabelWidget.c" \
"../Source/Application/UIWidgets/DynamicTextLabel/DynamicTextLabelWidgetCfg.c" \

C_SRCS += \
../Source/Application/UIWidgets/DynamicTextLabel/DynamicTextLabelWidget.c \
../Source/Application/UIWidgets/DynamicTextLabel/DynamicTextLabelWidgetCfg.c \

S_SRCS += \

OBJS += \
Application/UIWidgets/DynamicTextLabel/DynamicTextLabelWidget.obj \
Application/UIWidgets/DynamicTextLabel/DynamicTextLabelWidgetCfg.obj \

OBJS_QUOTED += \
"Application/UIWidgets/DynamicTextLabel/DynamicTextLabelWidget.obj" \
"Application/UIWidgets/DynamicTextLabel/DynamicTextLabelWidgetCfg.obj" \

C_DEPS += \
Application/UIWidgets/DynamicTextLabel/DynamicTextLabelWidget.d \
Application/UIWidgets/DynamicTextLabel/DynamicTextLabelWidgetCfg.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/UIWidgets/DynamicTextLabel/DynamicTextLabelWidget.d" \
"Application/UIWidgets/DynamicTextLabel/DynamicTextLabelWidgetCfg.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/UIWidgets/DynamicTextLabel/DynamicTextLabelWidget.obj: ../Source/Application/UIWidgets/DynamicTextLabel/DynamicTextLabelWidget.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/DynamicTextLabel/DynamicTextLabelWidget.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/DynamicTextLabel/DynamicTextLabelWidgetCfg.obj: ../Source/Application/UIWidgets/DynamicTextLabel/DynamicTextLabelWidgetCfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/DynamicTextLabel/DynamicTextLabelWidgetCfg.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/DynamicTextLabel/%.d: ../Source/Application/UIWidgets/DynamicTextLabel/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
