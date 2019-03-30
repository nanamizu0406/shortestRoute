#include"search.hpp"

AStar::AStar():status(NONE),cost(-1),heuristic(-1),coord(0, 0),parent(nullptr){
}

AStar::~AStar(){
}

bool compare1(const AStar* obj1, const AStar* obj2){
	bool result=obj1->getScore()!=obj2->getScore();
	return (result?obj1->getScore()<obj2->getScore():obj1->heuristic<obj2->heuristic);
}

double AStar::getScore() const{
	return this->cost+this->heuristic;
}

Dijkstra::Dijkstra():done(false),cost(-1),coord(0,0),parent(nullptr){
}

Dijkstra::~Dijkstra(){
}

bool compare2(const Dijkstra* obj1, const Dijkstra* obj2){
	if(obj1->cost==obj2->cost)
		return obj1->coord.first<obj2->coord.first;
	return obj1->cost<obj2->cost;
}

Search::Search(){
	this->inits();
}

Search::~Search(){
}

void Search::inits(){
	this->route.clear();
	this->astar.clear();
	this->dijkstra.clear();
}

void Search::initsAstar(){
	this->inits();
	this->astar.resize(fieldSizeHeight);
	std::for_each(this->astar.begin(), this->astar.end(), [this](auto& vec){
			vec.resize(fieldSizeWight);
		});
	for(int i=0;i<fieldSizeHeight;i++){
		for(int j=0;j<fieldSizeWight;j++){
			this->astar.at(i).at(j).coord=std::make_pair(j, i);
		}
	}
}

int Search::heuristicCost4(const point& coord, const point& goal) const{
	int dx=std::abs(goal.first-coord.first);
	int dy=std::abs(goal.second-coord.second);
	return std::max(dx, dy);
}

int Search::heuristicCost8(const point& coord, const point& goal) const{
	int dx=std::abs(goal.first-coord.first);
	int dy=std::abs(goal.second-coord.second);
	return dx+dy;
}

bool Search::aStar4(Field& fields, Terrain& terrain){
	this->initsAstar();
	std::vector<AStar*> openList;
	AStar* start=&this->astar.at(fields.getStart().second).at(fields.getStart().first);
	start->status=OPEN;
	start->cost=0;
	start->heuristic=this->heuristicCost4(start->coord, fields.getGoal());
	openList.push_back(start);
	AStar* current;
	AStar* next;
	point searchCoord;
	while(!openList.empty()){
		std::sort(openList.begin(), openList.end(), compare1);
		current=openList.at(0);
		if(current->coord==fields.getGoal())
			return true;
		std::for_each(this->dir4.begin(), this->dir4.end(), [&,this](auto& dir){
				searchCoord=current->coord+dir;
				next=&this->astar.at(searchCoord.second).at(searchCoord.first);
				if(!fields.isWall(searchCoord)&&next->status==NONE){
					next->status=OPEN;
					point nextCoord=next->coord;
					next->cost=current->cost+1+terrain.get(nextCoord);
					next->heuristic=this->heuristicCost4(next->coord, fields.getGoal());
					next->parent=current;
					openList.push_back(next);
				}
			});
		current->status=CLOSED;
		openList.erase(openList.begin());
	}
	return false;
}

bool Search::aStar8(Field& fields, Terrain& terrain){
	this->initsAstar();
	std::vector<AStar*> openList;
	AStar* start=&this->astar.at(fields.getStart().second).at(fields.getStart().first);
	start->status=OPEN;
	start->cost=0;
	start->heuristic=this->heuristicCost8(start->coord, fields.getGoal());
	openList.push_back(start);
	AStar* current;
	AStar* next;
	point searchCoord;
	while(!openList.empty()){
		std::sort(openList.begin(), openList.end(), compare1);
		current=openList.at(0);
		if(current->coord==fields.getGoal())
			return true;
		std::for_each(this->dir8.begin(), this->dir8.end(), [&,this](auto& dir){
				searchCoord=current->coord+dir;
				next=&this->astar.at(searchCoord.second).at(searchCoord.first);
				if(!fields.isWall(searchCoord)&&next->status==NONE){
					next->status=OPEN;
					point nextCoord=next->coord;
					next->cost=current->cost+1+terrain.get(nextCoord);
					next->heuristic=this->heuristicCost8(next->coord, fields.getGoal());
					next->parent=current;
					openList.push_back(next);
				}
			});
		current->status=CLOSED;
		openList.erase(openList.begin());
	}
	return false;
}

