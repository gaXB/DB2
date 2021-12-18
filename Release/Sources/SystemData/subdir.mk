################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/SystemData/SystemData.c \
../Sources/SystemData/VehicleData.c 

OBJS += \
./Sources/SystemData/SystemData.o \
./Sources/SystemData/VehicleData.o 

C_DEPS += \
./Sources/SystemData/SystemData.d \
./Sources/SystemData/VehicleData.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/SystemData/%.o: ../Sources/SystemData/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/SystemData/SystemData.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


