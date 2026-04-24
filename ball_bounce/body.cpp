#include "body.h"
#include <cmath>
#include <iostream>
#include <glad/glad.h>  
#include <GLFW/glfw3.h>

using namespace std;

Body::Body(double m, Point p) :mass(m), postion(p),redius(0.002+m/30000000) {

    glGenBuffers(1,&VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 200* sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);





};

void Body::applyGF(Body body,double duration) {
    
    double G = 1.1 * pow(10, -8);
    double dx = body.postion.x - postion.x;
    double dy = body.postion.y - postion.y;
    double r = sqrt(dx * dx + dy * dy + 0.00001);

    double F = G * body.mass * mass / (r * r);
    double a1 = F / mass;

    acc.x += a1 * (dx / r);
    acc.y += a1 * (dy / r);

    

};
void Body::updateRedius() {
    this->redius = 0.002 + (this->mass / 30000000.0);
}
void Body::updatePhysics(double duration) {
    speed.x += acc.x * duration;
    speed.y += acc.y * duration;
    postion.x += speed.x * duration;
    postion.y += speed.y * duration;

    acc = Point(0, 0);
}
