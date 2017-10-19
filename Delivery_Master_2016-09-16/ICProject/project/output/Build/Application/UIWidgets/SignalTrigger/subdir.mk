################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/UIWidgets/SignalTrigger/SignalTriggerWidget.c" \
"../Source/Application/UIWidgets/SignalTrigger/SignalTriggerWidgetCfg.c" \

C_SRCS += \
../Source/Application/UIWidgets/SignalTrigger/SignalTriggerWidget.c \
../Source/Application/UIWidgets/SignalTrigger/SignalTriggerWidgetCfg.c \

S_SRCS += \

OBJS += \
Application/UIWidgets/SignalTrigger/SignalTriggerWidget.obj \
Application/UIWidgets/SignalTrigger/SignalTriggerWidgetCfg.obj \

OBJS_QUOTED += \
"Application/UIWidgets/SignalTrigger/SignalTriggerWidget.obj" \
"Application/UIWidgets/SignalTrigger/SignalTriggerWidgetCfg.obj" \

C_DEPS += \
Application/UIWidgets/SignalTrigger/SignalTriggerWidget.d \
Application/UIWidgets/SignalTrigger/SignalTriggerWidgetCfg.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/UIWidgets/SignalTrigger/SignalTriggerWidget.d" \
"Application/UIWidgets/SignalTrigger/SignalTriggerWidgetCfg.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/UIWidgets/SignalTrigger/SignalTriggerWidget.obj: ../Source/Application/UIWidgets/SignalTrigger/SignalTriggerWidget.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/SignalTrigger/SignalTriggerWidget.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/SignalTrigger/SignalTriggerWidgetCfg.obj: ../Source/Application/UIWidgets/SignalTrigger/SignalTriggerWidgetCfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/SignalTrigger/SignalTriggerWidgetCfg.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/SignalTrigger/%.d: ../Source/Application/UIWidgets/SignalTrigger/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
