################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/middleware/lin/transport/lin_commontl_api.c \
../SDK/middleware/lin/transport/lin_commontl_proto.c 

OBJS += \
./SDK/middleware/lin/transport/lin_commontl_api.o \
./SDK/middleware/lin/transport/lin_commontl_proto.o 

C_DEPS += \
./SDK/middleware/lin/transport/lin_commontl_api.d \
./SDK/middleware/lin/transport/lin_commontl_proto.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/middleware/lin/transport/%.o: ../SDK/middleware/lin/transport/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@SDK/middleware/lin/transport/lin_commontl_api.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


