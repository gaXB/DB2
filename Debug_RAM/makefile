################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Sources/drive/subdir.mk
-include Sources/SystemSave/subdir.mk
-include Sources/SystemData/subdir.mk
-include Sources/SYSTask/MainTask/subdir.mk
-include Sources/SYSTask/subdir.mk
-include Sources/PowerMange/subdir.mk
-include Sources/Module/subdir.mk
-include Sources/HardWareDrive/subdir.mk
-include Sources/Galib/subdir.mk
-include Sources/GWMMN/UDS/subdir.mk
-include Sources/GWMMN/NM/subdir.mk
-include Sources/GWMMN/InterLayCan/subdir.mk
-include Sources/GWMMN/DebugPrint/subdir.mk
-include Sources/GWMMN/Common/subdir.mk
-include Sources/GWMMN/Canif/subdir.mk
-include Sources/GWMMN/CANDrive/subdir.mk
-include Sources/CAUTO/subdir.mk
-include Sources/BSP/subdir.mk
-include Sources/subdir.mk
-include SDK/rtos/osif/subdir.mk
-include SDK/platform/drivers/src/pins/subdir.mk
-include SDK/platform/drivers/src/lpuart/subdir.mk
-include SDK/platform/drivers/src/lptmr/subdir.mk
-include SDK/platform/drivers/src/lin/subdir.mk
-include SDK/platform/drivers/src/interrupt/subdir.mk
-include SDK/platform/drivers/src/flexcan/subdir.mk
-include SDK/platform/drivers/src/edma/subdir.mk
-include SDK/platform/drivers/src/clock/S32K1xx/subdir.mk
-include SDK/platform/devices/S32K144/startup/subdir.mk
-include SDK/platform/devices/subdir.mk
-include SDK/middleware/lin/transport/subdir.mk
-include SDK/middleware/lin/lowlevel/subdir.mk
-include SDK/middleware/lin/diagnostic/subdir.mk
-include SDK/middleware/lin/coreapi/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include Generated_Code/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_SIZE += \
S32K144_SamplePrj.siz \


# All Target
all: S32K144_SamplePrj.elf secondary-outputs

# Tool invocations
S32K144_SamplePrj.elf: $(OBJS) H:/S32KWORK/S32K144_SamplePrj/Project_Settings/Linker_Files/S32K144_64_ram.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Standard S32DS C Linker'
	arm-none-eabi-gcc -o "S32K144_SamplePrj.elf" "@S32K144_SamplePrj.args"  $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

S32K144_SamplePrj.siz: S32K144_SamplePrj.elf
	@echo 'Invoking: Standard S32DS Print Size'
	arm-none-eabi-size --format=berkeley S32K144_SamplePrj.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ./*
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
