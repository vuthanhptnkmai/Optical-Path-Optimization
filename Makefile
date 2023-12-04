# Compiler settings - Can be customized.
CXX = g++
#CXXFLAGS = -Wall -Wextra -std=c++17 -fopenmp
CXXFLAGS = -w -std=c++17 -fopenmp
LDFLAGS = -fopenmp

# App binary name
APPNAME = OpticalPathSimulation

# Source files
SOURCES = main.cpp thirdParty/tinyxml2/tinyxml2.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Include directories
INCLUDES = -IthirdParty/tinyxml2

# Build and link the application
all: $(APPNAME)

$(APPNAME): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

# Compile source files into objects
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	del /Q "main.o" "$(APPNAME).exe"


# Run the application
run: $(APPNAME)
	./$(APPNAME)