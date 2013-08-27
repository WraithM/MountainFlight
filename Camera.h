// Author: Matthew Wraith

#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Vector.h"

class Camera {
public:
    vec3 pos; // Position
    vec3 view; // View
    vec3 up; // Up

    void setPosition(
            double pos_x, double pos_y, double pos_z,
            double view_x, double view_y, double view_z,
            double up_x, double up_y, double up_z);

    void move(double speed);
    void roll(double speed);
    void pitch(double speed);
    void drawCross();
    void drawCamera();
};

#endif
