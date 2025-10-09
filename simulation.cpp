#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;


//Converting a single tempature value into 3 (Red, Green and Blue) color values
std::array<double, 3> TempaturetoColor(double temp) {
    double minTemp = -20;
    double maxTemp = 100;
    double midTemp = (100 + -20) / 2;

    double Blue;
    double Green;
    double Red;

    double ratio = 0;
    if(temp < midTemp){
        ratio = (temp - minTemp) / (midTemp - minTemp); 
        Blue = 0;
        Green = 255 * (1 - ratio); 
        Red   = ; 
    }
    else if(temp == midTemp){
        Blue = 255;
        Green = 255;
        Red = 255;
        
    }
    else{
        ratio = (temp - midTemp) / (maxTemp - midTemp);
        Blue = 0;
        Green = 0;
        Red = 255 * (ratio);
    }
    
    std::array<double, 3> color = {Blue, Green, Red};

    return color;
}

int main(int argc, char* argv[]) {
    int M, N;
    Mat img;

    double lowestDiff = 0;
    double currentDiff = 0;

    if (argc < 3) {
        printf("Provide two arguments:\n");
        printf("- number of rows (example: 256)\n");
        printf("- number of columns (example: 256)\n");
        exit(1);
    }

    M = atoi(argv[1]);
    N = atoi(argv[2]);

    std::vector<std::vector<double>> temp(M, std::vector<double>(N, -20.0)); //creating tempature vector; temp
    
    
    img = Mat(M, N, CV_8UC3, Scalar(255, 255, 255)); //creating imagel img

    //initiating start state
    for(int i = 0; i < M; i++){
        for(int j = 0; j < N; j++){
            if(i < 1 || i > 254 || j < 1 || j > 254){
                
                if(i < 254 && 120 <= j && j <= 136){
                    temp[i][j] = 100;
                }
                else{
                    temp[i][j] = -20;
                }
            }
            else{
                    temp[i][j] = 20;    
            }

            std::array<double, 3> colors = TempaturetoColor(temp[i][j]);
            img.data[i * img.step[0] + j * img.step[1]     ] = colors[0]; // blue 
            img.data[i * img.step[0] + j * img.step[1] + 1 ] = colors[1]; // green
            img.data[i * img.step[0] + j * img.step[1] + 2 ] = colors[2]; // red
            
        }

    }
    int num = 0;

    imshow("color display", img);
    printf("Press any key to terminate the program.\n");

    //added this as pressing a key to terminate was inconsistent on mac
    while (true) {
        int key = waitKey(1);
        if (key >= 0) break; 

        std::vector<std::vector<double>> newTemp = temp;
        
        for (int i = 1; i < M - 1; i++) {
            for (int j = 1; j < N - 1; j++) {
                newTemp[i][j] = (temp[i+1][j] + temp[i-1][j] + temp[i][j+1] + temp[i][j-1]) / 4;
                
                }

            }
        
        
        temp = newTemp;
    
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                auto colors = TempaturetoColor(temp[i][j]);
                img.data[i * img.step[0] + j * img.step[1]     ] = 0;
                img.data[i * img.step[0] + j * img.step[1] + 1 ] = colors[1];
                img.data[i * img.step[0] + j * img.step[1] + 2 ] = colors[2];
            }
        }
    
        imshow("color display", img);
    }
    return 0;
}

