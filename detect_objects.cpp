#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <stdio.h>

using namespace cv;

/**
 * @brief   Tracks faces 
 *  
 */
int main(int argc, char** argv)
{
    Mat frame;

    VideoCapture src(0);
    if(!src.isOpened()) return -1;

    CascadeClassifier face_cascade;
    face_cascade.load("lbpcascade_frontalface.xml");

    while(src.read(frame)) {
        std::vector<Rect> face;
        Mat frame_gray;

        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
        equalizeHist(frame_gray, frame_gray);

        face_cascade.detectMultiScale(frame_gray, face, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30));
        for (size_t i = 0; i < face.size(); i++) {
            Point center(face[i].x + face[i].width/2, face[i].y + face[i].height/2);
            ellipse(frame, center, Size(face[i].width/2, face[i].height/2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
        }

        imshow("face", frame);
        waitKey(30);
    }
}