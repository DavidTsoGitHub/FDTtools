################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/UIWidgets/BlockBar/BlockBarWidget.c" \
"../Source/Application/UIWidgets/BlockBar/BlockBarWidgetCfg.c" \

C_SRCS += \
../Source/Application/UIWidgets/BlockBar/BlockBarWidget.c \
../Source/Application/UIWidgets/BlockBar/BlockBarWidgetCfg.c \

S_SRCS += \

OBJS += \
Application/UIWidgets/BlockBar/BlockBarWidget.obj \
Application/UIWidgets/BlockBar/BlockBarWidgetCfg.obj \

OBJS_QUOTED += \
"Application/UIWidgets/BlockBar/BlockBarWidget.obj" \
"Application/UIWidgets/BlockBar/BlockBarWidgetCfg.obj" \

C_DEPS += \
Application/UIWidgets/BlockBar/BlockBarWidget.d \
Application/UIWidgets/BlockBar/BlockBarWidgetCfg.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/UIWidgets/BlockBar/BlockBarWidget.d" \
"Application/UIWidgets/BlockBar/BlockBarWidgetCfg.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/UIWidgets/BlockBar/BlockBarWidget.obj: ../Source/Application/UIWidgets/BlockBar/BlockBarWidget.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/BlockBar/BlockBarWidget.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/BlockBar/BlockBarWidgetCfg.obj: ../Source/Application/UIWidgets/BlockBar/BlockBarWidgetCfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/BlockBar/BlockBarWidgetCfg.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/BlockBar/%.d: ../Source/Application/UIWidgets/BlockBar/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
