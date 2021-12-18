################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/SystemSave/FLASH_KEA_EEPROM.c \
../Sources/SystemSave/SystemSaveFlash.c 

OBJS += \
./Sources/SystemSave/FLASH_KEA_EEPROM.o \
./Sources/SystemSave/SystemSaveFlash.o 

C_DEPS += \
./Sources/SystemSave/FLASH_KEA_EEPROM.d \
./Sources/SystemSave/SystemSaveFlash.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/SystemSave/%.o: ../Sources/SystemSave/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/SystemSave/FLASH_KEA_EEPROM.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


