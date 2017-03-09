#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
//#include <iostream>
//using namespace std;
class wcPt3D {
   public:
      GLfloat x, y, z;
};

wcPt3D downPt = {0,0,0};
/*  Set initial size of the display window.  */
GLsizei winWidth = 600, winHeight = 600;  

bool wheelUpButton = false, wheelDownButton = false,leftButtonDown = false,leftButtonUp = false;
GLfloat downX, downY;

/*  Set size of world-coordinate clipping window.  */
GLfloat xwcMin = -50.0, xwcMax = 50.0;
GLfloat ywcMin = -50.0, ywcMax = 50.0;

/*  Set current number of control points  */
GLint nCtrlPts = 5;
/*  Set the initial control points  */
wcPt3D ctrlPts [5] = { {-45.0, -45.0, 0.0}, {-21.0, 180.0, 0.0},
{0.0, -180.0, 0.0}, {21.0, 180.0, 0.0},{45.0,-45.0} };

int flag = 0;
int pts;

void init (void)
{
   /*  Set color of display window to white.  */
   glClearColor (1.0, 1.0, 1.0, 0.0);
}

void plotPoint (wcPt3D bezCurvePt)
{
    glBegin (GL_POINTS);
        glVertex2f (bezCurvePt.x, bezCurvePt.y);
    glEnd ( );
}

/*  Compute binomial coefficients C for given value of n.  */
void binomialCoeffs (GLint n, GLint * C)
{
   GLint k, j;

   for (k = 0;  k <= n;  k++) {
      /*  Compute n!/(k!(n - k)!).  */
      C [k] = 1;
      for (j = n;  j >= k + 1;  j--)
        C [k] *= j;
      for (j = n - k;  j >= 2;  j--)
        C [k] /= j;
   }
}

void computeBezPt (GLfloat t, wcPt3D * bezPt, GLint nCtrlPts,
                    wcPt3D * ctrlPts, GLint * C)
{
   GLint k, n = nCtrlPts - 1;
   GLfloat bezBlendFcn;

   bezPt->x = bezPt->y = bezPt->z = 0.0;

   /*  Compute blending functions and blend control points. */
   for (k = 0; k < nCtrlPts; k++) {
      bezBlendFcn = C [k] * pow (t, k) * pow (1 - t, n - k);
      bezPt->x += ctrlPts [k].x * bezBlendFcn;
      bezPt->y += ctrlPts [k].y * bezBlendFcn;
      bezPt->z += ctrlPts [k].z * bezBlendFcn;
   }
}

void bezier (wcPt3D * ctrlPts, GLint nCtrlPts, GLint nBezCurvePts)
{
   wcPt3D bezCurvePt;
   GLfloat t;
   GLint *C;

   /*  Allocate space for binomial coefficients  */
   C = new GLint [nCtrlPts];

   binomialCoeffs (nCtrlPts - 1, C);
   for (int i = 0;  i <= nBezCurvePts;  i++) {
      t = GLfloat (i) / GLfloat (nBezCurvePts);
      computeBezPt (t, &bezCurvePt, nCtrlPts, ctrlPts, C);
      plotPoint (bezCurvePt);
   }
   delete [ ] C;
}

int movePts(int x, int y)
{
	/* x and y are in world coordinates*/
	for(int i =0; i< nCtrlPts; i++)
	{
		if( (fabs(x - ctrlPts[i].x) < 10)&&(fabs(y - ctrlPts[i].y) < 10))
			return i;
	}
	return -1;
}


void MouseCallback(int button, int state, int x, int y)
{
   downX = 100*((float)x/(float)winWidth) - 50; // convert those window's coordinates to world coordinates.
   downY = 200 - 400*((float)y/(float)winHeight);
   /* judge whether the point(in world coordinates) is within a reasonable distance of a control point.*/
   if(flag == 0)
		pts = movePts(downX,downY);// if(flag == 1)means the left button is down, there is no meaning to calculate movePts()
   leftButtonDown = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN));
   if(leftButtonDown&&(pts!=-1))
   {
	    ctrlPts[pts].x = downX;
        ctrlPts[pts].y = downY;
        ctrlPts[pts].z = 0;
		flag = 1; // the flag is to state whether a control point has been selected.
   }

   if(leftButtonUp = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)))
   {
       if(flag == 1)
	   {
		   ctrlPts[pts].x = downX;
		   ctrlPts[pts].y = downY;
		   ctrlPts[pts].z = 0;
	   }
	   flag = 0;
   }
   glutPostRedisplay();
}

void MotionCallback(int x, int y)
{
  if (leftButtonDown && (pts != -1))
  {
	if(x<winWidth && y<winHeight && x>0 && y>0)
	{
	 downX = 100.0*((float)x/(float)winWidth) - 50; 
         downY = 200 - 400.0*((float)y/(float)winHeight); 
	   ctrlPts[pts].x = downX;
         ctrlPts[pts].y = downY;
         ctrlPts[pts].z = 0;
	}
  }
  glutPostRedisplay();
}

void displayFcn (void)
{
   /*  Set example number of
    *  curve positions to be plotted along the Bezier curve.
    */
   GLint  nBezCurvePts = 1000;

   glClear (GL_COLOR_BUFFER_BIT);   //  Clear display window.

   glPointSize (4);
   glColor3f (1.0, 0.0, 0.0);      //  Set point color to red.
   bezier (ctrlPts, nCtrlPts, nBezCurvePts);

   glPointSize (2);
   glColor3f(0.0, 0.0, 1.0);	// set line colour to blue
   glBegin(GL_LINE_STRIP);
    for (int k = 0; k < nCtrlPts; k++)
      glVertex3f(ctrlPts[k].x,ctrlPts[k].y,ctrlPts[k].z);
  glEnd();  
   glutSwapBuffers();
}

void winReshapeFcn (GLint newWidth, GLint newHeight)
{
   /*  Maintain an aspect accordance with the new window.  */
   glViewport (0, 0, newWidth, newHeight);

   winWidth = newWidth;  // new window size
   winHeight = newHeight;
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ( );

   gluOrtho2D (xwcMin, xwcMax, -200, 200);//let other two control points displayed in the screen.
   //gluOrtho2D (xwcMin, xwcMax, ywcMin, ywcMax);
   glutPostRedisplay();
}

int main (int argc, char** argv)
{
   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowPosition (50, 50);
   glutInitWindowSize (winWidth, winHeight);
   glutCreateWindow ("Bezier Curve");

   init ( );
   glutDisplayFunc (displayFcn);
   glutReshapeFunc (winReshapeFcn);
   glutMouseFunc(MouseCallback);
   glutMotionFunc(MotionCallback);
   glutMainLoop ( );
}
