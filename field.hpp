#pragma once

#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<random>
#include<chrono>
#include<cmath>
#include<queue>
#include<GL/glut.h>

#include"tools.hpp"

class Field{
private:
	enum State{
		EMPTY,
		WALL,
	};
private:
	std::vector<std::vector<int>> field;
	std::vector<point> mouseLog;
	point start;
	point goal;
public:
	Field();
	~Field();
	void inits();
	void recordCoord(int x, int y);
	void setStartGoal(int x, int y);
	void plotWall();
	void changeCoord(point& obj) const;
	void printConsole() const;
	void printField() const;
	void printWall() const;
	int getState(const point& coord) const;
	point getStart() const;
	point getGoal() const;
	bool isWall(const point& coord) const;
};

