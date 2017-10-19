################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/Services/SegmentDisplayManager/SegmentDisplayManager.c" \
"../Source/Application/Services/SegmentDisplayManager/SegmentDisplayManager_cfg.c" \

C_SRCS += \
../Source/Application/Services/SegmentDisplayManager/SegmentDisplayManager.c \
../Source/Application/Services/SegmentDisplayManager/SegmentDisplayManager_cfg.c \

S_SRCS += \

OBJS += \
Application/Services/SegmentDisplayManager/SegmentDisplayManager.obj \
Application/Services/SegmentDisplayManager/SegmentDisplayManager_cfg.obj \

OBJS_QUOTED += \
"Application/Services/SegmentDisplayManager/SegmentDisplayManager.obj" \
"Application/Services/SegmentDisplayManager/SegmentDisplayManager_cfg.obj" \

C_DEPS += \
Application/Services/SegmentDisplayManager/SegmentDisplayManager.d \
Application/Services/SegmentDisplayManager/SegmentDisplayManager_cfg.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/Services/SegmentDisplayManager/SegmentDisplayManager.d" \
"Application/Services/SegmentDisplayManager/SegmentDisplayManager_cfg.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/Services/SegmentDisplayManager/SegmentDisplayManager.obj: ../Source/Application/Services/SegmentDisplayManager/SegmentDisplayManager.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/SegmentDisplayManager/SegmentDisplayManager.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/SegmentDisplayManager/SegmentDisplayManager_cfg.obj: ../Source/Application/Services/SegmentDisplayManager/SegmentDisplayManager_cfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/SegmentDisplayManager/SegmentDisplayManager_cfg.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/SegmentDisplayManager/%.d: ../Source/Application/Services/SegmentDisplayManager/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
