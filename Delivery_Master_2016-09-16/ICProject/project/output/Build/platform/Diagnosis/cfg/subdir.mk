################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/platform/Diagnosis/cfg/DIDCALMapping.c" \
"../Source/platform/Diagnosis/cfg/DTCManager.c" \

C_SRCS += \
../Source/platform/Diagnosis/cfg/DIDCALMapping.c \
../Source/platform/Diagnosis/cfg/DTCManager.c \

S_SRCS += \

OBJS += \
platform/Diagnosis/cfg/DIDCALMapping.obj \
platform/Diagnosis/cfg/DTCManager.obj \

OBJS_QUOTED += \
"platform/Diagnosis/cfg/DIDCALMapping.obj" \
"platform/Diagnosis/cfg/DTCManager.obj" \

C_DEPS += \
platform/Diagnosis/cfg/DIDCALMapping.d \
platform/Diagnosis/cfg/DTCManager.d \

S_DEPS += \

C_DEPS_QUOTED += \
"platform/Diagnosis/cfg/DIDCALMapping.d" \
"platform/Diagnosis/cfg/DTCManager.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
platform/Diagnosis/cfg/DIDCALMapping.obj: ../Source/platform/Diagnosis/cfg/DIDCALMapping.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "platform/Diagnosis/cfg/DIDCALMapping.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

platform/Diagnosis/cfg/DTCManager.obj: ../Source/platform/Diagnosis/cfg/DTCManager.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "platform/Diagnosis/cfg/DTCManager.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

platform/Diagnosis/cfg/%.d: ../Source/platform/Diagnosis/cfg/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
