################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../abc/client.c \
../abc/rdict_client.c \
../abc/rdict_clnt.c \
../abc/rdict_server.c \
../abc/rdict_svc.c \
../abc/rdict_xdr.c \
../abc/server.c 

OBJS += \
./abc/client.o \
./abc/rdict_client.o \
./abc/rdict_clnt.o \
./abc/rdict_server.o \
./abc/rdict_svc.o \
./abc/rdict_xdr.o \
./abc/server.o 

C_DEPS += \
./abc/client.d \
./abc/rdict_client.d \
./abc/rdict_clnt.d \
./abc/rdict_server.d \
./abc/rdict_svc.d \
./abc/rdict_xdr.d \
./abc/server.d 


# Each subdirectory must supply rules for building sources it contributes
abc/%.o: ../abc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


