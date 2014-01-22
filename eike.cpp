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
 *
 * Letzte Aenderung: 08.05.13
 */

#include <stdlib.h>
#include <GL/glut.h>
extern "C"
{
	#include "gltx.h"
	#include "glm.h"
}


float angleX = 0.0;
float angleY = 0.0;
float angleZ = 0.0;
int achse = 0;
float rotUntersterPunktZ = 0.0;
float rotUntersterPunktY = 0.0;
float rotMittlererPunktZ = 0.0;
float rotObersterPunktZ = 0.0;
GLMmodel* model;

/*	Create checkerboard texture	*/
#define	checkImageWidth 64
#define	checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

#ifdef GL_VERSION_1_1
static GLuint texName;
#endif

void makeCheckImage(void)
{
   int i, j, c;
    
   for (i = 0; i < checkImageHeight; i++) {
      for (j = 0; j < checkImageWidth; j++) {
         c = ((((i&0x8)==0)^((j&0x8)==0)))*255;
         checkImage[i][j][0] = (GLubyte) c;
         checkImage[i][j][1] = (GLubyte) c;
         checkImage[i][j][2] = (GLubyte) c;
         checkImage[i][j][3] = (GLubyte) 255;
      }
   }
}

void init(void) 
{
	GLTXimage* image;		// Pointer auf die externe Bilddatei (RGB-Datei)

   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 50.0 };
   GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
   GLfloat light_position1[] = { -1.0, 1.0, 1.0, 0.0};
   GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
   GLfloat green[] = {0.0, 1.0, 0.0, 0.0};
   	model = glmReadOBJ("TexturObjekt.obj");
	glmUnitize(model);
	glmFacetNormals(model);
   


   glClearColor (0.0, 0.0, 0.0, 0.0);		/* Farbe zum Loeschen des Bildschirms zwischen 0.0 bis 1.0 */
   glShadeModel (GL_FLAT);	/* Schattierungsmodus */
   glShadeModel(GL_SMOOTH);
  

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, red);
   glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, green);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);

   /* Aus Wrap.c uebernommen*/
  // makeCheckImage();	// Textur erstellen
   image = gltxReadRGB("paisley.rgb");		// Externe Datei wird eingelesen
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);		//Bytes pro Pixel > 4 Bytes, so dass R|G|B| |R|G|B| |...

#ifdef GL_VERSION_1_1			// Kompiler-Schalter
   glGenTextures(1, &texName);
   glBindTexture(GL_TEXTURE_2D, texName);
#endif

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		// GL_REPEAT - Textur wird mehrmals auf das Objekt angewendet
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	// Einstellungen fuer Anti-Alising
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#ifdef GL_VERSION_1_1
   /* wird fuer eine interne Textur benoetigt*/
   // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 
             //   0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);						// GL-RGBA - 4 Bytes pro Pixel, zweite Angabe fuer Reihenfolge der Farbkodierung

   /* wird fuer eine externe Textur benoetigt*/
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (*image).width, (*image).height, 0, GL_RGB, GL_UNSIGNED_BYTE, (*image).data);	
#else
   glTexImage2D(GL_TEXTURE_2D, 0, 4, checkImageWidth, checkImageHeight, 
                0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
#endif

}

