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

int Capture(string);
int Contourn(Mat);

int Capture(string video_name) {
	Mat frame, roi, fgMask, image;
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
	namedWindow("Contours");

	int frame_width = cap.get(CAP_PROP_FRAME_WIDTH); 
	int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT); 

	int codec = VideoWriter::fourcc('M','J','P','G');
	VideoWriter video(video_name,codec,20, Size(frame_width,frame_height));

	int aux = -1;

	Rect rect(400,100,200,200);	
	while(true) {

		cap>>frame;
		flip(frame,frame,1);

		frame(rect).copyTo(roi);
		pBackSub->apply(roi, fgMask, aux);

		rectangle(frame, rect,Scalar(255,0,0));

		imshow("Frame",frame);
		imshow("Roi",roi);
		imshow("Foreground Mask",fgMask);

		//Contourn(fgMask);

		video.write(frame);

		int c = waitKey(40);

		if ((char)c == 's') {
			if (aux == 0) {
				aux = -1; 
			} else aux = 0;
		}

		if ((char)c =='q') break;
	}
	video.release();
	cap.release();
	destroyAllWindows();
}

int OpenVid(string video_name) {
	Mat frame;
	VideoCapture cap;
	cap.open(video_name);

	if (!cap.isOpened()) {
		printf("Error opening cam\n");
		return -1;
	}
	namedWindow("Frame");
	
	while(true) {

		cap>>frame;
		if (frame.empty())
			break;		
		imshow("Frame",frame);

		int c = waitKey(40);

		if ((char)c =='q') break;
	}
	cap.release();
	destroyAllWindows();
}

int Contourn(Mat image) {
	Mat gray;

	namedWindow("Contours");
	vector<vector<Point> > contours;

	cvtColor(image,gray,COLOR_RGB2GRAY);
	threshold(gray,gray,127,255,THRESH_BINARY);
	
	findContours(gray,contours,RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	drawContours(image, contours, -1, Scalar(0,255,0),3);
	
	imshow("Contours", image);

	waitKey(0);
}

int main(int argc, char* argv[])
{
	string video_name = "out.avi";
	Capture(video_name);
	//OpenVid(video_name);
}
