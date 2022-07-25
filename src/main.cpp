
#include "opencv4/opencv2/opencv.hpp"
#include "Shape.hpp"
#include <pagmo/problem.hpp>
#include <pagmo/types.hpp>
#include <pagmo/algorithm.hpp>

#include <pagmo/algorithms/simulated_annealing.hpp>
#include <pagmo/algorithms/bee_colony.hpp>
#include <pagmo/algorithms/pso.hpp>
#include <pagmo/algorithms/sade.hpp>
#include <pagmo/algorithms/gaco.hpp>

#include <pagmo/archipelago.hpp>
#include <pagmo/population.hpp>
#include <cmath>
#include <initializer_list>
#include <pagmo/archipelago.hpp>
#include <iostream>
#include <cstdlib>
#include <utility>
using namespace cv;
using namespace pagmo;
struct problem_v0
{
    Mat target;
    Mat current;
    vector_double fitness(const vector_double &dv) const
    {

        Mat current2;
        Mat overlay;
        double alpha = 0.5;
        Point2d P1(dv[0], dv[1]);
        Point2d P2(dv[2], dv[3]);
        Mat selection(target, Rect(P1, P2));
                Scalar col(mean(selection));
        //Scalar col(dv[4], dv[5], dv[6]);
        current.copyTo(current2);
        current.copyTo(overlay);
        rectangle(overlay, P1, P2, col, FILLED);
        addWeighted(overlay, alpha, current2, 1 - alpha, 0, current2);
        return {norm(target - current2)};
    }
    std::pair<vector_double, vector_double> get_bounds() const
    {
        auto a = target.size;
        double x = a[0];
        double y = a[1];
        return {{0., 0., 0., 0.}, {x, y, x, y}};
    }
};

int main()
{
    std::random_device rd{};
    std::mt19937 gen{rd()};
 
    double alpha = 0.5;
    Mat image, image2;
    image = imread("../testtest.png");
    image.copyTo(image2);
    namedWindow("Display Image", WINDOW_AUTOSIZE);
    Rectangle rec;
    auto a = image.size;
    std::cout << a[0] << std::endl;
    rec.br = Point2f(255, 255);
    rec.tl = Point2f(0, 0);
    rec.cl = Scalar(155);
    auto avg = mean(image);
    image2.setTo(Scalar(avg));
    double currerr = 1e10;
    algorithm algo;
    for (int j = 0; j < 10; j++)
    {
        problem p{problem_v0{image, image2}};
        /*if(j<10)
         algo = bee_colony();
        else*/
         algo=simulated_annealing();
        //algo = gaco(2);
        double currerrtemp = 1e10;

        // 4 - Evolve the population
        archipelago archi{10u, algo, p, 30u};

        // 4 - Run the evolution in parallel on the 16 separate islands 10 times.

        archi.evolve(1);
        archi.wait_check();

        for (int k = 0; k < 10; k++)
        {

            auto best = archi.get_champions_x()[k];
            auto bestval = archi.get_champions_f()[k][0];
            if (true)
            { // bestval<currerr){
                std::cout << "best value number " << k << ":" << archi.get_champions_f()[k][0] << std::endl;
                Point2d P1(best[0], best[1]);
                Point2d P2(best[2], best[3]);
                Mat selection(image, Rect(P1, P2));
                Mat overlay;
                image2.copyTo(overlay);
                    avg = mean(selection);
                //avg = Scalar(best[4], best[5], best[6]);
                rectangle(overlay, Point2d(best[0], best[1]), Point2d(best[2], best[3]), Scalar(avg), FILLED);
                addWeighted(overlay, alpha, image2, 1 - alpha, 0, image2);
                // 5 - Output the population
                currerr = bestval;
            }
        }
    }
    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", image2);
    waitKey(0);
    std::cout << norm(image - image2);
    return 0;
}