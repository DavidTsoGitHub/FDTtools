################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/UIWidgets/Icon/IconWidget.c" \
"../Source/Application/UIWidgets/Icon/IconWidgetCfg.c" \

C_SRCS += \
../Source/Application/UIWidgets/Icon/IconWidget.c \
../Source/Application/UIWidgets/Icon/IconWidgetCfg.c \

S_SRCS += \

OBJS += \
Application/UIWidgets/Icon/IconWidget.obj \
Application/UIWidgets/Icon/IconWidgetCfg.obj \

OBJS_QUOTED += \
"Application/UIWidgets/Icon/IconWidget.obj" \
"Application/UIWidgets/Icon/IconWidgetCfg.obj" \

C_DEPS += \
Application/UIWidgets/Icon/IconWidget.d \
Application/UIWidgets/Icon/IconWidgetCfg.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/UIWidgets/Icon/IconWidget.d" \
"Application/UIWidgets/Icon/IconWidgetCfg.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/UIWidgets/Icon/IconWidget.obj: ../Source/Application/UIWidgets/Icon/IconWidget.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/Icon/IconWidget.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/Icon/IconWidgetCfg.obj: ../Source/Application/UIWidgets/Icon/IconWidgetCfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/Icon/IconWidgetCfg.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/Icon/%.d: ../Source/Application/UIWidgets/Icon/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
