#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <string>
#include <iostream>
#include <fstream>

/**
 * @brief takes pictures, formates them to find edges,
 *        and then saves them along with a .txt file describing
 *        their file location and name. 
 */
int main(int argc, char** argv)
{
    int threshold = 40;
    int ratio = 2;
    int kernel_size = 3;

    cv::Mat image;

    //gets the name of each file and stores it in file_names_neg
    std::vector<cv::String> file_names_neg;
    cv::glob("neg/*.pgm", file_names_neg, true);

    std::ofstream neg_list("neg-formatted.txt");
    for(int i=0; i<file_names_neg.size(); ++i) {
        image = cv::imread(file_names_neg[i]);
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

    //gets the name of each file and stores it in file_names_pos
    std::vector<cv::String> file_names_pos;
    cv::glob("pos/*.pgm", file_names_pos, true);

    std::ofstream pos_list("pos-formatted.txt");
    for(int i=0; i<file_names_pos.size(); ++i) {
        image = cv::imread(file_names_pos[i]);
        if(!image.empty()) {
            cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
            cv::blur(image, image, cv::Size(5,5));
            cv::Canny(image, image, threshold, threshold*ratio, kernel_size);

            //same formatted image in pos-formatted
            std::string name = "pos-formatted/neg-" + std::to_string(i) + ".jpg " + "1 0 0 "
                                + std::to_string(image.cols) + " " + std::to_string(image.rows);
            cv::imwrite(name, image);

            //write file name to pos-formatted.txt
            pos_list << name << std::endl;

        }
    }
}

//g++ -std=c++11 find_edges.cpp -o find_edges `pkg-config opencv --cflags --libs` 