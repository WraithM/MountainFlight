// Author: Matthew Wraith

#include "Camera.h"

// Set the position of the camera. I use this for initialization.
// pos is the position. view is where the camera is looking. up is
// the up direction.
void Camera::setPosition(
        double px, double py, double pz,
        double vx, double vy, double vz,
        double ux, double uy, double uz) {
    pos = vec3(px, py, pz);
    view = vec3(vx, vy, vz);
    up = vec3(ux, vy, vz); // Define up.
}

// Draw the screen, and draw a crosshair
void Camera::drawCamera() {
	gluLookAt(
        pos.x,  pos.y,  pos.z,
        view.x, view.y, view.z,
        up.x,   up.y,   up.z
    );

    glDisable(GL_LIGHTING);
    drawCross();
    glEnable(GL_LIGHTING);
}

// Draw a crosshair
void Camera::drawCross() {
    // red
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_POINTS);
        glVertex3f(view.x, view.y, view.z);
    glEnd();
}

// Move forward
void Camera::move(double speed) {
    vec3 fwdv = view - pos; // viewing direction
    fwdv.normalize();

    pos += fwdv * speed;
    view += fwdv * speed;
}

// rotate about the viewing direction
void Camera::roll(double speed) {
    vec3 v = view - pos; // viewing direction

    up.rotate(v, speed);
}

// pitch up or down
void Camera::pitch(double speed) {
    vec3 fwdv = view - pos; // viewing direction

    vec3 v = cross(up, fwdv); // vector pointing to the left of forward motion
    
    fwdv.rotate(v, speed); // rotate the view direction

    up.rotate(v, speed); // rotate up
    view = fwdv + pos; // set the viewing direction to the rotated position
}
