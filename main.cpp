#include"tools.hpp"
#include"field.hpp"
#include"search.hpp"

void inits();
void resize(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void display();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);

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
		result=search.aStar4(field);
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
		break;
	case 'b':
	case 'B':
		timer.begin();
		result=search.aStar8(field);
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

