#include "figure.hpp"

Square::Square(const Point2D& a, const Point2D& b) {
    this->vertices.assign({ a,b });
    this->name = "square";
    if (DEBUG) std::cout << "constructor called for " << this->name << " with vertices: "
        << vertices.at(0) << ' '
        << vertices.at(1) << std::endl;
}
Square::~Square() {
    if (DEBUG) std::cout << "destructor called for " << this->name << " with vertices: "
        << vertices.at(0) << ' '
        << vertices.at(1) << std::endl;
}

void Square::Read(std::istream& is) {
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

[[nodiscard]] Point2D Square::GetCentroid() const {
    return Point2D(
        (vertices[0].x + vertices[1].x) / 2,
        (vertices[0].y + vertices[1].y) / 2);
}

[[nodiscard]] double Square::GetArea() const {
    return abs((vertices[0].x - vertices[1].x) * (vertices[0].y - vertices[1].y));
}