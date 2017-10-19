################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/MCAL/Target/CAN/CanDrv.c" \
"../Source/MCAL/Target/CAN/CanInterrupts.c" \

C_SRCS += \
../Source/MCAL/Target/CAN/CanDrv.c \
../Source/MCAL/Target/CAN/CanInterrupts.c \

S_SRCS += \

OBJS += \
MCAL/Target/CAN/CanDrv.obj \
MCAL/Target/CAN/CanInterrupts.obj \

OBJS_QUOTED += \
"MCAL/Target/CAN/CanDrv.obj" \
"MCAL/Target/CAN/CanInterrupts.obj" \

C_DEPS += \
MCAL/Target/CAN/CanDrv.d \
MCAL/Target/CAN/CanInterrupts.d \

S_DEPS += \

C_DEPS_QUOTED += \
"MCAL/Target/CAN/CanDrv.d" \
"MCAL/Target/CAN/CanInterrupts.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
MCAL/Target/CAN/CanDrv.obj: ../Source/MCAL/Target/CAN/CanDrv.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/CAN/CanDrv.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/CAN/CanInterrupts.obj: ../Source/MCAL/Target/CAN/CanInterrupts.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/CAN/CanInterrupts.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/CAN/%.d: ../Source/MCAL/Target/CAN/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
