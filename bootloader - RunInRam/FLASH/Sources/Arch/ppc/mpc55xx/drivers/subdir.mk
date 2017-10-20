################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Arch/ppc/mpc55xx/drivers/Fls.c" \
"../Sources/Arch/ppc/mpc55xx/drivers/Gpt.c" \
"../Sources/Arch/ppc/mpc55xx/drivers/Mcu.c" \
"../Sources/Arch/ppc/mpc55xx/drivers/Nvm.c" \
"../Sources/Arch/ppc/mpc55xx/drivers/QuadSPI.c" \
"../Sources/Arch/ppc/mpc55xx/drivers/Wdg.c" \
"../Sources/Arch/ppc/mpc55xx/drivers/flash_h7f_c90.c" \
"../Sources/Arch/ppc/mpc55xx/drivers/flash_ll_h7f_c90.c" \

C_SRCS += \
../Sources/Arch/ppc/mpc55xx/drivers/Fls.c \
../Sources/Arch/ppc/mpc55xx/drivers/Gpt.c \
../Sources/Arch/ppc/mpc55xx/drivers/Mcu.c \
../Sources/Arch/ppc/mpc55xx/drivers/Nvm.c \
../Sources/Arch/ppc/mpc55xx/drivers/QuadSPI.c \
../Sources/Arch/ppc/mpc55xx/drivers/Wdg.c \
../Sources/Arch/ppc/mpc55xx/drivers/flash_h7f_c90.c \
../Sources/Arch/ppc/mpc55xx/drivers/flash_ll_h7f_c90.c \

OBJS += \
./Sources/Arch/ppc/mpc55xx/drivers/Fls_c.obj \
./Sources/Arch/ppc/mpc55xx/drivers/Gpt_c.obj \
./Sources/Arch/ppc/mpc55xx/drivers/Mcu_c.obj \
./Sources/Arch/ppc/mpc55xx/drivers/Nvm_c.obj \
./Sources/Arch/ppc/mpc55xx/drivers/QuadSPI_c.obj \
./Sources/Arch/ppc/mpc55xx/drivers/Wdg_c.obj \
./Sources/Arch/ppc/mpc55xx/drivers/flash_h7f_c90_c.obj \
./Sources/Arch/ppc/mpc55xx/drivers/flash_ll_h7f_c90_c.obj \

OBJS_QUOTED += \
"./Sources/Arch/ppc/mpc55xx/drivers/Fls_c.obj" \
"./Sources/Arch/ppc/mpc55xx/drivers/Gpt_c.obj" \
"./Sources/Arch/ppc/mpc55xx/drivers/Mcu_c.obj" \
"./Sources/Arch/ppc/mpc55xx/drivers/Nvm_c.obj" \
"./Sources/Arch/ppc/mpc55xx/drivers/QuadSPI_c.obj" \
"./Sources/Arch/ppc/mpc55xx/drivers/Wdg_c.obj" \
"./Sources/Arch/ppc/mpc55xx/drivers/flash_h7f_c90_c.obj" \
"./Sources/Arch/ppc/mpc55xx/drivers/flash_ll_h7f_c90_c.obj" \

C_DEPS += \
./Sources/Arch/ppc/mpc55xx/drivers/Fls_c.d \
./Sources/Arch/ppc/mpc55xx/drivers/Gpt_c.d \
./Sources/Arch/ppc/mpc55xx/drivers/Mcu_c.d \
./Sources/Arch/ppc/mpc55xx/drivers/Nvm_c.d \
./Sources/Arch/ppc/mpc55xx/drivers/QuadSPI_c.d \
./Sources/Arch/ppc/mpc55xx/drivers/Wdg_c.d \
./Sources/Arch/ppc/mpc55xx/drivers/flash_h7f_c90_c.d \
./Sources/Arch/ppc/mpc55xx/drivers/flash_ll_h7f_c90_c.d \

OBJS_OS_FORMAT += \
./Sources/Arch/ppc/mpc55xx/drivers/Fls_c.obj \
./Sources/Arch/ppc/mpc55xx/drivers/Gpt_c.obj \
./Sources/Arch/ppc/mpc55xx/drivers/Mcu_c.obj \
./Sources/Arch/ppc/mpc55xx/drivers/Nvm_c.obj \
./Sources/Arch/ppc/mpc55xx/drivers/QuadSPI_c.obj \
./Sources/Arch/ppc/mpc55xx/drivers/Wdg_c.obj \
./Sources/Arch/ppc/mpc55xx/drivers/flash_h7f_c90_c.obj \
./Sources/Arch/ppc/mpc55xx/drivers/flash_ll_h7f_c90_c.obj \

