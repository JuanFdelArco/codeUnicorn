#include "stdafx.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>


using namespace cv;
using namespace std;


int main(int argc, const char** argv)
{
	VideoCapture cam(0);
	if (!cam.isOpened()) {
		cout << "ERROR not opened " << endl;
		return -1;
	}
	Mat img;
	Mat img_threshold;
	Mat img_gray;
	Mat img_roi;
	Mat capturada;

//	namedWindow("Original_image", CV_WINDOW_AUTOSIZE);
//	namedWindow("Gray_image", CV_WINDOW_AUTOSIZE);
//	namedWindow("Thresholded_image", CV_WINDOW_AUTOSIZE);
//	namedWindow("ROI", CV_WINDOW_AUTOSIZE);

	char a[40];
	int count = 0;
	CvPoint pt1, pt2;

	while (1) {
		bool b = cam.read(img);
		if (!b) {
			cout << "ERROR : cannot read" << endl;
			return -1;
		}
		Rect roi(340, 100, 270, 270);
		rectangle(img, roi, 2, 10, 8);
		imshow("Original_image", img);
		img_roi = img(roi);


		if (waitKey(30) == 27) {
			capturada = img_roi.clone();
			cvtColor(capturada, img_gray, CV_RGB2GRAY);
			GaussianBlur(img_gray, img_gray, Size(19, 19), 0.0, 0);
			threshold(img_gray, img_threshold, 127, 255, THRESH_BINARY_INV + THRESH_OTSU);
			//Canny(img_gray, img_threshold, 100, 200, 3);

			
			//Valor de intensidad de los pixeles
			//Esto estaba en el programa anterior, funciona, no sé si servirá de algo
			float cx = 0.0, cy = 0.0;
			float sumi = 1.0;
			for (int i = 0; i < img_threshold.rows; i++) {
				for (int j = 0; j < img_threshold.cols; j++) {
					cy += img_threshold.at<uchar>(i, j) * i;
					cx += img_threshold.at<uchar>(i, j) * j;
					sumi += img_threshold.at<uchar>(i, j);
				}
			}
			cx /= sumi;
			cy /= sumi;
			cout << "Sumi: " << sumi << endl;

			//Encontrar contornos
			vector<vector<Point> >contours;
			vector<Vec4i>hierarchy;
			Mat temp = img_threshold.clone();
			findContours(temp, contours, hierarchy, RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point());
			

			//Calculamos el mayor valor en el vector contours y su posición
			int c_size = 0, c_idx = 0;
			Mat drawing = capturada.clone();
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

			//Pasamos el mayor valor al vector boundary0 (es el rectángulo)
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
					circle(capturada, center, 25, Scalar(0, 255, 0));
					//imshow("Contornos", temp);
				}
			}
				/// Show in a window
			
				imshow("Contours", drawing);
				imshow("Resultado", capturada);

			}

		}
	return 0;

}