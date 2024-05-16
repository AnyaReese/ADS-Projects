# Compiler settings - Can change to clang++ if required
CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2

# Project file structure
SRCDIR = .
BUILDDIR = ./build
TARGET = $(BUILDDIR)/2DBPP.out

# Source and object files
SOURCES = $(SRCDIR)/2DBPP.cpp
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

# Default make
all: $(TARGET)

# Link the target binary
$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile the source files into object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(BUILDDIR)/*.o $(TARGET)

test:
	@echo "Generating test cases..."
	cd ../Test && python3 gen_cases.py && cd ../Code

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
