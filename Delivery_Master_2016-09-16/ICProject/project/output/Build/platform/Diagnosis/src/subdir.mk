################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/platform/Diagnosis/src/Diagnosis.c" \
"../Source/platform/Diagnosis/src/J1939.c" \
"../Source/platform/Diagnosis/src/J1939_Adapt.c" \
"../Source/platform/Diagnosis/src/Transport.c" \
"../Source/platform/Diagnosis/src/UDSRes.c" \

C_SRCS += \
../Source/platform/Diagnosis/src/Diagnosis.c \
../Source/platform/Diagnosis/src/J1939.c \
../Source/platform/Diagnosis/src/J1939_Adapt.c \
../Source/platform/Diagnosis/src/Transport.c \
../Source/platform/Diagnosis/src/UDSRes.c \

S_SRCS += \

OBJS += \
platform/Diagnosis/src/Diagnosis.obj \
platform/Diagnosis/src/J1939.obj \
platform/Diagnosis/src/J1939_Adapt.obj \
platform/Diagnosis/src/Transport.obj \
platform/Diagnosis/src/UDSRes.obj \

OBJS_QUOTED += \
"platform/Diagnosis/src/Diagnosis.obj" \
"platform/Diagnosis/src/J1939.obj" \
"platform/Diagnosis/src/J1939_Adapt.obj" \
"platform/Diagnosis/src/Transport.obj" \
"platform/Diagnosis/src/UDSRes.obj" \

C_DEPS += \
platform/Diagnosis/src/Diagnosis.d \
platform/Diagnosis/src/J1939.d \
platform/Diagnosis/src/J1939_Adapt.d \
platform/Diagnosis/src/Transport.d \
platform/Diagnosis/src/UDSRes.d \

S_DEPS += \

C_DEPS_QUOTED += \
"platform/Diagnosis/src/Diagnosis.d" \
"platform/Diagnosis/src/J1939.d" \
"platform/Diagnosis/src/J1939_Adapt.d" \
"platform/Diagnosis/src/Transport.d" \
"platform/Diagnosis/src/UDSRes.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
platform/Diagnosis/src/Diagnosis.obj: ../Source/platform/Diagnosis/src/Diagnosis.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "platform/Diagnosis/src/Diagnosis.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

platform/Diagnosis/src/J1939.obj: ../Source/platform/Diagnosis/src/J1939.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "platform/Diagnosis/src/J1939.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

platform/Diagnosis/src/J1939_Adapt.obj: ../Source/platform/Diagnosis/src/J1939_Adapt.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "platform/Diagnosis/src/J1939_Adapt.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

platform/Diagnosis/src/Transport.obj: ../Source/platform/Diagnosis/src/Transport.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "platform/Diagnosis/src/Transport.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

platform/Diagnosis/src/UDSRes.obj: ../Source/platform/Diagnosis/src/UDSRes.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "platform/Diagnosis/src/UDSRes.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

platform/Diagnosis/src/%.d: ../Source/platform/Diagnosis/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
