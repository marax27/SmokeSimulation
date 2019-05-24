#include "BoundaryBox.hpp"

BoundaryBox::BoundaryBox(double width, double height, double depth, double x, double y, double z)
    : width(width), height(height), depth(depth), x(x), y(y), z(z) {
    mesh.setMode(OF_PRIMITIVE_LINES);

    mesh.addVertex(ofPoint(0, 0, 0)); // vertex 0
    mesh.addColor(ofColor(255, 255, 0));

    mesh.addVertex(ofPoint(width, 0, 0)); // vertex 1
    mesh.addColor(ofColor(255, 255, 0));

    mesh.addVertex(ofPoint(0, 0, depth)); // vertex 2
    mesh.addColor(ofColor(255, 255, 0));

    mesh.addVertex(ofPoint(width, 0, depth)); // vertex 3
    mesh.addColor(ofColor(255, 255, 0));

    mesh.addVertex(ofPoint(0, height, 0)); // vertex 4
    mesh.addColor(ofColor(255, 255, 0));

    mesh.addVertex(ofPoint(width, height, 0)); // vertex 5
    mesh.addColor(ofColor(255, 255, 0));

    mesh.addVertex(ofPoint(0, height, depth)); // vertex 6
    mesh.addColor(ofColor(255, 255, 0));

    mesh.addVertex(ofPoint(width, height, depth)); // vertex 7
    mesh.addColor(ofColor(255, 255, 0));

    mesh.addIndex(0); mesh.addIndex(1);
    mesh.addIndex(0); mesh.addIndex(2);
    mesh.addIndex(1); mesh.addIndex(3);
    mesh.addIndex(2); mesh.addIndex(3);

    mesh.addIndex(4); mesh.addIndex(5);
    mesh.addIndex(4); mesh.addIndex(6);
    mesh.addIndex(5); mesh.addIndex(7);
    mesh.addIndex(6); mesh.addIndex(7);

    mesh.addIndex(0); mesh.addIndex(4);
    mesh.addIndex(1); mesh.addIndex(5);
    mesh.addIndex(2); mesh.addIndex(6);
    mesh.addIndex(3); mesh.addIndex(7);
}

void BoundaryBox::draw() {
    mesh.draw();
}
