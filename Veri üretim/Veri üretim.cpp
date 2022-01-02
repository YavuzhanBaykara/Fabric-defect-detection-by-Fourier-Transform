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


//import CPP and Header
#include "Histogram_Esitle.h"
#include "FFT.h"



namespace fs = std::filesystem;     //dosya uzantı kütüphanesi
using namespace std;                //Std
using namespace cv;                 //Opencv kütüphanesi



int main()

{
    //fabric images path
    string path = "D:/Yavzan/x-x/Veri üretim/Resimler/";

    //normalized image path
    string path_norm = "D:/Yavzan/x-x/Veri üretim/Norm edilmiş/";

    string* Caga = new string[100]; //Empty Matrix
    int k = 0;
    string new_path;
    string new_path_norm;

    //files in folder
    for (const auto& entry : fs::directory_iterator(path))
    {
        Caga[k] = entry.path().filename().string().c_str();
        k = k + 1;
    }


    for (int i = 0; i < k; i++)
    {
        new_path = path + Caga[i];
        Hist_Esitle(new_path,k,i);
        new_path_norm = path_norm + "Sa" + to_string(i)+".PNG";
        FFT(new_path_norm); // Fast Fourier Transform Function
        std::cout << new_path <<endl;
    }

    cv::waitKey(0);




}
