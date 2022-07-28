#include "Shape.hpp"
int clamp(double val, int lb, int ub);
class Triangle final : public Shape
{
public:
    int maxwidth;
    int maxheight;
    std::array<Point, 3> pts;
    std::array<Point, 3> prevpts;

    Scalar prevcl;
    std::normal_distribution<> d;
    std::mt19937 gen;

    void draw(Mat &image);
    void undo();
    void mutate();
    // void optimize();
    void randomize();
    double value();
    inline void updateColor();
    Triangle(std::random_device &dev, const Mat &target, Mat &current, Mat *tint) : Shape(tint)
    {

        static thread_local std::mt19937 gen{dev()};
        std::normal_distribution<> nor(0, 16);
        std::uniform_int_distribution<> newcoord(0, 255);
        Mat current2;
        current.copyTo(current2);
        this->target = target;
        this->current = current;
        this->d = nor;
        this->gen = gen;
        auto shape = target.size;
        this->maxwidth = shape[0];
        this->maxheight = shape[1];
        std::uniform_int_distribution<> next(-16, 17);
        this->pts[0] = Point(newcoord(gen), newcoord(gen));
        for (int k = 1; k < 3; k++)
        {
            this->pts[k] = Point(clamp(this->pts[0].x + next(gen), 0, 255), clamp(this->pts[0].y + next(gen), 0, 255));
        }
        this->draw(current2);
        this->fitness = norm(target, current2, NORM_L2);
    };
};