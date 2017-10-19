################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/MCAL/Target/BSP.c" \
"../Source/MCAL/Target/systime.c" \

C_SRCS += \
../Source/MCAL/Target/BSP.c \
../Source/MCAL/Target/systime.c \

S_SRCS += \

OBJS += \
MCAL/Target/BSP.obj \
MCAL/Target/systime.obj \

OBJS_QUOTED += \
"MCAL/Target/BSP.obj" \
"MCAL/Target/systime.obj" \

C_DEPS += \
MCAL/Target/BSP.d \
MCAL/Target/systime.d \

S_DEPS += \

C_DEPS_QUOTED += \
"MCAL/Target/BSP.d" \
"MCAL/Target/systime.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
MCAL/Target/BSP.obj: ../Source/MCAL/Target/BSP.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/BSP.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/systime.obj: ../Source/MCAL/Target/systime.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/systime.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/%.d: ../Source/MCAL/Target/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
