#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <string>
#include <iostream>
#include <fstream>

/**
 * @brief takes neg pictures, formates them to find edges,
 *        and then saves them along with a .txt file describing
 *        their file location and name.
 * 
 *          NOT FINISHED 
 * 
 */
int main(int argc, char** argv)
{
    int threshold = 40;
    int ratio = 2;
    int kernel_size = 3;

    cv::Mat image;
    cv::String path("neg/*.pgm");
    std::vector<cv::String> file_name;

    cv::glob(path, file_name, true);

    std::ofstream neg_list("neg-formatted.txt");

    for(int i=0; i<file_name.size(); ++i) {
        image = cv::imread(file_name[i]);
        if(!image.empty()) {
            cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
            cv::blur(image, image, cv::Size(5,5));
            cv::Canny(image, image, threshold, threshold*ratio, kernel_size);

            //same formatted image in neg-formatted
            std::string name = "neg-formatted/neg-" + std::to_string(i) + ".jpg";
            cv::imwrite(name, image);

            //write file name to neg-formatted.txt
            neg_list << name << std::endl;
        }
    }

}

//g++ -std=c++11 find_edges.cpp -o find_edges `pkg-config opencv --cflags --libs` 