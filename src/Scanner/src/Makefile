CXX =       g++

CXXFLAGS =	-O0 -g -Wall -Wextra -fmessage-length=0

OBJS =		Scanner.o Buffer.o Automat.o Symboltable.o Token.o

TARGET =	scanner.out

$(TARGET):	clean $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	
Scanner.o:
	$(CXX) $(CXXFLAGS) -c Scanner.cpp

Buffer.o:
	$(CXX) $(CXXFLAGS) -c Buffer.cpp

Automat.o:
	$(CXX) $(CXXFLAGS) -c Automat.cpp

Symboltable.o:
	$(CXX) $(CXXFLAGS) -c Symboltable.cpp

Token.o:
	$(CXX) $(CXXFLAGS) -c Token.cpp

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
