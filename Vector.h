// Author: Matthew Wraith

#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

// My vector classes.
// 
// vec3 is a cartesian 3 vector
class vec3 {
public:
    double x, y, z; // Cartesian 3 vector

    vec3();
    vec3(double nx, double ny, double nz);

    vec3 operator+(vec3 v);
    vec3 operator-(vec3 v);
    vec3 operator*(double c);
    vec3 operator/(double c);
    void operator+=(vec3 v);

    double magnitude();
    void normalize();
    void rotate(vec3 v, double angle);
private:
    vec3 self();
    void set(vec3 v);
};

vec3 cross(vec3 a, vec3 b);
double dot(vec3 a, vec3 b);

#endif
