################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Boards/DETC/Config/Can_Cfg.c" \
"../Sources/Boards/DETC/Config/FlashLoader_Cfg.c" \
"../Sources/Boards/DETC/Config/Fls_Cfg.c" \

C_SRCS += \
../Sources/Boards/DETC/Config/Can_Cfg.c \
../Sources/Boards/DETC/Config/FlashLoader_Cfg.c \
../Sources/Boards/DETC/Config/Fls_Cfg.c \

OBJS += \
./Sources/Boards/DETC/Config/Can_Cfg_c.obj \
./Sources/Boards/DETC/Config/FlashLoader_Cfg_c.obj \
./Sources/Boards/DETC/Config/Fls_Cfg_c.obj \

OBJS_QUOTED += \
"./Sources/Boards/DETC/Config/Can_Cfg_c.obj" \
"./Sources/Boards/DETC/Config/FlashLoader_Cfg_c.obj" \
"./Sources/Boards/DETC/Config/Fls_Cfg_c.obj" \

C_DEPS += \
./Sources/Boards/DETC/Config/Can_Cfg_c.d \
./Sources/Boards/DETC/Config/FlashLoader_Cfg_c.d \
./Sources/Boards/DETC/Config/Fls_Cfg_c.d \

OBJS_OS_FORMAT += \
./Sources/Boards/DETC/Config/Can_Cfg_c.obj \
./Sources/Boards/DETC/Config/FlashLoader_Cfg_c.obj \
./Sources/Boards/DETC/Config/Fls_Cfg_c.obj \

C_DEPS_QUOTED += \
"./Sources/Boards/DETC/Config/Can_Cfg_c.d" \
"./Sources/Boards/DETC/Config/FlashLoader_Cfg_c.d" \
"./Sources/Boards/DETC/Config/Fls_Cfg_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Sources/Boards/DETC/Config/Can_Cfg_c.obj: ../Sources/Boards/DETC/Config/Can_Cfg.c
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Boards/DETC/Config/Can_Cfg.args" -o "Sources/Boards/DETC/Config/Can_Cfg_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Boards/DETC/Config/%.d: ../Sources/Boards/DETC/Config/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/Boards/DETC/Config/FlashLoader_Cfg_c.obj: ../Sources/Boards/DETC/Config/FlashLoader_Cfg.c
	@echo 'Building file: $<'
	@echo 'Executing target #18 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Boards/DETC/Config/FlashLoader_Cfg.args" -o "Sources/Boards/DETC/Config/FlashLoader_Cfg_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Boards/DETC/Config/Fls_Cfg_c.obj: ../Sources/Boards/DETC/Config/Fls_Cfg.c
	@echo 'Building file: $<'
	@echo 'Executing target #19 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Boards/DETC/Config/Fls_Cfg.args" -o "Sources/Boards/DETC/Config/Fls_Cfg_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


