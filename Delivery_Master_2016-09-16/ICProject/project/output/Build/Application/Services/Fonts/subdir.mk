################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
S_SRCS_QUOTED += \

C_SRCS_QUOTED += \
"../Source/Application/Services/Fonts/font1.c" \
"../Source/Application/Services/Fonts/font2.c" \
"../Source/Application/Services/Fonts/font4.c" \
"../Source/Application/Services/Fonts/font5.c" \
"../Source/Application/Services/Fonts/font6.c" \

C_SRCS += \
../Source/Application/Services/Fonts/font1.c \
../Source/Application/Services/Fonts/font2.c \
../Source/Application/Services/Fonts/font4.c \
../Source/Application/Services/Fonts/font5.c \
../Source/Application/Services/Fonts/font6.c \

S_SRCS += \

OBJS += \
Application/Services/Fonts/font1.obj \
Application/Services/Fonts/font2.obj \
Application/Services/Fonts/font4.obj \
Application/Services/Fonts/font5.obj \
Application/Services/Fonts/font6.obj \

OBJS_QUOTED += \
"Application/Services/Fonts/font1.obj" \
"Application/Services/Fonts/font2.obj" \
"Application/Services/Fonts/font4.obj" \
"Application/Services/Fonts/font5.obj" \
"Application/Services/Fonts/font6.obj" \

C_DEPS += \
Application/Services/Fonts/font1.d \
Application/Services/Fonts/font2.d \
Application/Services/Fonts/font4.d \
Application/Services/Fonts/font5.d \
Application/Services/Fonts/font6.d \

S_DEPS += \

C_DEPS_QUOTED += \
"Application/Services/Fonts/font1.d" \
"Application/Services/Fonts/font2.d" \
"Application/Services/Fonts/font4.d" \
"Application/Services/Fonts/font5.d" \
"Application/Services/Fonts/font6.d" \

S_DEPS_QUOTED += \



# Each subdirectory must supply rules for building sources it contributes
Application/Services/Fonts/font1.obj: ../Source/Application/Services/Fonts/font1.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/Fonts/font1.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/Fonts/font2.obj: ../Source/Application/Services/Fonts/font2.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/Fonts/font2.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/Fonts/font4.obj: ../Source/Application/Services/Fonts/font4.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/Fonts/font4.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/Fonts/font5.obj: ../Source/Application/Services/Fonts/font5.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/Fonts/font5.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/Fonts/font6.obj: ../Source/Application/Services/Fonts/font6.c
	@echo 'Building file: $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"MPC5606S.obj.args" -o "Application/Services/Fonts/font6.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Application/Services/Fonts/%.d: ../Source/Application/Services/Fonts/%.c
	@echo 'Regenerating dependency file: $@'

	@echo ' '
