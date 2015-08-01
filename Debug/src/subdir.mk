################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Compiler.c \
../src/Data.c \
../src/FirstTransition.c \
../src/SecondTransition.c \
../src/SymbolTable.c \
../src/Utilities.c 

OBJS += \
./src/Compiler.o \
./src/Data.o \
./src/FirstTransition.o \
./src/SecondTransition.o \
./src/SymbolTable.o \
./src/Utilities.o 

C_DEPS += \
./src/Compiler.d \
./src/Data.d \
./src/FirstTransition.d \
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


