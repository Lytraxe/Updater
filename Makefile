CXX = g++
CXXFLAGS = -std=c++20 -Wall -fdiagnostics-color=always --debug -g
LFLAGS = -lcurl -ljsoncpp -lzip -lyaml-cpp

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

SOURCES = main.cpp $(wildcard $(SRC_DIR)/**/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
EXECUTABLE = updater

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LFLAGS) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(LFLAGS) $(CXXFLAGS) -c $< -o $@ -I$(INC_DIR)

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)

$(shell mkdir -p $(dir $(OBJECTS)))