#include "Camera.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>

extern "C"{
#include "trackball.h"
}

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif


Camera::Camera(float ex, float ey, float ez, float lx, float ly, float lz, float w, float h)
{
	
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
}

Camera::Camera()
{}


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

	normalizeVector(left);
	normalizeVector(look);	
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


void Camera::rotateVector(float view[], float axis[], float Angle)
{
	Quaternion quat_view,temp,result;
	temp.x = axis[0] * sin(Angle/2);
	temp.y = axis[1] * sin(Angle/2);
	temp.z = axis[2] * sin(Angle/2);
	temp.w = cos(Angle/2);

	quat_view.x = view[0];
	quat_view.y = view[1];
	quat_view.z = view[2];
	quat_view.w = 0;

    result = mult(mult(temp, quat_view), conjugate(temp));

	view[0] = result.x;
	view[1] = result.y;
	view[2] = result.z;
}


Quaternion Camera::normalize(Quaternion quat)
{
  double L = quat.length(quat);

  quat.x /= L;
  quat.y /= L;
  quat.z /= L;
  quat.w /= L;

  return quat;
}

Quaternion Camera::conjugate(Quaternion quat)
{
  quat.x = -quat.x;
  quat.y = -quat.y;
  quat.z = -quat.z;
  return quat;
}

Quaternion Camera::mult(Quaternion A, Quaternion B)
{
  Quaternion C;
  C.x = A.w*B.x + A.x*B.w + A.y*B.z - A.z*B.y;
  C.y = A.w*B.y - A.x*B.z + A.y*B.w + A.z*B.x;
  C.z = A.w*B.z + A.x*B.y - A.y*B.x + A.z*B.w;
  C.w = A.w*B.w - A.x*B.x - A.y*B.y - A.z*B.z;
  return C;
}

// wendet matrix auf vektor an
void Camera::MatrixMulVector(float m[4][4], float v[], float out[] )
{
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			out[i] +=  m[i][j] * v[j];
		}

	}
}

void Camera::mouseMovement(int posx, int posy)
{
	float q[4]; // quaternion für rotation
	float mv[4][4]; // vertikale rotation
	float lookRotated[4]; // neuer 



	if(mxo == -1)
	{
		mxo = mxn;
		myo = myn;
		return;
	}

	mxn = posx;
	myn = posy;

	// quaternion erstellen
	// mausparameter müssen angepasst werden
	trackball(q, 
			(2*mxo - wWind ) / wWind, 
			(2*myo - hWind) / hWind,
			(2*mxn - wWind ) / wWind, 
			(2*myn - hWind) / hWind, left);

	/*
	Quaternion temp;
	temp.x = q[0];
	temp.y = q[1];
	temp.z = q[2];
	temp.w = 0;
	
	Quaternion quat_view;
	quat_view.x = look[0];
	quat_view.y = look[1];
	quat_view.z = look[2];
	quat_view.w = 0;

    lookRotated = mult(mult(temp, quat_view), conjugate(temp));
	look[0] = lookRotated.x;
	look[1] = lookRotated.y;
	look[2] = lookRotated.z;
	*/

	build_rotmatrix(mv, q);

	// rotieren mit neuer Rotationsmatrix mv
	MatrixMulVector(mv, look, lookRotated);
	memcpy(look, lookRotated, sizeof(lookRotated));

	left[0] = look[2];
	left[2] = -1 * look[0];
	
	normalizeVector(left);
	normalizeVector(look);	

	mxo = mxn;
	myo = myn;
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

	//rotateViewingDirection(-1*dx, -1*dy,0.0);
	if(abs(anglexz + dy) < 10) 
	{
		anglexz += dy;
		//rotateVector(look, left, dy);
		
	}

	// update left-vektor für strafe
	left[0] = look[2];
	left[2] = -1 * look[0];
	
	normalizeVector(left);
	normalizeVector(look);	

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

void Camera::crossProduct(float u[], float v[], float *out[])
{
	float res[3];

	res[0] = u[1]*v[2] - v[1]*u[2];
	res[1] = v[0]*u[2] - u[0]*v[2];
	res[2] = u[0]*v[1] - u[1]*v[0]; 

	u[0] = res[0];
	u[1] = res[1];
	u[2] = res[2];

}


void Camera::normalizeVector(float vec[])
{
	float len = sqrtf(pow(vec[0],2) + pow(vec[1],2) + pow(vec[2],2));
	vec[0] /= len;
	vec[1] /= len;
	vec[2] /= len;
}


Camera::~Camera(void)
{
}


