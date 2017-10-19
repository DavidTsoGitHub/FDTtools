################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/platform/GCL/src/GCL.c" \

C_SRCS += \
../Source/platform/GCL/src/GCL.c \

S_SRCS += \

OBJS += \
platform/GCL/src/GCL.obj \

OBJS_QUOTED += \
"platform/GCL/src/GCL.obj" \

C_DEPS += \
platform/GCL/src/GCL.d \

S_DEPS += \

C_DEPS_QUOTED += \
"platform/GCL/src/GCL.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
platform/GCL/src/GCL.obj: ../Source/platform/GCL/src/GCL.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "platform/GCL/src/GCL.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

platform/GCL/src/%.d: ../Source/platform/GCL/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
