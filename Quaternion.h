#pragma once
class Quaternion
{
public: 
	float w,x,y,z;

public:
Quaternion();
~Quaternion();
double length(Quaternion quat);

};
