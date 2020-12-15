#include "figure.hpp"

Point2D::Point2D(double x, double y) : x(x), y(y) {};

std::ostream& operator<<(std::ostream& os, const Point2D& d) {
    os << "[x: " << d.x << " y: " << d.y << ']';
    return os;
}

std::istream& operator>>(std::istream& is, Point2D& d) {
    is >> d.x >> d.y;
    return is;
}

uint64_t Figure::counter = 0;

Figure::Figure() : id(counter++) {
    if (DEBUG) std::cout << "constructor called for Figure with ID of " << this->id << std::endl;
};
Figure::~Figure() {
    if (DEBUG) std::cout << "destructor called for Figure with ID of " << this->id << std::endl;
};

[[nodiscard]] const std::string& Figure::GetName() const {
    return this->name;
}

void Figure::Write(std::ostream& os) const {
    os << this->name << ':';
    for (auto vertex : this->vertices) {
        os << ' ' << vertex;
    }
}
std::ostream& operator<<(std::ostream& os, Figure& figure) {
    figure.Write(os);
    return os;
}

std::istream& operator>>(std::istream& is, Figure& mc) {
    mc.Read(is);
    return is;
}

[[nodiscard]] const std::vector<Point2D>& Figure::GetVertices() const {
    return this->vertices;
}