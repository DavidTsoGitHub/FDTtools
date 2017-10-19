################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/MCAL/Target/ECUAbstraction/AD_Convert.c" \
"../Source/MCAL/Target/ECUAbstraction/Analog.c" \
"../Source/MCAL/Target/ECUAbstraction/Discrete.c" \
"../Source/MCAL/Target/ECUAbstraction/Display.c" \
"../Source/MCAL/Target/ECUAbstraction/ECU_DataFlash.c" \
"../Source/MCAL/Target/ECUAbstraction/FreqCntInput.c" \
"../Source/MCAL/Target/ECUAbstraction/Gauge.c" \
"../Source/MCAL/Target/ECUAbstraction/illuminations.c" \
"../Source/MCAL/Target/ECUAbstraction/Indicator.c" \
"../Source/MCAL/Target/ECUAbstraction/Memory.c" \
"../Source/MCAL/Target/ECUAbstraction/ModeSwitch.c" \
"../Source/MCAL/Target/ECUAbstraction/RTC.c" \
"../Source/MCAL/Target/ECUAbstraction/Sound.c" \

C_SRCS += \
../Source/MCAL/Target/ECUAbstraction/AD_Convert.c \
../Source/MCAL/Target/ECUAbstraction/Analog.c \
../Source/MCAL/Target/ECUAbstraction/Discrete.c \
../Source/MCAL/Target/ECUAbstraction/Display.c \
../Source/MCAL/Target/ECUAbstraction/ECU_DataFlash.c \
../Source/MCAL/Target/ECUAbstraction/FreqCntInput.c \
../Source/MCAL/Target/ECUAbstraction/Gauge.c \
../Source/MCAL/Target/ECUAbstraction/illuminations.c \
../Source/MCAL/Target/ECUAbstraction/Indicator.c \
../Source/MCAL/Target/ECUAbstraction/Memory.c \
../Source/MCAL/Target/ECUAbstraction/ModeSwitch.c \
../Source/MCAL/Target/ECUAbstraction/RTC.c \
../Source/MCAL/Target/ECUAbstraction/Sound.c \

S_SRCS += \

OBJS += \
MCAL/Target/ECUAbstraction/AD_Convert.obj \
MCAL/Target/ECUAbstraction/Analog.obj \
MCAL/Target/ECUAbstraction/Discrete.obj \
MCAL/Target/ECUAbstraction/Display.obj \
MCAL/Target/ECUAbstraction/ECU_DataFlash.obj \
MCAL/Target/ECUAbstraction/FreqCntInput.obj \
MCAL/Target/ECUAbstraction/Gauge.obj \
MCAL/Target/ECUAbstraction/illuminations.obj \
MCAL/Target/ECUAbstraction/Indicator.obj \
MCAL/Target/ECUAbstraction/Memory.obj \
MCAL/Target/ECUAbstraction/ModeSwitch.obj \
MCAL/Target/ECUAbstraction/RTC.obj \
MCAL/Target/ECUAbstraction/Sound.obj \

OBJS_QUOTED += \
"MCAL/Target/ECUAbstraction/AD_Convert.obj" \
"MCAL/Target/ECUAbstraction/Analog.obj" \
"MCAL/Target/ECUAbstraction/Discrete.obj" \
"MCAL/Target/ECUAbstraction/Display.obj" \
"MCAL/Target/ECUAbstraction/ECU_DataFlash.obj" \
"MCAL/Target/ECUAbstraction/FreqCntInput.obj" \
"MCAL/Target/ECUAbstraction/Gauge.obj" \
"MCAL/Target/ECUAbstraction/illuminations.obj" \
"MCAL/Target/ECUAbstraction/Indicator.obj" \
"MCAL/Target/ECUAbstraction/Memory.obj" \
"MCAL/Target/ECUAbstraction/ModeSwitch.obj" \
"MCAL/Target/ECUAbstraction/RTC.obj" \
"MCAL/Target/ECUAbstraction/Sound.obj" \

