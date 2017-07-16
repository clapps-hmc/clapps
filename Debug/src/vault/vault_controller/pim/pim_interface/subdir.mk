################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/vault/vault_controller/pim/pim_interface/pim_interface_read_write_controller.cpp 

OBJS += \
./src/vault/vault_controller/pim/pim_interface/pim_interface_read_write_controller.o 

CPP_DEPS += \
./src/vault/vault_controller/pim/pim_interface/pim_interface_read_write_controller.d 


# Each subdirectory must supply rules for building sources it contributes
src/vault/vault_controller/pim/pim_interface/%.o: ../src/vault/vault_controller/pim/pim_interface/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I/usr/local/systemc-2.3.1/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


