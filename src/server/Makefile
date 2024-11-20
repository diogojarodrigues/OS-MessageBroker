CXX = g++
CXXFLAGS = -std=c++17 -Wall

OBJ_DIR = bin
TARGET_DIR = ../..
TARGET = $(TARGET_DIR)/AS
SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: clean
