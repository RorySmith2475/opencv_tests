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
    cv::Mat resized_image;
    int num_pos = 0;
    int num_neg = 0;

    //gets the name of each file and stores it in file_names_pos
    std::vector<cv::String> file_names_pos;
    cv::glob("pos/*", file_names_pos, true);

    std::ofstream pos_list("pos.info");
    for(int i=0; i<file_names_pos.size(); ++i) {

        image = cv::imread(file_names_pos[i]);

        if(!image.empty()) {

            cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

            //saves formatted image in pos-formatted
            std::string name = "pos-formatted/pos-" + std::to_string(num_pos) + ".jpg";
            cv::imwrite(name, resized_image);

            //write file name to pos-formatted.txt
            pos_list << name << " 1 0 0 " << std::to_string(image.cols) << " " << std::to_string(image.rows) << std::endl;

            num_pos++;
        }
    }

    //gets the name of each file and stores it in file_names_neg
    std::vector<cv::String> file_names_neg;
    cv::glob("neg/*", file_names_neg, true);

    std::ofstream neg_list("neg.txt");
    for(int i=0; i<file_names_neg.size(); ++i) {

        image = cv::imread(file_names_neg[i]);

        if(!image.empty()/* && image.cols > min_w && image.rows > min_h*/) {

            cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

            //saves formatted image in neg-formatted
            std::string name = "neg-formatted/neg-" + std::to_string(num_neg) + ".jpg";
            cv::imwrite(name, image);

            //write file name to neg-formatted.txt
            neg_list << name << std::endl;

            num_neg++;
        }
    }
    std::cout << "Number of positive samples: " << num_pos << std::endl
              << "Number of negative samples: " << num_neg << std::endl;
}


//g++ -std=c++11 format_detection_data.cpp -o format_detection_data `pkg-config opencv --cflags --libs`