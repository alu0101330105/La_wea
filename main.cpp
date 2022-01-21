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

double angle(Point s, Point e, Point f) {
    double v1[2],v2[2];
    v1[0] = s.x - f.x;
    v1[1] = s.y - f.y;
    v2[0] = e.x - f.x;
    v2[1] = e.y - f.y;
    double ang1 = atan2(v1[1], v1[0]);
    double ang2 = atan2(v2[1], v2[0]);

    double ang = ang1 - ang2;
    if (ang > CV_PI) ang -= 2*CV_PI;
    if (ang < -CV_PI) ang += 2*CV_PI;

    return ang*180/CV_PI;
}

int Capture() {
	Mat frame, roi, fgMask;
	vector<vector<Point>> contours;
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

		vector<int> hull;
		for (int k = 0; k < contours.size(); k++) {
			convexHull(contours[k], hull,false,false);
			sort(hull.begin(),hull.end(),greater <int>());

			vector<Vec4i> defects;
			convexityDefects(contours[k], hull, defects);
			for (int i = 0; i < defects.size(); i++) {
				Point s = contours[k][defects[i][0]];
				Point e = contours[k][defects[i][1]];
				Point f = contours[k][defects[i][2]];
				float depth = (float)defects[i][3] / 256.0;
				double ang = angle(s,e,f);
				if (ang <= 90 && depth > 30) {
					circle(roi, f,5,Scalar(0,0,255),-1);
					line(roi,s,e,Scalar(255,0,0),2);
				}
			}
		}

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
