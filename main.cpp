#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;

int Capture() {
	Mat frame, roi, fgMask;
	vector<vector<Point> > contours;
	VideoCapture cap;
	cap.open(0);
	Ptr<BackgroundSubtractor> pBackSub=createBackgroundSubtractorMOG2();

	if (!cap.isOpened()) {
		printf("Error opening cam\n");
		return -1;
	}
	namedWindow("Frame");
	namedWindow("Roi");
	namedWindow("Foreground Mask");

	int frame_width = cap.get(CAP_PROP_FRAME_WIDTH); 
	int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT); 

	int aux = -1;

	Rect rect(400,100,200,200);	
	while(true) {

		cap >> frame;
		flip(frame,frame,1);

		frame(rect).copyTo(roi);
		pBackSub->apply(roi, fgMask, aux);

		rectangle(frame, rect,Scalar(255,0,0));

		findContours(fgMask,contours,RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		drawContours(roi, contours, -1, Scalar(0,255,0),3);

		vector<vector<Point> > hull(contours.size());
		for (size_t i = 0; i < contours.size(); i++)
			convexHull(contours[i], hull[i]);
		drawContours(roi, hull, -1, Scalar(255,0,0),3);

		imshow("Frame",frame);
		imshow("Roi",roi);
		imshow("Foreground Mask",fgMask);

		int c = waitKey(40);

		if ((char)c == 's') {
			if (aux == 0) {
				aux = -1; 
			} else aux = 0;
		}

		if ((char)c =='q') break;
	}
	cap.release();
	destroyAllWindows();
}


int main(int argc, char* argv[])
{
	Capture();
}
