################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/Module/A4980.c \
../Sources/Module/A4980_HWIF.c \
../Sources/Module/Deal_DTC.c \
../Sources/Module/MoudleDrive.c \
../Sources/Module/StepMotor.c \
../Sources/Module/TLE94112.c \
../Sources/Module/TLE94112_HWIF.c \
../Sources/Module/TLE94x1.c \
../Sources/Module/TLE94x1_SPI.c 

OBJS += \
./Sources/Module/A4980.o \
./Sources/Module/A4980_HWIF.o \
./Sources/Module/Deal_DTC.o \
./Sources/Module/MoudleDrive.o \
./Sources/Module/StepMotor.o \
./Sources/Module/TLE94112.o \
./Sources/Module/TLE94112_HWIF.o \
./Sources/Module/TLE94x1.o \
./Sources/Module/TLE94x1_SPI.o 

C_DEPS += \
./Sources/Module/A4980.d \
./Sources/Module/A4980_HWIF.d \
./Sources/Module/Deal_DTC.d \
./Sources/Module/MoudleDrive.d \
./Sources/Module/StepMotor.d \
./Sources/Module/TLE94112.d \
./Sources/Module/TLE94112_HWIF.d \
./Sources/Module/TLE94x1.d \
./Sources/Module/TLE94x1_SPI.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/Module/%.o: ../Sources/Module/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/Module/A4980.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


