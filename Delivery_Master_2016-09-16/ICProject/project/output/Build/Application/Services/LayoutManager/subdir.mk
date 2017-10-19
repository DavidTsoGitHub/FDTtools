################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/Services/LayoutManager/LayoutManager.c" \

C_SRCS += \
../Source/Application/Services/LayoutManager/LayoutManager.c \

S_SRCS += \

OBJS += \
Application/Services/LayoutManager/LayoutManager.obj \

OBJS_QUOTED += \
"Application/Services/LayoutManager/LayoutManager.obj" \

C_DEPS += \
Application/Services/LayoutManager/LayoutManager.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/Services/LayoutManager/LayoutManager.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/Services/LayoutManager/LayoutManager.obj: ../Source/Application/Services/LayoutManager/LayoutManager.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/LayoutManager/LayoutManager.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/LayoutManager/%.d: ../Source/Application/Services/LayoutManager/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
