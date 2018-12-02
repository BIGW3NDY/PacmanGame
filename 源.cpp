#include <GL/glut.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include "map.h"

using namespace std;
const float WindowSize = 500;
// ASCII字符总共只有0到127，一共128种字符
#define MAX_CHAR       128

map *newMap;
Pacman *pacman;
Beast *beastPink;
Beast *beastBlue;
Beast *beastRed;
Beast *beastGreen;

class Game {
public :
	int gameStat;
	int TIME;
	int SCORE;
	int LIFE;
	Game() {
		gameStat = 0;
		TIME = 0;
		SCORE = 0;
		LIFE = 3;
		newMap = new map("map/1.txt", 60, 50);
		pacman = new Pacman(0, 1);
		beastPink = new Beast(10, 10, colorOfBeastPink, UP);
		beastBlue = new Beast(9, 10, colorOfBeastBlue, UP);
		beastRed = new Beast(9, 9, colorOfBeastRed, DOWN);
		beastGreen = new Beast(10, 9, colorOfBeastGreen, DOWN);
	}
	~Game() {
		free(newMap);
		free(pacman);
		free(beastPink);
		free(beastBlue);
		free(beastRed);
		free(beastGreen);
	}
}*currentGame;

//int gameStat = 0; // 0-not start; 1-playing; 2-dead; 3-pause; 4-win
//int TIME = 0;
//int SCORE = 0;
//int LIFE = 3;

void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) {
		isFirstCall = 0;
		lists = glGenLists(MAX_CHAR);
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

void check() {
	//catch by the beast
	//add the offset to see if the pacman was catched 
	if ((pacman->xi - 0.5*dir[pacman->pacStat][0] == beastBlue->xi - 0.5*dir[beastBlue->BeastStat][0]
		&& pacman->yi - 0.5*dir[pacman->pacStat][1] == beastBlue->yi - 0.5*dir[beastBlue->BeastStat][1])
		|| (pacman->xi == beastBlue->xi && pacman->yi == beastBlue->yi)) {
		currentGame->gameStat = 3;
		cout << "beastBlue" << endl;
		currentGame->LIFE--;
	}
	if (pacman->xi - 0.5*dir[pacman->pacStat][0] == beastRed->xi - 0.5*dir[beastRed->BeastStat][0]
		&& pacman->yi - 0.5*dir[pacman->pacStat][1] == beastRed->yi - 0.5*dir[beastRed->BeastStat][1]
		|| (pacman->xi == beastRed->xi && pacman->yi == beastRed->yi)) {
		currentGame->gameStat = 3;
		cout << "beastRed" << endl;
		currentGame->LIFE--;
	}
	if (pacman->xi - 0.5*dir[pacman->pacStat][0] == beastGreen->xi - 0.5*dir[beastGreen->BeastStat][0]
		&& pacman->yi - 0.5*dir[pacman->pacStat][1] == beastGreen->yi - 0.5*dir[beastGreen->BeastStat][1]
		|| (pacman->xi == beastGreen->xi && pacman->yi == beastGreen->yi)) {
		currentGame->gameStat = 3;
		cout << "beastGreen" << endl;
		currentGame->LIFE--;
	}
	if (pacman->xi - 0.5*dir[pacman->pacStat][0] == beastPink->xi - 0.5*dir[beastPink->BeastStat][0]
		&& pacman->yi - 0.5*dir[pacman->pacStat][1] == beastPink->yi - 0.5*dir[beastPink->BeastStat][1]
		|| (pacman->xi == beastPink->xi && pacman->yi == beastPink->yi)) {
		currentGame->gameStat = 3;
		cout << "beastPink" << endl;
		currentGame->LIFE--;
	}
	if (!currentGame->LIFE)
		currentGame->gameStat = 2;
}

void StartGame(unsigned char key, int x, int y) {
	if (key == ' ') {
		if (currentGame->gameStat == 0)
			currentGame->gameStat = 1;
		if(currentGame->gameStat == 1)
			currentGame->gameStat = 3; // pause
		if (currentGame->gameStat == 3)  // pause and restart
			currentGame->gameStat = 1;
		if (currentGame->gameStat == 2 || currentGame->gameStat ==4) {  //dead or win and restart
			currentGame->gameStat = 0;
			currentGame = new Game();
		}
		glutPostRedisplay();
	}
}

void PlayScreen() {
	glClear(GL_COLOR_BUFFER_BIT);
	cout << "paintint table title" << endl;
	char Time[10], Score[10], Life[10];
	_itoa_s(currentGame->TIME, Time, 10);
	_itoa_s(currentGame->SCORE, Score, 10);
	_itoa_s(currentGame->LIFE, Life, 10);
	glRasterPos2f(WindowSize / 2.0 - 100, WindowSize - 50);
	drawString("TIME: ");
	drawString(Time);
	glRasterPos2f(WindowSize / 2.0 , WindowSize - 50);
	drawString("SCORE: ");
	drawString(Score);
	glRasterPos2f(WindowSize / 2.0 + 100, WindowSize - 50);
	drawString("LIFE: ");
	drawString(Life);

	cout << "paint game map" << endl;
	newMap->draw();
	//pacmanMove();
	pacman->draw();
	beastPink->draw();
	beastBlue->draw();
	beastRed->draw();
	beastGreen->draw();

}

void keyboard(int key, int x, int y) {
	if (key == GLUT_KEY_UP)
		pacman->pacStat = 0;
	else if (key == GLUT_KEY_LEFT)
		pacman->pacStat = 1;
	else if (key == GLUT_KEY_DOWN)
		pacman->pacStat = 2;
	else if (key == GLUT_KEY_RIGHT)
		pacman->pacStat = 3;
}

void idleFunc() {
	//pacmanMove();
	if (currentGame->gameStat==1) {
		currentGame->SCORE += pacman->move();
		beastBlue->move();
		beastRed->move();
		beastGreen->move();
		beastPink->move();
		check();
		glutPostRedisplay();
	}
}

void myDisplay(void)
{
	if (currentGame->SCORE == newMap->size*newMap->size)
		currentGame->gameStat = 4;
	glClear(GL_COLOR_BUFFER_BIT);
	if (!currentGame->gameStat) {
		glRasterPos2f(WindowSize / 2.0 - 70, WindowSize / 2.0);
		drawString("press space to play...");
	}
	else if (currentGame->gameStat == 2) {
		glRasterPos2f(WindowSize / 2.0 - 70, WindowSize / 2.0 + 10);
		drawString("your pacman is dead!");
		glRasterPos2f(WindowSize / 2.0 - 70, WindowSize / 2.0 -10);
		drawString("press space to restart game");
		
	}
	else if (currentGame->gameStat == 4) {
		glRasterPos2f(WindowSize / 2.0 - 70, WindowSize / 2.0 + 10);
		drawString("you win!\n");
		glRasterPos2f(WindowSize / 2.0 - 70, WindowSize / 2.0 - 10);
		drawString("you've eaten all the beans");
	}
	else {
		PlayScreen();
	}
	Sleep(100);
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	currentGame = new Game();
	srand(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WindowSize, WindowSize);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glutCreateWindow("Pacman Go");
	gluOrtho2D(0.0, WindowSize, 0.0, WindowSize);
	glutDisplayFunc(&myDisplay);
	glutIdleFunc(&idleFunc);
	glutSpecialFunc(&keyboard);
	glutKeyboardFunc(&StartGame);
	glutMainLoop();
	return 0;
}
