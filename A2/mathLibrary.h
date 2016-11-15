#ifndef MATHLIB_H
#define MATHLIB_H

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

// Represents a point in 2D plane.
class Point2D {
    // Private variables
    
public:
    int x, y;
    // Constructors
    Point2D();
    Point2D(int x, int y);
    
    // Print values to terminal
    void printValue();
    
    // Getter
    int getX();
    int getY();
};

// Represents a vector in 2D plane (with magntitude)
class Vector2D {
    // Private Variables
    
public:
    float x, y, magnitude;
    // Constructors
    Vector2D();
    Vector2D(int x, int y);
    
    // Print values to terminal
    void printValue();
    
    // Getter
    float getX();
    float getY();
    float getMagnitude();
};


//Represents a RGB colour value;
class Colour3f {
    // Private Variables
    
    
public:
    float red, green, blue;
    // Constructors
    Colour3f();
    Colour3f(int red, int green, int blue);
    
    // Print values to terminal
    void printValue();
};


// Solve the Euclidean Distance between 2 points
float distance(Point2D *point1, Point2D *point2);

//Quickly find out the relative distance, useful for calculating 3 points
float fastDistance(Point2D *point1, Point2D *point2);

// Calculate length of a vector
float length(Vector2D* vector);

// Normalizes a vector
Vector2D* normalize(Vector2D* vector);

// Multiplies vector by a scalar
Vector2D* vectorMultiply(Vector2D* vector, float scalar);

// Creates vector from 2 points
Vector2D* createVector(Point2D* point1, Point2D* point2);

// Moves a point given a vector
Point2D* movePoint(Point2D* point, Vector2D* vector);

// MARK: 3D Library
struct Light {
    float position[3];
    float ambient[4];
    float diffuse[4];
    float specular[4];
};

struct Material {
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float emission[4];
    float shine;
};

class Point3D{
public:
    Point3D();
    Point3D(float x, float y, float z);
    float x;
    float y;
    float z;
};

// 3D vector class
class Vector3D{
public:
    Vector3D();
    Vector3D(float x, float y, float z);
    float x;
    float y;
    float z;
    float length;
    void normalize();
    void add(Vector3D* vector);
    float calculateLength();
    Vector3D* crossProduct(Vector3D* vector);
};

// Calculate the vector between points a and b and return the corresponding vector object
Vector3D* vBetween(Point3D* point1, Point3D* point2);

// Sets the OpenGL lighting parameters using the Light Struct
void setLighting(Light light, int lightNumber);

void setMaterial(Material material);

#endif