void Search::initsDijkstra4(Field& fields,Terrain &terrain){
	this->inits();
	this->dijkstra.resize(fieldSizeHeight);
	std::for_each(this->dijkstra.begin(), this->dijkstra.end(), [this](auto& vec){
			vec.resize(fieldSizeWight);
		});
	for(int i=0;i<fieldSizeHeight;i++){
		for(int j=0;j<fieldSizeWight;j++){
			point current=std::make_pair(j, i);
			this->dijkstra.at(i).at(j).coord=current;
		}
	}
	for(int i=1;i<fieldSizeHeight-1;i++){
		for(int j=1;j<fieldSizeWight-1;j++){
			for(int k=0;k<this->dir4.size();k++){
				point current=std::make_pair(j, i);
				point next=current+this->dir4.at(k);
				if(!fields.isWall(next)){
					this->dijkstra.at(i).at(j).edgesTo.push_back(next);
					this->dijkstra.at(i).at(j).edgesCost.push_back(1+terrain.get(next));
				}
			}
		}	
	}
}

void Search::initsDijkstra8(Field& fields,Terrain &terrain){
	this->inits();
	this->dijkstra.resize(fieldSizeHeight);
	std::for_each(this->dijkstra.begin(), this->dijkstra.end(), [this](auto& vec){
			vec.resize(fieldSizeWight);
		});
	for(int i=0;i<fieldSizeHeight;i++){
		for(int j=0;j<fieldSizeWight;j++){
			point current=std::make_pair(j, i);
			this->dijkstra.at(i).at(j).coord=current;
		}
	}
	for(int i=1;i<fieldSizeHeight-1;i++){
		for(int j=1;j<fieldSizeWight-1;j++){
			for(int k=0;k<this->dir8.size();k++){
				point current=std::make_pair(j, i);
				point next=current+this->dir8.at(k);
				if(!fields.isWall(next)){
					this->dijkstra.at(i).at(j).edgesTo.push_back(next);
					this->dijkstra.at(i).at(j).edgesCost.push_back(1+terrain.get(next));
				}
			}
		}	
	}
}

bool Search::dijkstraSearch(Field& fields, Terrain& terrain){
	bool flag=false;
	this->initsDijkstra4(fields, terrain);
	
	std::vector<Dijkstra*> q;
	point start=fields.getStart();
	this->dijkstra.at(start.second).at(start.first).cost=0;
	q.push_back(&this->dijkstra.at(start.second).at(start.first));
	
	while(!q.empty()){
		std::sort(q.begin(), q.end(), compare2);
		Dijkstra* current=q.at(0);
		q.erase(q.begin());
		current->done=true;
		if(current->coord==fields.getGoal())
			flag=true;
		for(int i=0;i<current->edgesTo.size();i++){
			point to=current->edgesTo.at(i);
			double cost=current->cost+current->edgesCost.at(i);
			if(this->dijkstra.at(to.second).at(to.first).cost<0||cost<dijkstra.at(to.second).at(to.first).cost){
				dijkstra.at(to.second).at(to.first).cost=cost;
				dijkstra.at(to.second).at(to.first).parent=current;
				if(std::find(q.begin(), q.end(), &dijkstra.at(to.second).at(to.first))==q.end())
					q.push_back(&this->dijkstra.at(to.second).at(to.first));
			}
		}
	}
	return flag;
}

void Search::makeRoute(const point& goal){
	if(!this->astar.empty()){
		this->route.push_back(goal);
		point from=this->astar.at(goal.second).at(goal.first).parent->coord;
		this->route.push_back(from);
		while(true){
			if(this->astar.at(from.second).at(from.first).parent==nullptr)
				break;
			from=this->astar.at(from.second).at(from.first).parent->coord;
			this->route.push_back(from);
		}
		std::reverse(this->route.begin(), this->route.end());
	}

	
	if(!this->dijkstra.empty()){
		this->route.push_back(goal);
		point from=this->dijkstra.at(goal.second).at(goal.first).parent->coord;
		this->route.push_back(from);
		while(true){
			if(this->dijkstra.at(from.second).at(from.first).parent==nullptr)
				break;
			from=this->dijkstra.at(from.second).at(from.first).parent->coord;
			this->route.push_back(from);
		}
		std::reverse(this->route.begin(), this->route.end());
	}
}

void Search::printRoute() const{
	static const double val=cellSize/2;
	static const unsigned pointSize=cellSize+2;	
	glPointSize(pointSize);
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	for(int i=0;i<this->route.size();i++){
		point coord=this->route.at(i);
		glVertex2d(val+coord.first*cellSize, val+coord.second*cellSize);
	}
	glEnd();
}

