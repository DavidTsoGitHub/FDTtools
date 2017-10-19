################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/Services/IconRenderer/IconRenderer.c" \

C_SRCS += \
../Source/Application/Services/IconRenderer/IconRenderer.c \

S_SRCS += \

OBJS += \
Application/Services/IconRenderer/IconRenderer.obj \

OBJS_QUOTED += \
"Application/Services/IconRenderer/IconRenderer.obj" \

C_DEPS += \
Application/Services/IconRenderer/IconRenderer.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/Services/IconRenderer/IconRenderer.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/Services/IconRenderer/IconRenderer.obj: ../Source/Application/Services/IconRenderer/IconRenderer.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/IconRenderer/IconRenderer.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/IconRenderer/%.d: ../Source/Application/Services/IconRenderer/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
