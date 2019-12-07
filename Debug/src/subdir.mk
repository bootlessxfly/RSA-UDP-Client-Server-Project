################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Client.cpp \
../src/RSA_UDP_Client_Server_Project.cpp \
../src/Server.cpp 

OBJS += \
./src/Client.o \
./src/RSA_UDP_Client_Server_Project.o \
./src/Server.o 

CPP_DEPS += \
./src/Client.d \
./src/RSA_UDP_Client_Server_Project.d \
./src/Server.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


