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
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

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

		findContours(canny_img,contours,CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0)); //Voy a destruir al p*** creador de esta función

		int c_size = 0, c_idx = 0;
		Mat drawing = image.clone();
		for (int i = 0; i < contours.size(); i++) {
			cout << contours[i] << endl;
			if (contours.size() > c_size) { 
				c_idx = i;
				c_size = contours.size();
			}
			if (i != 0) {	//Dibujamos el contorno, quitamos el primer valor porque es el recuadro de la imagen
				drawContours(drawing, contours, i, (0, 0, 255), 2, 8, hierarchy, 0, Point());
			}
		}

		//Pasamos el mayor valor al vector boundary0 (es el rect醤gulo)
		vector<Point> boundary0;
		//Acumulamos el vector que se dibuja en una nueva variable
		vector<Point> boundary1;
		vector<Point> peaks_;
		if (!contours.empty()) {
			boundary0 = contours[c_idx]; //cnt
			boundary1 = contours[1];
			cout << boundary0 << endl;
		}

		//ESTO NO FUNCIONA
		//Aproximamos el contorno y lo dibujamos
		vector<Point> boundary;
		if (!boundary0.empty()) {
			approxPolyDP(boundary1, boundary, 2, false); //Aproxima el valor del contorno

			for (int i = 0; i < boundary.size(); i++) {
				cout << "Estoy dibujando" << endl;
				cout << (boundary[i] + Point(roi.x, roi.y)) << endl;
				Point center = boundary[i] + Point(roi.x, roi.y);
				circle(image, center, 25, Scalar(0, 255, 0));
				//imshow("Contornos", temp);
			}
		}
		/// Show in a window

		imshow("Contours", drawing);
		imshow("Resultado", image);



		//imshow("Demo", canny_img);

		char c = (char)waitKey(10);
		if (c == 27)
			break;		
	}	
	return 0;
}
