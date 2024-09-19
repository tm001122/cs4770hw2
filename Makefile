# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -g

# Target executable
TARGET = encrypt

# Source files
SRCS = encrypt.cc

# Object files
OBJS = $(SRCS:.cc=.o)

# Default rule: build the executable
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cc files into .o files
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up all object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)
