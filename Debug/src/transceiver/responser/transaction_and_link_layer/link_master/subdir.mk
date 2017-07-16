################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/transceiver/responser/transaction_and_link_layer/link_master/link_control.cpp \
../src/transceiver/responser/transaction_and_link_layer/link_master/mux_8x1.cpp 

OBJS += \
./src/transceiver/responser/transaction_and_link_layer/link_master/link_control.o \
./src/transceiver/responser/transaction_and_link_layer/link_master/mux_8x1.o 

CPP_DEPS += \
./src/transceiver/responser/transaction_and_link_layer/link_master/link_control.d \
./src/transceiver/responser/transaction_and_link_layer/link_master/mux_8x1.d 


# Each subdirectory must supply rules for building sources it contributes
src/transceiver/responser/transaction_and_link_layer/link_master/%.o: ../src/transceiver/responser/transaction_and_link_layer/link_master/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I/usr/local/systemc-2.3.1/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


