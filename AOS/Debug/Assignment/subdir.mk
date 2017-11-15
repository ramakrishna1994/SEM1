################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Assignment/client.c \
../Assignment/dictionary_clnt.c \
../Assignment/dictionary_svc.c \
../Assignment/dictionary_xdr.c \
../Assignment/server.c 

OBJS += \
./Assignment/client.o \
./Assignment/dictionary_clnt.o \
./Assignment/dictionary_svc.o \
./Assignment/dictionary_xdr.o \
./Assignment/server.o 

C_DEPS += \
./Assignment/client.d \
./Assignment/dictionary_clnt.d \
./Assignment/dictionary_svc.d \
./Assignment/dictionary_xdr.d \
./Assignment/server.d 


# Each subdirectory must supply rules for building sources it contributes
Assignment/%.o: ../Assignment/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


