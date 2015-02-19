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
    drawKeypoints(img,*kps,*pontos/*,Scalar(127,127,127)*/,4);
}

Mat cinzasSobel (Mat src_gray, Mat grad)
{
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
    /// Generate grad_x and grad_y
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;

    /// Gradient X
    //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
    Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );

    /// Gradient Y
    //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
    Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );

    /// Total Gradient (approximate)
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

    return grad;
}

void casar (Mat desc1, Mat desc2, vector<DMatch> *m)
{
    BFMatcher matcher(NORM_L2);
    matcher.match(desc1, desc2, *m);
}

int main()
{
    int fcount = 0;

    Mat padrao, modelo, kPadrao, kModelo, kCasada, exModelo, exPadrao;
    Mat padraoCinza, modeloCinza, padraoBlur, modeloBlur;
    Mat padraoSobel, modeloSobel;
    std::vector<KeyPoint> kpPadrao, kpModelo;
    vector<DMatch> pontosCasados;

    modelo =  imread("mesa.jpg");
    padrao =  imread("marca.jpg");

    /// Filtro gaussiano
    GaussianBlur(padrao, padrao, Size(3,3), 0, 0, BORDER_DEFAULT);
    GaussianBlur(modelo, modelo, Size(3,3), 0, 0, BORDER_DEFAULT);

    /// RGB -> Escala de cinza
    cvtColor(padrao, padraoCinza, CV_RGB2GRAY);
    cvtColor(modelo, modeloCinza, CV_RGB2GRAY);

    /// Filtro de Sobel
    //padraoSobel = cinzasSobel(padraoCinza,padraoSobel);
    //modeloSobel = cinzasSobel(modeloCinza,modeloSobel);

    /// Calculo de pontos chave
    imgPontosChaves(modeloCinza,&kpModelo,&kModelo);
    imgPontosChaves(padraoCinza,&kpPadrao,&kPadrao);

    /// Extracao de features
    imgExtracao(modelo,kpModelo,&exModelo);
    imgExtracao(padrao,kpPadrao,&exPadrao);

    casar(exModelo,exPadrao,&pontosCasados);

    drawMatches(modelo, kpModelo, padrao, kpPadrao, pontosCasados, kCasada);

    while (true)
    {
        if (waitKey(30) == 13)
            break;

        fcount++;
        //imshow("PG - CIn/UFPE 2014.2 - M",exModelo);
        //imshow("PG - CIn/UFPE 2014.2 - P",exPadrao);
        imshow("PG - CIn/UFPE 2014.2",kCasada);
    }
    imwrite("match.jpg", kCasada);
    imwrite("kModelo.jpg",kModelo);
    imwrite("kPadrao.jpg",kPadrao);
    printf("\n\t# de Frames: %d",fcount);
    return 0;
}
