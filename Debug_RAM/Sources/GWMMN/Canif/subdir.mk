################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/GWMMN/Canif/CanIf.c \
../Sources/GWMMN/Canif/InterLayMatrix.c 

OBJS += \
./Sources/GWMMN/Canif/CanIf.o \
./Sources/GWMMN/Canif/InterLayMatrix.o 

C_DEPS += \
./Sources/GWMMN/Canif/CanIf.d \
./Sources/GWMMN/Canif/InterLayMatrix.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/GWMMN/Canif/%.o: ../Sources/GWMMN/Canif/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/GWMMN/Canif/CanIf.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


