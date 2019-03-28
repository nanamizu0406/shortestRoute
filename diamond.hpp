#include"tools.hpp"

class Terrain{
private:
	std::vector<std::vector<double>> field;
	const std::vector<point> orthogonal1={
		{-1,0},{0,-1},{1,0},{0,1}
	};
	const std::vector<point> orthogonal2={
		{-2,0},{0,-2},{2,0},{0,2}
	};
	const std::vector<point> diagonal={
		{-1,-1},{1,-1},{1,1},{-1,1}
	};
	const double noise=0.25;
	pointDouble sea1;
	pointDouble sea2;
	pointDouble beach;
	pointDouble grass1;
	pointDouble grass2;	
	pointDouble froat1;
	pointDouble froat2;
	pointDouble mountain1;
	pointDouble mountain2;	
	
private:
	double random(const double min, const double max) const;
	double approximation2(const double val1, const double val2) const;
	double average2(const double val1, const double val2) const;
	double approximation4(const double val1, const double val2, const double val3, const double val4) const;
	double average4(const double val1, const double val2, const double val3, const double val4) const; 
public:
	Terrain();
	~Terrain();
	void inits();
	void clear();
	void print() const;
	void diamondSquare();
	double get(const point& coord) const;
};
