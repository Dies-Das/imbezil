
#include "Ellipse.hpp"
#include <cstdlib>
using namespace cv;
const double alpha = 0.7;

inline void Ellipse::updateColor()
{
     Mat mask = Mat::zeros(this->target.rows, this->target.cols, CV_8U);
    ellipse(mask, this->center, this->axes, 0, 0, 360, Scalar(1), FILLED);

    this->cl = mean(this->target, mask);
}
void Ellipse::draw(Mat &image)
{
    Mat overlay;
    image.copyTo(overlay);
    // this->cl = Scalar(255, 255, 255);
    ellipse(image, this->center, this->axes, 0, 0, 360, this->cl, FILLED);
    //            std::cout << "Color: " << this->cl << "\n";
    addWeighted(overlay, alpha, image, 1 - alpha, 0, image);
    /*            std::cout << "Color: " << this->cl << std::endl;
            std::cout << "center: " << this->center << std::endl;
            std::cout << "ax: " << this->axes << std::endl;
           */
}
void Ellipse::undo()
{
    this->center = this->prevcenter;
    this->axes = this->prevaxes;
    this->cl = this->prevcl;
}
void Ellipse::mutate()
{
    std::uniform_int_distribution<> newsize(1, 32);
    bool which;
    this->prevcenter = this->center;
    this->prevaxes = this->axes;
    this->prevcl = this->cl;
    which = rand() % 2;
    if (which)
    {
        this->center.x = CLAMP(this->d(this->gen) + this->center.x, 1, this->maxwidth - 1);
        this->center.y = CLAMP(this->d(this->gen) + this->center.y, 1, this->maxheight - 1);
    }
    else
    {
        this->axes = Size(newsize(gen), newsize(gen));
    }
    this->updateColor();
}

void Ellipse::randomize()
{
    Mat current2;
    current.copyTo(current2);
    std::uniform_int_distribution<> newcoordx(0, this->maxwidth);
    std::uniform_int_distribution<> newcoordy(0, this->maxheight);
    
    std::uniform_int_distribution<> newsize(1, 32);
    this->center = Point2f(newcoordx(gen), newcoordy(gen));
    this->axes = Size(newsize(gen), newsize(gen));
    this->updateColor();
    this->draw(current2);

    this->fitness = norm(target, current2, NORM_L2);
}