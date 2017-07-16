################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/vault/vault_controller/tsv_controller/subdir.mk
-include src/vault/vault_controller/rvu/subdir.mk
-include src/vault/vault_controller/response/subdir.mk
-include src/vault/vault_controller/request/subdir.mk
-include src/vault/vault_controller/pim/pim_interface/subdir.mk
-include src/vault/vault_controller/pim/pim_hmc/subdir.mk
-include src/vault/vault_controller/memory_controller/subdir.mk
-include src/vault/bank/subdir.mk
-include src/transceiver/responser/transaction_and_link_layer/link_slave/subdir.mk
-include src/transceiver/responser/transaction_and_link_layer/link_master/subdir.mk
-include src/transceiver/responser/physical_layer/physical_tx/subdir.mk
-include src/transceiver/responser/physical_layer/physical_rx/subdir.mk
-include src/transceiver/requester/transaction_and_link_layer/link_slave/subdir.mk
-include src/transceiver/requester/transaction_and_link_layer/link_master/subdir.mk
-include src/transceiver/requester/physical_layer/physical_tx/subdir.mk
-include src/transceiver/requester/physical_layer/physical_rx/subdir.mk
-include src/subdir.mk
-include .metadata/.plugins/org.eclipse.cdt.make.core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: clapps

# Tool invocations
clapps: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L/usr/local/systemc-2.3.1/lib-linux64 -o "clapps" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) clapps
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
