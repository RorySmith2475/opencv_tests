#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <stdio.h>

using namespace cv;

void detectAndDisplay(Mat frame);

CascadeClassifier circle_cascade;

int main( int argc, const char** argv ) {
    VideoCapture src(0);
    if(!src.isOpened()) return -1;

    circle_cascade.load("name of cascade file");

    Mat frame;
    while(src.read(frame)) {
        detectAndDisplay(frame);
        waitKey(30);
    }
}

void detectAndDisplay(Mat frame)
{
    std::vector<Rect> box;
    Mat frame_gray;

    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    circle_cascade.detectMultiScale(frame_gray, box, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(60, 60));
}