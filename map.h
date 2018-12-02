#pragma once
#include <iostream>
#include <fstream>
#include <GL\glut.h>
#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3
using namespace std;

const double PI = acos(-1.0);
const float BlockSize = 18;
const int MapSize = 20;
const float colorOfPacman[3] = { 1.0,1.0,0.0 }; //yellow
const float colorOfBeastPink[3] = { 1.0,0.0,1.0 }; //pink
const float colorOfBeastBlue[3] = { 0.0,0.0,1.0 }; //blue
const float colorOfBeastRed[3] = { 1.0,0.0,0.0 }; //red
const float colorOfBeastGreen[3] = { 0.0,1.0,0.0 }; //green
const int dir[4][2] = { { 0,1 },{ -1,0 },{ 0,-1 },{ 1,0 } };//上左下右


class map {
private:
	string mapFile;
public:
	int size;
	int startX, startY;
	char mapArr[100][100];
	map(string file, int x, int y);
	void draw();
};

extern map *newMap;

class block {
public:
	float x, y;
	block(int xx, int yy) :x(xx), y(yy) {};
	virtual void draw() {};
};

class Pacman :public block {
public:
	int xi, yi;//pacman在地图矩阵里的位置
	int pacStat;
	int angel;
	Pacman(int xx, int yy) :block(newMap->startX + xx*BlockSize, newMap->startY + yy*BlockSize), xi(xx), yi(yy), pacStat(RIGHT), angel(0) {};
	void draw();
	int move();
	
};

extern Pacman *pacman;//起始位置左下角

class Beast :public block {
public:
	int xi, yi;
	int BeastStat;
	int Color[3];
	Beast(int xx, int yy, const float color[], int stat) :block(newMap->startX + xx*BlockSize, newMap->startY + yy*BlockSize), xi(xx), yi(yy), BeastStat(stat){
		Color[0] = color[0]; Color[1] = color[1]; Color[2] = color[2];
	};
	void draw();
	void move();
};

extern Beast *beastPink;
extern Beast *beastBlue;
extern Beast *beastRed;
extern Beast *beastGreen;

