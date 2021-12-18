################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/SYSTask/MainTask/HVACMainControl.c \
../Sources/SYSTask/MainTask/KeyPadLogic.c \
../Sources/SYSTask/MainTask/SystemSave.c 

OBJS += \
./Sources/SYSTask/MainTask/HVACMainControl.o \
./Sources/SYSTask/MainTask/KeyPadLogic.o \
./Sources/SYSTask/MainTask/SystemSave.o 

C_DEPS += \
./Sources/SYSTask/MainTask/HVACMainControl.d \
./Sources/SYSTask/MainTask/KeyPadLogic.d \
./Sources/SYSTask/MainTask/SystemSave.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/SYSTask/MainTask/%.o: ../Sources/SYSTask/MainTask/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/SYSTask/MainTask/HVACMainControl.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


