#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include <stdio.h>
#include <iostream>

using namespace cv;

int main(int argc, char** argv)
{
    int threshold = 40;
    int ratio = 2;
    int kernel_size = 3;

    Mat edges;
    Mat color_iso;

    //VideoCapture vid("test_tape.mp4");
    VideoCapture src(0);
    if(!src.isOpened()) return -1;

    Mat frame;
    while(src.read(frame)) {
        src >> frame;
        imshow("frame", frame);

        cvtColor(frame, edges, COLOR_BGR2GRAY);
        blur(edges, edges, Size(5,5));
        Canny(edges, edges, threshold, threshold*ratio, kernel_size);
        imshow("edges", edges);

        inRange(frame, Scalar(0, 0, 0), Scalar(35, 35, 255), color_iso);
        imshow("color_iso", color_iso);

        waitKey(30);
    }

    src.release();
    return 0;
}

//g++ test.cpp -o test `pkg-config opencv --cflags --libs` 