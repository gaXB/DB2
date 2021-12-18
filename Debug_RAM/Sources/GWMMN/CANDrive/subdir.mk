################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/GWMMN/CANDrive/CANDrive.c \
../Sources/GWMMN/CANDrive/CANDriveWake.c 

OBJS += \
./Sources/GWMMN/CANDrive/CANDrive.o \
./Sources/GWMMN/CANDrive/CANDriveWake.o 

C_DEPS += \
./Sources/GWMMN/CANDrive/CANDrive.d \
./Sources/GWMMN/CANDrive/CANDriveWake.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/GWMMN/CANDrive/%.o: ../Sources/GWMMN/CANDrive/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/GWMMN/CANDrive/CANDrive.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


