#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	Mat image, gray;
	vector<vector<Point> > contours;
	
	namedWindow("Contours");

    image = imread("hand.JPG");
  	cvtColor(image,gray,COLOR_RGB2GRAY);
   	threshold(gray,gray,127,255,THRESH_BINARY);
	
    
	findContours(gray,contours,RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	drawContours(image, contours, -1, Scalar(0,255,0),3);
    
    imshow("Contours", image);

    waitKey(0);
	
}
