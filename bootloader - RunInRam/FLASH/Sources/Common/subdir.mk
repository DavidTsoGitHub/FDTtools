################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Common/Appl.c" \
"../Sources/Common/FlashLoader.c" \
"../Sources/Common/Stubs.c" \
"../Sources/Common/main.c" \
"../Sources/Common/memcmp.c" \

C_SRCS += \
../Sources/Common/Appl.c \
../Sources/Common/FlashLoader.c \
../Sources/Common/Stubs.c \
../Sources/Common/main.c \
../Sources/Common/memcmp.c \

OBJS += \
./Sources/Common/Appl_c.obj \
./Sources/Common/FlashLoader_c.obj \
./Sources/Common/Stubs_c.obj \
./Sources/Common/main_c.obj \
./Sources/Common/memcmp_c.obj \

OBJS_QUOTED += \
"./Sources/Common/Appl_c.obj" \
"./Sources/Common/FlashLoader_c.obj" \
"./Sources/Common/Stubs_c.obj" \
"./Sources/Common/main_c.obj" \
"./Sources/Common/memcmp_c.obj" \

C_DEPS += \
./Sources/Common/Appl_c.d \
./Sources/Common/FlashLoader_c.d \
./Sources/Common/Stubs_c.d \
./Sources/Common/main_c.d \
./Sources/Common/memcmp_c.d \

OBJS_OS_FORMAT += \
./Sources/Common/Appl_c.obj \
./Sources/Common/FlashLoader_c.obj \
./Sources/Common/Stubs_c.obj \
./Sources/Common/main_c.obj \
./Sources/Common/memcmp_c.obj \

C_DEPS_QUOTED += \
"./Sources/Common/Appl_c.d" \
"./Sources/Common/FlashLoader_c.d" \
"./Sources/Common/Stubs_c.d" \
"./Sources/Common/main_c.d" \
"./Sources/Common/memcmp_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Sources/Common/Appl_c.obj: ../Sources/Common/Appl.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Common/Appl.args" -o "Sources/Common/Appl_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Common/%.d: ../Sources/Common/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/Common/FlashLoader_c.obj: ../Sources/Common/FlashLoader.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Common/FlashLoader.args" -o "Sources/Common/FlashLoader_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Common/Stubs_c.obj: ../Sources/Common/Stubs.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Common/Stubs.args" -o "Sources/Common/Stubs_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Common/main_c.obj: ../Sources/Common/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Common/main.args" -o "Sources/Common/main_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Common/memcmp_c.obj: ../Sources/Common/memcmp.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Common/memcmp.args" -o "Sources/Common/memcmp_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


