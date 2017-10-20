################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Common/Diagnostic/Can/Can.c" \

C_SRCS += \
../Sources/Common/Diagnostic/Can/Can.c \

OBJS += \
./Sources/Common/Diagnostic/Can/Can_c.obj \

OBJS_QUOTED += \
"./Sources/Common/Diagnostic/Can/Can_c.obj" \

C_DEPS += \
./Sources/Common/Diagnostic/Can/Can_c.d \

OBJS_OS_FORMAT += \
./Sources/Common/Diagnostic/Can/Can_c.obj \

C_DEPS_QUOTED += \
"./Sources/Common/Diagnostic/Can/Can_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Sources/Common/Diagnostic/Can/Can_c.obj: ../Sources/Common/Diagnostic/Can/Can.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Common/Diagnostic/Can/Can.args" -o "Sources/Common/Diagnostic/Can/Can_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Common/Diagnostic/Can/%.d: ../Sources/Common/Diagnostic/Can/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


