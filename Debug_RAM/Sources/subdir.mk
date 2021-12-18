################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/LowerDrive.c \
../Sources/SystemApp.c \
../Sources/can_api.c \
../Sources/file1.c \
../Sources/file2.c \
../Sources/main.c 

OBJS += \
./Sources/LowerDrive.o \
./Sources/SystemApp.o \
./Sources/can_api.o \
./Sources/file1.o \
./Sources/file2.o \
./Sources/main.o 

C_DEPS += \
./Sources/LowerDrive.d \
./Sources/SystemApp.d \
./Sources/can_api.d \
./Sources/file1.d \
./Sources/file2.d \
./Sources/main.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/LowerDrive.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


