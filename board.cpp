#include "board.h"

Board::Board()
{
	//this->loadTexture();
	this->_setBoard = new BitField(200);
	srand(time(NULL));
	this->_fallDelay = 800;
	this->_level = 0;
	this->_totalLinesCleared = 0;
	this->_score = 0;
	this->_isPaused = true;
	this->_nextPiece = rand() % 7;
	this->_menuText = "ILLITRIS";
	memset(this->_stats,0,7*sizeof(uint));
}

Board::~Board()
{
	//glDeleteTextures( 1, &this->texture );
}

void Board::draw()
{
	static const int piece[7][4] = { {3,4,5,14},	// TEE
									 {4,5,14,15},	// OH
									 {3,4,5,6},		// EYE
									 {3,4,14,15},	// ZEE
									 {4,5,13,14},	// ES
									 {3,4,5,13},	// EL
									 {3,4,5,15}};	// JAY
	static const double sPiece[7][4][2] =	{ {{-8,15}, {-7,15}, {-6,15},  {-7,14}},	// TEE	-- STAT PIECES
										  {{-8,12.5}, {-7,12.5}, {-8,11.5}, {-7,11.5}},	// OH
										  {{-8,10}, {-7,10}, {-6,10},  {-5,10}},	// EYE
										  {{-8,8.5}, {-7,8.5}, {-7,7.5}, {-6,7.5}},	// ZEE
										  {{-6,6}, {-7,6}, {-7,5}, {-8,5}},	// ES
										  {{-8,3.5}, {-7,3.5}, {-6,3.5},  {-8,2.5}},	// EL
										  {{-8,1}, {-7,1}, {-6,1},  {-6,0}}};	// JAY
	static const int tIndices[7] = { 249, 250, 251, 252, 253, 254, 255 };

	if(this->_isPaused)
	{
		// menu
		glBegin(GL_QUADS);
			//-10.0,20.0		-0.6875,20.6875
			glColor4f(0.0, 0.0, 0.0, 0.5);
			glVertex3f(-10.0, -0.6875, -0.5);
			glVertex3f(-10.0, 20.6875, -0.5);
			glVertex3f( 20.0, 20.6875, -0.5);
			glVertex3f( 20.0, -0.6875, -0.5);

			glColor4f(1.0, 1.0, 1.0, 1.0);
			glVertex3f(0.5, 5, -0.7);
			glVertex3f(0.5,17, -0.7);
			glVertex3f(9.5,17, -0.7);
			glVertex3f(9.5, 5, -0.7);

			glColor3f(1.0, 0, 0);
			glVertex3f(2.0, 12.9 + this->_option, -0.8);
			glVertex3f(2.4, 12.9 + this->_option, -0.8);
			glVertex3f(2.4, 13.3 + this->_option, -0.8);
			glVertex3f(2.0, 13.3 + this->_option, -0.8);

		glEnd();

		double xpos = 5 - (strlen(this->_menuText) * 0.425);
		writeString(this->_menuText, xpos, 15.5);
		writeString("NEW GAME", 2.5, 12.5);
		writeString("EXIT", 2.5, 11.0);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture( GL_TEXTURE_2D, texture );

		glBegin(GL_QUADS);
		int x, y;
		glColor3f(1.0, 1.0, 1.0);
		for(int i = 0; i < 200; i++)
		{
			if(this->_setBoard->isBitSet(i))
			{
				x = i % 10;
				y = 19 - (i / 10);
				//glColor3ubv(this->_colors[i]);
				float tx = (this->_textureIndices[i] % 16) * 0.0625;
				glTexCoord2d(tx,          1.0);		glVertex3i(x,     y,     0);
				glTexCoord2d(tx + 0.0625, 1.0);		glVertex3i(x,     y + 1, 0);
				glTexCoord2d(tx + 0.0625, 0.9375);	glVertex3i(x + 1, y + 1, 0);
				glTexCoord2d(tx,          0.9375);	glVertex3f(x + 1, y,     0);
			}
		}

		for(int i = 0; i < 4; i++)
		{
			x = this->_pieceLocation[i] % 10;
			y = 19 - (this->_pieceLocation[i] / 10);

			float tx = (this->_textureIndex % 16) * 0.0625;
			glTexCoord2d(tx,          1.0);		glVertex3i(x,     y,     0);
			glTexCoord2d(tx + 0.0625, 1.0);		glVertex3i(x,     y + 1, 0);
			glTexCoord2d(tx + 0.0625, 0.9375);	glVertex3i(x + 1, y + 1, 0);
			glTexCoord2d(tx,          0.9375);	glVertex3f(x + 1, y,     0);
		}

		for(int i = 0; i < 4; i++)
		{
			float x = 10 + piece[this->_nextPiece][i] % 10;
			float y = 16 - (piece[this->_nextPiece][i] / 10);

			float tx = (tIndices[this->_nextPiece] % 16) * 0.0625;
			glTexCoord2d(tx,          1.0);		glVertex3f(x,     y,     -0.25);
			glTexCoord2d(tx + 0.0625, 1.0);		glVertex3f(x,     y + 1, -0.25);
			glTexCoord2d(tx + 0.0625, 0.9375);	glVertex3f(x + 1, y + 1, -0.25);
			glTexCoord2d(tx,          0.9375);	glVertex3f(x + 1, y,     -0.25);
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, texture );

	glBegin(GL_QUADS);
	for(int spieceIndex = 0; spieceIndex < 7; spieceIndex++)
	{
		for(int i = 0; i < 4; i++)
		{
			float x = sPiece[spieceIndex][i][0];
			float y = sPiece[spieceIndex][i][1];

			float tx = (tIndices[spieceIndex] % 16) * 0.0625;
			glTexCoord2d(tx,          1.0);		glVertex3f(x,     y,     -1);
			glTexCoord2d(tx + 0.0625, 1.0);		glVertex3f(x,     y + 1, -1);
			glTexCoord2d(tx + 0.0625, 0.9375);	glVertex3f(x + 1, y + 1, -1);
			glTexCoord2d(tx,          0.9375);	glVertex3f(x + 1, y,     -1);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	char* buffer = new char[100];
	writeString(itoa(this->_totalLinesCleared, buffer, 10), -4.25, 18.50);
	writeString(itoa(this->_level, buffer, 10), 15.75, 11.50);
	writeString(itoa(this->_score, buffer, 10), 12.5, 7.0);
	writeString(itoa(this->_stats[0], buffer, 10), -3.75, 14.5);	// TEE
	writeString(itoa(this->_stats[1], buffer, 10), -3.75, 12.0);	// OH
	writeString(itoa(this->_stats[2], buffer, 10), -3.75, 10.0);	// EYE
	writeString(itoa(this->_stats[3], buffer, 10), -3.75,  8.0);	// ZEE
	writeString(itoa(this->_stats[4], buffer, 10), -3.75,  5.5);	// ES
	writeString(itoa(this->_stats[5], buffer, 10), -3.75,  3.0);	// EL
	writeString(itoa(this->_stats[6], buffer, 10), -3.75,  0.5);	// JAY
	delete buffer;
}

void Board::toggleOption()
{
	this->_option = (this->_option == 0) ? -1.5 : 0;
}

int Board::getOption()
{
	return (this->_option == 0) ? 0 : 1;
}

void Board::reset()
{
	//this->_setBoard = new BitField(200);
	this->_setBoard->clear();
	//srand(time(NULL));
	this->_fallDelay = 800;
	this->_level = 0;
	this->_totalLinesCleared = 0;
	this->_score = 0;
	this->_isPaused = 0;
	this->_nextPiece = rand() % 7;
	memset(this->_stats,0,7*sizeof(uint));
	this->nextPiece();
}

void Board::nextPiece()
{
	static const int piece[7][4] = { {3,4,5,14},	// TEE
									 {4,5,14,15},	// OH
									 {3,4,5,6},		// EYE
									 {3,4,14,15},	// ZEE
									 {4,5,13,14},	// ES
									 {3,4,5,13},	// EL
									 {3,4,5,15}};	// JAY
	static const int tIndices[7] = { 249, 250, 251, 252, 253, 254, 255 };

	this->_pieceIndex = this->_nextPiece;
	this->_textureIndex = tIndices[this->_pieceIndex];
	this->_stats[this->_pieceIndex]++;
	this->_nextPiece = rand() % 7;
	this->_pieceRotation = 0;
	const int* position = piece[this->_pieceIndex];

	if(!this->collision(piece[this->_pieceIndex][0], piece[this->_pieceIndex][1], piece[this->_pieceIndex][2], piece[this->_pieceIndex][3]))
	{
		this->_pieceLocation[0] = piece[this->_pieceIndex][0];
		this->_pieceLocation[1] = piece[this->_pieceIndex][1];
		this->_pieceLocation[2] = piece[this->_pieceIndex][2];
		this->_pieceLocation[3] = piece[this->_pieceIndex][3];
	}
	else
	{
		this->_isPaused = true;
		this->_menuText = "GAME OVER";
	}
}

void Board::moveLeft()
{
	if(this->_pieceLocation[0]%10 && this->_pieceLocation[1]%10 && this->_pieceLocation[2]%10 && this->_pieceLocation[3]%10)
	{
		if(!this->collision(this->_pieceLocation[0] - 1, this->_pieceLocation[1] - 1, this->_pieceLocation[2] - 1, this->_pieceLocation[3] - 1))
		{
			this->_pieceLocation[0]--;
			this->_pieceLocation[1]--;
			this->_pieceLocation[2]--;
			this->_pieceLocation[3]--;
		}
	}
}

void Board::moveRight()
{
	if((this->_pieceLocation[0]+1)%10 && (this->_pieceLocation[1]+1)%10 && (this->_pieceLocation[2]+1)%10 && (this->_pieceLocation[3]+1)%10)
	{
		if(!this->collision(this->_pieceLocation[0] + 1, this->_pieceLocation[1] + 1, this->_pieceLocation[2] + 1, this->_pieceLocation[3] + 1))
		{
			this->_pieceLocation[0]++;
			this->_pieceLocation[1]++;
			this->_pieceLocation[2]++;
			this->_pieceLocation[3]++;
		}
	}
}

void Board::moveDown(bool isUser)
{
	if(this->_pieceLocation[0] < 190 && this->_pieceLocation[1] < 190 && this->_pieceLocation[2] < 190 && this->_pieceLocation[3] < 190)
	{
		if(!this->collision(this->_pieceLocation[0] + 10, this->_pieceLocation[1] + 10, this->_pieceLocation[2] + 10, this->_pieceLocation[3] + 10))
		{
			this->_pieceLocation[0] += 10;
			this->_pieceLocation[1] += 10;
			this->_pieceLocation[2] += 10;
			this->_pieceLocation[3] += 10;
		}
		else if(!isUser)
			this->lockPiece();
	}
	else if(!isUser)
		this->lockPiece();
}

void Board::rotate()
{
	/*
	 The rotation function needs to check to make sure pieces don't rotate
	 above the top of the board.  If they do, they won't move anymore,
	 except to keep rotating back into the field of play.
	 */

	static const int rotation[7][4][4] = {{{-9,0,9,-11},  {11,0,-11,-9},  {9,0,-9,11},   {-11,0,11,9}},		// TEE
										  {{0,0,0,0},     {0,0,0,0},      {0,0,0,0},     {0,0,0,0}},		// OH
										  {{-9,0,9,18},   {9,0,-9,-18},   {-9,0,9,18},   {9,0,-9,-18}},		// EYE
										  {{-8,1,-10,-1}, {8,-1,10,1},    {-8,1,-10,-1}, {8,-1,10,1}},		// ZEE
										  {{0,-11,2,-9},  {0,11,-2,9},    {0,-11,2,-9},  {0,11,-2,9}},		// ES
										  {{-9,0,9,-20},  {11,0,-11,2},   {9,0,-9,20},   {-11,0,11,-2}},	// ELL
										  {{-9,0,9,-2},   {11,0,-11,-20}, {9,0,-9,2},    {-11,0,11,20}} };	// JAY

	if(this->_pieceRotation == 360)
		this->_pieceRotation = 0;
	const int* rot = rotation[this->_pieceIndex][this->_pieceRotation / 90];

	uint p1 = this->_pieceLocation[0] + rot[0];
	uint p2 = this->_pieceLocation[1] + rot[1];
	uint p3 = this->_pieceLocation[2] + rot[2];
	uint p4 = this->_pieceLocation[3] + rot[3];

	uint d1 = abs(this->_pieceLocation[0]%10 - p1%10);
	uint d2 = abs(this->_pieceLocation[1]%10 - p2%10);
	uint d3 = abs(this->_pieceLocation[2]%10 - p3%10);
	uint d4 = abs(this->_pieceLocation[3]%10 - p4%10);

	if(d1 < 5 &&  d2 < 5 && d3 < 5 && d4 < 5)
	{
		if(p1 < 200 && p2 < 200 && p3 < 200 && p4 < 200)
		{
			if(!this->collision(p1, p2, p3, p4))
			{
				this->_pieceLocation[0] = p1;
				this->_pieceLocation[1] = p2;
				this->_pieceLocation[2] = p3;
				this->_pieceLocation[3] = p4;

				this->_pieceRotation += 90;
			}
		}
	}
}

void Board::lockPiece()
{
	this->_setBoard->setBit(this->_pieceLocation[0]);
	this->_setBoard->setBit(this->_pieceLocation[1]);
	this->_setBoard->setBit(this->_pieceLocation[2]);
	this->_setBoard->setBit(this->_pieceLocation[3]);

	for(int i = 0; i < 4; i++)
	{
		this->_setBoard->setBit(this->_pieceLocation[i]);
		this->_textureIndices[this->_pieceLocation[i]] = this->_textureIndex;
	}

	this->checkRows();
	this->nextPiece();
}

int Board::checkRows()
{
	static const uint score[4] = { 40, 100, 300, 1200 };
	int complete = 0;

	for(int i = 19; i >= 0; i--)
	{
		while(this->_setBoard->isBitRangeSet(i*10,i*10+9))
		{
			this->_setBoard->bitCopy(0, 10, i*10);
			this->_setBoard->unsetBitRange(0,10);

			int tmpTextures[200];
			memcpy(tmpTextures, this->_textureIndices, 200*sizeof(int));
			memcpy(&this->_textureIndices[10], tmpTextures, i*10*sizeof(int));
			memset(this->_textureIndices, 0, 10*sizeof(int)); 
			complete++;
		}
	}

	if(complete > 0)
	{
		this->_totalLinesCleared += complete;
		this->_score += (this->_level+1) * score[complete-1];
		this->_level = (this->_totalLinesCleared / 10);
		if(this->_level < 30)
			this->_fallDelay = 800 - ((this->_level)*16);
	}

	return complete;
}

bool Board::collision(uchar p1, uchar p2, uchar p3, uchar p4)
{
	if(this->_setBoard->isBitSet(p1))
		return true;
	if(this->_setBoard->isBitSet(p2))
		return true;
	if(this->_setBoard->isBitSet(p3))
		return true;
	if(this->_setBoard->isBitSet(p4))
		return true;

	return false;
}
uint Board::getDelay()
{
	return this->_fallDelay;
}

bool Board::isPaused()
{
	return this->_isPaused;
}

void Board::pause()
{
	this->_isPaused = !this->_isPaused;
	if(this->_isPaused)
		this->_menuText = "PAUSED";
}

void Board::writeString(char* str, float x, float y)
{
	static const int indexByCharacter[128] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		                                       0,0,0,0,0,0,0,0,0,0,63,0,0,0,0,0,0,0,0,0,0,0,
										       0,0,0,0, 52, 53, 54, 55, 56, 57, 58, 59, 60,
										       61, 0,0,0,0,0,0,0, 0,1,2,3,4,5,6,7,8,9,10,11,
										       12,13,14,15,16,17,18,19,20,21,22,23,24,25,0,0,
										       0,0,0,0,26,27,28,29,30,31,32,33,34,35,36,37,
											   38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,0,0,0,0 };

	int idx;
	float cX, cY;
	float j = 0;
	for(int i = 0; i < strlen(str); i++)
	{
		idx = indexByCharacter[str[i]];
		cX = (idx % 16) * 0.0625;
		cY = (idx / 16) * 0.0625;
		glEnable(GL_TEXTURE_2D);
		glBindTexture( GL_TEXTURE_2D, this->texture);
		glBegin(GL_QUADS);
			glTexCoord2d(cX,          cY+0.0625); glVertex3d(x + j,       y,      -1);
			glTexCoord2d(cX + 0.0625, cY+0.0625); glVertex3d(x + j + 1.0, y,      -1);
			glTexCoord2d(cX + 0.0625, cY+0.002);  glVertex3d(x + j + 1.0, y + 1.0,-1);
			glTexCoord2d(cX,          cY+0.002);  glVertex3d(x + j,       y + 1.0,-1);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		j += 0.85;
	}
}