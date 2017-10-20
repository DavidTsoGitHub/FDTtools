################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Boards/DETC/Board.c" \

C_SRCS += \
../Sources/Boards/DETC/Board.c \

OBJS += \
./Sources/Boards/DETC/Board_c.obj \

OBJS_QUOTED += \
"./Sources/Boards/DETC/Board_c.obj" \

C_DEPS += \
./Sources/Boards/DETC/Board_c.d \

OBJS_OS_FORMAT += \
./Sources/Boards/DETC/Board_c.obj \

C_DEPS_QUOTED += \
"./Sources/Boards/DETC/Board_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Sources/Boards/DETC/Board_c.obj: ../Sources/Boards/DETC/Board.c
	@echo 'Building file: $<'
	@echo 'Executing target #20 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Boards/DETC/Board.args" -o "Sources/Boards/DETC/Board_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Boards/DETC/%.d: ../Sources/Boards/DETC/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


