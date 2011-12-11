#include "include/c_ballPivot.h"

GLfloat razm_x, razm_y, razm_z;
int COUNT_OF_POINTS_X, COUNT_OF_POINTS_Y, COUNT_OF_POINTS_Z;
int STEP;
c_point camera_pos(0,0,-10);
float angle;
_data_size data_size;
_scale scale;
c_point *all_points;
t_triangle *triangles_res;
int TRIANGLES_COUNT;

void DrawTriangle(int &p1, int &p2, int &p3)
{
	int HalfCountX=data_size.x/(2*STEP);
	int HalfCountY=data_size.y/(2*STEP);
	int HalfCountZ=data_size.z/(2*STEP);

	glBegin(GL_TRIANGLES);

	glVertex3f((all_points[p1].getX()-HalfCountX)*scale.x, (all_points[p1].getY()-HalfCountY)*scale.y, (all_points[p1].getZ()-HalfCountZ)*scale.z);
	glVertex3f((all_points[p2].getX()-HalfCountX)*scale.x, (all_points[p2].getY()-HalfCountY)*scale.y, (all_points[p2].getZ()-HalfCountZ)*scale.z);
	glVertex3f((all_points[p3].getX()-HalfCountX)*scale.x, (all_points[p3].getY()-HalfCountY)*scale.y, (all_points[p3].getZ()-HalfCountZ)*scale.z);
	
	glEnd();
}

void drawAxises()
{
	glColor4f(1,0,0,0.1f);
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(10000000,0,0);
	glEnd();

	glColor4f(0,1,0,0.1f);
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(0,10000000,0);
	glEnd();

	glColor4f(0,0,1,0.1f);
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(0,0,10000000);
	glEnd();

}

void RenderScene(void)
{
	int HalfCountX=COUNT_OF_POINTS_X/2;
	int HalfCountY=COUNT_OF_POINTS_Y/2;
	int HalfCountZ=COUNT_OF_POINTS_Z/2;

	int N=10;
	int RazmX_By_N=razm_x*N;
	int RazmY_By_N=razm_y*N;
	int RazmZ_By_N=razm_z*N;

	glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPushMatrix();

	glColor4f(1,1,1,0.1f);

	for(int i=0; i<TRIANGLES_COUNT; i++)
		DrawTriangle(triangles_res[i].point1, triangles_res[i].point2, triangles_res[i].point3);

	drawAxises();
    glPopMatrix();
    glutSwapBuffers();
}

void ManageKeys(unsigned char key, int x, int y)
{
	switch ( key )
	{
		case (unsigned char)233:	//й
		case 'q':
		{
		   // матрица поворота
		   camera_pos=c_point(camera_pos.getX(), camera_pos.getY()*cos(angle)+camera_pos.getZ()*sin(angle), -camera_pos.getY()*sin(angle)+camera_pos.getZ()*cos(angle));
		   glRotatef(5.0f,1.0,0.0,0.0);
		   glutPostRedisplay();
		   break;
		}

		case (unsigned char)244:	//ф
		case 'a':
		{
		   // матрица поворота
		   camera_pos=c_point(camera_pos.getX(), camera_pos.getY()*cos(-angle)+camera_pos.getZ()*sin(-angle), -camera_pos.getY()*sin(-angle)+camera_pos.getZ()*cos(-angle));
		   glRotatef(-5.0f,1.0,0.0,0.0);
		   glutPostRedisplay();
		   break;
		}

		case (unsigned char)246:	//ц
		case 'w':
		{
		   // матрица поворота
		   camera_pos=c_point(camera_pos.getX()*cos(angle)-camera_pos.getZ()*sin(angle), camera_pos.getY(), camera_pos.getX()*sin(angle)+camera_pos.getZ()*cos(angle));
		   glRotatef(5.0f,0.0,1.0,0.0);
		   glutPostRedisplay();
		   break;
		}

		case (unsigned char)251:	//ы
		case 's':
		{
		   // матрица поворота
		   camera_pos=c_point(camera_pos.getX()*cos(-angle)-camera_pos.getZ()*sin(-angle), camera_pos.getY(), camera_pos.getX()*sin(-angle)+camera_pos.getZ()*cos(-angle));
		   glRotatef(-5.0f,0.0,1.0,0.0);
		   glutPostRedisplay();
		   break;
		}

		case (unsigned char)243:	//у
		case 'e':
		{
		   // матрица поворота
		   camera_pos=c_point(camera_pos.getX()*cos(angle)+camera_pos.getY()*sin(angle), -camera_pos.getX()*sin(angle)+camera_pos.getY()*cos(angle), camera_pos.getZ());
		   glRotatef(5.0f,0.0,0.0,1.0);
		   glutPostRedisplay();
		   break;
		}

		case (unsigned char)226:	//в
		case 'd':
		{
		   // матрица поворота
		   camera_pos=c_point(camera_pos.getX()*cos(-angle)+camera_pos.getY()*sin(-angle), -camera_pos.getX()*sin(-angle)+camera_pos.getY()*cos(-angle), camera_pos.getZ());
		   glRotatef(-5.0f,0.0,0.0,1.0);
		   glutPostRedisplay();
		   break;
		}

		case (unsigned char)234:	//к
		case 'r':
		{
		   glTranslatef(camera_pos.getX(), camera_pos.getY(), camera_pos.getZ());
		   glutPostRedisplay();
		   break;
		}

		case (unsigned char)224:	//а
		case 'f':
		{
		   glTranslatef(-camera_pos.getX(), -camera_pos.getY(), -camera_pos.getZ());
		   glutPostRedisplay();
		   break;
		}

	}
}

void initGlobalVar(ballPivot &obj)
{
	razm_x=obj.razm_x, razm_y=obj.razm_y, razm_z=obj.razm_z;
	COUNT_OF_POINTS_X=obj.COUNT_OF_POINTS_X, COUNT_OF_POINTS_Y=obj.COUNT_OF_POINTS_Y, COUNT_OF_POINTS_Z=obj.COUNT_OF_POINTS_Z;
	STEP=obj.STEP;
	camera_pos=obj.camera_pos;
	angle=obj.angle;
	data_size=obj.data_size;
	scale=obj.scale;
	all_points=obj.all_points;
	triangles_res=obj.triangles_res;
	TRIANGLES_COUNT=obj.TRIANGLES_COUNT;
}

void SetupRC(void)
{
     glClearColor(0.0f,0.0f,0.0f,1.0f);
     glColor3f(0.0f, 1.0f, 0.0f); 
}

void ChangeSize(GLsizei w, GLsizei h)
{
     GLfloat aspectRatio;
     if(h==0) h=1;
     glViewport(0,0,w,h);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     aspectRatio=(GLfloat)w/(GLfloat)h;
     gluPerspective(40.0, (GLfloat) w/(GLfloat) h, .0, 20.0);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
	 gluLookAt(0,0,500,0,0,0,0,1,0);

	 camera_pos.setX(0);
	 camera_pos.setY(0);
	 camera_pos.setZ(-10);
}

void main(void)
{
	ballPivot qw("head.bin");
	qw.buildMesh();
	initGlobalVar(qw);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500,500);
	glutCreateWindow("Simple");
	   
	SetupRC();
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(ManageKeys);
	   
	glutMainLoop();
}
