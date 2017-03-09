
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#define M 60
#define N 40
//GLfloat xwcMin=-10,xwcMax=10,ywcMin=-10,ywcMax=10;
GLfloat winWidth = 600, winHeight = 500;
struct Pt2f{
	GLfloat x;
	GLfloat y;
	GLfloat red;
	GLfloat green;
	GLfloat blue;
}vertexPts[]={{0,M,1.0,0,0},{-M,0,0,1.0,0},{0,-2*M,0,0,1.0},{2*M,N,0,0,0}},pts01[M+1],pts12[2*M+1],pts23[2*M+1+N],pts30[2*M+1];

void init()
{
	glClearColor (1.0, 1.0, 1.0, 0);
	glViewport(0,0,winWidth,winHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity ( );
	gluOrtho2D(-winWidth/2,winWidth/2,-winHeight/2,winHeight/2);
}

void paintPoint(Pt2f pt)
{
	glBegin(GL_POINTS);
		glColor3f(pt.red,pt.green,pt.blue);
		glVertex2f(pt.x,pt.y);
	glEnd();
}

void scanline(Pt2f s, Pt2f e)
{
	int num = e.x - s.x + 1;
	Pt2f nxpt = s;
	for(int i=0; i<=num; i++)
	{
		paintPoint(nxpt);
		nxpt.x++;
		float u1 = fabs((float)(e.x-nxpt.x)/(e.x-s.x));
		float u2 = fabs((nxpt.x-s.x)/(e.x-s.x));
		nxpt.red = u1*s.red + u2*e.red;
		nxpt.green = u1*s.green + u2*e.green;
		nxpt.blue = u1*s.blue + u2*e.blue;
	}
}

void bresenhamLines(Pt2f s, Pt2f e, Pt2f* pt)
{
	float deltay = fabs(e.y - s.y);
	float deltax = fabs(e.x - s.x);
	float m = deltay/deltax;
	int flag;
	if((e.y - s.y)/(e.x - s.x) > 0)
		flag = 1;
	else flag = -1;
	Pt2f nxpt=s;
	int i;
	pt[0] = s;
	if(m < 1)
	{
		float p = 2*deltay - deltax;
		for(i=1;i<deltax+1;i++)
		{
			if(p<0)
			{
				nxpt.x ++;
				p += 2*deltay;
			}
			else
			{
				nxpt.x++;
				nxpt.y+= flag;
				p += 2*deltay - 2*deltax;
			}
		    float u1 = fabs((float)(e.x-nxpt.x)/(e.x-s.x));
		    float u2 = fabs((nxpt.x-s.x)/(e.x-s.x));
			nxpt.red = u1*s.red + u2*e.red;
			nxpt.green = u1*s.green + u2*e.green;
			nxpt.blue = u1*s.blue + u2*e.blue;
			pt[i] = nxpt;
		}
	}
	else
	{
		float p = 2*deltax - deltay;
		for(i=1;i<deltay+1;i++)
		{
			if(p<0)
			{
				nxpt.y += flag;
				p += 2*deltax;
			}
			else
			{
				nxpt.x++;
				nxpt.y += flag;
				p += 2*deltax - 2*deltay;
			}
		float u1 = fabs((float)(e.x-nxpt.x)/(e.x-s.x));
		float u2 = fabs((nxpt.x-s.x)/(e.x-s.x));
			nxpt.red = u1*s.red + u2*e.red;
			nxpt.green = u1*s.green + u2*e.green;
			nxpt.blue = u1*s.blue + u2*e.blue;
			pt[i] = nxpt;
		}
	}

}

void paintLine(Pt2f pts0[],Pt2f pts1[],int num1,int num2)
	{
		int i=0,j=0;
		while(i<=num1 && j<=num2)
		{
			if(pts0[i].y == pts1[j].y)
			{
				scanline(pts0[i],pts1[j]);
				i++;
				j++;
			}
			else if(pts0[i].y > pts1[j].y)
			{
				paintPoint(pts0[i]);
				i++;
			}
			else
			{
				paintPoint(pts1[j]);
				j++;
			}
		}
		if(i<=num1)
		{
			while(i<=num1)
			{
				paintPoint(pts0[i]);
				i++;
			}
		}
		if(j<=num2)
		{
			while(j<=num2)
			{
				paintPoint(pts1[j]);
				j++;
			}
		}
}

void polygon()
{
	bresenhamLines(vertexPts[1],vertexPts[0],pts01);
	bresenhamLines(vertexPts[1],vertexPts[2],pts12);
	bresenhamLines(vertexPts[2],vertexPts[3],pts23);
	bresenhamLines(vertexPts[0],vertexPts[3],pts30);
	int i,j=0;
	/* pts0[]: store those points on the line which are in the left side of y axis together with y values decending
	   pts1[]: store those points on the line which are in the right side of y axis together with y values decending*/
	Pt2f pts0[3*M+2],pts1[4*M+N+2];
	for(i=0;i<=M;i++)
		pts0[i]=pts01[M-i];
	for(i=0;i<=2*M;i++)
	{
		pts1[i] = pts30[i];
		pts1[2*M+1+i] = pts23[2*M+N-i];
		pts0[i+M+1] = pts12[i];
	}
	for(i=0;i<N;i++)
		pts1[4*M+2+i] = pts23[N-1-i];
	paintLine(pts0,pts1,3*M+1,4*M+N+1);
	paintLine(pts23,pts30,2*M+N,2*M);
}


void display(void)
{
/* display callback, clear frame buffer and z buffer,
   rotate cube and draw, swap buffers */

	glClear (GL_COLOR_BUFFER_BIT);
	polygon();
	glFlush();
	glutSwapBuffers();
}


void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity ( );
	gluOrtho2D(-w/2,w/2,-h/2,h/2);
    //glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

/* need both double buffering and z buffer */

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("fill");
	init();
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutMainLoop();

    return 1;
}

