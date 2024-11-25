# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Source files and headers
SRC = main1.cpp main2.cpp main3.cpp InstituteSystem.cpp
HEADERS = InstituteSystem.h

# Targets
DEBUG_BINARY = debug_version
OPT_BINARY = optimized_version

# Debug version (includes debug symbols)
debug: CXXFLAGS += -g
debug: $(DEBUG_BINARY)

# Optimized version (optimized for speed)
optimized: CXXFLAGS += -O2
optimized: $(OPT_BINARY)

# Compile the debug binary
$(DEBUG_BINARY): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(DEBUG_BINARY) $(SRC)

# Compile the optimized binary
$(OPT_BINARY): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(OPT_BINARY) $(SRC)

# Clean up generated files
clean:
	rm -f $(DEBUG_BINARY) $(OPT_BINARY)
