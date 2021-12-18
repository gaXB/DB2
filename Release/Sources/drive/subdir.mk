################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/drive/ADC.c 

OBJS += \
./Sources/drive/ADC.o 

C_DEPS += \
./Sources/drive/ADC.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/drive/%.o: ../Sources/drive/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/drive/ADC.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


