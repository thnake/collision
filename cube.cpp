/*
* Copyright (c) 1993-1997, Silicon Graphics, Inc.
* ALL RIGHTS RESERVED 
* Permission to use, copy, modify, and distribute this software for 
* any purpose and without fee is hereby granted, provided that the above
* copyright notice appear in all copies and that both the copyright notice
* and this permission notice appear in supporting documentation, and that 
* the name of Silicon Graphics, Inc. not be used in advertising
* or publicity pertaining to distribution of the software without specific,
* written prior permission. 
*
* THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
* AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
* INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
* FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
* GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
* SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
* KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
* LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
* THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
* ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
* POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
* 
* US Government Users Restricted Rights 
* Use, duplication, or disclosure by the Government is subject to
* restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
* (c)(1)(ii) of the Rights in Technical Data and Computer Software
* clause at DFARS 252.227-7013 and/or in similar or successor
* clauses in the FAR or the DOD or NASA FAR Supplement.
* Unpublished-- rights reserved under the copyright laws of the
* United States.  Contractor/manufacturer is Silicon Graphics,
* Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
*
* OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
*/

/*
*  cube.c
*  This program demonstrates a single modeling transformation,
*  glScalef() and a single viewing transformation, gluLookAt().
*  A wireframe cube is rendered.

http://pyopengl.sourceforge.net/documentation/manual-3.0/index.xhtml#GL
*/

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <windows.h>
#include "lina.h"
#include "Camera.h"
#include "bowl.h"
#include "body.h"
#include "triangle.h"
#include "intersect.h"
extern "C"
{
	#include "gltx.h"
	#include "glm.h"

}
#include <GL/glut.h>

#define MAX_BOWLS = 100
#define T(x) (model->triangles[(x)])

std::vector<body> objects;
float triangleTilt = 0.1;


DWORD time = 0;

GLMmodel* model;

int windowSizeX = 1000;
int windowSizeY = 1000;

int elapsedTime = 0;
float deltaT = 0;

float rj1 = 0.0;
float rj2 = 0.0;
float rj3 = 0.0;
float rj4 = 0.0;

/*Camera cam(0.0,0.0,-5.0,
		   0.0,0.0,1.0,
		   windowSizeX, windowSizeY);
*/

// überblick
/*
Camera cam(-6.1036506,3.2070398,8.4127731,
		   0.58223242,-0.29908794,-0.75601047,
		   windowSizeX, windowSizeY);*/
// nah dran
Camera cam(0.73181111,-1.2484027,-0.56966168,
		   -0.97636563,-0.21209368,-0.041551121,
		   windowSizeX, windowSizeY);



int bcount = 0;
int pcount = 1;
bowl bowls[50];
plane planes[10];
plane *bottom;
bool mouse = false;
bool friction = false;
bool fix = false;
bool _throw = false;

int drawCoordinates = 1;

float h = 0.5;

//float gAcc[] = {0.0, -9.81, 0.0};
float gAcc[] = {0.0, -0.001, 0.0};

/*	Create checkerboard texture	*/
#define	checkImageWidth 64
#define	checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];


#ifdef GL_VERSION_1_1
static GLuint texName;
#endif




