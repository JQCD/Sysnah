/*
 * Buffer.cpp
 *
 *  Created on: 27.03.2015
 *      Author: tobias
 */

#include "../includes/Buffer.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// 1)wieso vielfaches von 512 Bytes?
//5) grund für diese entscheidung 2 buffer  zu nehmen?
#define BUFFERSIZE (1024 * int(sizeof(char)))
//line feed
#define LF 10 //Unix
//carriage return
#define CR 13
#define DEBUG false // if(DEBUG) fprintf(stderr, "Meldung\n");

Buffer::Buffer(const char *path) {
	// Datei öffnen
	//3) was macht NODIRECT?
    #ifdef NODIRECT
	if ((this->fileDescriptor = open(path, O_RDONLY)) < 0) {
    #else
    /*  The O_DIRECT flag may impose alignment restrictions on the length and
              address of user-space buffers and the file offset of I/Os.  In Linux
              alignment restrictions vary by filesystem and kernel version and
              might be absent entirely.  However there is currently no
              filesystem-independent interface for an application to discover these
              restrictions for a given file or filesystem.  Some filesystems
              provide their own interfaces for doing so, for example the
              */
    if ((this->fileDescriptor = open(path, O_RDONLY | O_DIRECT)) < 0) {
    #endif
		perror("Öffnen fehlgeschlagen");
		exit(-1);
	}

	// Buffer 1 initialisieren (Platz für BUFFERSIZE + 1 Char)
	//Speicher belegen
	allocMem(&memptr1, BUFFERSIZE, BUFFERSIZE + sizeof(char));
	this->buffer1 = (char*)this->memptr1;
	this->buffer1[BUFFERSIZE] = '\0';

	// Andere Variablen initialisieren
	this->totalBufferIndex = this->currentBufferIndex = 0;
	this->column = this->oldColumn = 1;
	this->line = 1;
    this->atEof = false;

	// Buffer 1 füllen
	if (this->readBytes(this->buffer1, BUFFERSIZE) == 0) {
		fprintf(stderr, "Datei ist leer.\n");
		exit(-1);
	}
	// Starte mit Buffer 1
	this->currentBuffer = &this->buffer1;
}

// Inline Methode, die Speicher alloziert, nur zur Übersichtlichkeit ausgelagert.
inline void Buffer::allocMem(void **mempt, int align, int size) {
//1) Unterschied align, size?
}
//belegt size, wobei der allozierte Speicher das mehrfache von align ist.mempt speichert adresse ab der alloziert wurde
	if (posix_memalign(mempt, align, size) != 0) {
		perror("Speicher allozieren fehlgeschlagen");
		exit(-1);
	}
}

// Destruktor, gibt alle Ressourcen wieder frei
Buffer::~Buffer(){
	if (this->fileDescriptor > 0) close(this->fileDescriptor);
	free(this->memptr1);
	free(this->memptr2);
}

// Lese count chars in buf, gibt die Anzahl der gelesenen Bytes zurück
// Wird beim Initialisieren des Buffer 1 aufgerufen
int Buffer::readBytes(char* buf, int count) {
//ist fileDescriptor >0, so wurde erfolgreich gelesen
//Fall bytesRead == BUFFERSIZE wird hier nicht beachtet, da später der 2. Buffer zur Hilfe gezogen wird
	if (fileDescriptor > 0) {

// liest aus fileDescriptor und schreibt count-viele Bytes in buf
		int bytesRead = read(this->fileDescriptor, buf, count);

		if (bytesRead < 0) {
			/* ERRORS:
			   EINVAL: fd is attached to an object which is unsuitable for reading;
			   or the file was opened with the O_DIRECT flag, and either the address
			   specified in buf, the value specified in count, or the current file
			   offset is not suitably aligned.
            */
			perror("Fehler beim lesen");
			exit(-1);
		}

		// Wenn weniger Bytes zurückgeliefert wurden als angefordert wurden
		// (Dateiende ist nahe), setze EOF-Markierer hinter letztes gelesenen char
		if (bytesRead < BUFFERSIZE) buf[bytesRead] = '\0';
		return bytesRead;
	}
	return -1; // Falls kein fileDescriptor, sollte nicht auftreten
}



