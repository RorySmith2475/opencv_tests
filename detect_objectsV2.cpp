#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <cmath>
#include <chrono>
//#include <ctime>

struct point_data {
    cv::Point p;
    int n;
};

cv::Point find_point(std::vector<point_data> points);
void add_point(std::vector<point_data> &points, cv::Point point, int radius);
void remove_point(std::vector<point_data> &points);
void print_vector(std::vector<point_data> points);


int main(int argc, char** argv)
{
    cv::Mat frame;
    std::vector<point_data> points;
    int radius = 100;
    int reset_time = 50;

    cv::VideoCapture src(0);
    if(!src.isOpened()) return -1;

    cv::CascadeClassifier object_cascade;
    object_cascade.load("lbpcascade_frontalface.xml");

    auto start = std::chrono::steady_clock::now();
    while(src.read(frame))
    {
        cv::Mat frame_gray;
        cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(frame_gray, frame_gray);

        std::vector<cv::Rect> object;
        object_cascade.detectMultiScale(frame_gray, object, 1.1, 2, 0|cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

        if(!object.empty()) {
            cv::Point curr_point(object[0].x + object[0].width/2, object[0].y + object[0].height/2);
            add_point(points, curr_point, radius);
            cv::Point best_point = find_point(points);

            cv::circle(frame, best_point, 50, cv::Scalar(255,255,255), 2, 8, 0);
        }
        if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count()
           >= reset_time) {
               remove_point(points);
               start = std::chrono::steady_clock::now();
           }

        print_vector(points);
        cv::imshow("Window", frame);
        cv::waitKey(10);
    }
    return 0;
}

cv::Point find_point(std::vector<point_data> points)
{
    int max = 0;
    int max_index = 0;
    for(int i=0; i<points.size(); i++)
    {
        if(points.at(i).n > max) {
            max = points.at(i).n;
            max_index = i;
        }
    }
    if(max > 3) return points.at(max_index).p;
    return {0,0};
}

void add_point(std::vector<point_data> &points, cv::Point point, int radius)
{
    for(int i=0; i<points.size(); i++)
    {
        if(sqrt(pow(point.x - points.at(i).p.x, 2) + pow(point.y - points.at(i).p.y, 2)) <= radius) {
            points.at(i).p.x = point.x;
            points.at(i).p.y = point.y;
            points.at(i).n = (points.at(i).n < 10) ? points.at(i).n + 1 : 10;
            return;
        }
    }
    points.push_back({point, 1});
}

void remove_point(std::vector<point_data> &points)
{
    for(int i=0; i<points.size(); i++)
    {
        points.at(i).n = points.at(i).n - 1;
        if(points.at(i).n == 0) {
            points.erase(points.begin() + i);
            i--;
        }
    }
}

void print_vector(std::vector<point_data> points)
{
    for(int i=0; i<points.size(); i++)
    {
        std::cout << "point: " << points.at(i).p.x << "," << points.at(i).p.y
                  << " num: " << points.at(i).n << std::endl;
    }
    std::cout << "\n\n";
}