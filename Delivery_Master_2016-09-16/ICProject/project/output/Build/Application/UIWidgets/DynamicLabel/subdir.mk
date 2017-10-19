################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/UIWidgets/DynamicLabel/DynamicLabelWidget.c" \
"../Source/Application/UIWidgets/DynamicLabel/DynamicLabelWidgetCfg.c" \

C_SRCS += \
../Source/Application/UIWidgets/DynamicLabel/DynamicLabelWidget.c \
../Source/Application/UIWidgets/DynamicLabel/DynamicLabelWidgetCfg.c \

S_SRCS += \

OBJS += \
Application/UIWidgets/DynamicLabel/DynamicLabelWidget.obj \
Application/UIWidgets/DynamicLabel/DynamicLabelWidgetCfg.obj \

OBJS_QUOTED += \
"Application/UIWidgets/DynamicLabel/DynamicLabelWidget.obj" \
"Application/UIWidgets/DynamicLabel/DynamicLabelWidgetCfg.obj" \

C_DEPS += \
Application/UIWidgets/DynamicLabel/DynamicLabelWidget.d \
Application/UIWidgets/DynamicLabel/DynamicLabelWidgetCfg.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/UIWidgets/DynamicLabel/DynamicLabelWidget.d" \
"Application/UIWidgets/DynamicLabel/DynamicLabelWidgetCfg.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/UIWidgets/DynamicLabel/DynamicLabelWidget.obj: ../Source/Application/UIWidgets/DynamicLabel/DynamicLabelWidget.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/DynamicLabel/DynamicLabelWidget.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/DynamicLabel/DynamicLabelWidgetCfg.obj: ../Source/Application/UIWidgets/DynamicLabel/DynamicLabelWidgetCfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/DynamicLabel/DynamicLabelWidgetCfg.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/DynamicLabel/%.d: ../Source/Application/UIWidgets/DynamicLabel/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
