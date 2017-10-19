################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/platform/COM/src/CAN_IF.c" \
"../Source/platform/COM/src/CAN_IF_Cfg.c" \
"../Source/platform/COM/src/CAN_Message_Data.c" \
"../Source/platform/COM/src/COM.c" \
"../Source/platform/COM/src/COM_GCL_Relay.c" \
"../Source/platform/COM/src/COM_Stack.c" \
"../Source/platform/COM/src/PDU_Router.c" \

C_SRCS += \
../Source/platform/COM/src/CAN_IF.c \
../Source/platform/COM/src/CAN_IF_Cfg.c \
../Source/platform/COM/src/CAN_Message_Data.c \
../Source/platform/COM/src/COM.c \
../Source/platform/COM/src/COM_GCL_Relay.c \
../Source/platform/COM/src/COM_Stack.c \
../Source/platform/COM/src/PDU_Router.c \

S_SRCS += \

OBJS += \
platform/COM/src/CAN_IF.obj \
platform/COM/src/CAN_IF_Cfg.obj \
platform/COM/src/CAN_Message_Data.obj \
platform/COM/src/COM.obj \
platform/COM/src/COM_GCL_Relay.obj \
platform/COM/src/COM_Stack.obj \
platform/COM/src/PDU_Router.obj \

OBJS_QUOTED += \
"platform/COM/src/CAN_IF.obj" \
"platform/COM/src/CAN_IF_Cfg.obj" \
"platform/COM/src/CAN_Message_Data.obj" \
"platform/COM/src/COM.obj" \
"platform/COM/src/COM_GCL_Relay.obj" \
"platform/COM/src/COM_Stack.obj" \
"platform/COM/src/PDU_Router.obj" \

C_DEPS += \
platform/COM/src/CAN_IF.d \
platform/COM/src/CAN_IF_Cfg.d \
platform/COM/src/CAN_Message_Data.d \
platform/COM/src/COM.d \
platform/COM/src/COM_GCL_Relay.d \
platform/COM/src/COM_Stack.d \
platform/COM/src/PDU_Router.d \

S_DEPS += \

C_DEPS_QUOTED += \
"platform/COM/src/CAN_IF.d" \
"platform/COM/src/CAN_IF_Cfg.d" \
"platform/COM/src/CAN_Message_Data.d" \
"platform/COM/src/COM.d" \
"platform/COM/src/COM_GCL_Relay.d" \
"platform/COM/src/COM_Stack.d" \
"platform/COM/src/PDU_Router.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
platform/COM/src/CAN_IF.obj: ../Source/platform/COM/src/CAN_IF.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "platform/COM/src/CAN_IF.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

platform/COM/src/CAN_IF_Cfg.obj: ../Source/platform/COM/src/CAN_IF_Cfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "platform/COM/src/CAN_IF_Cfg.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

platform/COM/src/CAN_Message_Data.obj: ../Source/platform/COM/src/CAN_Message_Data.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "platform/COM/src/CAN_Message_Data.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

platform/COM/src/COM.obj: ../Source/platform/COM/src/COM.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "platform/COM/src/COM.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

platform/COM/src/COM_GCL_Relay.obj: ../Source/platform/COM/src/COM_GCL_Relay.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "platform/COM/src/COM_GCL_Relay.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

platform/COM/src/COM_Stack.obj: ../Source/platform/COM/src/COM_Stack.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "platform/COM/src/COM_Stack.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

platform/COM/src/PDU_Router.obj: ../Source/platform/COM/src/PDU_Router.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "platform/COM/src/PDU_Router.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

platform/COM/src/%.d: ../Source/platform/COM/src/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
