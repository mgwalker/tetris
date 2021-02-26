#include <iostream>

#include <stdlib.h>
#include <time.h>

#include "shapes.h"
#include <GL\glut.h>

#define MIN_X 0
#define MAX_X 1
#define MIN_Y 2
#define TRANS_X 3
#define TRANS_Y 4
#define ROTATE 5

#define TEE 0
#define OH 1
#define EL 2
#define JAY 3
#define ES 4
#define ZEE 5
#define EYE 6

#define MIN_XR 0
#define MAX_XR 1
#define MIN_YR 2

void Shape::rotate()
{
	static const unsigned int rots[7][3][8] =
		// minX :			maxX :			 minY : 
		// 0, 90, 180, 270  0, 90, 180, 270  0, 90, 180, 270
		{{{ 1, -1, 0, 0 },  { 0, 0, -1, 1 }, { 0, 1, -1, 0 }},  // TEE
		 {{ 0, 0, 0, 0 },   { 0, 0, 0, 0 },  { 0, 0, 0, 0 }},   // OH
		 {{ -1, 0, 0, 1 },  { 0, -1, 1, 0 }, { 1, -1, 0, 0 }},  // EL
		 {{ 0, 1, -1, 0 },  { 1, 0, 0, -1 }, { 0, 0, 1, -1 }},  // JAY
		 {{ 1, -1, 0, 0 },  { 0, 0, 0, 0 },  { 0, 0, 0, 0 }},   // ES
		 {{ 1, -1, 0, 0 },  { 0, 0, 0, 0 },  { 0, 0, 0, 0 }},   // ZEE
		 {{ -1, 1, 0, 0 },  { 2, -2, 0, 0 }, { 2, -2, 0, 0 }}}; // EYE 

	if(this->_currentDisplayListIndex == 1 || !this->_hasDropped)
		return;

	bool canRotate = true;
	unsigned int currRot = this->_rotation / 90;

	int minXR = this->_minX + rots[this->_currentDisplayListIndex][MIN_XR][currRot]; 
	int	maxXR = this->_maxX + rots[this->_currentDisplayListIndex][MAX_XR][currRot]; 
	int	minYR = this->_minY + rots[this->_currentDisplayListIndex][MIN_YR][currRot];

	/*switch(this->_currentDisplayListIndex + this->_rotation)
	{
		case 0:		// TEE
			minXR++;
			break;
		case 90:
			minXR--;
			minYR++;
			break;
		case 180:
			maxXR--;
			minYR--;
			break;
		case 270:
			maxXR++;
			break;

		case 2:		// ELL
			minXR--;
			minYR++;
			break;
		case 92:
			maxXR--;
			minYR--;
			break;
		case 182:
			maxXR++;
			break;
		case 272:
			minXR++;
			break;

		case 3:		// JAY
			maxXR++;
			break;
		case 93:
			minXR++;
			break;
		case 183:
			minXR--;
			minYR++;
			break;
		case 273:
			maxXR--;
			minYR--;
			break;

		case 4:		// ESS
			minXR++;
			break;
		case 94:
			minXR--;
			break;

		case 5:		// ZEE
			minXR++;
			break;
		case 95:
			minXR--;
			break;

		case 6:		// EYE
			minXR--;
			maxXR += 2;
			minYR += 2;
			break;
		case 96:
			minXR++;
			maxXR -= 2;
			minYR -= 2;
			break;
	}*/

	if(minXR >= 0 && maxXR <= 10 && minYR >= 0)
	{
		if(_currentDisplayListIndex < 4)
			(this->_rotation < 270) ? this->_rotation+=90 : this->_rotation = 0;
		else 
			(this->_rotation == 0) ? this->_rotation = 90 : this->_rotation = 0;
		this->_minX = minXR;
		this->_maxX = maxXR;
		this->_minY = minYR;
	}
}

