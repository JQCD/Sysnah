CXX = g++
CXXFLAGS = -O2 -Wall -Wextra -Wno-unused-parameter
#Bei raspi:
#CXXFLAGS = -O2 -Wall -Wextra -Wno-unused-parameter -Wno-type-limits

ObjsDir = objs

BufferDir = Buffer
Buffer = $(ObjsDir)/Buffer.o
AutomatDir = Automat
Automat = $(ObjsDir)/Automat.o
ScannerDir = Scanner
SymboltableDir = Symboltable
Symboltable = $(ObjsDir)/Symtable.o
ScannerDir = Scanner
Scanner = $(ObjsDir)/Scanner.o
ParserDir = Parser
Parser = $(ObjsDir)/Parser.o

Headers = $(BufferDir)/includes/Buffer.h $(AutomatDir)/includes/Automat.h \
		  $(SymboltableDir)/includes/Symboltable.h $(ScannerDir)/includes/Scanner.h \
		  $(ScannerDir)/includes/Token.h $(SymboltableDir)/includes/Information.h

Objects = $(Buffer) $(Symboltable) $(Automat) $(Scanner) $(ObjsDir)/Token.o $(ObjsDir)/Tree.o
Binary = parser

all: clean init $(Binary)

clean:
		rm -rfv $(ObjsDir)/*
		rm -rfv $(Binary)

init:
		mkdir -p objs

$(Buffer): $(BufferDir)/src/Buffer.cpp $(BufferDir)/includes/Buffer.h
		$(CXX) $(CXXFLAGS) -c -o $(Buffer) $(BufferDir)/src/Buffer.cpp

$(Symboltable): $(SymboltableDir)/src/Symboltable.cpp $(SymboltableDir)/includes/Symboltable.h $(ObjsDir)/LinkedList.o
		$(CXX) $(CXXFLAGS) -c -o $(Symboltable) $(SymboltableDir)/src/Symboltable.cpp

$(Automat): $(AutomatDir)/src/Automat.cpp $(AutomatDir)/includes/Automat.h
		$(CXX) $(CXXFLAGS) -c -o $(Automat) $(AutomatDir)/src/Automat.cpp

$(Scanner): $(ScannerDir)/src/Scanner.cpp $(ScannerDir)/includes/Scanner.h $(Buffer) $(Symboltable) $(Automat) $(ObjsDir)/Token.o
		$(CXX) $(CXXFLAGS) -c -o $(Scanner) $(ScannerDir)/src/Scanner.cpp

$(Binary): init $(Objects) $(Headers)
		$(CXX) $(CXXFLAGS) -o $(Binary) $(ParserDir)/src/Parser.cpp $(ObjsDir)/*

$(ObjsDir)/LinkedList.o: $(SymboltableDir)/src/LinkedList.cpp $(SymboltableDir)/includes/LinkedList.h $(ObjsDir)/ListElement.o
		$(CXX) $(CXXFLAGS) -c -o $(ObjsDir)/LinkedList.o $(SymboltableDir)/src/LinkedList.cpp

$(ObjsDir)/ListElement.o: $(SymboltableDir)/src/ListElement.cpp $(SymboltableDir)/includes/ListElement.h $(ObjsDir)/Information.o
		$(CXX) $(CXXFLAGS) -c -o $(ObjsDir)/ListElement.o $(SymboltableDir)/src/ListElement.cpp

$(ObjsDir)/Information.o: $(SymboltableDir)/src/Information.cpp $(SymboltableDir)/includes/Information.h
		$(CXX) $(CXXFLAGS) -c -o $(ObjsDir)/Information.o $(SymboltableDir)/src/Information.cpp

$(ObjsDir)/Token.o: $(ScannerDir)/src/Token.cpp $(ScannerDir)/includes/Token.h
		$(CXX) $(CXXFLAGS) -c -o $(ObjsDir)/Token.o $(ScannerDir)/src/Token.cpp

$(ObjsDir)/Tree.o: $(ParserDir)/src/Tree.cpp $(ParserDir)/includes/Tree.h
		$(CXX) $(CXXFLAGS) -c -o $(ObjsDir)/Tree.o $(ParserDir)/src/Tree.cpp
