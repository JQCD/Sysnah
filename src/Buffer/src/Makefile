CXX =       g++

CXXFLAGS =	-O0 -g -Wall -Wextra -fmessage-length=0

OBJS =		sysprog2.o Buffer.o

TARGET =	sysprog2.out

$(TARGET):	clean sysprog2.o Buffer.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	
sysprog2.o:
	$(CXX) $(CXXFLAGS) -c sysprog2.cpp

Buffer.o:
	$(CXX) $(CXXFLAGS) -c Buffer.cpp

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
