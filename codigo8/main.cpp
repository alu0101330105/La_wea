#include <iostream>
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
	
	vector<int> hull;
    convexHull(contours[0], hull,false,false);
    sort(hull.begin(),hull.end(),greater <int>());

    vector<Vec4i> defects;
    convexityDefects(contours[0], hull, defects);
    for (int i = 0; i < defects.size(); i++) {
    	Point s = contours[0][defects[i][0]];
        Point e = contours[0][defects[i][1]];
        Point f = contours[0][defects[i][2]];
        float depth = (float)defects[i][3] / 256.0;
        double ang = angle(s,e,f);
        circle(image, f,5,Scalar(0,0,255),-1);
        line(image,s,e,Scalar(255,0,0),2);
    }

    imshow("Contours", image);

    waitKey(0);
	
}
