//
//  p2.cpp
//  VisionProject2
//
//  Created by Elizabeth Kelly on 9/20/16.
//  Copyright © 2016 Elizabeth Kelly. All rights reserved.
//

#include "Image.h"
#include "DisjSets.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>

using namespace std;
using namespace ComputerVisionProjects;

/**
 * Function that segments a binary image into several connected components via sequential labeling.
 * in_file is the binary .pgm file to label.
 * out_file is the labeled image produced.
 * Returns true if the image was read, labeled, and written.
 **/
bool LabelImage(string in_file, string out_file) {
    
    Image orig_image;
    Image labeled_image;
    
    int label = 15;
    
    DisjSets equiv_class(400);
    
    if (!ReadImage(in_file, &orig_image) ) {
        cout <<"Can't open file " << in_file << endl;
        return false;
    }
    
    size_t rows = orig_image.num_rows();
    size_t cols = orig_image.num_columns();
    
    labeled_image.AllocateSpaceAndSetSize(rows, cols);
    labeled_image.SetNumberGrayLevels(255);
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            labeled_image.SetPixel(i, j, 0);
        }
    }
    //first pass - raster scan to label to image
    for (int i = 1; i < rows; i++) {
        for (int j = 1; j < cols; j++) {
            
            int current_pixel = orig_image.GetPixel(i, j); //pixel being scanned
            int upper_neighbor = labeled_image.GetPixel(i-1, j);
            int corner_neighbor = labeled_image.GetPixel(i-1, j-1);
            int left_neighbor = labeled_image.GetPixel(i, j-1);
            
            if (current_pixel == 0) { //if scanned pixel is part of background
                continue;
            }
            
            else { //if scanned pixel (A) is not a background pixel
                //if D (corner) has a label
                if (left_neighbor == 0 && upper_neighbor == 0) {//no labels
                    label++;
                    labeled_image.SetPixel(i, j, label);
                }
                else if (corner_neighbor != 0) {
                    labeled_image.SetPixel(i, j, corner_neighbor); //set the pixel to it's corner label
                }
                else if (upper_neighbor != 0 && left_neighbor == 0) {//if corner labeled
                    labeled_image.SetPixel(i, j, upper_neighbor); //if upper has a label
                }
                else if (left_neighbor != 0 && upper_neighbor == 0) {//if left labeled
                    labeled_image.SetPixel(i, j, left_neighbor);
                }
                //if both left and upper neighbors labeled
                else if (left_neighbor != 0 && upper_neighbor != 0) {
                    
                    //set current to either (this case, upper neighbor)
                    labeled_image.SetPixel(i, j, upper_neighbor);
                    
                    if (upper_neighbor != left_neighbor) {
                        //put into equiv class
                        int set1 = equiv_class.find(upper_neighbor);
                        int set2 = equiv_class.find(left_neighbor);
                        
                        if (set1 != set2) {
                            //take note of the equivalence
                            equiv_class.unionSets(set1, set2);
                        }
                    }//end if
                }
            }
            
        }//end for j
    }//end for i
    
    //second pass to resolve equivalences between the labels
    for (int i = 0; i < labeled_image.num_rows(); i++) {
        for (int j = 0; j < labeled_image.num_columns(); j++) {
            if (labeled_image.GetPixel(i, j) == 0) {
                continue;
            }
            else {
                labeled_image.SetPixel(i, j, equiv_class.find(labeled_image.GetPixel(i, j)));
            }
        }
    }
    
    if (!WriteImage(out_file, labeled_image)){
        cout << "Can't write to file " << out_file << endl;
        return false;
    }
    
    return true;
}

int main(int argc, char **argv){
    
    if (argc!=3) {
        cout << "Usage: <input binary image> <output labeled image> " << endl;
        return 0;
    }
    const string input_file(argv[1]);
    const string output_file(argv[2]);

    LabelImage(input_file, output_file);
    
    return 0;
}
