#include "stdafx.h"
#include <iostream>
#include <opencv\cv.hpp>
#include <vector>

using namespace std;
using namespace cv;

Mat image;

int main()
{
	VideoCapture cap(0);
	Mat frame, grey_img, blur_img, bin_img, canny_img;
	Rect roi(140, 100, 200, 200);
	vector<vector<Point>> contornos;

	for (;;)
	{		
		cap >> frame;

		if (frame.empty())
			break;

		image.create(frame.rows,frame.cols,CV_8UC1);
		image = frame(roi);
		cvtColor(image, grey_img, CV_BGR2GRAY);
		blur(grey_img, blur_img, Size(3, 3));
		threshold(blur_img, bin_img, 0, 255, THRESH_BINARY_INV + THRESH_OTSU);
		Canny(bin_img, canny_img, 50, 100, 3);

		//findContours(canny_img,contornos,CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0)); //Voy a destruir al p*** creador de esta funci¨®n

		imshow("Demo", canny_img);

		char c = (char)waitKey(10);
		if (c == 27)
			break;		
	}	
	return 0;
}
