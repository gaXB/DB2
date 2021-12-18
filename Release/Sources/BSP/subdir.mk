################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/BSP/CommSet.c \
../Sources/BSP/ICDOut.c \
../Sources/BSP/Lcd_Display.c \
../Sources/BSP/Led.c \
../Sources/BSP/Signal_Refresh.c \
../Sources/BSP/Signal_TempGet.c 

OBJS += \
./Sources/BSP/CommSet.o \
./Sources/BSP/ICDOut.o \
./Sources/BSP/Lcd_Display.o \
./Sources/BSP/Led.o \
./Sources/BSP/Signal_Refresh.o \
./Sources/BSP/Signal_TempGet.o 

C_DEPS += \
./Sources/BSP/CommSet.d \
./Sources/BSP/ICDOut.d \
./Sources/BSP/Lcd_Display.d \
./Sources/BSP/Led.d \
./Sources/BSP/Signal_Refresh.d \
./Sources/BSP/Signal_TempGet.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/BSP/%.o: ../Sources/BSP/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/BSP/CommSet.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


