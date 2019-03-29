#pragma once

#include"tools.hpp"
#include"field.hpp"
#include"diamond.hpp"

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
  double cost;
	double heuristic;
	point coord;
	AStar* parent;
	double getScore() const;
};

extern bool compare(const AStar* obj1, const AStar* obj2);

class Dijkstra{
private:
public:
	Dijkstra();
	~Dijkstra();
	std::vector<point> edgesTo;
	std::vector<double> edgesCost;
	bool done;
	double cost;
	point coord;
	Dijkstra* parent;
};

extern bool compare2(const Dijkstra* obj1, const Dijkstra* obj2);

class Search{
private:
	std::vector<point> route;
	std::vector<std::vector<AStar>> astar;
	std::vector<std::vector<Dijkstra>> dijkstra;
private:
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
public:
	void initsAstar();
	int heuristicCost4(const point& coord, const point& goal) const;
	int heuristicCost8(const point& coord, const point& goal) const;
	bool aStar4(Field& fields, Terrain& terrain);
	bool aStar8(Field& fields, Terrain& terrain);
public:
	void initsDijkstra4(Field& fields, Terrain& terrain);
	void initsDijkstra8(Field& fields, Terrain& terrain);
	bool dijkstraSearch(Field& fields, Terrain& terrain);
public:
	void makeRoute(const point& goal);
	void printRoute() const;
};
