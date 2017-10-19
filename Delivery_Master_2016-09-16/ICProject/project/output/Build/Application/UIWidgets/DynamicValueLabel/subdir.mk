################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/UIWidgets/DynamicValueLabel/DynamicValueLabelWidget.c" \
"../Source/Application/UIWidgets/DynamicValueLabel/DynamicValueLabelWidgetCfg.c" \

C_SRCS += \
../Source/Application/UIWidgets/DynamicValueLabel/DynamicValueLabelWidget.c \
../Source/Application/UIWidgets/DynamicValueLabel/DynamicValueLabelWidgetCfg.c \

S_SRCS += \

OBJS += \
Application/UIWidgets/DynamicValueLabel/DynamicValueLabelWidget.obj \
Application/UIWidgets/DynamicValueLabel/DynamicValueLabelWidgetCfg.obj \

OBJS_QUOTED += \
"Application/UIWidgets/DynamicValueLabel/DynamicValueLabelWidget.obj" \
"Application/UIWidgets/DynamicValueLabel/DynamicValueLabelWidgetCfg.obj" \

C_DEPS += \
Application/UIWidgets/DynamicValueLabel/DynamicValueLabelWidget.d \
Application/UIWidgets/DynamicValueLabel/DynamicValueLabelWidgetCfg.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/UIWidgets/DynamicValueLabel/DynamicValueLabelWidget.d" \
"Application/UIWidgets/DynamicValueLabel/DynamicValueLabelWidgetCfg.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/UIWidgets/DynamicValueLabel/DynamicValueLabelWidget.obj: ../Source/Application/UIWidgets/DynamicValueLabel/DynamicValueLabelWidget.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/DynamicValueLabel/DynamicValueLabelWidget.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/DynamicValueLabel/DynamicValueLabelWidgetCfg.obj: ../Source/Application/UIWidgets/DynamicValueLabel/DynamicValueLabelWidgetCfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/DynamicValueLabel/DynamicValueLabelWidgetCfg.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/DynamicValueLabel/%.d: ../Source/Application/UIWidgets/DynamicValueLabel/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
