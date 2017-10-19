################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/Services/TextRenderer/TextRenderer.c" \

C_SRCS += \
../Source/Application/Services/TextRenderer/TextRenderer.c \

S_SRCS += \

OBJS += \
Application/Services/TextRenderer/TextRenderer.obj \

OBJS_QUOTED += \
"Application/Services/TextRenderer/TextRenderer.obj" \

C_DEPS += \
Application/Services/TextRenderer/TextRenderer.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/Services/TextRenderer/TextRenderer.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/Services/TextRenderer/TextRenderer.obj: ../Source/Application/Services/TextRenderer/TextRenderer.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/TextRenderer/TextRenderer.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/TextRenderer/%.d: ../Source/Application/Services/TextRenderer/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
