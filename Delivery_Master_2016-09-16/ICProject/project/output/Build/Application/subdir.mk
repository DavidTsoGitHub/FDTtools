################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/WCharDefines.c" \

C_SRCS += \
../Source/Application/WCharDefines.c \

S_SRCS += \

OBJS += \
Application/WCharDefines.obj \

OBJS_QUOTED += \
"Application/WCharDefines.obj" \

C_DEPS += \
Application/WCharDefines.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/WCharDefines.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/WCharDefines.obj: ../Source/Application/WCharDefines.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/WCharDefines.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/%.d: ../Source/Application/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
