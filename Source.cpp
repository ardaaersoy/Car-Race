/********************
STUDENT  : ARDA ERSOY
ID       :   21502589
SECTION  :          1
HOMEWORK :      HW #1
********************/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define TIMER_PERIOD   10 // Period for the timer.
#define TIMER_ON        1 // 0: disable timer, 1:enable timer
#define D2R     0.0174532
#define RUN             1 // 1: to run the game
#define END             0 // 0: to top the game

/* Global Variables for Template File */
int xCar[5] = { -400,-400,-400,-400,-400 }; // to initialize the cars's state to starting point 
int direction[5] = { 0 }, dir = 120; // define cars' direction state
int  winWidth, winHeight; // current Window width and height
int min = 0, sec = 0, mil = 0; // to calculate the time
int winner = 1; // to find winner

bool up = false, down = false, right = false, left = false;
bool game = END, activeTimer = false; // to define game and timer state

// to draw circle, center at (x,y), radius r
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void print(int x, int y, char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);

	for (i = 0; i < len; i++)
		glutBitmapCharacter(font, string[i]);
}

// display text with variables.
void vprint(int x, int y, void *font, char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);

	for (i = 0; i < len; i++)
		glutBitmapCharacter(font, str[i]);
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);

	for (i = 0; i < len; i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);

	glPopMatrix();
}

void background()
{
	// to display the background rectangles 
	glColor3ub(90, 120, 100);
	glRectf(-400, 200, 400, 300);

	glColor3ub(150, 152, 192);
	glRectf(-180, 290, 170, 240);

	// to display the time, winner, header and needed information 
	glColor3f(0, 0.1, 0.7);
	vprint(-340, 260, GLUT_BITMAP_HELVETICA_18, "TIME");
	vprint(270, 260, GLUT_BITMAP_HELVETICA_18, "WINNER");

	glColor3f(0.2, 0.4, 0);
	vprint(-170, 260, GLUT_BITMAP_HELVETICA_18, "Racing Car Animation By ARDA ERSOY");

	glColor3f(1, 1, 0);
	vprint(-170, 215, GLUT_BITMAP_8_BY_13, "Press <SpaceBar> to Stop/Restart Animation");

	// to display 5 lines to divide cars' part
	for (int i = 100; i > -300; i -= 100)
	{
		glLineWidth(3);
		glColor3ub(192, 192, 192);
		glBegin(GL_LINES);
		glVertex2f(-400, i);
		glVertex2f(400, i);
		glEnd();
	}
}

void findWinner()
{
	// to find the car which one is first while they are going to go through +400 
	if (direction[0] == 0 && direction[1] == 0 && direction[2] == 0 && direction[3] == 0 && direction[4] == 0)
	{
		if (xCar[0] > xCar[1] && xCar[0] > xCar[2] && xCar[0] > xCar[3] && xCar[0] > xCar[4])
			winner = 1;
		if (xCar[1] > xCar[0] && xCar[1] > xCar[2] && xCar[1] > xCar[3] && xCar[1] > xCar[4])
			winner = 2;
		if (xCar[2] > xCar[0] && xCar[2] > xCar[1] && xCar[2] > xCar[3] && xCar[2] > xCar[4])
			winner = 3;
		if (xCar[3] > xCar[0] && xCar[3] > xCar[1] && xCar[3] > xCar[2] && xCar[3] > xCar[4])
			winner = 4;
		if (xCar[4] > xCar[0] && xCar[4] > xCar[1] && xCar[4] > xCar[2] && xCar[4] > xCar[3])
			winner = 5;
	}

	// to find the car which one is first while they are going to go through -400 
	if (direction[0] == 1 && direction[1] == 1 && direction[2] == 1 && direction[3] == 1 && direction[4] == 1)
	{
		if (xCar[0] < xCar[1] && xCar[0] < xCar[1] && xCar[0] < xCar[1] && xCar[0] < xCar[1])
			winner = 1;
		if (xCar[1] < xCar[0] && xCar[1] < xCar[2] && xCar[1] < xCar[3] && xCar[1] < xCar[4])
			winner = 2;
		if (xCar[2] < xCar[0] && xCar[2] < xCar[1] && xCar[2] < xCar[3] && xCar[2] < xCar[4])
			winner = 3;
		if (xCar[3] < xCar[0] && xCar[3] < xCar[1] && xCar[3] < xCar[2] && xCar[3] < xCar[4])
			winner = 4;
		if (xCar[4] < xCar[0] && xCar[4] < xCar[1] && xCar[4] < xCar[2] && xCar[4] < xCar[3])
			winner = 5;
	}

}

