################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/Services/IconManager/IconManager.c" \

C_SRCS += \
../Source/Application/Services/IconManager/IconManager.c \

S_SRCS += \

OBJS += \
Application/Services/IconManager/IconManager.obj \

OBJS_QUOTED += \
"Application/Services/IconManager/IconManager.obj" \

C_DEPS += \
Application/Services/IconManager/IconManager.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/Services/IconManager/IconManager.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/Services/IconManager/IconManager.obj: ../Source/Application/Services/IconManager/IconManager.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/IconManager/IconManager.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/IconManager/%.d: ../Source/Application/Services/IconManager/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
