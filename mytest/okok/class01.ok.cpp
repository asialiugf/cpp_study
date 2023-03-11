#include <iostream>
#include <sstream>
#include <string>
using namespace std;
class Object {
   protected:
    string mName;
    string mInfo;
    int serial;

   public:
    Object() {
        mName = "Object";
        mInfo = "";
    }
    string name() { return mName; }
    string info() { return mInfo; }
};
class Point : public Object {
   private:
    int mX;
    int mY;

   public:
    Point(int x = 0, int y = 0) {
        ostringstream s;
        mX = x;
        mY = y;
        mName = "Point";
        serial = 100;
        s << "P(" << mX << ", " << mY << ")";
        mInfo = s.str();
    }
    int x() { return mX; }
    int y() { return mY; }
};
class Line : public Object {
   private:
    Point mP1;
    Point mP2;

   public:
    Line(Point p1, Point p2) {
        ostringstream s;
        mP1 = p1;
        mP2 = p2;
        mName = "Line";
        s << "Line from " << mP1.info() << " to " << mP2.info();
        mInfo = s.str();
    }
    Point begin() { return mP1; }
    Point end() { return mP2; }
};
int main() {
    Object o;
    Point p(1, 2);
    Point pn(5, 6);
    Line l(p, pn);

    std::cout << o.name() << std::endl;
    std::cout << o.info() << std::endl;
    std::cout << std::endl;
    std::cout << p.name() << std::endl;
    std::cout << p.info() << std::endl;
    std::cout << std::endl;
    std::cout << l.name() << std::endl;
    std::cout << l.info() << std::endl;

    return 0;
}
