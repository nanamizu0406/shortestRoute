#pragma once

#include"tools.hpp"
#include"field.hpp"

enum Status{
	NONE,
	OPEN,
	CLOSED,
};

class AStar{
private:
public:
	AStar();
	~AStar();
	Status status;
	int cost;
	int heuristic;
	point coord;
	AStar* parent;
	int getScore() const;
};

extern bool compare(const AStar* obj1, const AStar* obj2);

class Search{
private:
	std::vector<std::vector<AStar>> astar;
	std::vector<point> route;
	const std::vector<point> dir4={
		{-1,0},{0,-1},{1,0},{0,1}
	};
	const std::vector<point> dir8={
		{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1}
	};
public:
	Search();
	~Search();
	void inits();
	int heuristicCost4(const point& coord, const point& goal) const;
	int heuristicCost8(const point& coord, const point& goal) const;
	bool aStar4(Field& fields);
	bool aStar8(Field& fields);
	void makeRoute(const point& goal);
	void printRoute() const;
};
