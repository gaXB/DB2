################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/CAUTO/CAUTO_Vent.c \
../Sources/CAUTO/MIXVENT_PID.c \
../Sources/CAUTO/RunPID.c \
../Sources/CAUTO/RunPID_Increment.c \
../Sources/CAUTO/TempFilter.c 

OBJS += \
./Sources/CAUTO/CAUTO_Vent.o \
./Sources/CAUTO/MIXVENT_PID.o \
./Sources/CAUTO/RunPID.o \
./Sources/CAUTO/RunPID_Increment.o \
./Sources/CAUTO/TempFilter.o 

C_DEPS += \
./Sources/CAUTO/CAUTO_Vent.d \
./Sources/CAUTO/MIXVENT_PID.d \
./Sources/CAUTO/RunPID.d \
./Sources/CAUTO/RunPID_Increment.d \
./Sources/CAUTO/TempFilter.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/CAUTO/%.o: ../Sources/CAUTO/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/CAUTO/CAUTO_Vent.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


