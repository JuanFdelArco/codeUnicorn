// Practica_PIC.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "opencv2/opencv.hpp"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>


using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	//DISPLAY CAMARA
	VideoCapture cap;
	//CvCapture *capture;
	//capture = cvCaptureFromCAM(0);
	// open the default camera, use something different from 0 otherwise;
	// Check VideoCapture documentation.
	RNG rng(12345);

	Mat img, img_roi, img_gray, img_threshold;
	Rect roi(140, 100, 270, 270);
	vector<vector<Point>> contornos;
	vector<Vec4i> hierarchy;

	if (!cap.open(0))
		return 0;
	
	for (;;)
	{
	//	Mat frame;
	//	cap >> frame; //frame=video real
	//	//RGB TO GRAY
	//	Mat frame_HSV;
	//	cvtColor(frame, frame_HSV, CV_BGR2GRAY);
		//imshow("HSV", frame_HSV);
	//	Mat frame_BLUR;
	//	blur(frame_HSV, frame_BLUR, Size(3, 3));
		//imshow("BLUR", frame_BLUR);
	//	Mat frame_BINARIA;
	//	threshold(frame_BLUR, frame_BINARIA, 100, 255, CV_THRESH_BINARY+CV_THRESH_OTSU);
		//imshow("BINARIA", frame_BINARIA);

		

		namedWindow("Original_image", CV_WINDOW_AUTOSIZE);
		namedWindow("Gray_image", CV_WINDOW_AUTOSIZE);
		namedWindow("Thresholded_image", CV_WINDOW_AUTOSIZE);
		namedWindow("ROI", CV_WINDOW_AUTOSIZE);
		
		cap >> img;
		img_roi = img(roi);
		cvtColor(img_roi, img_gray, CV_RGB2GRAY);
		//GaussianBlur(img_gray, img_gray, Size(7, 7), 1.0, 1.5);
		Canny(img_gray, img_threshold, 100, 200, 3);
		//threshold(img_gray, img_threshold, 0, 255, THRESH_BINARY_INV + THRESH_OTSU);

		imshow("Original_image", img);
		imshow("ROI", img_roi);
		imshow("Gray_image", img_gray);
		imshow("Thresholded_image", img_threshold);
		
		//Encontrar contornos	
		
		findContours(img_threshold, contornos, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point());
		//findContours(img_threshold, contornos, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE); //CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

		Mat drawing = img_threshold.clone(); //Mat::zeros(img_threshold.size(), CV_8UC3);
		for (int i = 0; i< contornos.size(); i++)
		{
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(drawing, contornos, i, color, 2, 8, hierarchy, 0, Point());
			//drawContours(drawing, contornos, i, color);
		}
		//cout << hierarchy.size()<< endl;


		/// Show in a window
		namedWindow("Contours", WINDOW_AUTOSIZE);
		imshow("Contours", drawing);

		contornos.erase(contornos.begin(), contornos.end());
		hierarchy.erase(hierarchy.begin(), hierarchy.end());

		if (waitKey(1) == 27) break;
	}
	return 0;

}