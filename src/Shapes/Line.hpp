#include "Shape.hpp"
//int CLAMP(double val, int lb, int ub);
class MyLine final : public Shape
{
public:
    int maxwidth;
    int maxheight;
    std::array<Point, 3> pts;
    std::array<Point, 3> prevpts;
    std::array<Point, 100> polypoints;
    std::array<Point, 100> prevpoly;

    Scalar prevcl;
    std::mt19937_64 gen;

    std::normal_distribution<> d;

    void draw(Mat &image);
    void undo();
    void mutate();
    // void optimize();
    void randomize();
    double value();
    inline void updateColor();
    void eval();
    MyLine(std::random_device &dev, const Mat &target, Mat &current, Mat *tint) : Shape(tint)
    {

        static thread_local std::mt19937_64 gen{dev()};
                this->gen = gen;
        std::normal_distribution<> nor(0, 16);
    std::uniform_int_distribution<> newcoordx(0, this->maxwidth);
    std::uniform_int_distribution<> newcoordy(0, this->maxheight);
    
        Mat current2;
        current.copyTo(current2);
        this->target = target;
        this->current = current;
        this->d = nor;

        auto shape = target.size;
        this->maxwidth = shape[1];
        this->maxheight = shape[0];
        std::uniform_int_distribution<> next(-16, 17);
        auto p1x = newcoordx(this->gen);
        auto p2x = newcoordx(this->gen);
        this->pts[0] = Point(p1x, p2x);
        for (int k = 1; k < 3; k++)
        {
            this->pts[k] = Point(CLAMP(this->pts[0].x + next(this->gen), 0, this->maxwidth-1), CLAMP(this->pts[0].y + next(this->gen), 0, this->maxheight-1));
        }
        this->eval();
        this->draw(current2);
        this->fitness = norm(target, current2, NORM_L2);
    };
};