TARGET = PackageManager
OBJ_DIR = obj
INCLUDE_DIR = include

LIBS = -lcurl

CXX = g++
CC = gcc

CXXFLAGS = -Wall -Wextra -I./src -I$(INCLUDE_DIR) -O2 -std=c++20 -g 
CFLAGS = -Wall -Wextra -I./src -I$(INCLUDE_DIR) -O2 -std=c11 -g

SRCS_CPP := $(shell find src -name "*.cpp")
SRCS_C   := $(shell find src -name "*.c")

OBJS_CPP := $(patsubst src/%.cpp, $(OBJ_DIR)/%.o, $(SRCS_CPP))
OBJS_C   := $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRCS_C))

OBJS := $(OBJS_CPP) $(OBJS_C)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p compiled
	$(CXX) -o $@ $(OBJS) $(LIBS)

$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) compiled

RUN_ARGS := $(wordlist 2, $(words $(MAKECMDGOALS)), $(MAKECMDGOALS))
$(eval $(RUN_ARGS):;@:)
run: all
	./$(TARGET) $(RUN_ARGS)