void initSpheres()
{
	bowl *b;
	for(int i = 0; i < bcount; i++)
	{
		b = &bowls[i];
		b->pace[0] = 0.0f;
		b->pace[1] = 0.0f;
		b->pace[2] = 0.0f;
	}
}
void initPlanes()
{
	
	Vector p;
	p.push_back(-5.0);
	p.push_back(0.0);
	p.push_back(-5.0);

	Vector va(3,0);
	va[0] = 1.0;

	Vector vb(3,0);
	vb[2] = 1.0;
	bottom = new plane(p, va, vb, 10.0, 10.0);

	objects.resize(2);

	
	
	if(model)
	{
		int vertsLen = sizeof(GLfloat) * model->numtriangles*9;
		GLfloat *verts = (GLfloat*)malloc(vertsLen);
		vertsLen /= sizeof(GLfloat);
		int vcCount = 0;

		glmDrawMesh(model, verts);

		objects[1].triangles.resize(model->numtriangles);

		for(int i = 0; i < model->numtriangles;i++)
		{

			objects[1].triangles[i].a[0] = verts[vcCount++];
			objects[1].triangles[i].a[1] = verts[vcCount++];
			objects[1].triangles[i].a[2] = verts[vcCount++];

			objects[1].triangles[i].b[0] = verts[vcCount++];
			objects[1].triangles[i].b[1] = verts[vcCount++];
			objects[1].triangles[i].b[2] = verts[vcCount++];

			objects[1].triangles[i].c[0] = verts[vcCount++];
			objects[1].triangles[i].c[1] = verts[vcCount++];
			objects[1].triangles[i].c[2] = verts[vcCount++];

		}
	}

	//erstes Bodendreick
	objects[0].triangles.resize(0);
	//objects[0].triangles[0].a[0] = -5;
	//objects[0].triangles[0].a[1] = -1.0;
	//objects[0].triangles[0].a[2] = -5;

	//objects[0].triangles[0].b[0] = -5;
	//objects[0].triangles[0].b[1] = -1;
	//objects[0].triangles[0].b[2] = 5;

	//objects[0].triangles[0].c[0] = 5;
	//objects[0].triangles[0].c[1] = -1;
	//objects[0].triangles[0].c[2] = -5;
	//
	//// zweites Bodendreieck
	//
	//objects[0].triangles[1].a[0] = 5;
	//objects[0].triangles[1].a[1] = -1;
	//objects[0].triangles[1].a[2] = -5;

	//objects[0].triangles[1].b[0] = -5;
	//objects[0].triangles[1].b[1] = -1;
	//objects[0].triangles[1].b[2] = 5;

	//objects[0].triangles[1].c[0] = 5;
	//objects[0].triangles[1].c[1] = -1.0;
	//objects[0].triangles[1].c[2] = 5;


	

	
	triangle *tri;
	Vector t;
	t.push_back(-1);
	t.push_back(-1);
	t.push_back(-1);

	
	for(int i = 0; i < objects.size(); i++)
	{
		for(int j = 0; j < objects[i].triangles.size(); j++)
		{
		tri = &objects[i].triangles[j];
		tri->translate(t);

		tri->setNormal();
		}
	}

	
	

	pcount = 1;
}

void addBowl(float x, float y, float z)
{

	if(bcount == 50)
	{
		return; 
	}

	bowls[bcount].pos[0] = x;
	bowls[bcount].pos[1] = y;
	bowls[bcount].pos[2] = z;
	bowls[bcount].fixed = false;
	
	bcount++;
}


void init(void)
{
//
//	GLTXimage* image;
//	
//	glClearColor (0.0, 0.0, 0.0, 0.0);
//	   glShadeModel(GL_FLAT);
//	   glEnable(GL_DEPTH_TEST);
//
//	   //makeCheckImage();
//	   image = gltxReadRGB("paisley.rgb");
	   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//
		   model = glmReadOBJ("hpsmall.obj");
//

	   //glmUnitize(model);
	   //glmFacetNormals(model); // flächennormalen
	   //glmVertexNormals(model, 45.0, false); // eckennormalen
//
//
//	#ifdef GL_VERSION_1_1
//	   glGenTextures(1, &texName);
//	   glBindTexture(GL_TEXTURE_2D, texName);
//	#endif
//
//	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	
//	
//
//#ifdef GL_VERSION_1_1
//   /* wird fuer eine interne Textur benoetigt*/
//   // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 
//             //   0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);						// GL-RGBA - 4 Bytes pro Pixel, zweite Angabe fuer Reihenfolge der Farbkodierung
//
//   /* wird fuer eine externe Textur benoetigt*/
//   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (*image).width, (*image).height, 0, GL_RGB, GL_UNSIGNED_BYTE, (*image).data);	
//#else
//   glTexImage2D(GL_TEXTURE_2D, 0, 4, checkImageWidth, checkImageHeight, 
//                0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
//#endif


	   // eignes

//glutSetCursor(GLUT_CURSOR_NONE); 
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat light_position1[] = { 0.0, 1.0, 1.0, 0.0 };

	GLfloat red[] = {1.0,0.0,0.0,1.0};

	GLfloat green[] = {0.0,1.0,0.0,1.0};

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, red);

	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, green);

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT2);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	
	//bowls[0].move = false;
	initPlanes();
	initSpheres();
	//addBowl(0.0, 2,-1.0);
	

}


