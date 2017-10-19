################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/UIWidgets/ColumnLevel/ColumnLevelWidget.c" \
"../Source/Application/UIWidgets/ColumnLevel/ColumnLevelWidgetCfg.c" \

C_SRCS += \
../Source/Application/UIWidgets/ColumnLevel/ColumnLevelWidget.c \
../Source/Application/UIWidgets/ColumnLevel/ColumnLevelWidgetCfg.c \

S_SRCS += \

OBJS += \
Application/UIWidgets/ColumnLevel/ColumnLevelWidget.obj \
Application/UIWidgets/ColumnLevel/ColumnLevelWidgetCfg.obj \

OBJS_QUOTED += \
"Application/UIWidgets/ColumnLevel/ColumnLevelWidget.obj" \
"Application/UIWidgets/ColumnLevel/ColumnLevelWidgetCfg.obj" \

C_DEPS += \
Application/UIWidgets/ColumnLevel/ColumnLevelWidget.d \
Application/UIWidgets/ColumnLevel/ColumnLevelWidgetCfg.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/UIWidgets/ColumnLevel/ColumnLevelWidget.d" \
"Application/UIWidgets/ColumnLevel/ColumnLevelWidgetCfg.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/UIWidgets/ColumnLevel/ColumnLevelWidget.obj: ../Source/Application/UIWidgets/ColumnLevel/ColumnLevelWidget.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/ColumnLevel/ColumnLevelWidget.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/ColumnLevel/ColumnLevelWidgetCfg.obj: ../Source/Application/UIWidgets/ColumnLevel/ColumnLevelWidgetCfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/ColumnLevel/ColumnLevelWidgetCfg.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/ColumnLevel/%.d: ../Source/Application/UIWidgets/ColumnLevel/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
