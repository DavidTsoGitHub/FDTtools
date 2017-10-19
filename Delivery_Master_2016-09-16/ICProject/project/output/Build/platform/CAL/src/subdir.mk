################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/platform/CAL/src/CAL.c" \
"../Source/platform/CAL/src/CAL_cfg.c" \

C_SRCS += \
../Source/platform/CAL/src/CAL.c \
../Source/platform/CAL/src/CAL_cfg.c \

S_SRCS += \

OBJS += \
platform/CAL/src/CAL.obj \
platform/CAL/src/CAL_cfg.obj \

OBJS_QUOTED += \
"platform/CAL/src/CAL.obj" \
"platform/CAL/src/CAL_cfg.obj" \

C_DEPS += \
platform/CAL/src/CAL.d \
platform/CAL/src/CAL_cfg.d \

S_DEPS += \

C_DEPS_QUOTED += \
"platform/CAL/src/CAL.d" \
"platform/CAL/src/CAL_cfg.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
platform/CAL/src/CAL.obj: ../Source/platform/CAL/src/CAL.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "platform/CAL/src/CAL.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

platform/CAL/src/CAL_cfg.obj: ../Source/platform/CAL/src/CAL_cfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "platform/CAL/src/CAL_cfg.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

platform/CAL/src/%.d: ../Source/platform/CAL/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
