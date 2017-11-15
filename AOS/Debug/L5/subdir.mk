################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../L5/lamport.c \
../L5/sample.c 

OBJS += \
./L5/lamport.o \
./L5/sample.o 

C_DEPS += \
./L5/lamport.d \
./L5/sample.d 


# Each subdirectory must supply rules for building sources it contributes
L5/%.o: ../L5/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


