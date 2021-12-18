################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/platform/drivers/src/lin/lin_common.c \
../SDK/platform/drivers/src/lin/lin_driver.c \
../SDK/platform/drivers/src/lin/lin_irq.c 

OBJS += \
./SDK/platform/drivers/src/lin/lin_common.o \
./SDK/platform/drivers/src/lin/lin_driver.o \
./SDK/platform/drivers/src/lin/lin_irq.o 

C_DEPS += \
./SDK/platform/drivers/src/lin/lin_common.d \
./SDK/platform/drivers/src/lin/lin_driver.d \
./SDK/platform/drivers/src/lin/lin_irq.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/drivers/src/lin/%.o: ../SDK/platform/drivers/src/lin/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@SDK/platform/drivers/src/lin/lin_common.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


