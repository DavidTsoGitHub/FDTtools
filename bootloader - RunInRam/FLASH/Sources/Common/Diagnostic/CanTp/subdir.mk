################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Common/Diagnostic/CanTp/CanTp.c" \
"../Sources/Common/Diagnostic/CanTp/CanTp_Cfg.c" \

C_SRCS += \
../Sources/Common/Diagnostic/CanTp/CanTp.c \
../Sources/Common/Diagnostic/CanTp/CanTp_Cfg.c \

OBJS += \
./Sources/Common/Diagnostic/CanTp/CanTp_c.obj \
./Sources/Common/Diagnostic/CanTp/CanTp_Cfg_c.obj \

OBJS_QUOTED += \
"./Sources/Common/Diagnostic/CanTp/CanTp_c.obj" \
"./Sources/Common/Diagnostic/CanTp/CanTp_Cfg_c.obj" \

C_DEPS += \
./Sources/Common/Diagnostic/CanTp/CanTp_c.d \
./Sources/Common/Diagnostic/CanTp/CanTp_Cfg_c.d \

OBJS_OS_FORMAT += \
./Sources/Common/Diagnostic/CanTp/CanTp_c.obj \
./Sources/Common/Diagnostic/CanTp/CanTp_Cfg_c.obj \

C_DEPS_QUOTED += \
"./Sources/Common/Diagnostic/CanTp/CanTp_c.d" \
"./Sources/Common/Diagnostic/CanTp/CanTp_Cfg_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Sources/Common/Diagnostic/CanTp/CanTp_c.obj: ../Sources/Common/Diagnostic/CanTp/CanTp.c
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Common/Diagnostic/CanTp/CanTp.args" -o "Sources/Common/Diagnostic/CanTp/CanTp_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Common/Diagnostic/CanTp/%.d: ../Sources/Common/Diagnostic/CanTp/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/Common/Diagnostic/CanTp/CanTp_Cfg_c.obj: ../Sources/Common/Diagnostic/CanTp/CanTp_Cfg.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Common/Diagnostic/CanTp/CanTp_Cfg.args" -o "Sources/Common/Diagnostic/CanTp/CanTp_Cfg_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


