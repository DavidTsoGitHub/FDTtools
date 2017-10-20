################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Common/SecMLib/SecM/SecM.c" \

C_SRCS += \
../Sources/Common/SecMLib/SecM/SecM.c \

OBJS += \
./Sources/Common/SecMLib/SecM/SecM_c.obj \

OBJS_QUOTED += \
"./Sources/Common/SecMLib/SecM/SecM_c.obj" \

C_DEPS += \
./Sources/Common/SecMLib/SecM/SecM_c.d \

OBJS_OS_FORMAT += \
./Sources/Common/SecMLib/SecM/SecM_c.obj \

C_DEPS_QUOTED += \
"./Sources/Common/SecMLib/SecM/SecM_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Sources/Common/SecMLib/SecM/SecM_c.obj: ../Sources/Common/SecMLib/SecM/SecM.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Common/SecMLib/SecM/SecM.args" -o "Sources/Common/SecMLib/SecM/SecM_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Common/SecMLib/SecM/%.d: ../Sources/Common/SecMLib/SecM/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


