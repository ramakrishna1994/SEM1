################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../L4/l4p1.c \
../L4/l4p2.c \
../L4/l4p3.c \
../L4/mine.c 

OBJS += \
./L4/l4p1.o \
./L4/l4p2.o \
./L4/l4p3.o \
./L4/mine.o 

C_DEPS += \
./L4/l4p1.d \
./L4/l4p2.d \
./L4/l4p3.d \
./L4/mine.d 


# Each subdirectory must supply rules for building sources it contributes
L4/%.o: ../L4/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


