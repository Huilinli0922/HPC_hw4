CXX = mpic++ # or mpicxx
CXXFLAGS = -std=c++11 -O3 -march=native #-fopenmp -no-multibyte-chars

TARGETS = $(basename $(wildcard *.cpp))

all : $(TARGETS)

%:%.cpp *.h
	$(CXX) $(CXXFLAGS) $< $(LIBS) -o $@

clean:
	-$(RM) $(TARGETS) *~

.PHONY: all, clean

# # Uncomment the following line if you have OpenMP support installed
# # CXXFLAGS += -fopenmp
# LDFLAGS =

# # Uncomment the following line if you have the LLVM version of OpenMP installed on macOS
# # LDFLAGS += -lomp

# TARGET = int_ring

# all: $(TARGET)

# $(TARGET): int_ring.cpp
# 	$(CXX) $(CXXFLAGS) int_ring.cpp -o $(TARGET) $(LDFLAGS)

# clean:
# 	rm -f $(TARGET)
