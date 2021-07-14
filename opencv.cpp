#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

//opencv安装：https://blog.csdn.net/cocoaqin/article/details/78163171
//编译： g++ opencv.cpp -o test `pkg-config --cflags --libs opencv`

using namespace std;
using namespace cv;

int main(int argc, char const *argv[])
{
    IplImage* img = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
    cvNamedWindow("Image_show", CV_WINDOW_AUTOSIZE);
    cvShowImage("Image_show", img);
    cvWaitKey(0);
    cvReleaseImage(&img);
    cvDestroyWindow("Image_show");

    return 0;
}
