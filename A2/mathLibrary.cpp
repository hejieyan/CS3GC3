#include <stdio.h>
#include <math.h>
#include "mathLibrary.h"

// MARK: Point2D Methods

Point2D::Point2D() {
    x = 0.0f;
    y = 0.0f;
}

Point2D::Point2D(int x, int y) {
    this->x = x;
    this->y = y;
}

void Point2D::printValue() {
    printf("x = %f, y = %f \n", x, y);
}

int Point2D::getX() {
    return this->x;
}

int Point2D::getY() {
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

// MARK: 3D MATH LIBRARY
//3D point class
//constructors
Point3D::Point3D(){
  this->x = 0.0;
  this->y = 0.0;
  this->z = 0.0;
}

Point3D::Point3D(float x, float y, float z){
  this->x = x;
  this->y = y;
  this->z = z;
}

//calculate the length of this vector and update its length variable
float Vector3D::calculateLength(){
  this->length = sqrt(pow(this->x,2)+pow(this->y,2)+pow(this->z,2));
  return this->length;
}

//constructors
Vector3D::Vector3D(){
  this->x = 0.0;
  this->y = 0.0;
  this->z = 0.0;
  this->length = 0;
}

Vector3D::Vector3D(float x, float y, float z){
  this->x = x;
  this->y = y;
  this->z = z;
  this->calculateLength();
}

//normalize this vector and update its length
void Vector3D::normalize(){
  this->x = this->x/this->length;
  this->y = this->y/this->length;
  this->z = this->z/this->length;
  this->calculateLength();
}

//add vector vector to this vector
void Vector3D::add(Vector3D* vector){
  this->x += vector->x;
  this->y += vector->y;
  this->z += vector->z;
  this->calculateLength();
}

//cross this vector with point1 given vector and return the resultant vector
Vector3D* Vector3D::crossProduct(Vector3D* vector){
  Vector3D* crossed = new Vector3D((this->y*vector->z - this->z*vector->y), (this->z*vector->x - this->x*vector->z), (this->x*vector->y - this->y*vector->x));
  return crossed;
}

//calculate the vector between points point1 and point2 and return the corresponding vector object
Vector3D* vBetween(Point3D* point1, Point3D* point2){
  Vector3D* vector = new Vector3D(point2->x-point1->x, point2->y-point1->y, point2->z-point1->z);
  return vector;
}


void setLighting(Light light, int lightNumber){
    glLightfv(GL_LIGHT0 + lightNumber, GL_POSITION, light.position);
    glLightfv(GL_LIGHT0 + lightNumber, GL_AMBIENT, light.ambient);
    glLightfv(GL_LIGHT0 + lightNumber, GL_DIFFUSE, light.diffuse);
    glLightfv(GL_LIGHT0 + lightNumber, GL_SPECULAR, light.specular);
}

void setMaterial(Material material){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material.emission);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.shine);
}




