void moveObjects()
{
	Vector direction(3,0);
	float distance = 0.0f;

	int newElapsedTime = glutGet(GLUT_ELAPSED_TIME);
	deltaT = (newElapsedTime - elapsedTime) * 0.001;
	deltaT = 1.0;
	elapsedTime = newElapsedTime;

	for(int j = 0; j < bcount; j++)
	{

		if(!bowls[j].fixed)
		{
			
			bowls[j].pace[0] += gAcc[0];
 			bowls[j].pace[1] += gAcc[1];
			bowls[j].pace[2] += gAcc[2];
		}

		for(int i = 0; i < bcount; i++)
		{
			break;
			if(i == j)
			{ 
				continue;
			}

			// distanz zwischen schwerpunkt und objekt
			direction[0] = bowls[i].pos[0] - bowls[i].pos[0];
			direction[1] = bowls[i].pos[1] - bowls[i].pos[1];
			direction[2] = bowls[i].pos[2] - bowls[i].pos[2];

			distance = sqrtf( direction[0] * direction[0] + direction[1] * direction[1] + direction[2]*direction[2]);
			normalizeVector(&direction);

			if(distance < 1.0f)
			{
				distance = 1;
			}

			// freifelddämpfung
			bowls[j].pace[0] += (bowls[i].gravity * direction[0]) / (distance*distance);
			bowls[j].pace[1] += (bowls[i].gravity * direction[1]) / (distance*distance);
			bowls[j].pace[2] += (bowls[i].gravity * direction[2]) / (distance*distance);
		}	

		bowls[j].updatePosition(deltaT);

	}
}

void drawObjects()
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	for(int i = 0; i < bcount; i++)
	{
		glPushMatrix();
		glTranslatef(bowls[i].pos[0], bowls[i].pos[1], bowls[i].pos[2]);
		glutWireSphere(bowls[i].radius, 10, 10);
		glTranslatef(bowls[i].pace[0] + gAcc[0], bowls[i].pace[1] + gAcc[1], bowls[i].pace[2] + gAcc[2]);
		glutSolidSphere(0.05,1,1);
		glPopMatrix();

		glPushMatrix();
		bowl *b = &bowls[i];
		glColor3f(1,1,1);
		glBegin(GL_LINES);
		Vector dir = b->pace;
		normalizeVector(&dir);
		glVertex3f(b->pos[0],b->pos[1],b->pos[2]);

		glVertex3f(b->pos[0]+dir[0]*5,
			b->pos[1]+5*dir[1],
			b->pos[2]+5*dir[2]);
		glPopMatrix();
		glEnd();
	}

	glColor3f(1,1,1);
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < objects.size(); i++)
	{
		for(int j = 0; j < objects[i].triangles.size(); j++)
		{
			
			glVertex3f(objects[i].triangles[j].a[0], objects[i].triangles[j].a[1], objects[i].triangles[j].a[2]);
			glVertex3f(objects[i].triangles[j].b[0], objects[i].triangles[j].b[1], objects[i].triangles[j].b[2]);
			glVertex3f(objects[i].triangles[j].c[0], objects[i].triangles[j].c[1], objects[i].triangles[j].c[2]);

		}	
	}

	glEnd();
}


void mouseClick(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{ 
		//bcount = 0;
		//addBowl(-3, 1.31,-3);
		if(_throw)
		{
			addBowl(cam.eye[0], cam.eye[1], cam.eye[2]);
			bowls[bcount-1].pace[0] = cam.look[0]/20;
			bowls[bcount-1].pace[1] = cam.look[1]/20;
			bowls[bcount-1].pace[2] = cam.look[2]/20;
		}
		else
		{
			addBowl( -1.5467927, 2.3390250, -0.21634442);
			bowls[bcount-1].pace[0] = 0.0;
			bowls[bcount-1].pace[1] = 0.0;
			bowls[bcount-1].pace[2] = 0.0;
		}
	}
}



void idle()
{
	glutPostRedisplay();
}

void visible(int vis)
{
    if (vis == GLUT_VISIBLE)
        glutIdleFunc(idle);
    else
        glutIdleFunc(NULL);
}



