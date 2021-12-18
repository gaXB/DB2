################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/HardWareDrive/Galib_MCUComm.c \
../Sources/HardWareDrive/HardWareDrive.c \
../Sources/HardWareDrive/Init_MCU.c \
../Sources/HardWareDrive/MCUPMLogic.c 

OBJS += \
./Sources/HardWareDrive/Galib_MCUComm.o \
./Sources/HardWareDrive/HardWareDrive.o \
./Sources/HardWareDrive/Init_MCU.o \
./Sources/HardWareDrive/MCUPMLogic.o 

C_DEPS += \
./Sources/HardWareDrive/Galib_MCUComm.d \
./Sources/HardWareDrive/HardWareDrive.d \
./Sources/HardWareDrive/Init_MCU.d \
./Sources/HardWareDrive/MCUPMLogic.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/HardWareDrive/%.o: ../Sources/HardWareDrive/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/HardWareDrive/Galib_MCUComm.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


