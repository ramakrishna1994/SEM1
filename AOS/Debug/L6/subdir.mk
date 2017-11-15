################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../L6/DME.c \
../L6/dummy.c 

OBJS += \
./L6/DME.o \
./L6/dummy.o 

C_DEPS += \
./L6/DME.d \
./L6/dummy.d 


# Each subdirectory must supply rules for building sources it contributes
L6/%.o: ../L6/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


