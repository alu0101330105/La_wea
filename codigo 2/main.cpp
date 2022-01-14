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
	Mat frame;
	VideoCapture cap;
	cap.open(0);

	if (!cap.isOpened()) {
		printf("Error opening cam\n");
		return -1;
	}
	namedWindow("Frame");


	int frame_width = cap.get(CAP_PROP_FRAME_WIDTH); 
	int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT); 
	
	int codec = VideoWriter::fourcc('M','J','P','G');
	VideoWriter video("out.avi",codec,20, Size(frame_width,frame_height)); 
	
	while(true) {

		cap>>frame;		
		imshow("Frame",frame);

		video.write(frame);

		int c = waitKey(40);
		if ((char)c =='q') break;
	}
	video.release();
	cap.release();
	destroyAllWindows();
}
