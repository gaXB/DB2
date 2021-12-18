################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/SYSTask/Diag_task.c \
../Sources/SYSTask/Init_Task.c \
../Sources/SYSTask/PowerOff_Task.c \
../Sources/SYSTask/PowerStandBy_Task.c 

OBJS += \
./Sources/SYSTask/Diag_task.o \
./Sources/SYSTask/Init_Task.o \
./Sources/SYSTask/PowerOff_Task.o \
./Sources/SYSTask/PowerStandBy_Task.o 

C_DEPS += \
./Sources/SYSTask/Diag_task.d \
./Sources/SYSTask/Init_Task.d \
./Sources/SYSTask/PowerOff_Task.d \
./Sources/SYSTask/PowerStandBy_Task.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/SYSTask/%.o: ../Sources/SYSTask/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/SYSTask/Diag_task.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


