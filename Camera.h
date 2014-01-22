#include "lina.h"
#include <vector>


class Camera
{
public:	
	Camera(float ex, float ey, float ez, float lx, float ly, float lz, float w, float h);
	Camera(void);
	void mouseMovement(int posx, int posy);
	
	~Camera(void);
	
	void MatrixMulVector(float m[4][4], float v[], float out[]);
	void strafeSideways(bool right);
	void rotateViewingDirection(float anglex, float angley, float anglez);
	void showStatus(void);
	void strafeForward(bool backwards);
	//float dotProduct(float v1[], float v2[], int  dimension);

	// augenpunkt
	Vector eye;
	// blickrichtung
	Vector look;
	// up-vektor
	Vector up;

	//vertikaler winkel 
	float anglexz;
	
	// orientierung
	Vector left;

	// mouse x new, mouse x old, ...
	float mxn, mxo, myn, myo;
	
	// fenstergröße
	float wWind,hWind;

	//const int size;

};
