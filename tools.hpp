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

extern const unsigned fieldSizeWight;
extern const unsigned fieldSizeHeight;
extern const unsigned cellSize;

using point=std::pair<int, int>;

extern point operator+(const point &lhs, const point &rhs);

class Timer{
private:
	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;
public:
	void begin();
	void stop();
	void disp() const;
};