C_DEPS += \
MCAL/Target/ECUAbstraction/AD_Convert.d \
MCAL/Target/ECUAbstraction/Analog.d \
MCAL/Target/ECUAbstraction/Discrete.d \
MCAL/Target/ECUAbstraction/Display.d \
MCAL/Target/ECUAbstraction/ECU_DataFlash.d \
MCAL/Target/ECUAbstraction/FreqCntInput.d \
MCAL/Target/ECUAbstraction/Gauge.d \
MCAL/Target/ECUAbstraction/illuminations.d \
MCAL/Target/ECUAbstraction/Indicator.d \
MCAL/Target/ECUAbstraction/Memory.d \
MCAL/Target/ECUAbstraction/ModeSwitch.d \
MCAL/Target/ECUAbstraction/RTC.d \
MCAL/Target/ECUAbstraction/Sound.d \

S_DEPS += \

C_DEPS_QUOTED += \
"MCAL/Target/ECUAbstraction/AD_Convert.d" \
"MCAL/Target/ECUAbstraction/Analog.d" \
"MCAL/Target/ECUAbstraction/Discrete.d" \
"MCAL/Target/ECUAbstraction/Display.d" \
"MCAL/Target/ECUAbstraction/ECU_DataFlash.d" \
"MCAL/Target/ECUAbstraction/FreqCntInput.d" \
"MCAL/Target/ECUAbstraction/Gauge.d" \
"MCAL/Target/ECUAbstraction/illuminations.d" \
"MCAL/Target/ECUAbstraction/Indicator.d" \
"MCAL/Target/ECUAbstraction/Memory.d" \
"MCAL/Target/ECUAbstraction/ModeSwitch.d" \
"MCAL/Target/ECUAbstraction/RTC.d" \
"MCAL/Target/ECUAbstraction/Sound.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
MCAL/Target/ECUAbstraction/AD_Convert.obj: ../Source/MCAL/Target/ECUAbstraction/AD_Convert.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/ECUAbstraction/AD_Convert.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/ECUAbstraction/Analog.obj: ../Source/MCAL/Target/ECUAbstraction/Analog.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/ECUAbstraction/Analog.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/ECUAbstraction/Discrete.obj: ../Source/MCAL/Target/ECUAbstraction/Discrete.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/ECUAbstraction/Discrete.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/ECUAbstraction/Display.obj: ../Source/MCAL/Target/ECUAbstraction/Display.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/ECUAbstraction/Display.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/ECUAbstraction/ECU_DataFlash.obj: ../Source/MCAL/Target/ECUAbstraction/ECU_DataFlash.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/ECUAbstraction/ECU_DataFlash.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/ECUAbstraction/FreqCntInput.obj: ../Source/MCAL/Target/ECUAbstraction/FreqCntInput.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/ECUAbstraction/FreqCntInput.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/ECUAbstraction/Gauge.obj: ../Source/MCAL/Target/ECUAbstraction/Gauge.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/ECUAbstraction/Gauge.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/ECUAbstraction/illuminations.obj: ../Source/MCAL/Target/ECUAbstraction/illuminations.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/ECUAbstraction/illuminations.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/ECUAbstraction/Indicator.obj: ../Source/MCAL/Target/ECUAbstraction/Indicator.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/ECUAbstraction/Indicator.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/ECUAbstraction/Memory.obj: ../Source/MCAL/Target/ECUAbstraction/Memory.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/ECUAbstraction/Memory.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/ECUAbstraction/ModeSwitch.obj: ../Source/MCAL/Target/ECUAbstraction/ModeSwitch.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/ECUAbstraction/ModeSwitch.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/ECUAbstraction/RTC.obj: ../Source/MCAL/Target/ECUAbstraction/RTC.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/ECUAbstraction/RTC.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/ECUAbstraction/Sound.obj: ../Source/MCAL/Target/ECUAbstraction/Sound.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "MCAL/Target/ECUAbstraction/Sound.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

MCAL/Target/ECUAbstraction/%.d: ../Source/MCAL/Target/ECUAbstraction/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
