#include"tools.hpp"
#include"field.hpp"
#include"search.hpp"
#include"diamond.hpp"

void inits();
void resize(int w, int h);
void keyboard(unsigned char key, int x, int y);
void display();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);

void finish();
void reset();
void astarAlgorithm4();
void astarAlgorithm8();
void dijkstraAlgorithm4();
void dijkstraAlgorithm8();
void diamondSquareAlgorithm();

Timer timer;
Field field;
Search search;
Terrain terrain;

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutCreateWindow("A-star");
	inits();
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMainLoop();
	return 0;
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
		finish();
		break;
	case 'r':
	case 'R':
		reset();
		break;
	case 'a':
	case 'A':
		astarAlgorithm4();
		break;
	case 'b':
	case 'B':
		astarAlgorithm8();
		break;
	case 'c':
	case 'C':
		dijkstraAlgorithm4();
		break;
	case 'd':
	case 'D':
		dijkstraAlgorithm8();
		break;
	case 't':
	case 'T':
		diamondSquareAlgorithm();
		break;
	default:
		break;
	}
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	field.printField();
	terrain.print();
	search.printRoute();
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

void finish(){
	std::exit(0);
}

void reset(){
	field.inits();
	search.inits();
	terrain.clear();
	glutPostRedisplay();
}

void astarAlgorithm4(){
	std::cout<<"A* algorithm4"<<std::endl;
	timer.begin();
	bool result=search.aStar4(field, terrain);
	timer.stop();
	if(!result){
		std::cout<<"route is closed"<<std::endl;
		glutPostRedisplay();
	}
	else{
		timer.disp();
		search.makeRoute(field.getGoal());
		glutPostRedisplay();
	}
}

void astarAlgorithm8(){
	std::cout<<"A* algorithm8"<<std::endl;
	timer.begin();
	bool result=search.aStar8(field, terrain);
	timer.stop();
	if(!result){
		std::cout<<"route is closed"<<std::endl;
		glutPostRedisplay();
	}
	else{
		timer.disp();
		search.makeRoute(field.getGoal());
		glutPostRedisplay();
	}
}

void dijkstraAlgorithm4(){
	std::cout<<"dijkstra algorithm4"<<std::endl;
	timer.begin();
	bool result=search.dijkstra4(field, terrain);
	timer.stop();
	if(!result){
		std::cout<<"route is closed"<<std::endl;
		glutPostRedisplay();
	}
	else{
		timer.disp();
		search.makeRoute(field.getGoal());
		glutPostRedisplay();
	}
}

void dijkstraAlgorithm8(){
	std::cout<<"dijkstra algorithm8"<<std::endl;
	timer.begin();
	bool result=search.dijkstra8(field, terrain);
	timer.stop();
	if(!result){
		std::cout<<"route is closed"<<std::endl;
		glutPostRedisplay();
	}
	else{
		timer.disp();
		search.makeRoute(field.getGoal());
		glutPostRedisplay();
	}
}

void diamondSquareAlgorithm(){
	std::cout<<"diamond square algorithm"<<std::endl;
	timer.begin();
	terrain.diamondSquare();
	timer.stop();
	timer.disp();
	glutPostRedisplay();
}

