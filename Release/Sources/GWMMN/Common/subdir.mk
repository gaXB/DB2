################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/GWMMN/Common/Bsw_Common.c 

OBJS += \
./Sources/GWMMN/Common/Bsw_Common.o 

C_DEPS += \
./Sources/GWMMN/Common/Bsw_Common.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/GWMMN/Common/%.o: ../Sources/GWMMN/Common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/GWMMN/Common/Bsw_Common.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


