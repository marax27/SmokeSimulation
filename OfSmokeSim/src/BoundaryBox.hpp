#pragma once

#include "ofMain.h"

class BoundaryBox {
public:
    BoundaryBox() = default;
    BoundaryBox(double width, double height, double depth, double x = 0, double y = 0, double z = 0);

    void draw();

private:
    double width, height, depth;
    double x, y, z;
    ofVboMesh mesh;
};