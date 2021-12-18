################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/GWMMN/DebugPrint/printf.c 

OBJS += \
./Sources/GWMMN/DebugPrint/printf.o 

C_DEPS += \
./Sources/GWMMN/DebugPrint/printf.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/GWMMN/DebugPrint/%.o: ../Sources/GWMMN/DebugPrint/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/GWMMN/DebugPrint/printf.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


