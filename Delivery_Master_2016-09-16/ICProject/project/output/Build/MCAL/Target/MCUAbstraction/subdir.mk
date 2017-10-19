################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/MCAL/Target/MCUAbstraction/emios.c" \
"../Source/MCAL/Target/MCUAbstraction/LCD.c" \
"../Source/MCAL/Target/MCUAbstraction/MCU_Analog.c" \
"../Source/MCAL/Target/MCUAbstraction/MCU_FreqCntInput.c" \
"../Source/MCAL/Target/MCUAbstraction/MCU_Gauge.c" \
"../Source/MCAL/Target/MCUAbstraction/MCU_SwitchMode.c" \
"../Source/MCAL/Target/MCUAbstraction/SGL.c" \
"../Source/MCAL/Target/MCUAbstraction/Watchdog.c" \

C_SRCS += \
../Source/MCAL/Target/MCUAbstraction/emios.c \
../Source/MCAL/Target/MCUAbstraction/LCD.c \
../Source/MCAL/Target/MCUAbstraction/MCU_Analog.c \
../Source/MCAL/Target/MCUAbstraction/MCU_FreqCntInput.c \
../Source/MCAL/Target/MCUAbstraction/MCU_Gauge.c \
../Source/MCAL/Target/MCUAbstraction/MCU_SwitchMode.c \
../Source/MCAL/Target/MCUAbstraction/SGL.c \
../Source/MCAL/Target/MCUAbstraction/Watchdog.c \

S_SRCS += \

OBJS += \
MCAL/Target/MCUAbstraction/emios.obj \
MCAL/Target/MCUAbstraction/LCD.obj \
MCAL/Target/MCUAbstraction/MCU_Analog.obj \
MCAL/Target/MCUAbstraction/MCU_FreqCntInput.obj \
MCAL/Target/MCUAbstraction/MCU_Gauge.obj \
MCAL/Target/MCUAbstraction/MCU_SwitchMode.obj \
MCAL/Target/MCUAbstraction/SGL.obj \
MCAL/Target/MCUAbstraction/Watchdog.obj \

OBJS_QUOTED += \
"MCAL/Target/MCUAbstraction/emios.obj" \
"MCAL/Target/MCUAbstraction/LCD.obj" \
"MCAL/Target/MCUAbstraction/MCU_Analog.obj" \
"MCAL/Target/MCUAbstraction/MCU_FreqCntInput.obj" \
"MCAL/Target/MCUAbstraction/MCU_Gauge.obj" \
"MCAL/Target/MCUAbstraction/MCU_SwitchMode.obj" \
"MCAL/Target/MCUAbstraction/SGL.obj" \
"MCAL/Target/MCUAbstraction/Watchdog.obj" \

C_DEPS += \
MCAL/Target/MCUAbstraction/emios.d \
MCAL/Target/MCUAbstraction/LCD.d \
MCAL/Target/MCUAbstraction/MCU_Analog.d \
MCAL/Target/MCUAbstraction/MCU_FreqCntInput.d \
MCAL/Target/MCUAbstraction/MCU_Gauge.d \
MCAL/Target/MCUAbstraction/MCU_SwitchMode.d \
MCAL/Target/MCUAbstraction/SGL.d \
MCAL/Target/MCUAbstraction/Watchdog.d \

S_DEPS += \

C_DEPS_QUOTED += \
"MCAL/Target/MCUAbstraction/emios.d" \
"MCAL/Target/MCUAbstraction/LCD.d" \
"MCAL/Target/MCUAbstraction/MCU_Analog.d" \
"MCAL/Target/MCUAbstraction/MCU_FreqCntInput.d" \
"MCAL/Target/MCUAbstraction/MCU_Gauge.d" \
"MCAL/Target/MCUAbstraction/MCU_SwitchMode.d" \
"MCAL/Target/MCUAbstraction/SGL.d" \
"MCAL/Target/MCUAbstraction/Watchdog.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
MCAL/Target/MCUAbstraction/emios.obj: ../Source/MCAL/Target/MCUAbstraction/emios.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/MCUAbstraction/emios.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/MCUAbstraction/LCD.obj: ../Source/MCAL/Target/MCUAbstraction/LCD.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/MCUAbstraction/LCD.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/MCUAbstraction/MCU_Analog.obj: ../Source/MCAL/Target/MCUAbstraction/MCU_Analog.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/MCUAbstraction/MCU_Analog.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/MCUAbstraction/MCU_FreqCntInput.obj: ../Source/MCAL/Target/MCUAbstraction/MCU_FreqCntInput.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/MCUAbstraction/MCU_FreqCntInput.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/MCUAbstraction/MCU_Gauge.obj: ../Source/MCAL/Target/MCUAbstraction/MCU_Gauge.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/MCUAbstraction/MCU_Gauge.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/MCUAbstraction/MCU_SwitchMode.obj: ../Source/MCAL/Target/MCUAbstraction/MCU_SwitchMode.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/MCUAbstraction/MCU_SwitchMode.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/MCUAbstraction/SGL.obj: ../Source/MCAL/Target/MCUAbstraction/SGL.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/MCUAbstraction/SGL.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/MCUAbstraction/Watchdog.obj: ../Source/MCAL/Target/MCUAbstraction/Watchdog.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/MCUAbstraction/Watchdog.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/MCUAbstraction/%.d: ../Source/MCAL/Target/MCUAbstraction/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
