#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;

int main()
{
    int fcount = 0;
	VideoCapture cap("Resources/video.ogg");
	Mat image;

	if (!cap.isOpened())
	{
		cout << "error, could not open the capture" << endl;
		system("pause");
		exit(1);
	}

	while (true)
	{
		if (waitKey(120) == 27)
			break;

		cap >> image;
		fcount++;
		imshow("....",image);
	}

	return 0;
}
