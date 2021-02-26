
#pragma comment(lib,"winmm.lib")    // for timeGetTime

#include "board.h"

Board b;
long lastChecked;

void loadTexture()
{
	b.texture = ilutGLLoadImage("abc.png");
}

void writeString(char* str, float x, float y)
{
	static const int indexByCharacter[128] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,63,0,
		                                       0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										       0,0,0,0, 52, 53, 54, 55, 56, 57, 58, 59, 60,
										       61, 62,0,0,0,0,0,0, 0,1,2,3,4,5,6,7,8,9,10,11,
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
		glBindTexture( GL_TEXTURE_2D, b.texture );
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

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glBegin(GL_QUADS);

		// Background
		glColor3f(0.3f, 0.3f, 0.3f);
		glVertex3f(-50, 50, 4);
		glVertex3f( 50, 50, 4);
		glVertex3f( 50,-50, 4);
		glVertex3f(-50,-50, 4);

		// Next piece
		glColor3f(1, 1, 1);
		glVertex3f(12, 20, 0);
		glVertex3f(18, 20, 0);
		glVertex3f(18, 14, 0);
		glVertex3f(12, 14, 0);

		// Level
		glVertex3f(19.25, 13, 0);
		glVertex3f(10.75, 13, 0);
		glVertex3f(10.75, 11, 0);
		glVertex3f(19.25, 11, 0);

		// Score
		glVertex3f(19.25, 10, 0);
		glVertex3f(10.75, 10, 0);
		glVertex3f(10.75, 0, 0);
		glVertex3f(19.25, 0, 0);

		// Line count
		glVertex3f(-9.25, 20, 0);
		glVertex3f(-0.75, 20, 0);
		glVertex3f(-0.75, 18, 0);
		glVertex3f(-9.25, 18, 0);

		// Stats
		glVertex3f(-9.25, 16, 0);
		glVertex3f(-0.75, 16, 0);
		glVertex3f(-0.75, 0, 0);
		glVertex3f(-9.25, 0, 0);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, b.texture );
	glBegin(GL_QUADS);
		// Playing field
		glTexCoord2d(0.00, 0.5);	glVertex3f( 0,20, 1);
		glTexCoord2d(0.25, 0.5);	glVertex3f(10,20, 1);
		glTexCoord2d(0.25, 1.0);	glVertex3f(10, 0, 1);
		glTexCoord2d(0.00, 1.0);	glVertex3f( 0, 0, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	b.draw();

	writeString("Lines:", -9.0, 18.50);
	writeString("Level:", 11.0, 11.50);
	writeString("Score", 11.0, 8.5);
	writeString("Next", 13.0, 18.50);
	glutSwapBuffers();
}

void paused()
{
	Sleep(1);
}

void timer()
{
	if(timeGetTime() - lastChecked > b.getDelay())
	{
		b.moveDown(false);
		lastChecked = timeGetTime();
		glutPostRedisplay();
	}
	Sleep(1);
}

void keys(int key, int x, int y)
{
	if(!b.isPaused())
	{
		switch(key)
		{
			case GLUT_KEY_UP:
				b.rotate();
				break;
			case GLUT_KEY_LEFT:
				b.moveLeft();
				break;
			case GLUT_KEY_RIGHT:
				b.moveRight();
				break;
			case GLUT_KEY_DOWN:
				b.moveDown(true);
				break;
		}
		glutPostRedisplay();
	}
	else
	{
		switch(key)
		{
			case GLUT_KEY_UP:
			case GLUT_KEY_DOWN:
				b.toggleOption();
				glutPostRedisplay();
				break;
		}
	}
}

void keys2(unsigned char key, int x, int y)
{
	static uint pause= 0;
	switch(key)
	{
		case ' ':
			if(!b.isPaused())
			{
				glutIdleFunc(paused);
				pause = timeGetTime();
			}
			else
			{
				lastChecked += timeGetTime() - pause;
				glutIdleFunc(timer);
			}
			b.pause();
			break;
		case '\r':
			if(b.isPaused())
			{
				if(b.getOption() == 0)
				{
					glutIdleFunc(timer);
					b.reset();
					lastChecked = timeGetTime();
				}
				else
				{
					b.~Board();
					timeEndPeriod(10);
					exit(0);
				}
			}
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	timeBeginPeriod(1);
	// setup glut window
	glutInit(&argc, argv);
	glutInitWindowSize(640,480);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);
	glutCreateWindow("Tetris");
	glutDisplayFunc(display); // choose function that performs display
	glutIdleFunc(timer);

	// init
    glMatrixMode(GL_PROJECTION);			// use projection matrix
    glLoadIdentity();						// clear projection matrix
    glOrtho(-10.0,20.0,-0.6875,20.6875,5.0,-5.0);    // set ortho
    glMatrixMode(GL_MODELVIEW);				// use modelview matrix
    glLoadIdentity();						// clear modelview matrix
	glutKeyboardFunc(keys2);
    glutSpecialFunc(keys);		// send special key presses to changeRegion()

	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);
	loadTexture();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	b.nextPiece();
	lastChecked = timeGetTime();
	glutMainLoop(); // start main loop

	timeEndPeriod(10);

	return 0;
}