################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../L1/P1_A.c \
../L1/P1_B.c \
../L1/P2.c \
../L1/P3.c \
../L1/P4.c \
../L1/P5.c 

OBJS += \
./L1/P1_A.o \
./L1/P1_B.o \
./L1/P2.o \
./L1/P3.o \
./L1/P4.o \
./L1/P5.o 

C_DEPS += \
./L1/P1_A.d \
./L1/P1_B.d \
./L1/P2.d \
./L1/P3.d \
./L1/P4.d \
./L1/P5.d 


# Each subdirectory must supply rules for building sources it contributes
L1/%.o: ../L1/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


