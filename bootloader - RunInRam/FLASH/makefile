################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 32
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := "$(GNU_Make_Install_DirEnv)/rm" -f

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Sources/subdir.mk
-include Sources/Common/subdir.mk
-include Sources/Common/SecMLib/SecM/subdir.mk
-include Sources/Common/SecMLib/Cal/subdir.mk
-include Sources/Common/Diagnostic/Dcm/subdir.mk
-include Sources/Common/Diagnostic/CanTp/subdir.mk
-include Sources/Common/Diagnostic/Can/subdir.mk
-include Sources/Boards/DETC/Config/subdir.mk
-include Sources/Boards/DETC/subdir.mk
-include Sources/Arch/ppc/mpc55xx/drivers/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
STATICLIB += \

STATICLIB_QUOTED += \

STATICLIB_OS_FORMAT += \

EXECUTABLES += \

EXECUTABLES_QUOTED += \

EXECUTABLES_OS_FORMAT += \


# All Target
all: warning-messages explicit-dependencies D320_bootloader.elf

# Tool invocations
D320_bootloader.elf: $(OBJS) $(USER_OBJS) ../Project_Settings/Linker_Files/MPC5645S_FLASH.lcf
	@echo 'Building target: $@'
	@echo 'Executing target #33 $@'
	@echo 'Invoking: PowerPC Linker'
	"$(PAToolsDirEnv)/mwldeppc" -o "D320_bootloader.elf" @@"D320_bootloader.args"  $(USER_OBJS_QUOTED)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(S_DEPS_QUOTED) $(STATICLIB_QUOTED) "./*/*/*.obj" "./*/*/*/*/*/*.obj" "./*/*/*/*.obj" "./*/*/*/*/*.obj" "./*/*.obj"  $(CPP_DEPS_QUOTED) $(CC_DEPS_QUOTED) $(S_UPPER_DEPS_QUOTED) $(CXX_DEPS_QUOTED) $(ASM_DEPS_QUOTED) $(ASM_UPPER_DEPS_QUOTED) $(C_UPPER_DEPS_QUOTED) $(C++_DEPS_QUOTED) $(EXECUTABLES_QUOTED) "./*/*/*.d" "./*/*/*/*/*/*.d" "./*/*/*/*.d" "./*/*/*/*/*.d" "./*/*.d"  $(ProjDirPath)/FLASH/D320_bootloader.elf
	-@echo ' '

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:

-include ../makefile.targets

explicit-dependencies: ../Project_Settings/Linker_Files/MPC5645S_FLASH.lcf 

warning-messages: 

