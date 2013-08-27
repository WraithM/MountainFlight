// Author: Matthew Wraith

#include "Vector.h"

// Vector constructors
vec3::vec3() {}
vec3::vec3(double nx, double ny, double nz) { 
    x = nx; y = ny; z = nz; 
}

// Basic overloaded vector operations for nice arithmatic
vec3 vec3::operator+(vec3 v) { return vec3(v.x+x, v.y+y, v.z+z); }
vec3 vec3::operator-(vec3 v) { return vec3(x-v.x, y-v.y, z-v.z); }
vec3 vec3::operator*(double c) { return vec3(x*c, y*c, z*c); }
vec3 vec3::operator/(double c) { return vec3(x/c, y/c, z/c); }
void vec3::operator+=(vec3 v) { x += v.x; y += v.y; z += v.z; }

// setter
void vec3::set(vec3 v) {
    x = v.x;
    y = v.y;
    z = v.z;
}

// return self
vec3 vec3::self() {
    return vec3(x, y, z);
}

// Vector cross product
vec3 cross(vec3 a, vec3 b) {
    vec3 c;

    c.x = a.y*b.z - a.z*b.y;
    c.y = a.z*b.x - a.x*b.z;
    c.z = a.x*b.y - a.y*b.x;

    return c;
}

// Dot product of two vectors
double dot(vec3 a, vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

// Magnitude of a vector
double vec3::magnitude() {
    return sqrt(x*x + y*y + z*z);
}

// Normalize a vector
void vec3::normalize() {
    double m = magnitude();

    x = x / m;
    y = y / m;
    z = z / m;
}

// Rotate about an arbitrary axis v by angle
void vec3::rotate(vec3 v, double angle) {
    v.normalize();

    vec3 p = self();

    vec3 o = v * dot(p,v);
    vec3 a = p - o;
    vec3 b = cross(v, a);

    set(o + a * cos(angle) + b * sin(angle));
}