void drawCar(int xB, int yB, int num)
{
	// to draw a car which consists of rectangle, triangle, line and circle
	glColor3ub(0, 0, 0);
	glRectf(xB, yB, xB + 70, yB + 20);
	glRectf(xB + 20, yB + 20, xB + 45, yB + 35);

	glColor3f(0.5, 0.5, 0.7);
	glBegin(GL_TRIANGLES);
	glVertex2f(xB + 20, yB + 20);
	glVertex2f(xB + 10, yB + 20);
	glVertex2f(xB + 20, yB + 35);
	glEnd();

	glColor3f(0.5, 0.5, 0.7);
	glBegin(GL_TRIANGLES);
	glVertex2f(xB + 45, yB + 20);
	glVertex2f(xB + 45, yB + 35);
	glVertex2f(xB + 55, yB + 20);
	glEnd();

	glColor3f(0.7, 0.7, 0.7);
	circle(xB + 15, yB + 2, 5);
	circle(xB + 50, yB + 2, 5);

	glColor3f(0.1, 0.2, 0.7);
	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex2f(xB + 33.5, yB + 33);
	glVertex2f(xB + 33.5, yB);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(xB + 10, yB + 20);
	glVertex2f(xB + 55, yB + 20);
	glEnd();

	// to write car's numbers 
	glColor3f(0, 1, 0.5);
	vprint(xB + 24, yB + 22, GLUT_BITMAP_8_BY_13, "0");
	vprint(xB + 36, yB + 22, GLUT_BITMAP_8_BY_13, "%d", num);
}

void display()
{
	// clear window to black
	glClearColor(0, 0.4, 0.4, 0.2);
	glClear(GL_COLOR_BUFFER_BIT);

	background(); // to display background information and shapes

	// to draw 5 cars according their direction coordinates
	drawCar(xCar[0], 120, 1);
	drawCar(xCar[1], 20, 2);
	drawCar(xCar[2], -80, 3);
	drawCar(xCar[3], -180, 4);
	drawCar(xCar[4], -280, 5);

	if (mil % 2 == 0)
		glColor3f(0.7, 0, 0.1);
	else
		glColor3f(1, 1, 1);
	vprint(-345, 230, GLUT_BITMAP_9_BY_15, "%d:%d:%d", min, sec, mil); // to show the time (min:sec:mil)

	findWinner();
	if (mil % 2 == 0)
		glColor3f(0.7, 0, 0.1);
	else
		glColor3f(1, 1, 1);
	vprint(295, 230, GLUT_BITMAP_9_BY_15, "0%d", winner); // to show the winner

	if (game == RUN  && activeTimer == true)
	{
		for (int i = 0; i < 5; i++)
		{
			// to determine each cars' speed by giving them random numbers
			int random[5] = { { rand() % 3 + 1 },{ rand() % 3 + 1 },{ rand() % 3 + 1 },{ rand() % 3 + 1 },{ rand() % 3 + 1 } };

			// to reverse all cars when they reach the boundary (+400)
			if (xCar[i] < 330 && direction[i] == 0)
				xCar[i] += random[i];
			else
				direction[i] = 1;
			if (xCar[i] >= -400 && direction[i] == 1)
				xCar[i] -= random[i];
		}

		// to stop the game when any car reaches the (-400)
		if (xCar[0] <= -400 || xCar[1] <= -400 || xCar[2] <= -400 || xCar[3] <= -400 || xCar[4] <= -400)
		{
			game = END;
			activeTimer = false;
		}
	}

	glutSwapBuffers();
}

// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// stop and run the game and the time at the same time
	if (key == 32)
	{
		if (game == END)
		{
			activeTimer = true;
			game = RUN;
		}
		else
		{
			activeTimer = false;
			game = END;
		}
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT

void onSpecialKeyDown(int key, int x, int y)
{
	if (key == GLUT_KEY_F1 && activeTimer == false)
	{
		min = 0, sec = 0, mil = 0; // to initialize the time to 0

		// to reset all cars to initial step
		xCar[0] = -400, xCar[1] = -400, xCar[2] = -400, xCar[3] = -400, xCar[4] = -400;

		// to reset all cars direction to 0
		direction[0] = 0, direction[1] = 0, direction[2] = 0, direction[3] = 0, direction[4] = 0;
	}

	// case GLUT_KEY_F1: reset(); break;
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT

void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.

void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.

void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y)
{
	// Write your codes here.
	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y)
{
	// Write your codes here.
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v)
{
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);

	// to calculate the time
	if (activeTimer == true)
	{
		mil++;
		if (mil == 100)
		{
			sec++;
			mil = 0;
		}
		if (sec == 60)
		{
			min++;
			sec = 0;
		}
	}

	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()
}
#endif

void Init()
{
	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Homework: Animating a Complex Shape");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	// keyboard registration
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	// mouse registration
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

	srand(time(NULL)); // to produce random numbers for cars' speed

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}