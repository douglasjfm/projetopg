#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"

#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <vector>

using namespace cv;
using namespace std;


void imgExtracao (Mat img,  std::vector<KeyPoint> kps, Mat *desc)
{
    SurfDescriptorExtractor extractor;
    extractor.compute(img, kps, *desc);
}

void imgPontosChaves(Mat img, std::vector<KeyPoint> *kps, Mat *pontos)
{
    SurfFeatureDetector detector(400);

    detector.detect(img, *kps);
    drawKeypoints(img,*kps,*pontos,4);
}

void casar (Mat desc1, Mat desc2, vector<DMatch> *m)
{
    BFMatcher matcher(NORM_L2);
    matcher.match(desc1, desc2, *m);
}

int main()
{
    int fcount = 0;
	VideoCapture capModel("objs.jpg");
	VideoCapture capPadrao("obj.jpg");
	Mat padrao, modelo, kPadrao, kModelo, kCasada, exModelo, exPadrao;
	std::vector<KeyPoint> kpPadrao, kpModelo;
	vector<DMatch> pontosCasados;


	if (!capModel.isOpened() || !capPadrao.isOpened())
	{
		cout << "error, could not open one of the capture" << endl;
		system("pause");
		exit(1);
	}

    capModel >> modelo;
    capPadrao >> padrao;

    imgPontosChaves(modelo,&kpModelo,&kModelo);
    imgPontosChaves(padrao,&kpPadrao,&kPadrao);

    imgExtracao(modelo,kpModelo,&exModelo);
    imgExtracao(padrao,kpPadrao,&exPadrao);

    casar(exModelo,exPadrao,&pontosCasados);

    drawMatches(exModelo, kpModelo, exPadrao, kpPadrao, pontosCasados, kCasada);

	while (true)
	{
            if (waitKey(1) == 13)
			break;
        //if (image.rows == 0)
        //    break;

		//fcount++;
		imshow("PG - CIn/UFPE 2014.2 - M",kModelo);
		imshow("PG - CIn/UFPE 2014.2 - P",kPadrao);
		imshow("PG - CIn/UFPE 2014.2",kCasada);
	}

    printf("\n\t# de Frames: %d",fcount);

	return 0;
}
