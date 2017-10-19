################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/MCAL/Common/HAL.c" \

C_SRCS += \
../Source/MCAL/Common/HAL.c \

S_SRCS += \

OBJS += \
MCAL/Common/HAL.obj \

OBJS_QUOTED += \
"MCAL/Common/HAL.obj" \

C_DEPS += \
MCAL/Common/HAL.d \

S_DEPS += \

C_DEPS_QUOTED += \
"MCAL/Common/HAL.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
MCAL/Common/HAL.obj: ../Source/MCAL/Common/HAL.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Common/HAL.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Common/%.d: ../Source/MCAL/Common/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
