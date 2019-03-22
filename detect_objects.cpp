#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <stdio.h>
#include <iostream>

/**
 * @brief   Tracks faces 
 */
int main(int argc, char** argv)
{
    cv::Mat frame;

    cv::VideoCapture src(0);
    if(!src.isOpened()) return -1;

    cv::CascadeClassifier face_cascade;
    face_cascade.load("lbpcascade_frontalface.xml");

    while(src.read(frame)) {
        if(frame.empty()) {
            std::cout << "Frame Empty" << std::endl;
            continue;
        }
        std::vector<cv::Rect> face;
        cv::Mat frame_gray;

        cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(frame_gray, frame_gray);

        face_cascade.detectMultiScale(frame_gray, face, 1.1, 2, 0|cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

        //displays a circle around detected faces
        /*for (size_t i = 0; i < face.size(); i++) {
            cv::Point center(face[i].x + face[i].width/2, face[i].y + face[i].height/2);
            cv::ellipse(frame, center, cv::Size(face[i].width/2, face[i].height/2), 0, 0, 360, cv::Scalar(255, 255, 255), 1, 8, 0);
        }*/
        if(!face.empty()) {
            cv::Point center(frame.cols/2, frame.rows/2);
            cv::Point face_center(face[0].x + face[0].width/2, face[0].y + face[0].height/2);
            cv::line(frame, center, face_center, cv::Scalar(255,255,255), 2, 8, 0);
        }

        cv::imshow("Display Window", frame);
        cv::waitKey(15);
    }
}