#include <math.h>
#include "Quaternion.h"

Quaternion::Quaternion(void)
{
}

Quaternion::~Quaternion(void)
{
}

double Quaternion::length(Quaternion quat)
{
  return sqrt(quat.x * quat.x + quat.y * quat.y +
              quat.z * quat.z + quat.w * quat.w);
}
