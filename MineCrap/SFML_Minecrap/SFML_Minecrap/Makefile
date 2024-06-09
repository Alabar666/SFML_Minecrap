TARGET=a.out
CXX=g++
DEBUG=-g
OPT=-O0
WARN=-Wall
SFML=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
CXXFLAGS=$(DEBUG) $(OPT) $(WARN) $(SFML)
LD=g++
OBJS= minecrap.o
all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(CXXFLAGS)
	@rm *.o
	@./$(TARGET) 2>/dev/null

main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) main.cpp -o main.o

minecrap.o: minecrap.cpp
	$(CXX) -c $(CXXFLAGS) minecrap.cpp -o minecrap.o
