#include "Shape.hpp"

class Ellipse final: public Shape{
    int maxwidth;
    int maxheight;
    Point2f center;
    Size axes;
    Point2f prevcenter;
    Size prevaxes;
    Scalar cl;
    Scalar prevcl;
    std::normal_distribution<> d;
    std::mt19937 gen;
    public:
    void draw( Mat  &image);
    void undo();
    void mutate();
    //void optimize();
    void randomize();
    double value();
    inline void updateColor();
    Ellipse( std::random_device &dev, const Mat & target, Mat &current,Mat * tint):Shape(tint){
        
            static thread_local std::mt19937  gen{dev()};
    std::normal_distribution<> nor(0, 5);
    std::uniform_int_distribution<> newcoord(0,256);
    std::uniform_int_distribution<> axsize(1, 4);
        Mat current2;
        current.copyTo(current2);
        this->target = target;
        this->current = current;
        this->d = nor;
        this->gen = gen;
        auto shape = target.size;
        this->maxwidth = shape[0];
        this->maxheight = shape[1];
        this->center = Point2f(newcoord(gen),newcoord(gen));
        std::cout << this->center << std::endl;
        this->axes = Size(axsize(gen),axsize(gen));
        this->draw(current2);
        this->fitness = norm(target, current2, NORM_L2);
        
    };


};