void zeichneKS(void)	//Zeichnet ein Koordinatensystem
{
	glDisable(GL_LIGHTING);
	glPushMatrix();		// Pushmatrix speichert alle bisher ausgefuehrten Matrikpultiplikationen (Skalierungen)
	glScalef(10.0, 0.1, 0.1);
	glutWireCube(0.2);
	glPopMatrix();		// Pop-Matrik stellt das zuvor gespeicherte Matrix(-Produkt) wieder her
	glPushMatrix();
	glScalef(0.1, 10.0, 0.1);
	glutWireCube(0.2);
	glPopMatrix();
	glPushMatrix();
	glScalef(0.1, 0.1, 10.0);
	glutWireCube(0.2);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void zeichneSegment(float hoehe)
{
	zeichneKS();
	glPushMatrix();

	glTranslatef(0.0, hoehe/2, 0.0);
	glScalef(hoehe/2, hoehe, hoehe/2);	//Die Form bleibt immer die gleiche, unabhaengig von der Groesse
	glutSolidSphere(0.5, 50, 50);
	glPopMatrix();
}

void display(void)
{

	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
	glPushMatrix();
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);
   glLoadIdentity ();             /* clear the matrix */
           /* viewing transformation  */
   gluLookAt (0.0, 0.0, 7.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   glRotatef(angleX, 1.0, 0.0, 0.0);
   glRotatef(angleY, 0.0, 1.0, 0.0);
   glRotatef(angleZ, 0.0, 0.0, 1.0);
   

   glEnable(GL_TEXTURE_2D);									// Schaltet die 2D-Texturdarstellung ein
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);// Einstellung bei Textur + Beleuchtung
#ifdef GL_VERSION_1_1
   glBindTexture(GL_TEXTURE_2D, texName);
#endif

   glBegin(GL_QUADS);		//GL-QUADS - Vierecke - OpenGL verbindet immer vier Punkte zu einem Viereck
   glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);		// x=-2, Y=-1, Z=0 im Koordinatensystem - erster Eckpunkt
   glTexCoord2f(0.0, 3.0); glVertex3f(-2.0, 1.0, 0.0);		// x=-2, y=1, z=0 im Koordinatensystem - zweiter Eckpunkt
   glTexCoord2f(3.0, 3.0); glVertex3f(0.0, 1.0, 0.0);		// usw.
   glTexCoord2f(3.0, 0.0); glVertex3f(0.0, -1.0, 0.0);		// glTexCoord2f ordnet Eckpunkte der Textur zu
															// Adressierung zwischen 0 und 1 - Durch 3
   glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
   glTexCoord2f(0.0, 3.0); glVertex3f(1.0, 1.0, 0.0);
   glTexCoord2f(3.0, 3.0); glVertex3f(2.41421, 1.0, -1.41421);	//
   glTexCoord2f(3.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);		//Eckpunkte werden zu Polygonen auf dem Bildschirm zusammengefuegt

   
   // Reihenfolge beim Erstellen der Polygone wichtig

   glEnd();
   glDisable(GL_TEXTURE_2D);

   /* aus vorheriger Uebung*/

   /********* 3D-Objekt und Teekanne **********/
   
   glTranslatef(0.0, 0.0, 1.2);
   glDisable(GL_LIGHTING);
   glmDraw(model, GLM_TEXTURE);
   glEnable(GL_LIGHTING);
   glTranslatef(-2.0, 0.0, 0.0);
   glDisable(GL_TEXTURE_2D);
   glutSolidTeapot (0.7);
   glTranslatef(-1.5, 0.0, 0.0);
   glTranslatef(0.0, -0.5, 0.0);

   /********* Roboterarm ***********/
   glRotatef(rotUntersterPunktZ, 0.0, 0.0, 1.0);	//Neigung des untersten Gelenks
   glRotatef(rotUntersterPunktY, 0.0, 1.0, 0.0);	//Drehung des untersten Gelenks
   zeichneSegment(1.0);
   glTranslatef(0.0, 1.0, 0.0);
   glRotatef(rotMittlererPunktZ, 0.0, 0.0, 1.0);	//Neigung des mittleren Gelenks
   zeichneSegment(0.8);
   glTranslatef(0.0, 0.8, 0.0);
   glRotatef(rotObersterPunktZ, 0.0, 0.0, 1.0);		//Neigung des obersten Gelenks
   zeichneSegment(0.4);
   glTranslatef(0.0, 0.4, 0.0);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glutSwapBuffers();
   glPopMatrix();
   //glFlush ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
   glMatrixMode (GL_MODELVIEW);
}

/* ARGSUSED1 */
void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
	  case 'a':
		 angleY += 2.0;
		 if (angleY == 360)
			  angleY = 0.0;
		 glutPostRedisplay();
		 break;
	  case 'd':
		 angleY -= 2.0;
		 glutPostRedisplay();
		 break;
	  case 'w':
		  angleX += 2.0;
		  if (angleX >= 360)
			  angleX = 0.0;
		  glutPostRedisplay();
		  break;
	  case 's':
		  angleX -= 2.0;
		  glutPostRedisplay();
		  break;
	  case 'q':
		  angleZ += 2.0;
		  if (angleZ >= 360)
			  angleZ = 0.0;
		  glutPostRedisplay();
		  break;
	  case 'e':
		  angleZ -= 2.0;
		  glutPostRedisplay();
		  break;
	  case 'l':
		  rotUntersterPunktY += 2.0;	// Drehung des untersten Gelenks
		  glutPostRedisplay();
		  break;
	  case 'j':
		  rotUntersterPunktY -= 2.0;	// Drehung des untersten Gelenks
		  glutPostRedisplay();
		  break;
	  case 'i':
		  rotUntersterPunktZ += 2.0;	//Neigung des untersten Gelenks
		  glutPostRedisplay();
		  break;
	  case 'k':
		  rotUntersterPunktZ -= 2.0;	//Neigung des untersten Gelenks
		  glutPostRedisplay();
		  break;
	  case 'h':
		  rotMittlererPunktZ += 2.0;	//Neigung des mittleren Gelenks
		  glutPostRedisplay();
		  break;
	  case 'n':
		  rotMittlererPunktZ -= 2.0;	//Neigung des mittleren Gelenks
		  glutPostRedisplay();
		  break;
	  case 'g':
		  rotObersterPunktZ += 2.0;	//Neigung des obersten Gelenks
		  glutPostRedisplay();
		  break;
	  case 'b':
		  rotObersterPunktZ -= 2.0; //Neigung des obersten Gelenks
		  glutPostRedisplay();
		  break;



   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (650, 650); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);	/* Callback-Funktionen */
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();				/* Kontrolle wird ans System abgegeben */
   return 0;
}
