#include <stdio.h>
#include <math.h>
#include "mathLibrary.h"

// MARK: Point2D Methods

Point2D::Point2D() {
    x = 0.0f;
    y = 0.0f;
}

Point2D::Point2D(float x, float y) {
    this->x = x;
    this->y = y;
}

void Point2D::printValue() {
    printf("x = %f, y = %f \n", x, y);
}

float Point2D::getX() {
    return this->x;
}

float Point2D::getY() {
    return this->y;
}

// MARK: Vector2D Methods

Vector2D::Vector2D() {
   x = 0;
   y = 0;
   magnitude = 0;
}

Vector2D::Vector2D(int x, int y) {
   this->x = x;
   this->y = y;
   length(this);
}

void Vector2D::printValue() {
	printf("x = %f, y = %f, magnitude = %f \n", x, y, magnitude);
}

float Vector2D::getX() {
   return this->x;
}

float Vector2D::getY() {
   return this-> y;
}

float Vector2D::getMagnitude() {
   return this->magnitude;
}

// MARK: Colour3f Methods

Colour3f::Colour3f() {
   this->red = 0;
   this->green = 0;
   this->blue = 0;
}

Colour3f::Colour3f(int red, int green, int blue) {
   this->red = red;
   this->green = green;
   this->blue = blue;
}

void Colour3f::printValue() {
	printf("red = %f, green = %f, blue = %f", red, green, blue);
}

// MARK: mathLibrary Helper Functions

float distance(Point2D* point1, Point2D* point2) {
   float result = sqrt(pow((point2->x - point1->x), 2) + pow((point2->y - point1->y), 2));
   
   return result;
}

float fastDistance(Point2D* point1, Point2D* point2) {
   float result = (pow((point2->x - point1->x), 2) + pow((point2->y - point1->y), 2));
   
   return result;
}

float length(Vector2D* vector) {
   float result = sqrt(pow((vector->x), 2) + pow((vector->y), 2));
   
   vector->magnitude = result;
   
   return result;
}

Vector2D* normalize (Vector2D* vector) {
   float vectorLength = length(vector);
   
   float normalizedX = (vector->x)/vectorLength;
   float normalizedY = (vector->y)/vectorLength;
   
   Vector2D* normalizedVector = new Vector2D(normalizedX, normalizedY);
   
   return normalizedVector;
}

Vector2D* vectorMultiply(Vector2D* vector, float scalar) {
   float multipliedX = (vector->x) * scalar;
   float multipliedY = (vector->y) * scalar;
   
   Vector2D* multipliedVector = new Vector2D(multipliedX, multipliedY);
   
   return multipliedVector;
}

Vector2D* createVector(Point2D* point1, Point2D* point2) {
   float vectorX = point2->x - point1->x;
   float vectorY = point2->y - point1->y;
   
   Vector2D* vector = new Vector2D(vectorX, vectorY);
   
   return vector;
}

Point2D* movePoint(Point2D* point, Vector2D* vector) {
   float movedX = point->x + vector->x;
   float movedY = point->y + vector->y;

   Point2D* movedPoint = new Point2D(movedX, movedY);

   return movedPoint;
}





























