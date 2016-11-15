#ifndef MATHLIB_H
#define MATHLIB_H

// Represents a point in 2D plane.
class Point2D {
    //Private variables
    
    
public:
    float x, y;
    //Constructors
    Point2D();
    Point2D(float x, float y);
    
    //Print values to terminal
    void printValue();
    
    //Getter
    float getX();
    float getY();
};

// Represents a vector in 2D plane (with magntitude)
class Vector2D {
    //Private Variables
    
    
public:
    float x, y, magnitude;
    //Constructors
    Vector2D();
    Vector2D(int x, int y);
    
    //Print values to terminal
    void printValue();
    
    //Getter
    float getX();
    float getY();
    float getMagnitude();
};


//Represents a RGB colour value;
class Colour3f {
    //Private Variables
    
    
public:
    float red, green, blue;
    //Constructors
    Colour3f();
    Colour3f(int red, int green, int blue);
    
    //Print values to terminal
    void printValue();
};


//Solve the Euclidean Distance between 2 points
float distance(Point2D *point1, Point2D *point2);

//Quickly find out the relative distance, useful for calculating 3 points
float fastDistance(Point2D *point1, Point2D *point2);

//Calculate length of a vector
float length(Vector2D* vector);

//Normalizes a vector
Vector2D* normalize(Vector2D* vector);

//Multiplies vector by a scalar
Vector2D* vectorMultiply(Vector2D* vector, float scalar);

//Creates vector from 2 points
Vector2D* createVector(Point2D* point1, Point2D* point2);

//Moves a point given a vector
Point2D* movePoint(Point2D* point, Vector2D* vector);

#endif
