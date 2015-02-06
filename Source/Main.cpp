#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;

int main()
{
	VideoCapture cap("Resources\\video.mp4");
	Mat image;

	if (!cap.isOpened())
	{
		cout << "error, could not open the capture" << endl;
		system("pause");
		exit(1);
	}

	while (true)
	{
		if (waitKey(1) == 27)
			break;

		cap >> image;
		imshow("...",image);
	}

	return 0;
}