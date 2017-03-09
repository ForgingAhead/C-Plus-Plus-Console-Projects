#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

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
GLfloat winWidth = 600, winHeight = 600;
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
GLfloat offsetX=0.0,offsetY=0.0,newOffsetX=0.0,newOffsetY=0.0;// the offset to the origin when dragging
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
  glClearColor (0, 0, 0, 1.0);
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
			float sx = shapes[i].sx;
			float sy = shapes[i].sy;
			float x = shapes[i].ex;
			float y = shapes[i].ey;
			GLint radius = sqrt((double)((sx-x)*(sx-x)+ (sy-y)*(sy-y)));
			float r = 3.14159/180;
			   glBegin(GL_LINE_LOOP); 
			   for (int i=0; i<=360; i++)
			   {
			      float angle = i*r;
			      glVertex2f(sx+cos(angle)*radius,sy+sin(angle)*radius);
			   }
			   glEnd();
		}
	}
}
int count = 0,middleFlag = 0;
float times=1.0;
void MouseCallback(int button, int state, int x, int y)
{
	moveX = (xvcMax-xvcMin)*(float)x/winWidth - (xvcMax-xvcMin)/2;
	moveY = (yvcMax-yvcMin)/2 - (yvcMax-yvcMin)*(float)y/winHeight;
	downX = moveX;
	downY = moveY;
	middleButtonDown = false;
   if(leftButtonDown = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))&& (menuFlag==false))
   {
	   if(shapeFlag!=-1&&((offsetX<=50)&&(offsetX>=-50))&&(offsetY>=-50&&(offsetY<=50)))
	   {
		   shapes[current].ex = moveX-offsetX;
		   shapes[current].ey = moveY-offsetY;
		   
		   if(count == 1)
		   {	
			   current++;
			   draw( );
			   count = 0;
		   }
		   else
		   {		   
			   shapes[current].sx = downX-offsetX;
			   shapes[current].sy = downY-offsetY;
			   shapes[current].color = shapeColor;
			   shapes[current].size = penSize;
			   shapes[current].shape = shapeFlag;
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
			offsetX+=newOffsetX;
			offsetY+=newOffsetY;
			xvcMin -= newOffsetX;
			xvcMax -= newOffsetX;
			yvcMin -= newOffsetY;
			yvcMax -= newOffsetY;
			glMatrixMode (GL_PROJECTION);
			glLoadIdentity ( );
			gluOrtho2D (xvcMin, xvcMax, yvcMin, yvcMax);
			middleFlag = 0;
		}
   }
   
   if(((glutGetModifiers() == GLUT_ACTIVE_CTRL) &&  (button == GLUT_WHEEL_UP)))
   { // zoom in
	float dx = (float)winWidth / 500;
	float dy = (float)winHeight / 500;
	xvcMin += dx;
	xvcMax -= dx;
	yvcMin += dy;
	yvcMax -= dy;
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ( );
	gluOrtho2D (xvcMin, xvcMax, yvcMin, yvcMax);
   }
   if(((glutGetModifiers() == GLUT_ACTIVE_CTRL) && (button == GLUT_WHEEL_DOWN)))
   {// zoom out
	float dx = (float)winWidth / 500;
	float dy = (float)winHeight / 500;
	xvcMin -= dx;
	xvcMax += dx;
	yvcMin -= dy;
	yvcMax += dy;
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ( );
	gluOrtho2D (xvcMin, xvcMax, yvcMin, yvcMax);
   }
   glutPostRedisplay();
}

void MotionCallback(int x, int y)
{
	if(count)
	{// if have clicked the left botton once
		shapes[current].color = shapeColor;
		shapes[current].size = penSize;
		moveX = (xvcMax-xvcMin)*(float)x/winWidth - (xvcMax-xvcMin)/2;
		moveY = (yvcMax-yvcMin)/2 - (yvcMax-yvcMin)*(float)y/winHeight;
	    	shapes[current].ex = moveX-offsetX;
        	shapes[current].ey = moveY-offsetY;
	}
  if (leftButtonDown&&(count==1)){ draw( ); } 
  if(middleButtonDown)
  {
    	middleFlag = 1;
	moveX = (xvcMax-xvcMin)*(float)x/winWidth - (xvcMax-xvcMin)/2;
	moveY = (yvcMax-yvcMin)/2 - (yvcMax-yvcMin)*(float)y/winHeight;
	newOffsetX = moveX - downX;
	newOffsetY = moveY - downY;
/*  This makes the camera show different part of the canvas but will not across the boundary when clicking middle and dragging it*/

	if(newOffsetX+offsetX >50)
		newOffsetX = 50 - offsetX;
	if(newOffsetX + offsetX<-50)
		newOffsetX = -50 - offsetX;
	if(newOffsetY+offsetY > 50)
		newOffsetY = 50 - offsetY;
	if(newOffsetY + offsetY<-50)
		newOffsetY = -50 - offsetY;
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ( );
	gluOrtho2D (xvcMin-newOffsetX, xvcMax-newOffsetX, yvcMin-newOffsetY, yvcMax-newOffsetY);
  }
  glutPostRedisplay();
}

void displayFcn (void)
{
	//glViewport()
  glClear (GL_COLOR_BUFFER_BIT);   //  Clear display window.
  glColor3f(1.0,1.0,1.0);
  glRecti(xwcMin,ywcMin,xwcMax,ywcMax);// the canvas
    glColor3f(0,0,1.0);
	glLineWidth(1);
    glBegin(GL_LINES);
        glVertex2i(xwcMin,0);
        glVertex2i(xwcMax,0);
        glVertex2i(0,ywcMin);
        glVertex2i(0,ywcMax);
    glEnd();
    draw();
	
    glutSwapBuffers ( );
}

void winReshapeFcn (int newWidth, int newHeight)
{
    int x,y;
    y = newHeight - winHeight;
    x = newWidth - winWidth;
    float cx = ((float)x/winWidth/2)*(xvcMax-xvcMin);
    float cy = ((float)y/winHeight/2)*(yvcMax-yvcMin);
    xvcMin -=cx;
    xvcMax +=cx;
    yvcMin -=cy;
    yvcMax +=cy;

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ( );
    gluOrtho2D (xvcMin, xvcMax, yvcMin, yvcMax);
	/*  Reset display-window size parameters.  */
    winWidth = newWidth;
    winHeight = newHeight; 
    /*  Set the viewport to be the entire window  */
    glViewport(0,0,winWidth,winHeight);

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
    glutPassiveMotionFunc(MotionCallback);
	glutMotionFunc(MotionCallback);
    glutMainLoop ( );
}
