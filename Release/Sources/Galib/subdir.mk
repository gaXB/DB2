################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/Galib/ADTreat.c \
../Sources/Galib/ComdQueue.c \
../Sources/Galib/DealString.c \
../Sources/Galib/EnCoder35.c \
../Sources/Galib/Encoder28.c \
../Sources/Galib/EncoderESE24.c \
../Sources/Galib/FanControl.c \
../Sources/Galib/Ht16k23_Drive.c \
../Sources/Galib/IIC_Drive_Software.c \
../Sources/Galib/PTMETER_Fun.c \
../Sources/Galib/SPI_Drive_Software.c \
../Sources/Galib/SignalFilter.c \
../Sources/Galib/keypad.c \
../Sources/Galib/mathGA.c \
../Sources/Galib/mcp7708.c \
../Sources/Galib/motor.c \
../Sources/Galib/ncfmotor.c \
../Sources/Galib/timerout.c 

OBJS += \
./Sources/Galib/ADTreat.o \
./Sources/Galib/ComdQueue.o \
./Sources/Galib/DealString.o \
./Sources/Galib/EnCoder35.o \
./Sources/Galib/Encoder28.o \
./Sources/Galib/EncoderESE24.o \
./Sources/Galib/FanControl.o \
./Sources/Galib/Ht16k23_Drive.o \
./Sources/Galib/IIC_Drive_Software.o \
./Sources/Galib/PTMETER_Fun.o \
./Sources/Galib/SPI_Drive_Software.o \
./Sources/Galib/SignalFilter.o \
./Sources/Galib/keypad.o \
./Sources/Galib/mathGA.o \
./Sources/Galib/mcp7708.o \
./Sources/Galib/motor.o \
./Sources/Galib/ncfmotor.o \
./Sources/Galib/timerout.o 

C_DEPS += \
./Sources/Galib/ADTreat.d \
./Sources/Galib/ComdQueue.d \
./Sources/Galib/DealString.d \
./Sources/Galib/EnCoder35.d \
./Sources/Galib/Encoder28.d \
./Sources/Galib/EncoderESE24.d \
./Sources/Galib/FanControl.d \
./Sources/Galib/Ht16k23_Drive.d \
./Sources/Galib/IIC_Drive_Software.d \
./Sources/Galib/PTMETER_Fun.d \
./Sources/Galib/SPI_Drive_Software.d \
./Sources/Galib/SignalFilter.d \
./Sources/Galib/keypad.d \
./Sources/Galib/mathGA.d \
./Sources/Galib/mcp7708.d \
./Sources/Galib/motor.d \
./Sources/Galib/ncfmotor.d \
./Sources/Galib/timerout.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/Galib/%.o: ../Sources/Galib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/Galib/ADTreat.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


