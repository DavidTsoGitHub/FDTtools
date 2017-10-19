################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/platform/Scheduler/src/scheduler.c" \

C_SRCS += \
../Source/platform/Scheduler/src/scheduler.c \

S_SRCS += \

OBJS += \
platform/Scheduler/src/scheduler.obj \

OBJS_QUOTED += \
"platform/Scheduler/src/scheduler.obj" \

C_DEPS += \
platform/Scheduler/src/scheduler.d \

S_DEPS += \

C_DEPS_QUOTED += \
"platform/Scheduler/src/scheduler.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
platform/Scheduler/src/scheduler.obj: ../Source/platform/Scheduler/src/scheduler.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "platform/Scheduler/src/scheduler.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

platform/Scheduler/src/%.d: ../Source/platform/Scheduler/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
