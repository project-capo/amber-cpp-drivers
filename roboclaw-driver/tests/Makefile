CXX = g++
CXXFLAGS = -static -pedantic -W -Wall -Wextra -Wshadow -Wformat \
	-Winit-self -Wunused -Wfloat-equal -Wcast-qual -Wwrite-strings \
	-Winline -Wstack-protector -Wunsafe-loop-optimizations \
	-Wlogical-op -Wmissing-include-dirs -Wconversion \
	-Wmissing-declarations -Wno-long-long

LDFLAGS = -lrt -lpthread -lboost_thread -lprotobuf -llog4cxx -lboost_program_options -lboost_system

EXECUTABLES = read_tests roboclaw_test reset reset_and_go write_to_eeprom led_set
BINDIR = ../bin/

BIN_EXECUTABLES = $(patsubst %, $(BINDIR)%, $(EXECUTABLES))

AMBER_COMMON = ../../common
ROBOCLAW_DRIVER = ../src

INCLUDES = -I$(AMBER_COMMON) -I$(ROBOCLAW_DRIVER)

all: $(BIN_EXECUTABLES)

$(BINDIR)read_tests: read_tests.o $(ROBOCLAW_DRIVER)/RoboclawLib.o
	$(CXX) $^ $(LDFLAGS) -o $@ 

$(BINDIR)roboclaw_test: roboclaw_test.o $(ROBOCLAW_DRIVER)/RoboclawLib.o
	$(CXX) $^ $(LDFLAGS) -o $@ 

$(BINDIR)reset: reset.o $(ROBOCLAW_DRIVER)/RoboclawLib.o
	$(CXX) $^ $(LDFLAGS) -o $@ 

$(BINDIR)reset_and_go: reset_and_go.o $(ROBOCLAW_DRIVER)/RoboclawLib.o
	$(CXX) $^ $(LDFLAGS) -o $@ 

$(BINDIR)write_to_eeprom: write_to_eeprom.o $(ROBOCLAW_DRIVER)/RoboclawLib.o
	$(CXX) $^ $(LDFLAGS) -o $@ 

$(BINDIR)led_set: led_set.o $(ROBOCLAW_DRIVER)/RoboclawLib.o
	$(CXX) $^ $(LDFLAGS) -o $@ 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf *.o $(BIN_EXECUTABLES) $(BINDIR)

.PHONY: clean
