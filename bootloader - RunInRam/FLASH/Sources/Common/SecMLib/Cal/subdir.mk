################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Common/SecMLib/Cal/Cal.c" \

C_SRCS += \
../Sources/Common/SecMLib/Cal/Cal.c \

OBJS += \
./Sources/Common/SecMLib/Cal/Cal_c.obj \

OBJS_QUOTED += \
"./Sources/Common/SecMLib/Cal/Cal_c.obj" \

C_DEPS += \
./Sources/Common/SecMLib/Cal/Cal_c.d \

OBJS_OS_FORMAT += \
./Sources/Common/SecMLib/Cal/Cal_c.obj \

C_DEPS_QUOTED += \
"./Sources/Common/SecMLib/Cal/Cal_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Sources/Common/SecMLib/Cal/Cal_c.obj: ../Sources/Common/SecMLib/Cal/Cal.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Common/SecMLib/Cal/Cal.args" -o "Sources/Common/SecMLib/Cal/Cal_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Common/SecMLib/Cal/%.d: ../Sources/Common/SecMLib/Cal/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


