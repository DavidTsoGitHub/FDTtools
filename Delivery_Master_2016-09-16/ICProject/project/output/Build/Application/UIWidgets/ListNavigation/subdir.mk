################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/UIWidgets/ListNavigation/ListNavigationWidget.c" \
"../Source/Application/UIWidgets/ListNavigation/ListNavigationWidgetCfg.c" \

C_SRCS += \
../Source/Application/UIWidgets/ListNavigation/ListNavigationWidget.c \
../Source/Application/UIWidgets/ListNavigation/ListNavigationWidgetCfg.c \

S_SRCS += \

OBJS += \
Application/UIWidgets/ListNavigation/ListNavigationWidget.obj \
Application/UIWidgets/ListNavigation/ListNavigationWidgetCfg.obj \

OBJS_QUOTED += \
"Application/UIWidgets/ListNavigation/ListNavigationWidget.obj" \
"Application/UIWidgets/ListNavigation/ListNavigationWidgetCfg.obj" \

C_DEPS += \
Application/UIWidgets/ListNavigation/ListNavigationWidget.d \
Application/UIWidgets/ListNavigation/ListNavigationWidgetCfg.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/UIWidgets/ListNavigation/ListNavigationWidget.d" \
"Application/UIWidgets/ListNavigation/ListNavigationWidgetCfg.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/UIWidgets/ListNavigation/ListNavigationWidget.obj: ../Source/Application/UIWidgets/ListNavigation/ListNavigationWidget.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/ListNavigation/ListNavigationWidget.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/ListNavigation/ListNavigationWidgetCfg.obj: ../Source/Application/UIWidgets/ListNavigation/ListNavigationWidgetCfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/ListNavigation/ListNavigationWidgetCfg.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/ListNavigation/%.d: ../Source/Application/UIWidgets/ListNavigation/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
