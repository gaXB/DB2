################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/middleware/lin/coreapi/lin_common_api.c \
../SDK/middleware/lin/coreapi/lin_common_proto.c \
../SDK/middleware/lin/coreapi/lin_j2602_proto.c \
../SDK/middleware/lin/coreapi/lin_lin21_proto.c 

OBJS += \
./SDK/middleware/lin/coreapi/lin_common_api.o \
./SDK/middleware/lin/coreapi/lin_common_proto.o \
./SDK/middleware/lin/coreapi/lin_j2602_proto.o \
./SDK/middleware/lin/coreapi/lin_lin21_proto.o 

C_DEPS += \
./SDK/middleware/lin/coreapi/lin_common_api.d \
./SDK/middleware/lin/coreapi/lin_common_proto.d \
./SDK/middleware/lin/coreapi/lin_j2602_proto.d \
./SDK/middleware/lin/coreapi/lin_lin21_proto.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/middleware/lin/coreapi/%.o: ../SDK/middleware/lin/coreapi/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@SDK/middleware/lin/coreapi/lin_common_api.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


