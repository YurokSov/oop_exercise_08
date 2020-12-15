#include "figure.hpp"

Triangle::Triangle(const Point2D& a, const Point2D& b, const Point2D& c) {
    this->vertices.assign({ a,b,c });
    this->name = "triangle";
    if (DEBUG) std::cout << "constructor called for " << this->name << " with vertices: "
        << vertices.at(0) << ' '
        << vertices.at(1) << ' '
        << vertices.at(2) << std::endl;
}
Triangle::~Triangle() {
    if (DEBUG) std::cout << "destructor called for " << this->name << " with vertices: "
        << vertices.at(0) << ' '
        << vertices.at(1) << ' '
        << vertices.at(2) << std::endl;
}

void Triangle::Read(std::istream& is) {
    this->vertices.resize(3);
    Point2D center;
    is >> center >> vertices[0];
    double alpha = 120.0 / 180.0 * M_PI;
    for (int i = 1; i < 3; ++i) {
        vertices[i].x = -sin(alpha) * (vertices[i - 1].y - center.y) + cos(alpha) * (vertices[i - 1].x - center.x) + center.x;
        vertices[i].y = cos(alpha) * (vertices[i - 1].y - center.y) + sin(alpha) * (vertices[i - 1].x - center.x) + center.y;
    }
    //is >> this->vertices[0] >> this->vertices[1] >> this->vertices[2];
    if (is.fail()) {
        std::cerr << "Bad input!" << std::endl;
    }
}

[[nodiscard]] Point2D Triangle::GetCentroid() const {
    return Point2D((vertices[0].x + vertices[1].x + vertices[2].x) / 3,
        (vertices[0].y + vertices[1].y + vertices[2].y) / 3);
}

[[nodiscard]] double Triangle::GetArea() const {
    double a = (vertices[0].x - vertices[2].x) * (vertices[1].y - vertices[2].y);
    double b = (vertices[0].y - vertices[2].y) * (vertices[1].x - vertices[2].x);
    double c = (a - b) / 2.0;
    if (c >= 0) {
        return c;
    }
    else {
        return -c;
    }
    // (0 - 0.866) * (-0.5 - -0.5) - (1 - -0.5) * (-0.866 - 0.866);
}