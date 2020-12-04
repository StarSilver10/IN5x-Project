#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main()
{
	Mat img = imread("Canarticho.png");
	namedWindow("image", WINDOW_NORMAL);
	imshow("image", img);
	waitKey(0);
	return 0;
}