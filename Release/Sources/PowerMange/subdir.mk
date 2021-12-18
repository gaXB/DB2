################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/PowerMange/PowerMange.c \
../Sources/PowerMange/PowerMange_APP.c 

OBJS += \
./Sources/PowerMange/PowerMange.o \
./Sources/PowerMange/PowerMange_APP.o 

C_DEPS += \
./Sources/PowerMange/PowerMange.d \
./Sources/PowerMange/PowerMange_APP.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/PowerMange/%.o: ../Sources/PowerMange/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/PowerMange/PowerMange.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


