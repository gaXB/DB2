################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/GWMMN/NM/IndirectNM.c \
../Sources/GWMMN/NM/NM_APP.c \
../Sources/GWMMN/NM/OsekNm_App.c 

OBJS += \
./Sources/GWMMN/NM/IndirectNM.o \
./Sources/GWMMN/NM/NM_APP.o \
./Sources/GWMMN/NM/OsekNm_App.o 

C_DEPS += \
./Sources/GWMMN/NM/IndirectNM.d \
./Sources/GWMMN/NM/NM_APP.d \
./Sources/GWMMN/NM/OsekNm_App.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/GWMMN/NM/%.o: ../Sources/GWMMN/NM/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/GWMMN/NM/IndirectNM.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


