#include "map.h"
#include <queue>
#include <vector>
//map *newMap = new map("map/1.txt", 60, 50);
//Pacman *pacman = new Pacman(0, 1);
//Beast *beastPink = new Beast(10, 10, colorOfBeastPink, UP);
//Beast *beastBlue = new Beast(9, 10, colorOfBeastBlue, UP);
//Beast *beastRed = new Beast(9, 9, colorOfBeastRed, DOWN);
//Beast *beastGreen = new Beast(10, 9, colorOfBeastGreen, DOWN);


map::map(string file, int x, int y) {
	mapFile = file;
	ifstream infile(file);
	if (!infile) {
		cout << "file not found" << endl;
		exit(0);
	}
	infile >> size;
	cout << size << endl;
	char t;
	for (int i = 0; i < size / 2; i++) { 
		for (int j = 0; j < size / 2; j++) {
			infile >> mapArr[j][i];
			mapArr[size - 1 - j][i] = mapArr[j][size - 1 - i] = mapArr[size - 1 - j][size - 1 - i] = mapArr[j][i];
		}
	}
	startX = x;
	startY = y;
}

void map::draw() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (mapArr[i][j] == '1') {
				glColor3f(1.0, 0.4, 0.7);
				glPointSize(BlockSize);
			}
			else if(mapArr[i][j] == '0')
			{
				glColor3f(1.0, 1.0, 1.0);
				glPointSize(BlockSize / 10.0);
			}
			else {
				glColor3f(0.0, 0.0, 0.0);
				glPointSize(BlockSize);
			}
			glBegin(GL_POINTS);
			glVertex2f(60 + BlockSize*i, 50 + BlockSize*j);
			glEnd();
		}
	}
	glFlush();
}

void Pacman::draw() {
	this->x = newMap->startX + this->xi*BlockSize;
	this->y = newMap->startY + this->yi*BlockSize;
	
	glColor3f(colorOfPacman[0], colorOfPacman[1], colorOfPacman[2]);
	
	glBegin(GL_TRIANGLE_FAN);
	
	glVertex2f(x, y);
	double xx, yy;
	
	for (int i = -210-angel+90*pacStat; i < 30+angel+90*pacStat; i++) {
		xx = x + cos(i*PI / 180.0)*(BlockSize / 2.0 - 1.0);
		yy = y + sin(i*PI / 180.0)*(BlockSize / 2.0 - 1.0);
		glVertex2d(xx, yy);
	}

	if (angel > 40)
		angel = 0;
	angel += 13;
	
	glEnd();
	glFlush();
	
	cout << this->xi << " " << this->yi << endl;
}

int Pacman::move() {
	int d = pacStat;
	int posx, posy;
	posx = this->xi + dir[d][0];
	posy = this->yi + dir[d][1];
	if (posx < 0)
		posx = MapSize - 1;
	if (posx > MapSize - 1)
		posx = 0;
	if (posy < 0)
		posy = MapSize - 1;
	if (posy > MapSize - 1)
		posy = 0;
	if (newMap->mapArr[posx][posy] != '1') {
		this->xi = posx;
		this->yi = posy;
	}
	cout << newMap->mapArr[this->xi][this->yi] << endl;
	if (newMap->mapArr[this->xi][this->yi] == '0') {
		newMap->mapArr[this->xi][this->yi] = 'b';
		return 1;
	}
	return 0;
}

void Beast::draw() {
	this->x = newMap->startX + this->xi*BlockSize;
	this->y = newMap->startY + this->yi*BlockSize;

	glColor3f(Color[0], Color[1], Color[2]);
	glBegin(GL_TRIANGLE_FAN);
	double xx, yy;
	//怪物的头--半圆
	for (int i = 0; i < 180; i++) {
		xx = x + cos(i*PI / 180.0)*(BlockSize / 2.0 - 1.0);
		yy = y + sin(i*PI / 180.0)*(BlockSize / 2.0 - 1.0);
		glVertex2d(xx, yy);
	}
	glEnd();

	//怪物身体
	glRectf(x - BlockSize / 2.0 + 1.0, y+1.0, x + BlockSize / 2.0 - 1.0, y - BlockSize / 2.0);

	//怪物右眼
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_TRIANGLE_FAN);
	int xxx = x + BlockSize / 4.0, yyy = y + BlockSize / 4.0;
	for (int i = 0; i < 360; i++) {
		xx = xxx + cos(i*PI / 180.0)*(BlockSize / 10.0)-BlockSize/10.0;
		yy = yyy + sin(i*PI / 180.0)*(BlockSize / 10.0);
		glVertex2d(xx, yy);
	}
	glEnd();

	//怪物左眼
	glBegin(GL_TRIANGLE_FAN);
	xxx = x - BlockSize / 4.0;
	for (int i = 0; i < 360; i++) {
		xx = xxx + cos(i*PI / 180.0)*(BlockSize / 10.0) + BlockSize / 10.0;
		yy = yyy + sin(i*PI / 180.0)*(BlockSize / 10.0);
		glVertex2d(xx, yy);
	}
	glEnd();
	cout << this->xi << " " << this->yi << endl;
	glFlush();
}

void Beast::move() {
	int d = BeastStat;
	int posx, posy;
	posx = this->xi + dir[d][0];
	posy = this->yi + dir[d][1];
	if (posx < 0)
		posx = MapSize - 1;
	if (posx > MapSize - 1)
		posx = 0;
	if (posy < 0)
		posy = MapSize - 1;
	if (posy > MapSize - 1)
		posy = 0;


	if (newMap->mapArr[posx][posy] != '1') {
		if (BeastStat % 2 == 0) {//Beast is walking vertically
			if (newMap->mapArr[posx - 1][posy] != '1' || newMap->mapArr[posx + 1][posy] != '1') {
				if (newMap->mapArr[posx - 1][posy] != '1' && rand() % 2 == 1) {//walking left is possible
					BeastStat = LEFT;
				}
				else if(newMap->mapArr[posx+1][posy] != '1' && rand() % 2 == 1){ //walking right is possible
					BeastStat = RIGHT;
				}
			}
		}
		else { //Beast is walking horizontally
			if (newMap->mapArr[posx][posy-1] != '1' || newMap->mapArr[posx][posy+1] != '1') {
				if (newMap->mapArr[posx][posy-1] != '1' && rand() % 2 == 1) {//walking down is possible
					BeastStat = DOWN;
				}
				else if (newMap->mapArr[posx][posy+1] != '1' && rand() % 2 == 1) { //walking up is possible
					BeastStat = UP;
				}
			}
		}
		this->xi = posx;
		this->yi = posy;
	}
	else {
		while ((BeastStat = rand() % 4) == d);
	}
}

