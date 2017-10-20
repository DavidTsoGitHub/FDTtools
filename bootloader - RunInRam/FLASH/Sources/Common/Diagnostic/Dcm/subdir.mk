################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Common/Diagnostic/Dcm/Dcm.c" \
"../Sources/Common/Diagnostic/Dcm/Dcm_Cfg.c" \
"../Sources/Common/Diagnostic/Dcm/Dcm_Dsp.c" \

C_SRCS += \
../Sources/Common/Diagnostic/Dcm/Dcm.c \
../Sources/Common/Diagnostic/Dcm/Dcm_Cfg.c \
../Sources/Common/Diagnostic/Dcm/Dcm_Dsp.c \

OBJS += \
./Sources/Common/Diagnostic/Dcm/Dcm_c.obj \
./Sources/Common/Diagnostic/Dcm/Dcm_Cfg_c.obj \
./Sources/Common/Diagnostic/Dcm/Dcm_Dsp_c.obj \

OBJS_QUOTED += \
"./Sources/Common/Diagnostic/Dcm/Dcm_c.obj" \
"./Sources/Common/Diagnostic/Dcm/Dcm_Cfg_c.obj" \
"./Sources/Common/Diagnostic/Dcm/Dcm_Dsp_c.obj" \

C_DEPS += \
./Sources/Common/Diagnostic/Dcm/Dcm_c.d \
./Sources/Common/Diagnostic/Dcm/Dcm_Cfg_c.d \
./Sources/Common/Diagnostic/Dcm/Dcm_Dsp_c.d \

OBJS_OS_FORMAT += \
./Sources/Common/Diagnostic/Dcm/Dcm_c.obj \
./Sources/Common/Diagnostic/Dcm/Dcm_Cfg_c.obj \
./Sources/Common/Diagnostic/Dcm/Dcm_Dsp_c.obj \

C_DEPS_QUOTED += \
"./Sources/Common/Diagnostic/Dcm/Dcm_c.d" \
"./Sources/Common/Diagnostic/Dcm/Dcm_Cfg_c.d" \
"./Sources/Common/Diagnostic/Dcm/Dcm_Dsp_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Sources/Common/Diagnostic/Dcm/Dcm_c.obj: ../Sources/Common/Diagnostic/Dcm/Dcm.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Common/Diagnostic/Dcm/Dcm.args" -o "Sources/Common/Diagnostic/Dcm/Dcm_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Common/Diagnostic/Dcm/%.d: ../Sources/Common/Diagnostic/Dcm/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/Common/Diagnostic/Dcm/Dcm_Cfg_c.obj: ../Sources/Common/Diagnostic/Dcm/Dcm_Cfg.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Common/Diagnostic/Dcm/Dcm_Cfg.args" -o "Sources/Common/Diagnostic/Dcm/Dcm_Cfg_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Common/Diagnostic/Dcm/Dcm_Dsp_c.obj: ../Sources/Common/Diagnostic/Dcm/Dcm_Dsp.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Common/Diagnostic/Dcm/Dcm_Dsp.args" -o "Sources/Common/Diagnostic/Dcm/Dcm_Dsp_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


