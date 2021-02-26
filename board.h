#ifndef __BOARD_H
#define __BOARD_H

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <string.h>
#include <time.h>
#include <GL/glut.h>
#include "bitfield.h"
#include <IL\ilut.h>

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned char color[3];

class Board
{
	public:
		Board();
		~Board();
		void draw();
		uint getDelay();
		void nextPiece();
		void moveLeft();
		void moveRight();
		void moveDown(bool);
		void rotate();
		void lockPiece();
		int  checkRows();
		bool collision(uchar, uchar, uchar, uchar);
		bool isPaused();
		void pause();
		void toggleOption();
		int getOption();
		void reset();
		GLuint texture;

	private:
		//void loadTexture();
		BitField *_setBoard;
		int _textureIndices[200];

		void writeString(char* str, float x, float y);

		uint  _stats[7];
		uchar _nextPiece;
		uchar _pieceIndex;
		uchar _pieceLocation[4];
		int   _textureIndex;
		int   _pieceRotation;
		uint  _totalLinesCleared;
		uint  _score;
		uint  _level;
		uint  _fallDelay;
		bool  _isPaused;
		float _option;
		char* _menuText;
};

#endif