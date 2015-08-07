################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Compiler.c \
../src/DataEncoder.c \
../src/EntryEncoder.c \
../src/ExternEncoder.c \
../src/FirstTransition.c \
../src/OperationEncoder.c \
../src/SecondTransition.c \
../src/SymbolTable.c \
../src/Utilities.c 

OBJS += \
./src/Compiler.o \
./src/DataEncoder.o \
./src/EntryEncoder.o \
./src/ExternEncoder.o \
./src/FirstTransition.o \
./src/OperationEncoder.o \
./src/SecondTransition.o \
./src/SymbolTable.o \
./src/Utilities.o 

C_DEPS += \
./src/Compiler.d \
./src/DataEncoder.d \
./src/EntryEncoder.d \
./src/ExternEncoder.d \
./src/FirstTransition.d \
./src/OperationEncoder.d \
./src/SecondTransition.d \
./src/SymbolTable.d \
./src/Utilities.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


