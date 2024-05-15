#include "DataFusion.h"
// #include "Fusion.h"

void AhrsGetEarthAcceleration(float *quater , float *accel , float *EarthAccel)
{
    const float qwqw = (*quater)*(*quater); // calculate common terms to avoid repeated operation
    const float qwqx = (*quater)*(*(quater+1));
    const float qwqy = (*quater)*(*(quater+2));
    const float qwqz = (*quater)*(*(quater+3));
    const float qxqy = (*(quater+1))*(*(quater+2));
    const float qxqz = (*(quater+1))*(*(quater+3));
    const float qyqz = (*(quater+2))*(*(quater+3));

    *EarthAccel = 2.0f * ((qwqw - 0.5f +  *(quater+1) * *(quater+1)) * *accel + (qxqy - qwqz) *  *(accel+1) + (qxqz + qwqy) * *(accel+2)),
    // .x = 2.0f * ((qwqw - 0.5f + Q.x * Q.x) * A.x + (qxqy - qwqz) * A.y + (qxqz + qwqy) * A.z)
    *(EarthAccel+1) = 2.0f * ((qxqy + qwqz) * *accel + (qwqw - 0.5f + *(quater+2) * *(quater+2)) * *(accel+1) + (qyqz - qwqx) * *(accel+2)),
    // .y = 2.0f * ((qxqy + qwqz) * A.x + (qwqw - 0.5f + Q.y * Q.y) * A.y + (qyqz - qwqx) * A.z),
    *(EarthAccel+2) = 2.0f * ((qxqz - qwqy) * *accel + (qyqz + qwqx) * *(accel+1) + (qwqw - 0.5f + *(quater+3) * *(quater+3)) * *(accel+2)),
    // .z = 2.0f * ((qxqz - qwqy) * A.x + (qyqz + qwqx) * A.y + (qwqw - 0.5f + Q.z * Q.z) * A.z),
    // rotation matrix multiplied with the accelerometer
    // Remove gravity from accelerometer measurement
    *(EarthAccel+2) -= 1.0f;
}