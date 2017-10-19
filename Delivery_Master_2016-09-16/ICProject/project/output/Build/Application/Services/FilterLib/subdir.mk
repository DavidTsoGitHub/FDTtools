################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/Services/FilterLib/FilterLib.c" \

C_SRCS += \
../Source/Application/Services/FilterLib/FilterLib.c \

S_SRCS += \

OBJS += \
Application/Services/FilterLib/FilterLib.obj \

OBJS_QUOTED += \
"Application/Services/FilterLib/FilterLib.obj" \

C_DEPS += \
Application/Services/FilterLib/FilterLib.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/Services/FilterLib/FilterLib.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/Services/FilterLib/FilterLib.obj: ../Source/Application/Services/FilterLib/FilterLib.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/FilterLib/FilterLib.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/FilterLib/%.d: ../Source/Application/Services/FilterLib/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
