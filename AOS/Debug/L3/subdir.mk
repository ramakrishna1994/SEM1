################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../L3/client.c \
../L3/server.c \
../L3/sort_clnt.c \
../L3/sort_svc.c \
../L3/sort_xdr.c 

OBJS += \
./L3/client.o \
./L3/server.o \
./L3/sort_clnt.o \
./L3/sort_svc.o \
./L3/sort_xdr.o 

C_DEPS += \
./L3/client.d \
./L3/server.d \
./L3/sort_clnt.d \
./L3/sort_svc.d \
./L3/sort_xdr.d 


# Each subdirectory must supply rules for building sources it contributes
L3/%.o: ../L3/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


