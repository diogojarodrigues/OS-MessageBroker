CXX = g++
CXXFLAGS = -std=c++17 -Wall

TARGET_DIR = ../..
TARGET = $(TARGET_DIR)/user

SRC_DIRS = . command
SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))

OBJ_DIR = bin
OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: clean
