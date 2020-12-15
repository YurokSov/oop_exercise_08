#include "figure.hpp"

Rectangle::Rectangle(const Point2D& a, const Point2D& b) {
    this->vertices.assign({ a,b });
    this->name = "rectangle";
    if (DEBUG) std::cout << "constructor called for Rectangle with vertices: "
        << vertices.at(0) << ' '
        << vertices.at(1) << std::endl;
}
Rectangle::~Rectangle() {
    if (DEBUG) std::cout << "destructor called for Rectangle with vertices: "
        << vertices.at(0) << ' '
        << vertices.at(1) << std::endl;
}

void Rectangle::Read(std::istream& is) {
    this->vertices.resize(2);
    Point2D center;
    is >> center >> vertices[0];
    double alpha = M_PI;
    vertices[1].x = -sin(alpha) * (vertices[0].y - center.y) + cos(alpha) * (vertices[0].x - center.x) + center.x;
    vertices[1].y = cos(alpha) * (vertices[0].y - center.y) + sin(alpha) * (vertices[0].x - center.x) + center.y;
    if (is.fail()) {
        std::cerr << "Bad input!" << std::endl;
    }
}

[[nodiscard]] Point2D Rectangle::GetCentroid() const {
    return Point2D(
        (vertices[0].x + vertices[1].x) / 2,
        (vertices[0].y + vertices[1].y) / 2);
}

[[nodiscard]] double Rectangle::GetArea() const {
    return abs((vertices[0].x - vertices[1].x) * (vertices[0].y - vertices[1].y));
}