// Gib mir ein char!
char Buffer::getChar() {
    if (this->atEof) return '\0';
	char curr = getCharFromCurrentBuffer();

	// Bei Zeilenumbruch, zeile++ und column=1
	if (curr == LF) {
		this->oldColumn = this->column;
		this->column = 1;
		this->line++;
	} else this->column++;

    if (curr == '\0') {
        this->atEof = true;
    }

	// Buffer zeigt auf nächstes Zeichen
	this->currentBufferIndex++;
	this->totalBufferIndex++;

	return curr;
}

// Liest das aktuelle Zeichen aus dem Buffer, geht aber nicht vor oder zurück, das muss vorher erledigt werden

//Methode wird von getChar() aufgerufen, darin findet ein Fallunterscheidung abh. vom currentBufferIndex statt
char Buffer::getCharFromCurrentBuffer() {

	//Ist currentBufferIndex negativ, so wurde er noch nicht geladen, was dann getan wird
	// fehlerhafte Initialisierung o.ä.
	//Dieser Fall tritt ein wenn im buffer2 gelesen wird, ein Error bemerkt wird und er z.b. 10 zeichen zurück springen muss, welches sich aber in buffer 1 befindet
	// dementsprechend muss der andere buffer wieder gelesen werden und der index auf das vorletzte zeichengröße
	if (this->currentBufferIndex < 0) {

		// Buffer tauschen
		if (this->currentBuffer == &this->buffer1) {
			this->currentBuffer = &this->buffer2;
		} else if (this->currentBuffer == &this->buffer2) {
			this->currentBuffer = &this->buffer1;
		}
		// Index ans Ende des Buffers setzen
		this->currentBufferIndex = BUFFERSIZE-1;

// Ist currentBufferIndex größer gleich BUFFERSIZE, so muss der nächste Buffer geladen werden, sonst Überlauf
	} else if (this->currentBufferIndex >= BUFFERSIZE) {
		if (this->currentBuffer == &this->buffer1) {
		// Buffer 2 noch nicht initialisiert
			if (!this->memptr2) {
				allocMem(&this->memptr2, BUFFERSIZE, BUFFERSIZE + sizeof(char));
				this->buffer2 = (char*)this->memptr2;
				this->buffer2[BUFFERSIZE] = '\0';
			}
			// Buffer 2 vollladen
			this->readBytes(this->buffer2, BUFFERSIZE);
			// Aktueller Buffer ist Buffer 2
			this->currentBuffer = &this->buffer2;
		} else if (this->currentBuffer == &this->buffer2) {
			// Buffer 1 volladen
			this->readBytes(this->buffer1, BUFFERSIZE);
			// Aktueller Buffer ist Buffer 1
			this->currentBuffer = &this->buffer1;
		}
		// BufferIndex an den Anfang
		this->currentBufferIndex = 0;
	}
	// Char vom aktuellen Buffer an der Stelle index zurückgeben
	return (*this->currentBuffer)[this->currentBufferIndex];
}

int Buffer::getBuffersize() {
    return BUFFERSIZE;
}

int Buffer::getCol() {
	return this->column;
}


int Buffer::getLine() {
	return this->line;
}

// Gibt mehrere chars zurück
void Buffer::ungetChar(int count) {
	for (int i = 0; i < count; i++) {
		this->ungetChar();
	}
}

// Gibt genau ein Zeichen zurück
void Buffer::ungetChar() {
	// Wenn man am Anfang der Datei ist
	if (this->totalBufferIndex == 0) {
		fprintf(stderr, "Kann nicht weiter zurückgehen.\n");
		exit(-1);
	}

	// Eins zurück
	this->currentBufferIndex--;
	this->totalBufferIndex--;
	char curr = this->getCharFromCurrentBuffer();
	// Überprüfen, ob gerade ein Zeilenumbruch zurückgegeben wurde
	if (curr == LF) { // dann ans Ende der vorherigen Zeile
		this->column = this->oldColumn;
		this->line--;
	  // Sonst nur eine Spalte zurück
	} else this->column--;
}
