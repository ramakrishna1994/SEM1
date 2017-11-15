################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../dummy/dictionary_client.c \
../dummy/dictionary_clnt.c \
../dummy/dictionary_server.c \
../dummy/dictionary_svc.c \
../dummy/dictionary_xdr.c 

OBJS += \
./dummy/dictionary_client.o \
./dummy/dictionary_clnt.o \
./dummy/dictionary_server.o \
./dummy/dictionary_svc.o \
./dummy/dictionary_xdr.o 

C_DEPS += \
./dummy/dictionary_client.d \
./dummy/dictionary_clnt.d \
./dummy/dictionary_server.d \
./dummy/dictionary_svc.d \
./dummy/dictionary_xdr.d 


# Each subdirectory must supply rules for building sources it contributes
dummy/%.o: ../dummy/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