void inline collSpherePlanes()
{
	// p= startpunkt 
	// q=endpunkt
	// a,b,c = Dreieckspunkte
	// u,v,w baryzentrische koordinaten zum Schnittpunkt s
	// s = u*a + v*b + w*c
	triangle tri;
	bowl *b;
	float u,v,w;
	int check = 0;
	Vector s(3); // schnittpunkt mit dreieck
	float pace = 0;
	float lambda = 0;
	for(int h = 0; h < bcount; h++)
	{
		b = &bowls[h];
		
		if(b->fixed)
		{
			continue;
		}

		for(int i = 0; i < objects.size(); i++)
		{
			for(int j = 0; j < objects[i].triangles.size(); j++)
			{
				
				Vector dir(3);
				dir[0] = b->pace[0];
				dir[1] = b->pace[1];
				dir[2] = b->pace[2];
				normalizeVector(&dir);
				float len = getVectorLen(dir);
					tri = objects[i].triangles[j];
					// tri um länge des radius heranholen
					float sign = (scalarProd(dir, tri.normal) < 0) ? 1 : -1;
					tri.a[0] += b->radius*tri.normal[0]*sign;
					tri.a[1] += b->radius*tri.normal[1]*sign;
					tri.a[2] += b->radius*tri.normal[2]*sign;

					tri.b[0] += b->radius*tri.normal[0]*sign;
					tri.b[1] += b->radius*tri.normal[1]*sign;
					tri.b[2] += b->radius*tri.normal[2]*sign;

					tri.c[0] += b->radius*tri.normal[0]*sign;
					tri.c[1] += b->radius*tri.normal[1]*sign;
					tri.c[2] += b->radius*tri.normal[2]*sign;

					// broad phase:
					pace = getVectorLen(b->pace);

					// naiver test mit unendlicher  ebene
					check = checkIntersectRayPlane(tri, b->oldPos, b->pace, &lambda);
					if (check == 0 || lambda > pace)
					{
						continue;
					}

					std::cout << lambda << "\r\n"; 

					//check = IntersectLineTriangle(b->oldPos, b->pos, tri.a, tri.b, tri.c, u,v,w, &tri);
					s[0] = b->oldPos[0] + lambda*dir[0];
					s[1] = b->oldPos[1] + lambda*dir[1];
					s[2] = b->oldPos[2] + lambda*dir[2];
					check = checkPointInTriangle(s, &tri);
					

 					Vector face = s - b->oldPos;
					normalizeVector(&face);
					float dot = scalarProd(face, dir);

					if(check == 1 && dot > 0)
					{
						// schauen, ob schnittpunkt überschritten wurde
						if(lambda <= pace)
						{
							Vector m(3);
							m[0] = b->oldPos[0] + b->pace[0]/2;
							m[1] = b->oldPos[1] + b->pace[1]/2;
							m[2] = b->oldPos[2] + b->pace[2]/2;

							// schauen, ob schnittpunkt im radius ist
							float d = getDistance(s,m); 
							if(d < pace)
							{
								// kollision
								Vector out(3);
								float subLen = abs(getDistance(s, b->pos));
								getOutVector(dir, tri.normal, &out);

						

								b->pos[0] = s[0] + subLen * out[0]; // + out[0] * subLen;
								b->pos[1] = s[1] + subLen * out[1]; // + out[1] * subLen; 
								b->pos[2] = s[2] + subLen * out[2]; // + out[2] * subLen; 

								// neue schrittweite 
								// gleitanteil
								Vector slide = out - tri.normal;
								float outDotN = abs(scalarProd(out,tri.normal));
								
								slide[0]= (out[0] - outDotN*tri.normal[0]);
								slide[1]= (out[1] - outDotN*tri.normal[1]);
								slide[2]= (out[2] - outDotN*tri.normal[2]);
								
								out[0] = tri.normal[0] + 8*slide[0];
								out[1] = tri.normal[1] + 8*slide[1];
								out[2] = tri.normal[2] + 8*slide[2];
								normalizeVector(&out);

								
								// kugel fällt durch das mesh durch, wenn aktiviert
								
								if(friction)
								{
									pace *= b->friction;
								}
								if(pace <= 0.001 && fix)
								{
									pace = 0;
									b->fixed = true;
								}

								b->pace[0] = out[0] * pace;
								b->pace[1] = out[1] * pace;
								b->pace[2] = out[2] * pace;

							}
						}
					
					}
			}
		}
	}
}

