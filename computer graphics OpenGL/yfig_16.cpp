#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#if !defined(GLUT_WHEEL_UP)
# define GLUT_WHEEL_UP 3
# define GLUT_WHEEL_DOWN 4
#endif

#define INCREASE 0
#define DECREASE 1
#define RED 2
#define BLUE 5
#define GREEN 6
#define BLACK 7
#define LINE 8
#define RECTANGLE 9
#define CIRCLE 10
int shapeFlag = -1;
bool wheelUpButton = false, wheelDownButton = false,middleButtonDown = false,
	leftButtonDown = false,leftButtonUp = false,menuFlag = false;
GLfloat winWidth = 600, winHeight = 500;
GLfloat red=0,green=0,blue=0;
int shapeColor = BLACK;//current pen color
GLint penSize=3;//current pen thickness
int downX,downY,moveX,moveY;
/*  Set size of the canvas size.  */
GLfloat xwcMin = -100.0, xwcMax = 100.0;
GLfloat ywcMin = -100.0, ywcMax = 100.0;
/*  Set size of the initial clipping window size.  */
GLfloat xvcMin = -50.0, xvcMax = 50.0;
GLfloat yvcMin = -50.0, yvcMax = 50.0;
GLfloat offsetX=0.0,offsetY=0.0;// the offset to the origin when dragging
int num,current;//count the drawed graphics
struct ptShape
{
	float sx;
	float sy;
	float ex;
	float ey;
	int color;
	int size;
	int shape;
}shapes[100];

void init (void)
{
  glClearColor (1.0, 1.0, 1.0, 1.0);

    //glMatrixMode (GL_PROJECTION);
    //gluOrtho2D (0, 0, winWidth, winHeight);
}

class scrPt {
public:
  GLint x, y;
};

void setPixel (GLint x, GLint y)
{
	glPointSize(penSize);
	glBegin (GL_POINTS);
	glVertex2i (x, y);
	glEnd ( );
}

void circleMidpoint (scrPt circCtr, GLint radius)
{
  scrPt circPt;

  GLint p = 1 - radius;         // Initial value of midpoint parameter.

  circPt.x = 0;             // Set coordinates for top point of circle.
  circPt.y = radius;

  void circlePlotPoints (scrPt, scrPt);

  /* Plot the initial point in each circle quadrant. */
  circlePlotPoints (circCtr, circPt);

  /* Calculate next points and plot in each octant. */
  while (circPt.x < circPt.y) {
    circPt.x++;
    if (p < 0)
      p += 2 * circPt.x + 1;
    else {
      circPt.y--;
      p += 2 * (circPt.x - circPt.y) + 1;
    }
    circlePlotPoints (circCtr, circPt);
  }
}

void circlePlotPoints (scrPt circCtr, scrPt circPt)
{
  setPixel (circCtr.x + circPt.x, circCtr.y + circPt.y);
  setPixel (circCtr.x - circPt.x, circCtr.y + circPt.y);
  setPixel (circCtr.x + circPt.x, circCtr.y - circPt.y);
  setPixel (circCtr.x - circPt.x, circCtr.y - circPt.y);
  setPixel (circCtr.x + circPt.y, circCtr.y + circPt.x);
  setPixel (circCtr.x - circPt.y, circCtr.y + circPt.x);
  setPixel (circCtr.x + circPt.y, circCtr.y - circPt.x);
  setPixel (circCtr.x - circPt.y, circCtr.y - circPt.x);
}

void color(int i)
{
	if(shapes[i].color == BLACK)
	{
		red = 0;green = 0;blue = 0;
	}
	else if(shapes[i].color == RED)
	{
		red = 1.0; green = 0;blue = 0;
	}
	else if(shapes[i].color == BLUE)
	{
		red = 0; green = 0; blue = 1.0;
	}
	else if(shapes[i].color == GREEN)
	{
		red = 0; green = 1.0;blue = 0;
	}
}

