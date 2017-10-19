################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/UIWidgets/SelectableList/SelectableListWidget.c" \
"../Source/Application/UIWidgets/SelectableList/SelectableListWidgetCfg.c" \

C_SRCS += \
../Source/Application/UIWidgets/SelectableList/SelectableListWidget.c \
../Source/Application/UIWidgets/SelectableList/SelectableListWidgetCfg.c \

S_SRCS += \

OBJS += \
Application/UIWidgets/SelectableList/SelectableListWidget.obj \
Application/UIWidgets/SelectableList/SelectableListWidgetCfg.obj \

OBJS_QUOTED += \
"Application/UIWidgets/SelectableList/SelectableListWidget.obj" \
"Application/UIWidgets/SelectableList/SelectableListWidgetCfg.obj" \

C_DEPS += \
Application/UIWidgets/SelectableList/SelectableListWidget.d \
Application/UIWidgets/SelectableList/SelectableListWidgetCfg.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/UIWidgets/SelectableList/SelectableListWidget.d" \
"Application/UIWidgets/SelectableList/SelectableListWidgetCfg.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/UIWidgets/SelectableList/SelectableListWidget.obj: ../Source/Application/UIWidgets/SelectableList/SelectableListWidget.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/SelectableList/SelectableListWidget.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/SelectableList/SelectableListWidgetCfg.obj: ../Source/Application/UIWidgets/SelectableList/SelectableListWidgetCfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/SelectableList/SelectableListWidgetCfg.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/SelectableList/%.d: ../Source/Application/UIWidgets/SelectableList/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
