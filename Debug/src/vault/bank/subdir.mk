################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/vault/bank/bank.cpp 

OBJS += \
./src/vault/bank/bank.o 

CPP_DEPS += \
./src/vault/bank/bank.d 


# Each subdirectory must supply rules for building sources it contributes
src/vault/bank/bank.o: ../src/vault/bank/bank.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -D__GXX_EXPERIMENTAL_CXX0X__ -I/usr/local/systemc-2.3.1/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/vault/bank/bank.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