void draw( )
{
	int i;
	for(i=0;i<num;i++)
	{
		color(i);
		glColor3f (red, green, blue);       //  Set color
		glLineWidth(shapes[i].size);
		if(shapes[i].shape == LINE)
		{
			glBegin(GL_LINES);
				glVertex2i(shapes[i].sx,shapes[i].sy);
				glVertex2i(shapes[i].ex,shapes[i].ey);
			glEnd();
		}
		else if(shapes[i].shape == RECTANGLE)
		{
			glRecti(shapes[i].sx,shapes[i].sy,shapes[i].ex,shapes[i].ey);
		}
		else if(shapes[i].shape == CIRCLE)
		{
			scrPt circle;
			/*
			circle.x = (float)((shapes[i].sx-xvcMin)/(xvcMax-xvcMin))*winWidth;
			circle.y = (float)((shapes[i].sy-yvcMin)/(yvcMax-yvcMin))*winHeight;
			float x = (float)((shapes[i].ex-xvcMin)/(xvcMax-xvcMin))*winWidth;
			float y = (float)((shapes[i].ey-yvcMin)/(yvcMax-yvcMin))*winHeight;
			printf("shapes.sx: %f  sy:%f  ex:%f ey:%f\n",shapes[i].sx,shapes[i].sy,shapes[i].ex,shapes[i].ey);
			printf("circle.x: %f %f   x:%f y:%f\n",circle.x,circle.y,x,y);/////////
			*/
			circle.x=shapes[i].sx;
			circle.y=shapes[i].sy;
			float x = shapes[i].ex;
			float y = shapes[i].ey;
			GLint r = sqrt((double)((circle.x-x)*(circle.x-x)+ (circle.y-y)*(circle.y-y)));
			circleMidpoint(circle,r);
		}
	}
}
int count = 0,middleFlag = 0;
void MouseCallback(int button, int state, int x, int y)
{
	moveX = (xvcMax-xvcMin)*(float)x/winWidth - xvcMax;
	moveY = yvcMax - (yvcMax-yvcMin)*(float)y/winHeight;//////////////////dai gai
	downX = moveX;
	downY = moveY;
   if(leftButtonDown = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))&& (menuFlag==false))
   {
	   if(shapeFlag!=-1)
	   {
		   shapes[current].ex = moveX+offsetX;
		   shapes[current].ey = moveY+offsetY;
		   
		   printf("cur %d ex %d, ey %d\n",current,shapes[current].ex,shapes[current].ey);////////////////////////
		   printf("cur %d sx %d, sy %d\n",current,downX,downY);////////////////////////
		   
		   if(count == 1)
		   {	
			   current++;
			   draw( );
			   count = 0;
		   }
		   else
		   {		   
			   shapes[current].sx = downX+offsetX;
			   shapes[current].sy = downY+offsetY;
			   shapes[current].color = shapeColor;
			   shapes[current].size = penSize;
			   shapes[current].shape = shapeFlag;
			  // printf("offsetX: %f offsetY: %f \n",offsetX,offsetY);
			   printf("current %d .sx: %f sy: %f \n",current,shapes[current].sx,shapes[current].sy);/////
			   num++;
			   draw();
			   count++;
		   }
	   }
   }
   if(leftButtonUp = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))&&(menuFlag==false))
   {

   }
   middleButtonDown = ((button == GLUT_MIDDLE_BUTTON)&&(state == GLUT_DOWN));
   if((button == GLUT_MIDDLE_BUTTON)&&(state == GLUT_UP))
   {
		if(middleFlag == 1)
		{
			xvcMin -= offsetX;
			xvcMax -= offsetX;
			yvcMin -= offsetY;
			yvcMax -= offsetY;
			glMatrixMode (GL_PROJECTION);
			glLoadIdentity ( );
			gluOrtho2D (xvcMin, xvcMax, yvcMin, yvcMax);
			middleFlag = 0;
		}
   }
   float times=1.0;
   if(((glutGetModifiers() == GLUT_ACTIVE_CTRL) &&  (button == GLUT_WHEEL_UP)))
   {
	   times+=0.2; glViewport (0, 0, winHeight*times, winHeight*times);
   }
   if(((glutGetModifiers() == GLUT_ACTIVE_CTRL) && (button == GLUT_WHEEL_DOWN)))
   {
	   times-=0.2;glViewport (0, 0, winHeight*times, winHeight*times);
   }
   glutPostRedisplay();
}

