#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<random>
#include<chrono>
#include<cmath>
#include<queue>
#include<GL/glut.h>

const unsigned fieldSizeWight=60;
const unsigned fieldSizeHeight=70;
const unsigned cellSize=10;
using point=std::pair<int, int>;

point operator+(const point &lhs, const point &rhs){
	point p = { lhs.first + rhs.first, lhs.second + rhs.second };
	return p;
}

void inits();
void resize(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void display();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);

class Timer{
private:
	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;
public:
	void begin();
	void stop();
	void disp() const;
};

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

bool compare(const AStar* obj1, const AStar* obj2);

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
	int heuristicCost4(const point& coord) const;
	int heuristicCost8(const point& coord) const;
	bool aStar4();
	bool aStar8();
	void makeRoute();
	void printRoute() const;
};

Timer timer;
Field field;
Search search;

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutCreateWindow("A-star");
	inits();
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMainLoop();
	return 0;
}

void Timer::begin(){
	this->start=std::chrono::system_clock::now();
}

void Timer::stop(){
	this->end=std::chrono::system_clock::now();
}

void Timer::disp() const{
	unsigned val=std::chrono::duration_cast<std::chrono::microseconds>(this->end-this->start).count();
	std::cout<<val<<"micro"<<std::endl;
}

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
	static const unsigned lineSize=1;
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
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2d(val+cellSize*this->start.first, val+cellSize*this->start.second);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex2d(val+cellSize*this->goal.first, val+cellSize*this->goal.second);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
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

AStar::AStar():status(NONE),cost(-1),heuristic(-1),coord(0, 0),parent(nullptr){
}

AStar::~AStar(){
}

int AStar::getScore() const{
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

int Search::heuristicCost4(const point& coord) const{
	int dx=std::abs(field.getGoal().first-coord.first);
	int dy=std::abs(field.getGoal().second-coord.second);
	return std::max(dx, dy);
}

int Search::heuristicCost8(const point& coord) const{
	int dx=std::abs(field.getGoal().first-coord.first);
	int dy=std::abs(field.getGoal().second-coord.second);
	return dx+dy;
}

bool Search::aStar4(){
	this->inits();
	std::vector<AStar*> openList;
	AStar* start=&this->astar.at(field.getStart().second).at(field.getStart().first);
	start->status=OPEN;
	start->cost=0;
	start->heuristic=this->heuristicCost4(start->coord);
	openList.push_back(start);
	AStar* current;
	AStar* next;
	point searchCoord;
	while(!openList.empty()){
		std::sort(openList.begin(), openList.end(), compare);
		current=openList.at(0);
		if(current->coord==field.getGoal())
			return true;
		std::for_each(this->dir4.begin(), this->dir4.end(), [&,this](auto& dir){
				searchCoord=current->coord+dir;
				next=&this->astar.at(searchCoord.second).at(searchCoord.first);
				if(!field.isWall(searchCoord)&&next->status==NONE){
					next->status=OPEN;
					next->cost=current->cost+1;
					next->heuristic=this->heuristicCost4(next->coord);
					next->parent=current;
					openList.push_back(next);
				}
			});
		current->status=CLOSED;
		openList.erase(openList.begin());
	}
	return false;
}

bool Search::aStar8(){
	this->inits();
	std::vector<AStar*> openList;
	AStar* start=&this->astar.at(field.getStart().second).at(field.getStart().first);
	start->status=OPEN;
	start->cost=0;
	start->heuristic=this->heuristicCost8(start->coord);
	openList.push_back(start);
	AStar* current;
	AStar* next;
	point searchCoord;
	while(!openList.empty()){
		std::sort(openList.begin(), openList.end(), compare);
		current=openList.at(0);
		if(current->coord==field.getGoal())
			return true;
		std::for_each(this->dir8.begin(), this->dir8.end(), [&,this](auto& dir){
				searchCoord=current->coord+dir;
				next=&this->astar.at(searchCoord.second).at(searchCoord.first);
				if(!field.isWall(searchCoord)&&next->status==NONE){
					next->status=OPEN;
					next->cost=current->cost+1;
					next->heuristic=this->heuristicCost8(next->coord);
					next->parent=current;
					openList.push_back(next);
				}
			});
		current->status=CLOSED;
		openList.erase(openList.begin());
	}
	return false;
}

void Search::makeRoute(){
	point goal=field.getGoal();
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
	static const unsigned pointSize=cellSize-1;	
	glPointSize(pointSize);
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	for(int i=0;i<this->route.size();i++){
		point coord=this->route.at(i);
		glVertex2d(val+coord.first*cellSize, val+coord.second*cellSize);
	}
	glEnd();
}

void inits(){
	static const unsigned WindowSizeWight=800;
	static const unsigned WindowSizeHeight=800;
	static const unsigned WindowPossitionWight=100;
	static const unsigned WindowPossitionHeight=100;
	gluOrtho2D(0, 100, 100, 0);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowPosition(WindowPossitionWight, WindowPossitionHeight);
	glutInitWindowSize(WindowSizeWight, WindowSizeHeight);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void resize(int w, int h){
	glViewport(0, 0, w, h);
	glLoadIdentity();
	glOrtho(-0.5, (GLdouble)w-0.5, (GLdouble)h-0.1, -0.5, -1.0, 1.0);
}

void keyboard(unsigned char key, int x, int y){
	bool result;
	switch(key){
	case 'q':
	case 'Q':
	case '\033':
		std::exit(0);
		break;
	case 'r':
	case 'R':
		field.inits();
		search.inits();
		glutPostRedisplay();
		break;
	case 'a':
	case 'A':
		timer.begin();
		result=search.aStar4();
		timer.stop();
		if(!result){
			std::cout<<"route is closed"<<std::endl;
			glutPostRedisplay();
		}
		else{
			timer.disp();
			search.makeRoute();
			glutPostRedisplay();
		}
		break;
	case 'b':
	case 'B':
		timer.begin();
		result=search.aStar8();
		timer.stop();
		if(!result){
			std::cout<<"route is closed"<<std::endl;
			glutPostRedisplay();
		}
		else{
			timer.disp();
			search.makeRoute();
			glutPostRedisplay();
		}
	default:
		break;
	}
}

void special(int key, int x, int y){
	switch(key){
	case GLUT_KEY_F5:
		field.plotWall();
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	search.printRoute();
	field.printField();
	field.printWall();
	glutSwapBuffers();
	glFlush();
}

void mouse(int button, int state, int x, int y){
	switch(button){
	case GLUT_LEFT_BUTTON:
		if(state==GLUT_DOWN){
			field.recordCoord(x, y);
			field.plotWall();
			glutPostRedisplay();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if(state==GLUT_DOWN){
			field.setStartGoal(x, y);
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
}

void motion(int x, int y){
	field.recordCoord(x, y);
	field.plotWall();
	glutPostRedisplay();
}

