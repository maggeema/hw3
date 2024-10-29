# Compiler
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

# Source files
SRC = image.cc h3.cc h4.cc

# Object files
OBJ = $(SRC:.cc=.o)

# Executable names
EXEC_HOUGH3 = h3
EXEC_HOUGH4 = h4

# Targets
all: $(EXEC_HOUGH3) $(EXEC_HOUGH4)

$(EXEC_HOUGH3): h3.o image.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EXEC_HOUGH4): h4.o image.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile the object files
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJ) $(EXEC_HOUGH3) $(EXEC_HOUGH4)

.PHONY: all clean
