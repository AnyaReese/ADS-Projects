# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -g -Iincludes -std=c++11 -O2

# Directories
INCDIR = includes
SRCDIR = src
OBJDIR = build

# Target executable
TARGET = build/program

# Source files
SRCS = $(SRCDIR)/main.cpp $(SRCDIR)/algorithms.cpp $(SRCDIR)/utils.cpp

# Object files
OBJS = $(OBJDIR)/main.o $(OBJDIR)/algorithms.o $(OBJDIR)/utils.o

# Default rule
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compilation
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean up
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Dependencies
$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(INCDIR)/algorithms.h $(INCDIR)/utils.h
$(OBJDIR)/algorithms.o: $(SRCDIR)/algorithms.cpp $(INCDIR)/algorithms.h $(INCDIR)/utils.h
$(OBJDIR)/utils.o: $(SRCDIR)/utils.cpp $(INCDIR)/utils.h