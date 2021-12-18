################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/GWMMN/UDS/AppLayInterface.c \
../Sources/GWMMN/UDS/ApplayLay.c \
../Sources/GWMMN/UDS/DealDID.c \
../Sources/GWMMN/UDS/NetLaye.c \
../Sources/GWMMN/UDS/UDS_APP.c 

OBJS += \
./Sources/GWMMN/UDS/AppLayInterface.o \
./Sources/GWMMN/UDS/ApplayLay.o \
./Sources/GWMMN/UDS/DealDID.o \
./Sources/GWMMN/UDS/NetLaye.o \
./Sources/GWMMN/UDS/UDS_APP.o 

C_DEPS += \
./Sources/GWMMN/UDS/AppLayInterface.d \
./Sources/GWMMN/UDS/ApplayLay.d \
./Sources/GWMMN/UDS/DealDID.d \
./Sources/GWMMN/UDS/NetLaye.d \
./Sources/GWMMN/UDS/UDS_APP.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/GWMMN/UDS/%.o: ../Sources/GWMMN/UDS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/GWMMN/UDS/AppLayInterface.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