void collisionsSphereBottom()
{
	float lambda =  0.0; // länge bis schnittpunkt
	int check = 0;
	Vector m(3,0);	// mittelpunkt zwischen alter und neuer position
	Vector s(3,0); // schnittpunkt
	Vector dir(3,0); // richtung normalisiert
	float pace;
	bowl *b;

	for(int i = 0; i < bcount; i++)
	{
		b = &bowls[i];
		pace = getVectorLen(b->pace);
		
		if(pace == 0)
		{
			continue;
		}
		
		b->oldPos[0] = b->pos[0] - b->pace[0];
		b->oldPos[1] = b->pos[1] - b->pace[1];
		b->oldPos[2] = b->pos[2] - b->pace[2];


		// für jede ebene, jede kugel
		//check = checkIntersectRayPlane(*bottom, b->oldPos, b->pace, &lambda);
		check = 0;
		if(check)
		{
			// wenn distanz zum schnittpunkt < schrittweite
			lambda -= b->radius;
			if(lambda <= pace)
			{
				
				m[0] = b->oldPos[0] + b->pace[0]/2;
				m[1] = b->oldPos[1] + b->pace[1]/2;
				m[2] = b->oldPos[2] + b->pace[2]/2;

				dir[0] = b->pace[0];
				dir[1] = b->pace[1];
				dir[2] = b->pace[2];
				normalizeVector(&dir);

				s[0] = b->oldPos[0] + lambda*dir[0];
				s[1] = b->oldPos[1] + lambda*dir[1];
				s[2] = b->oldPos[2] + lambda*dir[2];

				// schauen, ob schnittpunkt im radius ist
				float d = getDistance(s,m); 

				if(d <= pace)
				{
					// kollision
					Vector out(3);
					float subLen = abs(getDistance(s, b->pos)) - b->radius;
					getOutVector(dir, bottom->normal, &out);
					
					b->pos[0] = s[0];// + out[0] * subLen;
					b->pos[1] = s[1];// + out[1] * subLen; 
					b->pos[2] = s[2];// + out[2] * subLen; 

					// neue schrittweite 
					pace *= b->friction;
					if(pace <= 0.004)
					{
						pace = 0;
						b->fixed = true;
					}

					b->pace[0] = out[0] * pace;
					b->pace[1] = out[1] * pace;
					b->pace[2] = out[2] * pace;

				}
			}
		}
	}
}






void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);
	glLoadIdentity ();             
	/* clear the matrix */
	
	/* viewing transformation  */
	gluLookAt (cam.eye[0], cam.eye[1], cam.eye[2], 
				cam.look[0] + cam.eye[0], 
				cam.look[1] + cam.eye[1], 
				cam.look[2] + cam.eye[2],
				cam.up[0], cam.up[1],cam.up[2]);

	moveObjects();
//	collisionsSphereBottom();
	collSpherePlanes();
	drawObjects();
	
	glPushMatrix();
	glPopMatrix();

	glutSwapBuffers();
}


void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	glMatrixMode (GL_MODELVIEW);
}

void lookAtModel()
{
	if(model)
	{
		cam.look[0] = model->vertices[3] - cam.eye[0];
		cam.look[1] = model->vertices[4] - cam.eye[1];
		cam.look[2] = model->vertices[5] - cam.eye[2];
		normalizeVector(&cam.look);
		cam.rotateViewingDirection(0,0,0); // erzeugt neuen Linksvektor
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 'w':
			cam.strafeForward(false);
			break;
		case 's':
			cam.strafeForward(true);
			break;
		case 'a':
			cam.strafeSideways(false);
			break;
		case 'd':
			cam.strafeSideways(true);
			break;
		case 'x': 
			fix = !fix;
			break;
		case 't':
			_throw = !_throw;
			break;
		case 'm':
			mouse = !mouse;
			break;
		case 'l':
			lookAtModel();
			break;
		case 'f':
			friction = !friction;
			break;
		case 27:
		  exit(0);
		  break;
	}


}

void mouseMove(int x, int y)
{
	if(mouse){
	cam.mouseMovement(x,y);
	}
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (windowSizeX, windowSizeY); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init ();

	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseClick);
	glutVisibilityFunc(visible);
	glutPassiveMotionFunc(mouseMove);
	
	glutMainLoop();
	return 0;
}


