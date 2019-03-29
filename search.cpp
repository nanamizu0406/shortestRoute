#include"search.hpp"

AStar::AStar():status(NONE),cost(-1),heuristic(-1),coord(0, 0),parent(nullptr){
}

AStar::~AStar(){
}

double AStar::getScore() const{
	return this->cost+this->heuristic;
}

bool compare(const AStar* obj1, const AStar* obj2){
	bool result=obj1->getScore()!=obj2->getScore();
	return (result?obj1->getScore()<obj2->getScore():obj1->heuristic<obj2->heuristic);
}

Search::Search(){
	this->inits();
}

Search::~Search(){
}

void Search::inits(){
	this->astar.clear();
	this->astar.resize(fieldSizeHeight);
	std::for_each(this->astar.begin(), this->astar.end(), [this](auto& vec){
			vec.resize(fieldSizeWight);
		});
	for(int i=0;i<fieldSizeHeight;i++){
		for(int j=0;j<fieldSizeWight;j++){
			this->astar.at(i).at(j).coord=std::make_pair(j, i);
		}
	}
	this->route.clear();
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
	this->inits();
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
		std::sort(openList.begin(), openList.end(), compare);
		current=openList.at(0);
		if(current->coord==fields.getGoal())
			return true;
		std::for_each(this->dir4.begin(), this->dir4.end(), [&,this](auto& dir){
				searchCoord=current->coord+dir;
				next=&this->astar.at(searchCoord.second).at(searchCoord.first);
				if(!fields.isWall(searchCoord)&&next->status==NONE){
					next->status=OPEN;
					
					//修正いるかも
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
	this->inits();
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
		std::sort(openList.begin(), openList.end(), compare);
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

void Search::makeRoute(const point& goal){
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

