#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <stdio.h>
#include <iostream>
#include <cmath>

cv::Point filter(std::vector<cv::Point> &prev_points, cv::Point point, int index);
int standardDeviation(std::vector<cv::Point> prev_points, cv::Point mean);

struct point_data {
    cv::Point point;
    int n;
};

/**
 * @brief   Tracks faces 
 */
int main(int argc, char** argv)
{
    cv::Mat frame;

    cv::VideoCapture src(0);
    if(!src.isOpened()) return -1;

    cv::CascadeClassifier object_cascade;
    object_cascade.load("lbpcascade_frontalface.xml");

    std::vector<cv::Point> prev_points;
    cv::Point prev_center;
    int index = 0;
    bool found_object = false;

    while(src.read(frame))
    {
        if(frame.empty()) {
            std::cout << "Frame Empty" << std::endl;
            continue;
        }
        cv::Mat frame_gray;
        cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(frame_gray, frame_gray);

        std::vector<cv::Rect> object;
        object_cascade.detectMultiScale(frame_gray, object, 1.1, 2, 0|cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

        if(!object.empty()) {
            cv::Point point(object[0].x + object[0].width/2, object[0].y + object[0].height/2);
            cv::Point center_mean = filter(prev_points, point, index);
            cv::circle(frame, center_mean, 50, cv::Scalar(255,255,255), 2, 8, 0);
        }
        else {
            prev_points.at(index).x = 0;
            prev_points.at(index).y = 0;
            }
        index = (index + 1) % 10;

        cv::imshow("Display Window", frame);
        cv::waitKey(15);
    }
    return 0;
}

cv::Point filter(std::vector<cv::Point> &prev_points, cv::Point point, int index)
{
    int memory_size = 10;
    int max_sd = 35;
    int curr_size = prev_points.size();

    if(curr_size >= memory_size) prev_points.at(index) = point;
    else {
        prev_points.push_back(point);
        point.x = 0;
        point.y = 0;
        return point;
    }

    cv::Point mean(0, 0);
    for(int i=0; i<curr_size; i++)
    {
        mean.x += prev_points.at(i).x;
        mean.y += prev_points.at(i).y;
    }
    mean.x = mean.x / memory_size;
    mean.y = mean.y / memory_size;

    int sd = standardDeviation(prev_points, mean);
    std::cout << "\r" << sd << std::endl;
    fflush(stdout);

    if(sd > max_sd) {
        point.x = 0;
        point.y = 0;
    }
    return point;
}

int standardDeviation(std::vector<cv::Point> prev_points, cv::Point mean)
{
    int n = prev_points.size();
    int sd = 0;

    for(int i=0; i<n; i++)
    {
        sd += sqrt(((mean.x - prev_points.at(i).x) * (mean.x - prev_points.at(i).x))
              + ((mean.y - prev_points.at(i).y) * (mean.y - prev_points.at(i).y)));
    }
    sd = (int)((float)sd * (1.0/(float)n));
    return sd;
}






//opencv_createsamples -info pos.info -num 83 -w 24 -h 24 -vec pos.vec
//opencv_annotation --annotations=./pos.info --images=./pos-formatted/
//opencv_traincascade -data data -vec pos.vec  -bg neg.txt  -numPos 83 -numNeg 391 -numStages 10 -w 24 -h 24 -featureType LBP