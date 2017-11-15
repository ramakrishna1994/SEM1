################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Concurrent/client.c \
../Concurrent/dictionary_client.c \
../Concurrent/dictionary_clnt.c \
../Concurrent/dictionary_server.c \
../Concurrent/dictionary_svc.c \
../Concurrent/dictionary_xdr.c \
../Concurrent/server.c 

OBJS += \
./Concurrent/client.o \
./Concurrent/dictionary_client.o \
./Concurrent/dictionary_clnt.o \
./Concurrent/dictionary_server.o \
./Concurrent/dictionary_svc.o \
./Concurrent/dictionary_xdr.o \
./Concurrent/server.o 

C_DEPS += \
./Concurrent/client.d \
./Concurrent/dictionary_client.d \
./Concurrent/dictionary_clnt.d \
./Concurrent/dictionary_server.d \
./Concurrent/dictionary_svc.d \
./Concurrent/dictionary_xdr.d \
./Concurrent/server.d 


# Each subdirectory must supply rules for building sources it contributes
Concurrent/%.o: ../Concurrent/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


