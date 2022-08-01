#include "Shape.hpp"

class Triangle final : public Shape
{
public:
    int maxwidth;
    int maxheight;
    std::array<Point, 3> pts;
    std::array<Point, 3> prevpts;

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
    Triangle(std::random_device &dev, const Mat &target, Mat &current, Mat *tint) : Shape(tint)
    {

        static thread_local std::mt19937_64 gen{dev()};
        std::normal_distribution<> nor(0, 16);
    std::uniform_int_distribution<> newcoordx(0, this->maxwidth);
    std::uniform_int_distribution<> newcoordy(0, this->maxheight);
    
        Mat current2;
        current.copyTo(current2);
        this->target = target;
        this->current = current;
        this->d = nor;
        this->gen = gen;
        auto shape = target.size;
        this->maxwidth = shape[1];
        this->maxheight = shape[0];
        std::uniform_int_distribution<> next(-16, 17);
        this->pts[0] = Point(newcoordx(gen), newcoordy(gen));
        for (int k = 1; k < 3; k++)
        {
            this->pts[k] = Point(CLAMP(this->pts[0].x + next(gen), 0, this->maxwidth-1), CLAMP(this->pts[0].y + next(gen), 0, this->maxheight-1));
        }
        this->draw(current2);
        this->fitness = norm(target, current2, NORM_L2);
    };
};