// ConsoleApplication2.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//

#include "pch.h"
#include <iostream>
#include "opencv2/opencv.hpp"


using namespace cv;
using namespace std;

 Mat whiteFilter(cv::Mat img) {
	cv::Mat temp;
	cv::cvtColor(img, temp, COLOR_BGR2HSV);
	
	cv::Scalar lower(0, 0, 110, 0), higher(180, 255, 255, 0);
	cv::inRange(temp, lower, higher, img);
	cv::cvtColor(img, img, COLOR_GRAY2BGR);
	
	return img;
}
 VideoCapture videoLoad(String video) {

	 VideoCapture cap = VideoCapture(video); 
	 if (!cap.isOpened())  
		 return -1;
	 return cap;
 }
 Mat regionOfInterest(Mat img) {
	 
	  Mat frame=img(Rect(0, img.rows / 2 - 200, img.cols, img.rows / 2 + 200));
	 return frame;
 }

 Mat drawLine(vector<Vec4i> linesP, Mat img) {
	 
	 for (size_t i = 0; i < linesP.size(); i++)
	 {
		 if (abs(linesP[i].val[1] - linesP[i].val[3])>30) {
			 Vec4i l = linesP[i];
			 line(img, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
		 }
	 }
	 return img;
 }

 void lineDetection(String a) {

	 VideoCapture cap = videoLoad(a);
	 Mat frame, edges, cdst, cdstP, frameOrigin;
	 vector<Vec4i> linesP;
	 namedWindow("edges", 1);

	 for (;;) {
		 cap >> frame;
		 frame = regionOfInterest(frame);
		 frameOrigin = frame.clone();
		 frame = whiteFilter(frame);
		 cvtColor(frame, edges, COLOR_BGR2GRAY);
		 //GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		 Canny(edges, edges, 0, 30, 3);
		 cvtColor(edges, cdst, COLOR_GRAY2BGR);
		 cdstP = cdst.clone();
		 HoughLinesP(edges, linesP, 1, CV_PI / 180, 50, 30, 10);
		 cdstP = drawLine(linesP, frameOrigin);
		 imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);
		 if (waitKey(1) >= 0) break;
	 }
 }

int main(int argc, char** argv)
{
	lineDetection("C:\\Users\\adamm\\Videos\\2.mp4");

}


