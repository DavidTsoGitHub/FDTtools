################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/UIWidgets/Label/LabelWidget.c" \
"../Source/Application/UIWidgets/Label/LabelWidgetCfg.c" \

C_SRCS += \
../Source/Application/UIWidgets/Label/LabelWidget.c \
../Source/Application/UIWidgets/Label/LabelWidgetCfg.c \

S_SRCS += \

OBJS += \
Application/UIWidgets/Label/LabelWidget.obj \
Application/UIWidgets/Label/LabelWidgetCfg.obj \

OBJS_QUOTED += \
"Application/UIWidgets/Label/LabelWidget.obj" \
"Application/UIWidgets/Label/LabelWidgetCfg.obj" \

C_DEPS += \
Application/UIWidgets/Label/LabelWidget.d \
Application/UIWidgets/Label/LabelWidgetCfg.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/UIWidgets/Label/LabelWidget.d" \
"Application/UIWidgets/Label/LabelWidgetCfg.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/UIWidgets/Label/LabelWidget.obj: ../Source/Application/UIWidgets/Label/LabelWidget.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/Label/LabelWidget.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/Label/LabelWidgetCfg.obj: ../Source/Application/UIWidgets/Label/LabelWidgetCfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/Label/LabelWidgetCfg.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/Label/%.d: ../Source/Application/UIWidgets/Label/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