void Shape::nextShape()
{
	static const float ShapeInit[7][6] =	
		// minx, maxx, miny, xtrans, ytrans, rotation
		{{ 3, 6, 18, 4.5, 19.5, 0 },   // TEE
		 { 4, 6, 18, 5,   19,   0 },   // OH
		 { 3, 6, 18, 4.5, 19.5, 270 }, // EL
		 { 4, 7, 18, 5.5, 19.5, 90 },  // JAY
		 { 3, 6, 18, 4.5, 19.5, 0 },   // ES
		 { 3, 6, 18, 4.5, 19.5, 0 },   // ZEE
		 { 3, 7, 19, 4.5, 19.5, 90 }}; // EYE 

	//this->_rotation = 0;
	this->_hasDropped = false;
	this->_currentDisplayListIndex = rand()%7;
	
	this->_rotation = (unsigned int)ShapeInit[this->_currentDisplayListIndex][ROTATE];
	this->_minX =	  (unsigned int)ShapeInit[this->_currentDisplayListIndex][MIN_X];
	this->_maxX =	  (unsigned int)ShapeInit[this->_currentDisplayListIndex][MAX_X];
	this->_minY =	  (unsigned int)ShapeInit[this->_currentDisplayListIndex][MIN_Y];
	this->_xTrans =	  ShapeInit[this->_currentDisplayListIndex][TRANS_X];
	this->_yTrans =   ShapeInit[this->_currentDisplayListIndex][TRANS_Y];

	/*switch(this->_currentDisplayListIndex)
	{
		case 0:	// TEE
			this->_minX = 3;
			this->_maxX = 6;
			this->_minY = 18;
			this->_xTrans = 4.5;
			this->_yTrans = 19.5;
			break;
		case 1:	// OH
			this->_minX = 4;
			this->_maxX = 6;
			this->_minY = 18;
			this->_xTrans = 5;
			this->_yTrans = 19;
			break;
		case 2:	// ELL
			this->_minX = 3;
			this->_maxX = 6;
			this->_minY = 18;
			this->_rotation = 270;
			this->_xTrans = 4.5;
			this->_yTrans = 19.5;
			break;
		case 3:	// JAY
			this->_minX = 4;
			this->_maxX = 7;
			this->_minY = 18;
			this->_rotation = 90;
			this->_xTrans = 5.5;
			this->_yTrans = 19.5;
			break;
		case 4:	// ESS
			this->_minX = 3;
			this->_maxX = 6;
			this->_minY = 18;
			this->_xTrans = 4.5;
			this->_yTrans = 19.5;
			break;
		case 5:	// ZEE
			this->_minX = 3;
			this->_maxX = 6;
			this->_minY = 18;
			this->_xTrans = 4.5;
			this->_yTrans = 19.5;
			break;
		case 6:	// EYE
			this->_minX = 3;
			this->_maxX = 7;
			this->_minY = 19;
			this->_rotation = 90;
			this->_xTrans = 4.5;
			this->_yTrans = 19.5;
			break;
	}*/
}

unsigned int Shape::rotation()
{
	return this->_rotation;
}

float Shape::xTrans()
{
	return this->_xTrans;
}

float Shape::yTrans()
{
	return this->_yTrans;
}

void Shape::moveLeft()
{
	if(this->_minX > 0)
	{
		this->_minX--;
		this->_maxX--;
		this->_xTrans -= 1;
	}
}

void Shape::moveRight()
{	
	if(this->_maxX < 10)
	{
		this->_minX++;
		this->_maxX++;
		this->_xTrans += 1;
	}
}

void Shape::moveDown()
{
	if(this->_minY > 0)
	{
		this->_hasDropped = true;
		this->_yTrans -= 1;
		this->_minY--;
	}
}

unsigned int Shape::currentShape()
{
	return this->_shapeLists[this->_currentDisplayListIndex];
}

