CXX = g++
CXXFLAGS = -static -pedantic -W -Wall -Wextra -Wshadow -Wformat \
	-Winit-self -Wunused -Wfloat-equal -Wcast-qual -Wwrite-strings \
	-Winline -Wstack-protector -Wunsafe-loop-optimizations \
	-Wlogical-op -Wmissing-include-dirs -Wconversion \
	-Wmissing-declarations -Wno-long-long

LDFLAGS = -lrt -lpthread -lboost_thread -lprotobuf -llog4cxx -lboost_program_options -lboost_system

EXECUTABLES = ninedof_test
BINDIR = ../bin/

BIN_EXECUTABLES = $(patsubst %, $(BINDIR)%, $(EXECUTABLES))

AMBER_COMMON = ../../common
DRIVER_SRC = ../src

INCLUDES = -I$(AMBER_COMMON) -I$(DRIVER_SRC)

all: $(BIN_EXECUTABLES)

$(BINDIR)ninedof_test: ninedof_test.o $(DRIVER_SRC)/I2c.o
	$(CXX) $^ $(LDFLAGS) -o $@ 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf *.o $(BIN_EXECUTABLES) $(BINDIR)

.PHONY: clean
