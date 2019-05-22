#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include <stdio.h>
#include <iostream>

//using namespace cv;

int main(int argc, char** argv)
{
    int threshold = 40;
    int ratio = 2;
    int kernel_size = 3;

    cv::Mat edges;
    cv::Mat color_iso;

    cv::VideoCapture src(0);
    if(!src.isOpened()) return -1;

    cv::Mat frame;
    while(src.read(frame)) {
        //src >> frame;
        cv::imshow("frame", frame);

        cv::cvtColor(frame, edges, cv::COLOR_BGR2GRAY);
        cv::blur(edges, edges, cv::Size(5,5));
        cv::Canny(edges, edges, threshold, threshold*ratio, kernel_size);
        cv::imshow("edges", edges);

        cv::inRange(frame, cv::Scalar(0, 0, 0), cv::Scalar(35, 35, 255), color_iso);
        cv::imshow("color_iso", color_iso);

        cv::waitKey(30);
    }

    src.release();
    return 0;
}

//g++ test.cpp -o test `pkg-config opencv --cflags --libs` 