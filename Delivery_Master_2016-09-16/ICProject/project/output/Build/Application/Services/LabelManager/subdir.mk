################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/Services/LabelManager/LabelManager.c" \

C_SRCS += \
../Source/Application/Services/LabelManager/LabelManager.c \

S_SRCS += \

OBJS += \
Application/Services/LabelManager/LabelManager.obj \

OBJS_QUOTED += \
"Application/Services/LabelManager/LabelManager.obj" \

C_DEPS += \
Application/Services/LabelManager/LabelManager.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/Services/LabelManager/LabelManager.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/Services/LabelManager/LabelManager.obj: ../Source/Application/Services/LabelManager/LabelManager.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/LabelManager/LabelManager.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/LabelManager/%.d: ../Source/Application/Services/LabelManager/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
