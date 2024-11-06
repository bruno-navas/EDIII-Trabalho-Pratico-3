# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++20

# Source files and output executable
SRC = main.cpp Metodos_de_classes_e_funcoes_criadas.cpp
TARGET = programa_trab_3

# 'all' command compiles the program
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# 'run' command to execute the compiled program
run: all
	./$(TARGET)

# Clean up build files
clean:
	rm -f $(TARGET)