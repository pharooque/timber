# Variables
CXXFLAGS = -Wall -Wextra -pedantic -std=c++20
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lopengl32

# Target
timber.exe: timber.cpp
	$(CXX) $(CXXFLAGS) timber.cpp -o timber.exe $(LDFLAGS)

# Clean up
clean:
	rm -f *.o timber.exe
