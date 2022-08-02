#include "Shape.hpp"

class Ellipse final : public Shape
{
public:
    int maxwidth;
    int maxheight;
    Point2f center;
    Size axes;
    Point2f prevcenter;
    Size prevaxes;

    Scalar prevcl;
    std::normal_distribution<> d;
    std::mt19937_64 gen;

    void draw(Mat &image);
    void undo();
    void mutate();
    // void optimize();
    void randomize();
    double value();
    inline void updateColor();
    Ellipse(std::random_device &dev, const Mat &target, Mat &current, Mat *tint) : Shape(tint)
    {

        static thread_local std::mt19937_64 gen{dev()};
        std::normal_distribution<> nor(0, 16);
    std::uniform_int_distribution<> newcoordx(0, this->maxwidth);
    std::uniform_int_distribution<> newcoordy(0, this->maxheight);
    
        std::uniform_int_distribution<> axsize(1, 32);
        Mat current2;
        current.copyTo(current2);
        this->target = target;
        this->current = current;
        this->d = nor;
        this->gen = gen;
        auto shape = target.size;
        this->maxwidth = shape[1];
        this->maxheight = shape[0];
        this->center = Point2f(newcoordx(gen), newcoordy(gen));
        this->axes = Size(axsize(gen), axsize(gen));
        this->draw(current2);
        this->fitness = norm(target, current2, NORM_L2);
    };
};