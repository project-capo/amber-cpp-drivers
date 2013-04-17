CXX = g++
CXXFLAGS = -static -pedantic -W -Wall -Wextra -Wshadow -Wformat \
	-Winit-self -Wunused -Wfloat-equal -Wcast-qual -Wwrite-strings \
	-Winline -Wstack-protector -Wunsafe-loop-optimizations \
	-Wlogical-op -Wmissing-include-dirs -Wconversion \
	-Wmissing-declarations -Wno-long-long

LDFLAGS = -lrt -lpthread -lboost_thread -lprotobuf -llog4cxx -lboost_program_options

EXECUTABLES = read_tests
BINDIR = ../bin/

BIN_EXECUTABLES = $(patsubst %, $(BINDIR)%, $(EXECUTABLES))

AMBER_COMMON = ../../common
ROBOCLAW_DRIVER = ../src

INCLUDES = -I$(AMBER_COMMON) -I$(ROBOCLAW_DRIVER)

all: $(BIN_EXECUTABLES)

$(BINDIR)read_tests: read_tests.o $(ROBOCLAW_DRIVER)/RoboclawLib.o
	$(CXX) $^ $(LDFLAGS) -o $@ 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf *.o $(BIN_EXECUTABLES) $(BINDIR)

.PHONY: clean
