################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/UIWidgets/UIWidget/UIWidget.c" \

C_SRCS += \
../Source/Application/UIWidgets/UIWidget/UIWidget.c \

S_SRCS += \

OBJS += \
Application/UIWidgets/UIWidget/UIWidget.obj \

OBJS_QUOTED += \
"Application/UIWidgets/UIWidget/UIWidget.obj" \

C_DEPS += \
Application/UIWidgets/UIWidget/UIWidget.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/UIWidgets/UIWidget/UIWidget.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/UIWidgets/UIWidget/UIWidget.obj: ../Source/Application/UIWidgets/UIWidget/UIWidget.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/UIWidgets/UIWidget/UIWidget.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/UIWidgets/UIWidget/%.d: ../Source/Application/UIWidgets/UIWidget/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
