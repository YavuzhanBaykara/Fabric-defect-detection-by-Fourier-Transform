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
#include "Histogram_Esitle.h"


using namespace std;
using namespace cv;

//Histogram Matrix
void imhist(Mat image, int histogram[])
{
    // Create zeros[1x255] vector matrix
    for (int i = 0; i < 256; i++)
    {
        histogram[i] = 0;
    }

    // image values transfer process
    for (int y = 0; y < image.rows; y++)
        for (int x = 0; x < image.cols; x++)
            histogram[(int)image.at<uchar>(y, x)]++;
}
// Histogram Display
void histDisplay(int histogram[], const char* name)
{
    int hist[256];
    for (int i = 0; i < 256; i++)
    {
        hist[i] = histogram[i];
    }
    //Histogram image matrix
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound((double)hist_w / 256); //yuvarlama eðri çizim sýnýrlarý
    Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));
    int max = hist[0];
    for (int i = 1; i < 256; i++) {
        if (max < hist[i]) {
            max = hist[i];
        }
    }

    // Normalization by max value
    for (int i = 0; i < 256; i++)
    {
        hist[i] = ((double)hist[i] / max) * histImage.rows;
    }
    //Histogram drawing settings

    for (int i = 0; i < 256; i++)
    {
        line(histImage, Point(bin_w * (i), hist_h), Point(bin_w * (i), hist_h - hist[i]), Scalar(0, 0, 0), 1, 8, 0);
    }
}

//Histogram Normalization
void Hist_Esitle(string &Uzanti, int sayac, int birim_artis)
{
    string Write_image_norm;
    Mat image;
    
    // Load the image
    image = imread(Uzanti, cv::IMREAD_GRAYSCALE);
    int Weight = 1150;             //resized normalization image
    int Height = 1150;            // resized normalization image
    cv::Rect crop_region(image.cols/2,image.rows/2, Height, Weight);
    
    image = image(crop_region);

    // histogram void deðer aktarýmý
    int histogram[256];
    imhist(image, histogram);

    // Ýmage ayrýntý deðerleri
    int size = image.rows * image.cols;

    // her deðer için min deðer
    float PrRk[256];
    for (int i = 0; i < 256; i++)
    {
        PrRk[i] = (double)histogram[i] / size;
    }

    // eþitlenmiþ deðer aktarýmý
    float PsSk[256];
    for (int i = 0; i < 256; i++)
    {
        PsSk[i] = 0;
    }

    for (int i = 0; i < 256; i++)
        for (int j = 0; j <= i; j++)
            PsSk[i] += PrRk[j];

    int final[256];
    for (int i = 0; i < 256; i++)
        final[i] = cvRound(PsSk[i] * 255);

    for (int i = 0; i < 256; i++)
        for (int j = 0; j <= 255; j++)
            if (final[i] == final[j] && i != j)
            {
                final[i] += final[j];
            }

    int final1[256];
    for (int i = 0; i < 256; i++)
    {
        final1[i] = 0;
    }

    for (int i = 0; i < 256; i++)
    {
        final1[cvRound(PsSk[i] * 255)] = cvRound(PrRk[i] * size);
    }

    for (int i = 0; i < 256; i++)
        for (int j = 0; j < 256; j++)
            if (final1[i] == final[j] && i != j)
            {
                final1[i] += final1[j];
            }

    // New image 
    Mat new_image;
    new_image = image.clone();


    for (int y = 0; y < image.rows; y++)
        for (int x = 0; x < image.cols; x++)
            new_image.at<uchar>(y, x) = saturate_cast<uchar>(final[image.at<uchar>(y, x)]);


    histDisplay(histogram, "Orjinal Histogram");
    histDisplay(final1, "Eþitlenmiþ Histogram");
    Write_image_norm = "Norm edilmiþ/Sa" + to_string(birim_artis) + ".png";
    cv::imwrite(Write_image_norm, new_image);
    std::cout << birim_artis;


}