void MotionCallback(int x, int y)
{
	if(count)
	{
		shapes[current].color = shapeColor;
		shapes[current].size = penSize;
		//shapes[current].shape = shapeFlag;
		moveX = (xvcMax-xvcMin)*(float)x/winWidth - xvcMax + offsetX;
		moveY = yvcMax - (yvcMax-yvcMin)*(float)y/winHeight + offsetY;
	    shapes[current].ex = moveX;
        shapes[current].ey = moveY;
	}
  if (leftButtonDown&&(count==1)){ draw( ); } 
  if(middleButtonDown)
  {
	    middleFlag = 1;
		moveX = (xvcMax-xvcMin)*(float)x/winWidth - xvcMax;
		moveY = yvcMax - (yvcMax-yvcMin)*(float)y/winHeight;
		offsetX = moveX - downX;
		offsetY = moveY - downY;
		printf("moveX %f moveY %f  downX %f downY %f\n",moveX,moveY,downX,downY);///////
		printf("%f %f \n",offsetX,offsetY);///
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ( );
		gluOrtho2D (xvcMin-offsetX, xvcMax-offsetX, yvcMin-offsetY, yvcMax-offsetY);
  }
  glutPostRedisplay();
}

void displayFcn (void)
{
	//glViewport()
    glClear (GL_COLOR_BUFFER_BIT);   //  Clear display window.
    glColor3f(0,0,1.0);
	glLineWidth(1);
    glBegin(GL_LINES);
        glVertex2i(-100,0);
        glVertex2i(100,0);
        glVertex2i(0,-100);
        glVertex2i(0,100);
    glEnd();
	////////////////////////////////////////
	glViewport(0,0,winWidth,winHeight);
	glBegin(GL_LINES);
	glVertex2i(300,300);
	glVertex2i(500,500);
	glEnd();
    draw();
	
    glutSwapBuffers ( );
}

void winReshapeFcn (int newWidth, int newHeight)
{
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ( );
    gluOrtho2D (xvcMin, xvcMax, yvcMin, yvcMax);
	/*  Reset display-window size parameters.  */
    winWidth = newWidth;
    winHeight = newHeight; 
    /*  Set the viewport to be the entire window  */
    glViewport(0,0,newWidth,newHeight);


    glutPostRedisplay();
}

void processMenuEvents(int option) {
	menuFlag = true;
	switch (option) {                    
		case RED: shapeColor = RED;shapes[current].color = shapeColor;break;
		case GREEN: shapeColor = GREEN;shapes[current].color = shapeColor;break;
		case BLUE: shapeColor = BLUE;shapes[current].color = shapeColor;break;
		case BLACK: shapeColor = BLACK;shapes[current].color = shapeColor;break;
		case INCREASE: penSize++;shapes[current].size++; break;
		case DECREASE: if(penSize>1)penSize--;shapes[current].size--;break;
    }
    glutPostRedisplay();
	menuFlag = false;
}

void keyboard(unsigned char key, int x, int y)
{
 if(key == 'l') shapeFlag = LINE;
 else if(key == 'r') shapeFlag = RECTANGLE;
 else if(key == 'c') shapeFlag = CIRCLE;
 glutPostRedisplay();
}

void createGLUTMenus() {
	int menu,submenu1,submenu2;

	submenu1 = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Increase 1 pixel",INCREASE);
	glutAddMenuEntry("Dcrease 1 pixel",DECREASE);

	submenu2 = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Red",RED);
	glutAddMenuEntry("Blue",BLUE);
	glutAddMenuEntry("Green",GREEN);
	glutAddMenuEntry("Black",BLACK);

	menu = glutCreateMenu(processMenuEvents);
	glutAddSubMenu("Pen Thickness",submenu1);
	glutAddSubMenu("Pen Color",submenu2);
        glutAttachMenu(GLUT_RIGHT_BUTTON);
}



int main (int argc, char** argv)
{
    glutInit (&argc, argv);
    //    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition (100, 100);
    glutInitWindowSize (winWidth, winHeight);
    glutCreateWindow ("yfig");

    init ( );
    createGLUTMenus(); 
    glutDisplayFunc (displayFcn);
    glutReshapeFunc (winReshapeFcn);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(MouseCallback);
    //glutPassiveMotionFunc(MotionCallback);
	glutMotionFunc(MotionCallback);
    glutMainLoop ( );
}
