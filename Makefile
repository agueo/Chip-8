CXX			:= g++
CXXFLAGS 	:= -pedantic-errors -Wall -Wextra -Werror -std=c++20
CXXFLAGS 	+= `sdl2-config --cflags`
LDFLAGS 	:= `sdl2-config --libs` -lm
BUILD 		:= ./build
OBJ_DIR		:= $(BUILD)/objects
APP_DIR		:= $(BUILD)/apps
TARGET		:= chip_8
INCLUDE		:= -Iinclude/
SRC			:= $(wildcard src/*.cpp)

OBJECTS		:= $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

.PHONY: all build clean debug release run

build:
	@mkdir -p ${APP_DIR}
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O3
release: all

run: 
	$(APP_DIR)/$(TARGET) ROMS/games/Breakout\ \(Brix\ hack\)\ \[David\ Winter,\ 1997\].ch8

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*
