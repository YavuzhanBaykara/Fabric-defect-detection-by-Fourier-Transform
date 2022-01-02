#pragma once
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

using namespace std;
using namespace cv;

void magnitudeSpectrum(Mat complex);
void fftShift(Mat magI);
cv::Mat computeDFT(Mat image, int secim);
cv::Mat SumColumn(Mat Img);
cv::Mat SumRown(Mat Img);
void FFT(string& Uzanti);