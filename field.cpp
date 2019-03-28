#include"field.hpp"

Field::Field(){
	this->inits();
}

Field::~Field(){
}

void Field::inits(){
	this->field.clear();
	this->field.resize(fieldSizeHeight);
	std::for_each(this->field.begin(), this->field.end(), [this](auto& vec){
			vec.resize(fieldSizeWight);
			std::fill(vec.begin(), vec.end(), WALL);
		});
	std::for_each(this->field.begin()+1, this->field.end()-1, [this](auto& vec){
			std::for_each(vec.begin()+1, vec.end()-1, [this](auto& val){
					val=EMPTY;
				});
		});
	this->mouseLog.clear();
	this->start=std::make_pair(1, 1);
	this->goal=std::make_pair(fieldSizeWight-2, fieldSizeHeight-2);
}

void Field::recordCoord(int x, int y){
	bool result(x<=0||y<=0||x>=fieldSizeWight*cellSize||y>=fieldSizeHeight*cellSize);
	if(result)
		return;
	point coord=std::make_pair(x, y);
	this->changeCoord(coord);
	result=(this->field.at(coord.second).at(coord.first)==WALL||coord==this->start||coord==this->goal);
	if(result)
		return;
	auto itr=std::find(this->mouseLog.begin(), this->mouseLog.end(), coord);
	if(itr==this->mouseLog.end())
		this->mouseLog.push_back(coord);	
}

void Field::setStartGoal(int x, int y){
	bool result(x<=0||y<=0||x>=fieldSizeWight*cellSize||y>=fieldSizeHeight*cellSize);
	if(result)
		return;
	point coord=std::make_pair(x, y);
	this->changeCoord(coord);
	result=(this->field.at(coord.second).at(coord.first)==WALL);
	if(result)
		return;
	static int flag=1;
	(flag?this->start:this->goal)=coord;
	flag=(flag+1)%2;
}

void Field::plotWall(){
	if(this->mouseLog.empty())
		return;
	point coord=this->mouseLog.at(this->mouseLog.size()-1);
	this->field.at(coord.second).at(coord.first)=WALL;
}

void Field::changeCoord(point& obj) const{
	obj.first=obj.first/cellSize;
	obj.second=obj.second/cellSize;
}

void Field::printConsole() const{
	point state;
	for(int i=0;i<fieldSizeHeight;i++){
		for(int j=0;j<fieldSizeWight;j++){
			std::cout<<"\x1b[34m";
			state.first=j;
			state.second=i;
			if(state==this->start){
				std::cout<<"Ｓ"<<"\x1b[39m";
				continue;
			}
			if(state==this->goal){
				std::cout<<"Ｇ"<<"\x1b[39m";
				continue;
			}
			switch(this->field.at(i).at(j)){
			case EMPTY:
				std::cout<<"\x1b[39m";
				std::cout<<"　";
				break;
			case WALL:
				std::cout<<"\x1b[31m";
				std::cout<<"＠";
				std::cout<<"\x1b[39m";
				break;
			default:
				break;
			}
		}
		std::cout<<std::endl;
	}
}

void Field::printField() const{
	static const unsigned lineSize=1.0;
	glColor3f(0.0f, 1.0f, 0.0);
	glLineWidth(lineSize);
	glBegin(GL_LINES);
	for(int i=0;i<=fieldSizeWight;i++){
		glVertex2i(cellSize*i, 0);
		glVertex2i(cellSize*i, cellSize*fieldSizeHeight);
	}
	for(int j=0;j<=fieldSizeHeight;j++){
		glVertex2i(0, cellSize*j);
		glVertex2i(cellSize*fieldSizeWight, cellSize*j);
	}
	glEnd();
}

void Field::printWall() const{
	static const double val=cellSize/2;
	static const unsigned pointSize=cellSize-1;
	static const unsigned specialPointSize=10;
	glPointSize(specialPointSize);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex2d(val+cellSize*this->start.first, val+cellSize*this->start.second);
	glColor3f(1.0f, 0.0f, 0.5f);
	glVertex2d(val+cellSize*this->goal.first, val+cellSize*this->goal.second);
	glEnd();
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.0f, 0.0f);
	for(int i=0;i<fieldSizeHeight;i++){
		for(int j=0;j<fieldSizeWight;j++){
			if(this->field.at(i).at(j)==WALL)
				glVertex2d(val+cellSize*j, val+cellSize*i);
		}
	}
	glEnd();
}

int Field::getState(const point& coord) const{
	return this->field.at(coord.second).at(coord.first);
}

point Field::getStart() const{
	return this->start;
}

point Field::getGoal() const{
	return this->goal;
}

bool Field::isWall(const point& coord) const{
	return (this->field.at(coord.second).at(coord.first)==WALL);
}