C_DEPS_QUOTED += \
"./Sources/Arch/ppc/mpc55xx/drivers/Fls_c.d" \
"./Sources/Arch/ppc/mpc55xx/drivers/Gpt_c.d" \
"./Sources/Arch/ppc/mpc55xx/drivers/Mcu_c.d" \
"./Sources/Arch/ppc/mpc55xx/drivers/Nvm_c.d" \
"./Sources/Arch/ppc/mpc55xx/drivers/QuadSPI_c.d" \
"./Sources/Arch/ppc/mpc55xx/drivers/Wdg_c.d" \
"./Sources/Arch/ppc/mpc55xx/drivers/flash_h7f_c90_c.d" \
"./Sources/Arch/ppc/mpc55xx/drivers/flash_ll_h7f_c90_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Sources/Arch/ppc/mpc55xx/drivers/Fls_c.obj: ../Sources/Arch/ppc/mpc55xx/drivers/Fls.c
	@echo 'Building file: $<'
	@echo 'Executing target #21 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Arch/ppc/mpc55xx/drivers/Fls.args" -o "Sources/Arch/ppc/mpc55xx/drivers/Fls_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Arch/ppc/mpc55xx/drivers/%.d: ../Sources/Arch/ppc/mpc55xx/drivers/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/Arch/ppc/mpc55xx/drivers/Gpt_c.obj: ../Sources/Arch/ppc/mpc55xx/drivers/Gpt.c
	@echo 'Building file: $<'
	@echo 'Executing target #22 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Arch/ppc/mpc55xx/drivers/Gpt.args" -o "Sources/Arch/ppc/mpc55xx/drivers/Gpt_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Arch/ppc/mpc55xx/drivers/Mcu_c.obj: ../Sources/Arch/ppc/mpc55xx/drivers/Mcu.c
	@echo 'Building file: $<'
	@echo 'Executing target #23 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Arch/ppc/mpc55xx/drivers/Mcu.args" -o "Sources/Arch/ppc/mpc55xx/drivers/Mcu_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Arch/ppc/mpc55xx/drivers/Nvm_c.obj: ../Sources/Arch/ppc/mpc55xx/drivers/Nvm.c
	@echo 'Building file: $<'
	@echo 'Executing target #24 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Arch/ppc/mpc55xx/drivers/Nvm.args" -o "Sources/Arch/ppc/mpc55xx/drivers/Nvm_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Arch/ppc/mpc55xx/drivers/QuadSPI_c.obj: ../Sources/Arch/ppc/mpc55xx/drivers/QuadSPI.c
	@echo 'Building file: $<'
	@echo 'Executing target #25 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Arch/ppc/mpc55xx/drivers/QuadSPI.args" -o "Sources/Arch/ppc/mpc55xx/drivers/QuadSPI_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Arch/ppc/mpc55xx/drivers/Wdg_c.obj: ../Sources/Arch/ppc/mpc55xx/drivers/Wdg.c
	@echo 'Building file: $<'
	@echo 'Executing target #26 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Arch/ppc/mpc55xx/drivers/Wdg.args" -o "Sources/Arch/ppc/mpc55xx/drivers/Wdg_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Arch/ppc/mpc55xx/drivers/flash_h7f_c90_c.obj: ../Sources/Arch/ppc/mpc55xx/drivers/flash_h7f_c90.c
	@echo 'Building file: $<'
	@echo 'Executing target #27 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Arch/ppc/mpc55xx/drivers/flash_h7f_c90.args" -o "Sources/Arch/ppc/mpc55xx/drivers/flash_h7f_c90_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Arch/ppc/mpc55xx/drivers/flash_ll_h7f_c90_c.obj: ../Sources/Arch/ppc/mpc55xx/drivers/flash_ll_h7f_c90.c
	@echo 'Building file: $<'
	@echo 'Executing target #28 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Arch/ppc/mpc55xx/drivers/flash_ll_h7f_c90.args" -o "Sources/Arch/ppc/mpc55xx/drivers/flash_ll_h7f_c90_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


