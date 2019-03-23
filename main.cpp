#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<random>
#include<chrono>
#include<cmath>
#include<queue>
#include<GL/glut.h>

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
}timer;

class Field{
private:
	enum State{
		EMPTY,
		WALL,
	};
private:
	std::vector<std::vector<int>> field;
	std::vector<point> mouseLog;
	const unsigned wight=40;
	const unsigned height=40;
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
	int getWight() const;
	int getHeight() const;
	int getState(const point& coord) const;
	point getStart() const;
	point getGoal() const;
	bool isWall(const point& coord) const;
}field;

class AStar{
private:
public:
	AStar();
	~AStar();
	enum Status{
		 NONE,
		 OPEN,
		 CLOSED,
	};
	Status status;
	int cost;
	int heuristic;
	point coord;
	AStar* parent;
	int getScore() const;
};

class Search{
private:
	std::vector<std::vector<AStar>> astar;
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
	bool aStar();
	bool compare(const AStar& obj1, const AStar& obj2) const;
	void deleter(std::vector<AStar*>& vec) const;
}search;

int main(int argc, char* argv[]){
	field.printConsole();
	
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
	this->field.resize(height);
	std::for_each(this->field.begin(), this->field.end(), [this](auto& vec){
			vec.resize(this->wight);
			std::fill(vec.begin(), vec.end(), WALL);
		});
	std::for_each(this->field.begin()+1, this->field.end()-1, [this](auto& vec){
			std::for_each(vec.begin()+1, vec.end()-1, [this](auto& val){
					val=EMPTY;
				});
		});
	this->mouseLog.clear();
	this->start=std::make_pair(1, 1);
	this->goal=std::make_pair(this->wight-2, this->height-2);
}
void Field::recordCoord(int x, int y){
	bool result(x<=0||y<=0||x>=this->wight*cellSize||y>=this->height*cellSize);
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
	bool result(x<=0||y<=0||x>=this->wight*cellSize||y>=this->height*cellSize);
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
	std::for_each(this->mouseLog.begin(), this->mouseLog.end(), [this](auto& coord){
			if(this->field.at(coord.second).at(coord.first)==EMPTY)
				this->field.at(coord.second).at(coord.first)=WALL;
		});
	this->mouseLog.clear();
}
void Field::changeCoord(point& obj) const{
	obj.first=obj.first/cellSize;
	obj.second=obj.second/cellSize;
}
void Field::printConsole() const{
	point state;
	for(int i=0;i<this->height;i++){
		for(int j=0;j<this->wight;j++){
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
	for(int i=0;i<=this->wight;i++){
		glVertex2i(cellSize*i, 0);
		glVertex2i(cellSize*i, cellSize*this->wight);
	}
	for(int j=0;j<=this->height;j++){
		glVertex2i(0, cellSize*j);
		glVertex2i(cellSize*this->height, cellSize*j);
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
	for(int i=0;i<this->height;i++){
		for(int j=0;j<this->wight;j++){
			if(this->field.at(i).at(j)==WALL)
				glVertex2d(val+cellSize*j, val+cellSize*i);
		}
	}
	glEnd();
}
int Field::getWight() const{
	return this->wight;
}
int Field::getHeight() const{
	return this->height;
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
	delete this->parent;
}
int AStar::getScore() const{
	return this->cost+this->heuristic;
}

Search::Search(){
	this->inits();
}
Search::~Search(){
}
void Search::inits(){
	this->astar.clear();
	this->astar.resize(field.getHeight());
	std::for_each(this->astar.begin(), this->astar.end(), [this](auto& vec){
			vec.resize(field.getWight());
		});
	for(int i=0;i<field.getHeight();i++){
		for(int j=0;j<field.getWight();j++){
			this->astar.at(i).at(j).coord=std::make_pair(j, i);
		}
	}
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
bool Search::aStar(){
	std::vector<AStar*> openList;
	
	
	this->deleter(openList);
	return false;
}
bool Search::compare(const AStar& obj1, const AStar& obj2) const{
	bool result=obj1.getScore()!=obj2.getScore();
	return (result?obj1.getScore()<obj2.getScore():obj1.heuristic<obj2.heuristic);
}
void Search::deleter(std::vector<AStar*>& vec) const{
	std::for_each(vec.begin(), vec.end(), [](auto astar){
			delete astar;
		});
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
	switch(key){
	case 'q':
	case 'Q':
	case '\033':
		std::exit(0);
		break;
	case 'r':
	case 'R':
		field.inits();
		field.printConsole();
		glutPostRedisplay();
		break;
	case 'a':
	case 'A':
		field.plotWall();
		field.printConsole();
		glutPostRedisplay();
		break;
	default:
		break;
	}
}
void special(int key, int x, int y){
	switch(key){
	case GLUT_KEY_F5:
		field.plotWall();
		field.printConsole();
		glutPostRedisplay();
		break;
	default:
		break;
	}
}
void display(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	field.printField();
	field.printWall();
	glutSwapBuffers();
	glFlush();
}
void mouse(int button, int state, int x, int y){
	switch(button){
	case GLUT_LEFT_BUTTON:
		if(state==GLUT_DOWN)
			field.recordCoord(x, y);
		break;
	case GLUT_RIGHT_BUTTON:
		if(state==GLUT_DOWN){
			field.setStartGoal(x, y);
			field.printConsole();
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
}
void motion(int x, int y){
	field.recordCoord(x, y);
}

