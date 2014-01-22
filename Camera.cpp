#include "Camera.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "lina.h"
#include <vector>


extern "C"{
//#include "trackball.h"
}


#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif


Camera::Camera(float ex, float ey, float ez, float lx, float ly, float lz, float w, float h)
{
	eye.resize(4);
	look.resize(4);
	left.resize(4);
	up.resize(4);


	eye[0] = ex;
	eye[1] = ey;
	eye[2] = ez;
	eye[3] = 1.0;

	look[0] = lx;
	look[1] = ly;
	look[2] = lz;
	look[3] = 1;

	left[0] = (float)cos(90.0) * (look[0]) + (float)sin(90.0) * (look[2]);
	left[1] = 0;
	left[2] = (float)sin(90.0) * (look[0]) * (-1) + (float)cos(90.0) * (look[2]);
	left[3] = 0;
	
	up[0] = 0;
	up[1] = 1.0;;
	up[2] = 0;
	up[3] = 0;

	mxn = 0;
	mxo = -1;
	myn = 0;
	myo = -1;

	hWind = h;
	wWind = w;
	
	anglexz = 0;
//	this->size = 0;


}

Camera::Camera()
{
	//this->size = 0;

}


void Camera::rotateViewingDirection(float anglex, float angley, float anglez)
{

	// vertikaler blick
	look[1] += sinf(angley);

	// drehmatrix für x,z-ebene
	if(anglex)
	{
		look[0] = cosf(anglex) * look[0] + sinf(anglex) * look[2];
		look[2] = -1.0f * sin(anglex) * look[0] + cos(anglex) * look[2];
	}

	// update left-vektor für strafe
	left[0] = look[2];
	left[2] = -1 * look[0];

	normalizeVector(&left);
	normalizeVector(&look);	
}


void Camera::strafeSideways(bool right)
{
	float pace = 0.1f;
	if(right)
	{
		pace *= -1.0f;
	}

	eye[0] += left[0] * pace;
	eye[1] += left[1] * pace;
	eye[2] += left[2] * pace;

}

void Camera::strafeForward(bool backwards)
{
	float pace = 0.1f;
	if(backwards)
	{
		pace *= -1.0f;
	}
	eye[0] += look[0] * pace;
	eye[1] += look[1] * pace;
	eye[2] += look[2] * pace;
}




// mausdelta berechnen
void Camera::mouseMovement(int posx, int posy)
{
	if(mxo == -1)
	{
		mxo = mxn;
		myo = myn;
		return;
	}

	mxn = posx;
	myn = posy;
	float dx = (float)(mxn - mxo)*0.01f;
	float dy = (float)(myn - myo)*0.01f;

	rotateViewingDirection(-1*dx, -1*dy,0.0);
	

	// update left-vektor für strafe
	left[0] = look[2];
	left[2] = -1 * look[0];
	
	normalizeVector(&left);
	normalizeVector(&look);	

	mxo = mxn;
	myo = myn;
}


void Camera::showStatus()
{
	//std::cout << "eye\r\n" << eye[0] << " " << eye[1] << " " << eye[2] << "\r\n";
	//std::cout << "look\r\n" << look[0] << " " << look[1] << " " << look[2] << "\r\n";
	//std::cout << "sum\r\n" << eye[0] + look[0]<< " " << eye[1] + look[1]<< " " << eye[2] + look[2]<< "\r\n";
	
	//std::cout << "left " << left[0] << " " << left[1] << " " << left[2] << "\r\n";
	//std::cout << "look " << look[0] << " " << look[1] << " " << look[2] << "\r\n";
	
	//std::cout << "look dot left " << dotProduct(left, look, 3) << "\r\n";
	std::cout << anglexz << "\r\n";

	//system("cls");
}



Camera::~Camera(void)
{
}


