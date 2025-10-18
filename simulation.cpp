#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;


//Converting a single tempature value into 3 (Blue, Red, Green) color values
std::array<double, 3> TempaturetoColor(double temp) {
    double minTemp = -20;
    double maxTemp = 100;
    double midTemp = 40;

    double Blue;
    double Green;
    double Red;

    double ratio = 0;
    if(temp <= midTemp){
        ratio = (temp - minTemp) / (midTemp - minTemp); 
        Blue = 255 * ratio;
        Green = 255;
        Red   = 255 * ratio; 
    }
    else{
        ratio = (temp - midTemp) / (maxTemp - midTemp);
        Blue = 255 * (1 - ratio);
        Green = 255 * (1 - ratio);
        Red = 255;
    }
    
    std::array<double, 3> color = {Blue, Green, Red};
    return color;
}

int main(int argc, char* argv[]) {
    int M, N;
    Mat img;

    double highestDiff = 1;
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

    //drawing start state
    for(int i = 0; i < M; i++){
        for(int j = 0; j < N; j++){

            if(i==0 || i == M - 1 || j ==0 || j ==  N - 1 ){
                if (i == 0 && (j >= (N / 2 - 5) && j <= (N / 2 + 5))) {
                    temp[i][j] = 100;
                    //setting tempature for the heater
                }
                else{
                    temp[i][j] = 20;
                    //setting tempature for the room
                }
            }
            else{
                    temp[i][j] = -20; 
                    //setting tempature for the walls   
            }

            std::array<double, 3> colors = TempaturetoColor(temp[i][j]);
            img.data[i * img.step[0] + j * img.step[1]     ] = colors[0]; // blue 
            img.data[i * img.step[0] + j * img.step[1] + 1 ] = colors[1]; // green
            img.data[i * img.step[0] + j * img.step[1] + 2 ] = colors[2]; // red
            
        }

    }
    
    imshow("color display", img);

    while (true) {
        int key = waitKey(1);
        if (key >= 0) break; 

        //checking to see if we have reached the minium epsilon 
        if (highestDiff < 0.01) break;
        highestDiff = 0;

        std::vector<std::vector<double>> newTemp = temp;

        //calculating new tempatures for each cell in our image
        for (int i = 1; i < M - 1; i++) {
            for (int j = 1; j < N - 1; j++) {
                if (i == M - 1 && (j >= (N / 2 - 5) && j <= (N / 2 + 5))) continue; //making sure not to change tempature of heater

                newTemp[i][j] = (temp[i+1][j] + temp[i-1][j] + temp[i][j+1] + temp[i][j-1]) / 4.0;
                
                //checking for the highest difference in this iteration
                currentDiff = abs(newTemp[i][j] - temp[i][j]);
                if(currentDiff > highestDiff) highestDiff = currentDiff;
                
            }

        }
        
        temp = newTemp;
    
        //printing our new image with new tempatures.
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                auto colors = TempaturetoColor(temp[i][j]);
                img.data[i * img.step[0] + j * img.step[1]     ] = colors[0];
                img.data[i * img.step[0] + j * img.step[1] + 1 ] = colors[1];
                img.data[i * img.step[0] + j * img.step[1] + 2 ] = colors[2];
            }
        }
    
        imshow("color display", img);
    }
    return 0;
}

