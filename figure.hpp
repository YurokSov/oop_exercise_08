#pragma once

#include <iostream>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef DEBUG
#define DEBUG false
#endif

class Point2D {
public:
    double x;
    double y;

    Point2D(double x = 0, double y = 0);
    friend std::ostream& operator<<(std::ostream& os, const Point2D& d);
    friend std::istream& operator>>(std::istream& is, Point2D& d);
};

class Figure {
public:
    static uint64_t counter;

    Figure();
    virtual ~Figure();

    [[nodiscard]] virtual Point2D GetCentroid() const = 0;
    [[nodiscard]] virtual double GetArea() const = 0;
    [[nodiscard]] const std::string& GetName() const;

    virtual void Write(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, Figure& figure);
    virtual void Read(std::istream& is) = 0;
    friend std::istream& operator>>(std::istream& is, Figure& mc);

    [[nodiscard]] const std::vector<Point2D>& GetVertices() const;

protected:
    std::vector<Point2D> vertices;
    std::string name;
private:
    const uint64_t id;
};

class Triangle : public Figure {
public:
    Triangle(const Point2D& a = Point2D(), const Point2D& b = Point2D(), const Point2D& c = Point2D());
    ~Triangle() override;
    void Read(std::istream& is) override;
    [[nodiscard]] Point2D GetCentroid() const override;
    [[nodiscard]] double GetArea() const override;

};

class Square : public Figure {
public:
    Square(const Point2D& a = Point2D(), const Point2D& b = Point2D());
    ~Square() override;
    void Read(std::istream& is) override;
    [[nodiscard]] Point2D GetCentroid() const override;
    [[nodiscard]] double GetArea() const override;

};

class Rectangle : public Figure {
public:
    Rectangle(const Point2D& a = Point2D(), const Point2D& b = Point2D());
    ~Rectangle() override;
    void Read(std::istream& is) override;
    [[nodiscard]] Point2D GetCentroid() const override;
    [[nodiscard]] double GetArea() const override;

};