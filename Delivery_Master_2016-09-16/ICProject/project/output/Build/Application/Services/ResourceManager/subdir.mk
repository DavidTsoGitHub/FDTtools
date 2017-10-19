################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/Services/ResourceManager/ResourceManager.c" \

C_SRCS += \
../Source/Application/Services/ResourceManager/ResourceManager.c \

S_SRCS += \

OBJS += \
Application/Services/ResourceManager/ResourceManager.obj \

OBJS_QUOTED += \
"Application/Services/ResourceManager/ResourceManager.obj" \

C_DEPS += \
Application/Services/ResourceManager/ResourceManager.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/Services/ResourceManager/ResourceManager.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/Services/ResourceManager/ResourceManager.obj: ../Source/Application/Services/ResourceManager/ResourceManager.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/ResourceManager/ResourceManager.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/ResourceManager/%.d: ../Source/Application/Services/ResourceManager/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
