//
//  p1.cpp
//  VisionProject2
//
//  Created by Elizabeth Kelly on 9/20/16.
//  Copyright © 2016 Elizabeth Kelly. All rights reserved.
//

#include "Image.h"
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;

/**
 * Function that converts a gray-level image to a binary image using a threshold value.
 * in_file is the gray-level .pgm file to scan.
 * out_file is the binary image produced.
 * Returns true if the image was read, processed, and written.
 **/
bool ConvertToBinary(string in_file, string out_file, int threshold) {
    
    Image orig_image;
    
    if (!ReadImage(in_file, &orig_image)) {
        cout <<"Can't open file " << in_file << endl;
        return false;
    }
    //scan the image and set the pixels using the threshold
    for (int i = 0; i < orig_image.num_rows(); ++i) {
        for (int j = 0; j < orig_image.num_columns(); ++j) {
            if (orig_image.GetPixel(i, j) < threshold) {
                orig_image.SetPixel(i, j, 0); //make the pixel black
            }
            else { //if the pixel is > threshold
                orig_image.SetPixel(i, j, 1); //make the pixel white
            }
        }
    }
    //set number of colors in PGM as 1 in the header
    orig_image.SetNumberGrayLevels(1);
    
    if (!WriteImage(out_file, orig_image)){
        cout << "Can't write to file " << out_file << endl;
        return false;
    }
    return true;
}

int main(int argc, char **argv){
    
    if (argc!=4) {
        cout << "Usage: <input gray-level image> <threshold value> <output binary image> " << endl;
        return 0;
    }
    const string input_file(argv[1]);
    int threshold = atoi(argv[2]); //120 is best
    const string output_file(argv[3]);

    ConvertToBinary(input_file, output_file, threshold);
    
    return 0;
}

