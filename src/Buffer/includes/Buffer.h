/*
 * Buffer.h
 *
 *  Created on: 27.03.2015
 *      Author: tobias
 */

#ifndef BUFFER_H_
#define BUFFER_H_

class Buffer {
public:
	Buffer(const char *path);
	virtual ~Buffer();
	char getChar();
	void ungetChar();
	void ungetChar(int count);
    int getBuffersize();
	int getCol();
	int getLine();
private:
	int readBytes(char* buf, int count);
	inline void allocMem(void **mempt, int align, int size);
	char getCharFromCurrentBuffer();
	int fileDescriptor, currentBufferIndex, totalBufferIndex;
	//oldColumn speichert die # der letzten zeile, wenns n zeilenumbruch gab z.b. wissen wir nicht wie viele spalten es in der
    	//vorherigen zeile gab
	int oldColumn, column, line;
	char **currentBuffer;
	char *buffer1, *buffer2;
	void *memptr1, *memptr2;
	bool atEof;
};

#endif /* BUFFER_H_ */
