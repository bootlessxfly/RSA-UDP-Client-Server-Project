################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Client.cpp \
../src/Key.cpp \
../src/RSA.cpp \
../src/RSA_UDP_Client_Server_Project.cpp \
../src/Server.cpp \
../src/Utility.cpp 

OBJS += \
./src/Client.o \
./src/Key.o \
./src/RSA.o \
./src/RSA_UDP_Client_Server_Project.o \
./src/Server.o \
./src/Utility.o 

CPP_DEPS += \
./src/Client.d \
./src/Key.d \
./src/RSA.d \
./src/RSA_UDP_Client_Server_Project.d \
./src/Server.d \
./src/Utility.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


