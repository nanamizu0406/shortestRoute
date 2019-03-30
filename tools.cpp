#include"tools.hpp"

// 2ⁿ+1 × 2ⁿ+1にする65 129...
const unsigned fieldSizeWight=129;
const unsigned fieldSizeHeight=129;
const unsigned cellSize=5;

point operator+(const point &lhs, const point &rhs){
	point p={lhs.first+rhs.first, lhs.second+rhs.second};
	return p;
}
	
void Timer::begin(){
	this->start=std::chrono::system_clock::now();
}

void Timer::stop(){
	this->end=std::chrono::system_clock::now();
}

void Timer::disp() const{
	unsigned val=std::chrono::duration_cast<std::chrono::microseconds>(this->end-this->start).count();
	std::cout<<val<<" micro"<<std::endl;
}
