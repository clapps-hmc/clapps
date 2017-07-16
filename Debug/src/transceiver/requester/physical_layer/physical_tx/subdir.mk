################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/transceiver/requester/physical_layer/physical_tx/serializer_requester.cpp 

OBJS += \
./src/transceiver/requester/physical_layer/physical_tx/serializer_requester.o 

CPP_DEPS += \
./src/transceiver/requester/physical_layer/physical_tx/serializer_requester.d 


# Each subdirectory must supply rules for building sources it contributes
src/transceiver/requester/physical_layer/physical_tx/%.o: ../src/transceiver/requester/physical_layer/physical_tx/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I/usr/local/systemc-2.3.1/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


