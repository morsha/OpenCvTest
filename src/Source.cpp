#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;

int main()
{
	Mat a(300, 300, CV_8UC3, Scalar(0, 0, 0));

	imshow("dd", a);
	waitKey(0);

	//good!

	return 0;
}