void Shape::createShapes()
{
	srand(time(NULL));

	// TEE
	this->_shapeLists[0] = glGenLists(7);
	glNewList(this->_shapeLists[0], GL_COMPILE);
		glBegin(GL_QUADS);
			glVertex2f(-1.5f, 0.5f);
			glVertex2f(-0.5f, 0.5f);
			glVertex2f(-0.5f,-0.5f);
			glVertex2f(-1.5f,-0.5f);

			glVertex2f(-0.5f, 0.5f);
			glVertex2f( 0.5f, 0.5f);
			glVertex2f( 0.5f,-0.5f);
			glVertex2f(-0.5f,-0.5f);

			glVertex2f(0.5f, 0.5f);
			glVertex2f(1.5f, 0.5f);
			glVertex2f(1.5f,-0.5f);
			glVertex2f(0.5f,-0.5f);

			glVertex2f(-0.5f,-0.5f);
			glVertex2f( 0.5f,-0.5f);
			glVertex2f( 0.5f,-1.5f);
			glVertex2f(-0.5f,-1.5f);
		glEnd();
	glEndList();

	// OH
	this->_shapeLists[1] = this->_shapeLists[0]+1;
	glNewList(this->_shapeLists[1], GL_COMPILE);
		glBegin(GL_QUADS);
			glVertex2f(-1.0f, 1.0f);
			glVertex2f(0.0f, 1.0f);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(-1.0f, 0.0f);

			glVertex2f(0.0f, 1.0f);
			glVertex2f(1.0f, 1.0f);
			glVertex2f(1.0f, 0.0f);
			glVertex2f(0.0f, 0.0f);

			glVertex2f(-1.0f, 0.0f);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(0.0f, -1.0f);
			glVertex2f(-1.0f, -1.0f);

			glVertex2f(0.0f, 0.0f);
			glVertex2f(1.0f, 0.0f);
			glVertex2f(1.0f, -1.0f);
			glVertex2f(0.0f, -1.0f);
		glEnd();
	glEndList();

	// EL
	this->_shapeLists[2] = this->_shapeLists[1]+1;
	glNewList(this->_shapeLists[2], GL_COMPILE);
		glBegin(GL_QUADS);
			glVertex2f(-0.5f, 1.5f);
			glVertex2f( 0.5f, 1.5f);
			glVertex2f( 0.5f, 0.5f);
			glVertex2f(-0.5f, 0.5f);

			glVertex2f(-0.5f, 0.5f);
			glVertex2f( 0.5f, 0.5f);
			glVertex2f( 0.5f,-0.5f);
			glVertex2f(-0.5f,-0.5f);

			glVertex2f(-0.5f,-0.5f);
			glVertex2f( 0.5f,-0.5f);
			glVertex2f( 0.5f,-1.5f);
			glVertex2f(-0.5f,-1.5f);

			glVertex2f( 0.5f,-0.5f);
			glVertex2f( 1.5f,-0.5f);
			glVertex2f( 1.5f,-1.5f);
			glVertex2f( 0.5f,-1.5f);
		glEnd();
	glEndList();

	// JAY
	this->_shapeLists[3] = this->_shapeLists[2]+1;
	glNewList(this->_shapeLists[3], GL_COMPILE);
		glBegin(GL_QUADS);
			glVertex2f(-0.5f, 1.5f);
			glVertex2f( 0.5f, 1.5f);
			glVertex2f( 0.5f, 0.5f);
			glVertex2f(-0.5f, 0.5f);

			glVertex2f(-0.5f, 0.5f);
			glVertex2f( 0.5f, 0.5f);
			glVertex2f( 0.5f,-0.5f);
			glVertex2f(-0.5f,-0.5f);

			glVertex2f(-0.5f,-0.5f);
			glVertex2f( 0.5f,-0.5f);
			glVertex2f( 0.5f,-1.5f);
			glVertex2f(-0.5f,-1.5f);

			glVertex2f(-1.5f,-0.5f);
			glVertex2f(-0.5f,-0.5f);
			glVertex2f(-0.5f,-1.5f);
			glVertex2f(-1.5f,-1.5f);
		glEnd();
	glEndList();

	// ES
	this->_shapeLists[4] = this->_shapeLists[3]+1;
	glNewList(this->_shapeLists[4], GL_COMPILE);
		glBegin(GL_QUADS);
			glVertex2f(-0.5f, 0.5f);
			glVertex2f( 0.5f, 0.5f);
			glVertex2f( 0.5f,-0.5f);
			glVertex2f(-0.5f,-0.5f);

			glVertex2f( 0.5f, 0.5f);
			glVertex2f( 1.5f, 0.5f);
			glVertex2f( 1.5f,-0.5f);
			glVertex2f( 0.5f,-0.5f);

			glVertex2f(-1.5f,-0.5f);
			glVertex2f(-0.5f,-0.5f);
			glVertex2f(-0.5f,-1.5f);
			glVertex2f(-1.5f,-1.5f);

			glVertex2f(-0.5f,-0.5f);
			glVertex2f( 0.5f,-0.5f);
			glVertex2f( 0.5f,-1.5f);
			glVertex2f(-0.5f,-1.5f);
		glEnd();
	glEndList();

	// ZEE
	this->_shapeLists[5] = this->_shapeLists[4]+1;
	glNewList(this->_shapeLists[5], GL_COMPILE);
		glBegin(GL_QUADS);
			glVertex2f(-0.5f, 0.5f);
			glVertex2f( 0.5f, 0.5f);
			glVertex2f( 0.5f,-0.5f);
			glVertex2f(-0.5f,-0.5f);

			glVertex2f(-1.5f, 0.5f);
			glVertex2f(-0.5f, 0.5f);
			glVertex2f(-0.5f,-0.5f);
			glVertex2f(-1.5f,-0.5f);

			glVertex2f( 0.5f,-0.5f);
			glVertex2f( 1.5f,-0.5f);
			glVertex2f( 1.5f,-1.5f);
			glVertex2f( 0.5f,-1.5f);

			glVertex2f(-0.5f,-0.5f);
			glVertex2f( 0.5f,-0.5f);
			glVertex2f( 0.5f,-1.5f);
			glVertex2f(-0.5f,-1.5f);
		glEnd();
	glEndList();

	// EYE
	this->_shapeLists[6] = this->_shapeLists[5] + 1;
	glNewList(this->_shapeLists[6], GL_COMPILE);
		glBegin(GL_QUADS);
			glVertex2f(-0.5f, 1.5f);
			glVertex2f( 0.5f, 1.5f);
			glVertex2f( 0.5f, 0.5f);
			glVertex2f(-0.5f, 0.5f);

			glVertex2f(-0.5f, 0.5f);
			glVertex2f( 0.5f, 0.5f);
			glVertex2f( 0.5f,-0.5f);
			glVertex2f(-0.5f,-0.5f);

			glVertex2f(-0.5f,-0.5f);
			glVertex2f( 0.5f,-0.5f);
			glVertex2f( 0.5f,-1.5f);
			glVertex2f(-0.5f,-1.5f);

			glVertex2f(-0.5f,-1.5f);
			glVertex2f( 0.5f,-1.5f);
			glVertex2f( 0.5f,-2.5f);
			glVertex2f(-0.5f,-2.5f);
		glEnd();
	glEndList();

	this->nextShape();
}