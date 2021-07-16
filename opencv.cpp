#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

//opencv安装：https://blog.csdn.net/cocoaqin/article/details/78163171
//编译： g++ opencv.cpp -o test `pkg-config --cflags --libs opencv`

using namespace std;
using namespace cv;

/* 车牌提取 */
int plate_pickup(const char *name, Mat *plate)
{
	Mat image, img, img_gauss, img_gray, img_sobel_x, img_abs_x;
	Mat kernelX, kernelY;
	Mat img_contours;
	vector<vector<Point> > contours;
    vector<Vec4i> vec_4f;

	img = imread(name);							   //读取图片
	image = img;
	GaussianBlur(img, img_gauss, Size(3, 3), 1);   //高斯处理
	cvtColor(img_gauss, img_gray, COLOR_RGB2GRAY); //灰度处理

	/* 边缘处理 */
	Sobel(img_gray, img_sobel_x, CV_16S, 1, 0);
	convertScaleAbs(img_sobel_x, img_abs_x);

	threshold(img_abs_x, img, 0, 255, THRESH_OTSU); //二值化

	/* 腐蚀和膨胀 */
	//先腐蚀后膨胀叫开运算，常用于去除噪声
	//先膨胀后腐蚀叫闭运算，常用于填充前景物体中的小洞，或者前景物体上的小黑点.
	kernelX = getStructuringElement(MORPH_RECT, Size(25,5));
    morphologyEx(img, img, MORPH_CLOSE, kernelX);
	kernelX = getStructuringElement(MORPH_RECT, Size(10,1));
    morphologyEx(img, img, MORPH_OPEN, kernelX); 
	kernelY = getStructuringElement(MORPH_RECT, Size(1,19));
	morphologyEx(img, img, MORPH_OPEN, kernelY);

	medianBlur(img, img, 15); //中值滤波

	findContours(img, contours, vec_4f, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //查找轮廓
	//drawContours(image, contours, -1, Scalar(0,0,255), 2); //绘制轮廓

	/* 遍历所有轮廓，判断是否符合条件 */
	int i;
	Rect rect;
	for (i = 0; i < contours.size(); i++)
	{
		rect = boundingRect(contours[i]);   //为该轮廓绘制最小矩形
		if ((rect.width > (rect.height * 2.2)) && (rect.width < (rect.height * 4.2))) //判定为车牌
		{
			//printf("%d: %d, %d, %d, %d\n", i, rect.x, rect.y, rect.width, rect.height);
			rectangle(image, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), Scalar(0, 255, 0), 2);
			*plate = image(rect);
			//imshow("plate", *plate);
			return 0;
		}
	}

	return -1;
}

/* 车牌文字识别 */
int text_pickup(Mat *plate)
{
	Mat img;

	Canny(*plate, img, 450, 120, 3);
	threshold(img, *plate, 50, 255, CV_THRESH_BINARY);

	imshow("plate", *plate);
}

int main(int argc, char const *argv[])
{
	Mat plate;
	int key;

	plate_pickup(argv[1], &plate);
	text_pickup(&plate);

	do
	{
		key = waitKey(0);
	} while (key <= 0);
	

	return 0;
}
