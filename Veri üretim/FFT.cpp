#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <array>
#include "opencv2/opencv.hpp"
#include <opencv2/imgcodecs.hpp>
#include <tchar.h>
#include<map>;
#include <conio.h>;
#include <cmath>
#include <stdio.h>
#include <opencv2/core/mat.hpp>
#include "FFT.h"


using namespace std;
using namespace cv;




#define CV_PI   3.1415926535897932384626433832795

using namespace std;
using namespace cv;

typedef Vec <uchar, 3> vec2b;
typedef Vec <uchar, 3> vec3b;

void FFT(string& Uzanti)
{
    /// Gray scale transform
    cv::Mat img;
    img = imread(Uzanti, IMREAD_GRAYSCALE);


    //DFT
    cv::Mat dft = computeDFT(img, 0);
    cv::Mat dftR = computeDFT(img, 1);
    cv::Mat dftÝ = computeDFT(img, 2);
    cv::Mat Mutlak(size(dft), CV_32F);
    
    // For more information reading Fourier dönüþümü kullanýlarak gerçek zamanlý kumaþ hatasý tespiti.pdf
    ///P4-P7 values
    double P4 = 0;
    double P7 = 0;

    //Complex number, integer conversion
    for (int i = 0; i < dft.cols; i++)
    {
        for (int j = 0; j < dft.rows; j++)
        {
            //Mutlak=((Re)^2+(Im)^2)^1/2
            fixed;
            Mutlak.at<float>(i, j) = sqrt(pow(dftR.at<float>(i, j), 2) + pow(dftÝ.at<float>(i, j), 2));
        }

    }


   
    //Max rows and cols values information
    double max_val_C;
    Point maxloc_C;
    Point minloc_C;
    double max_val_R;
    Point maxloc_R;
    Point minloc_R;

    //Max Rows and cols Locations
    cv::minMaxLoc(SumColumn(Mutlak), NULL, &max_val_C, NULL, &maxloc_C);
    cv::minMaxLoc(SumRown(Mutlak), NULL, &max_val_R, NULL, &maxloc_R);

    //Min Rows and cols Locations
    cv::minMaxLoc(SumColumn(Mutlak), NULL, NULL, &minloc_C, NULL);
    cv::minMaxLoc(SumRown(Mutlak), NULL, NULL, &minloc_R, NULL);


    double Fx = maxloc_C.y+1;
    double Fy = maxloc_R.y+1;

    // For more information reading Fourier dönüþümü kullanýlarak gerçek zamanlý kumaþ hatasý tespiti.pdf
    //Sum P4 values
    for (int i = 0; i < Fx; i++)
    {
        P4 = Mutlak.at<float>(i, 0) + P4;

    }
    // For more information reading Fourier dönüþümü kullanýlarak gerçek zamanlý kumaþ hatasý tespiti.pdf
    //Sum P7 values
    for (int i = 0; i < Fy; i++)
    {
        P7 = Mutlak.at<float>(0, i) + P7;
    }

    // For more information reading Fourier dönüþümü kullanýlarak gerçek zamanlý kumaþ hatasý tespiti.pdf
    //Result statistical Fourier attribute values
    cv::Mat Result(1, 7, CV_32F);
    Result.at<float>(0, 0) = Mutlak.at<float>(0, 0);
    Result.at<float>(0, 1) = Mutlak.at<float>(maxloc_C.y, 0);
    Result.at<float>(0, 2) = Fx;
    Result.at<float>(0, 3) = P4;
    Result.at<float>(0, 4) = Mutlak.at<float>(0, maxloc_R.y);
    Result.at<float>(0, 5) = Fy;
    Result.at<float>(0, 6) = P7;
    cout << Result << endl;


    Point p1((maxloc_C.y + 1)*2,(maxloc_R.y+1)*2); Point p2(minloc_C.y,minloc_R.y );
    int thickness = 2;
    cout << "Min_Col= " << minloc_C.y << endl;
    cout << "Max_Col= " << maxloc_C.y << endl;
    cout << "Min_Row= " << minloc_R.y << endl;
    cout << "Max_Row= " << maxloc_R.y << endl;

    rectangle(img, p1, p2, thickness, LINE_8);
    cv::imshow("S", img);
    waitKey(0);
      
    
}





//DFT dönüþüm

cv::Mat computeDFT(Mat image, int secim) {
    Mat padded;                            //giriþ görüntüsünü en uygun(optimal) þekilde geniþlet veya daraltma matrisi
    int m = getOptimalDFTSize(image.rows);
    int n = getOptimalDFTSize(image.cols); // 
    copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));
    Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
    Mat complex;
    merge(planes, 2, complex);
    dft(complex, complex, DFT_COMPLEX_OUTPUT);
    split(complex, planes);

    if (secim == 0)
    {
        return complex;
    }
    else if (secim == 1)
    {
        split(complex, planes);
        return planes[0];
    }
    else if (secim == 2)
    {
        split(complex, planes);
        return planes[1];
    }
}



void fftShift(Mat magI) {


    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    int cx = magI.cols / 2;
    int cy = magI.rows / 2;

    Mat q0(magI, Rect(0, 0, cx, cy));
    Mat q1(magI, Rect(cx, 0, cx, cy));
    Mat q2(magI, Rect(0, cy, cx, cy));
    Mat q3(magI, Rect(cx, cy, cx, cy));

    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}


//Toplam sütun
cv::Mat SumColumn(Mat Img)
{
    Mat columnSum;
    for (int i = 1; i < Img.cols / 2; i++)
    {
        columnSum.push_back(cv::sum(Img.col(i))[0]);
    }
    return columnSum;

}

//Toplam Satýr
cv::Mat SumRown(Mat Img)
{
    Mat RownSum;
    for (int i = 1; i < Img.rows / 2; i++)
    {
        RownSum.push_back(cv::sum(Img.row(i))[0]);
    }
    return RownSum;

}

void magnitudeSpectrum(Mat complex) {
    Mat magI;
    Mat planes[] = {
        Mat::zeros(complex.size(), CV_32F),
        Mat::zeros(complex.size(), CV_32F)
    };
    split(complex, planes); // planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], magI); // sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)

    // logaritmik görüntü deðiþimi: log(1 + magnitude)
    magI += Scalar::all(1);
    log(magI, magI);
    fftShift(magI);

    normalize(magI, magI, 1, 0, NORM_INF);
    namedWindow("DFT Daðýlýmý", WINDOW_NORMAL);
 //   imshow("DFT Daðýlýmý", magI);
}
