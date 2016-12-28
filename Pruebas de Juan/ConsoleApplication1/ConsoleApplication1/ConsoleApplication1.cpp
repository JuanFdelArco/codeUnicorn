#include "stdafx.h"
#include <iostream>

#include <opencv\cv.hpp>

using namespace std;
using namespace cv;

Mat image;

int main()
{
	VideoCapture cap(0);
	Mat frame,grey;

	for (;;)
	{		
		cap >> frame;
				
		if (frame.empty())
			break;

		frame.copyTo(image);
		cvtColor(image, grey, CV_BGR2GRAY);

		imshow("Demo", grey);
	
		char c = (char)waitKey(10);
		if (c == 27)
			break;		
	}	
	return 0;
}
