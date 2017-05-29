#include <GL/glut.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#define PI 3.14159265

using namespace std;


typedef vector< vector<int> > coords_;
int tam_square = 5;
coords_ my_vect;

void save_axis()
{
	ofstream archivo;
	archivo.open("coordenadas.txt");
    for (int i = 0; i < my_vect.size(); ++i)
	{   
		int pos_i=my_vect[i][0], pos_j=my_vect[i][1], pos_k=my_vect[i][2];
		archivo <<pos_i<<" "<<pos_j<<" "<<pos_k<< endl;
	}
	archivo.close();
}

static void resize(int width, int height){
const float ar = (float) width / (float) height;

glViewport(0, 0, width, height);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho (-6.4,6.4,-4.8,4.8, 10.0, -10.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
}

void crear_esferas(int tam_square ,coords_ &vect)
{
	
	for (int i = 0; i < tam_square; ++i)
	{
		for (int j = 0; j < tam_square; ++j)
		{
			for (int k = 0; k < tam_square; ++k)
			{
				vector<int> tmp;
				tmp.push_back(i);
				tmp.push_back(j);
				tmp.push_back(k);
				vect.push_back(tmp);
			    tmp.empty();
			}
		}

	}
}


static void display(void){

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glColor3d(1,0,0);

	for (int i = 0; i < my_vect.size(); ++i)
	{   
		int pos_i=my_vect[i][0], pos_j=my_vect[i][1], pos_k=my_vect[i][2];
		if (pos_i==0 || pos_j==0 ||pos_k==0)
		{	
			glPushMatrix();
			glTranslatef (pos_j,pos_i,pos_k);
			glColor3f(1,1,1);	
			glutWireSphere(0.4,20,20);
			glPopMatrix();
		
		}
		else
		{
			glPushMatrix();
			glTranslatef (pos_j,pos_i,pos_k);
			glColor3f(1.0,1.0,0.0);
			glutWireSphere(0.4,20,20);
			glPopMatrix();
		}
	}


glutSwapBuffers();
}

/***********************************************************************************************************/
static void idle(void)
{
glutPostRedisplay();
}

void ControlFlechas(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_UP :                  // GLUT_KEY_UP y GLUT_KEY_DOWN para control de rotacion general vertical
      glRotatef(-1.0, 1.0, 0.0, 0.0); 
    break;

    case GLUT_KEY_DOWN :
      glRotatef(1.0, 1.0, 0.0, 0.0);
    break;

    case GLUT_KEY_LEFT :                // GLUT_KEY_LEFT y GLUT_KEY_RIGHT para control de rotacion general horizontal
      glRotatef(-1.0, 0.0, 1.0, 0.0);
    break;

    case GLUT_KEY_RIGHT :
      glRotatef(1.0, 0.0, 1.0, 0.0);
    break;
  }
  glutPostRedisplay();
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(10,10);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("GG las Pelotas");

	glutReshapeFunc(resize);
	crear_esferas(tam_square,my_vect);
	save_axis();
	glutDisplayFunc(display);
	glutSpecialFunc(ControlFlechas);
	glutIdleFunc(idle);

	glClearColor(0,0,0,0);

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();

	return
	EXIT_SUCCESS;
} 