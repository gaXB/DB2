################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/GWMMN/InterLayCan/InterLayCan.c \
../Sources/GWMMN/InterLayCan/InterLay_App.c \
../Sources/GWMMN/InterLayCan/MP5Logic.c 

OBJS += \
./Sources/GWMMN/InterLayCan/InterLayCan.o \
./Sources/GWMMN/InterLayCan/InterLay_App.o \
./Sources/GWMMN/InterLayCan/MP5Logic.o 

C_DEPS += \
./Sources/GWMMN/InterLayCan/InterLayCan.d \
./Sources/GWMMN/InterLayCan/InterLay_App.d \
./Sources/GWMMN/InterLayCan/MP5Logic.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/GWMMN/InterLayCan/%.o: ../Sources/GWMMN/InterLayCan/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/GWMMN/InterLayCan/InterLayCan.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


