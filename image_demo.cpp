//
//  main.cpp
//  VisionProject2
//
//  Created by Elizabeth Kelly on 9/12/16.
//  Copyright © 2016 Elizabeth Kelly. All rights reserved.
//
// Sample test file for the image class.
// Reads a given pgm image, draws a line, and saves it to
// another pgm image.
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

bool ConvertToBinary(string in_file, string out_file, int threshold) {
    
    Image orig_image;
    
    if (!ReadImage(in_file, &orig_image)) {
        cout <<"Can't open file " << in_file << endl;
        return false;
    }
    
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
    orig_image.SetNumberGrayLevels(1);//set number of colors in PGM as 1 in the header
    
    if (!WriteImage(out_file, orig_image)){
        cout << "Can't write to file " << out_file << endl;
        return false;
    }
    return true;
}


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
    int labels_array[rows][cols];
    
    labeled_image.AllocateSpaceAndSetSize(rows, cols);
    labeled_image.SetNumberGrayLevels(255);
    cout << "done" << endl;

    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            labels_array[i][j] = 0;
        }
    }
    cout << "done" << endl;
    for (int i = 1; i < rows; ++i) {
        for (int j = 1; j < cols; ++j) {
            labeled_image.SetPixel(i, j, 0);
        }
    }
    cout << "done" << endl;
    
    for (int i = 1; i < orig_image.num_rows()-1; i++) {
        for (int j = 1; j < orig_image.num_columns()-1; j++) {
            
            int current_pixel = orig_image.GetPixel(i, j); //pixel being scanned
            int upper_neighbor = labeled_image.GetPixel(i-1, j);
            int corner_neighbor = labeled_image.GetPixel(i-1, j-1);
            int left_neighbor = labeled_image.GetPixel(i, j-1);
    /*
            D B
            C A*
    */
            if (current_pixel == 0) { //if scanned pixel is part of background
                labels_array[i][j] = 0;
                continue;
            }

            else { //if scanned pixel (A) is not a background pixel
                //if D (corner) has a label
                if (left_neighbor == 0 && upper_neighbor == 0 /*&& corner_neighbor == 0*/) {//no labels
                    label++;
                    labeled_image.SetPixel(i, j, label);
                    labels_array[i][j] = label;
                }
                else if (corner_neighbor != 0) {
                    labels_array[i][j] = corner_neighbor;

                    labeled_image.SetPixel(i, j, corner_neighbor); //set the pixel to it's corner label
                }
                else if (upper_neighbor != 0 && left_neighbor == 0) {//if corner labeled
                    labels_array[i][j] = upper_neighbor;
                    labeled_image.SetPixel(i, j, upper_neighbor); //if upper has a label
                }
                else if (left_neighbor != 0 && upper_neighbor == 0) {//if left labeled
                    labels_array[i][j] = left_neighbor;
                    labeled_image.SetPixel(i, j, left_neighbor);
                }
                //if both left and upper neighbors labeled
                else if (left_neighbor != 0 && upper_neighbor != 0) {
                    
                    //set current to either (this case, left neighbor)
                    labels_array[i][j] = upper_neighbor;
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
    cout << "done" << endl;

    //second pass to resolve equivalences
    for (int i = 1; i < labeled_image.num_rows(); i++) {
        for (int j = 1; j < labeled_image.num_columns(); j++) {
            if (labeled_image.GetPixel(i, j) == 0) {
                continue;
            }
            else {
                labeled_image.SetPixel(i, j, equiv_class.find(labeled_image.GetPixel(i, j)));
                labels_array[i][j] = equiv_class.find(labels_array[i][j]);
            }
        }
    }
 
    orig_image.SetNumberGrayLevels(equiv_class.getSize());
    cout << label << endl;
    cout << "done" << endl;

    ofstream outfile("SeqLabelOutput.txt");
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (labels_array[i][j] != 0) {
                outfile << labels_array[i][j] << " ";
            }
        }
    }
    
    if (!WriteImage(out_file, labeled_image)){
        cout << "Can't write to file " << out_file << endl;
        return false;
    }
    cout << "done" << endl;
    cout << equiv_class.getSize() << endl;

    return true;
}

void ComputeAttributes(string in_file, string database, string out_file) {
    /*Takes a labeled image as input, and computes the following:
     1) Object's label
     2) Row position of the center
     3) Column position of the center
     4) Minimum moment of inertia
     5) Orientation (angle in degrees b/t the axis of minimum inertia and the vertical axis
     
     */
    
    vector<int> labels;
    vector<int>::const_iterator iter;
    vector<string> attributes;
    
    Image modeled_image;
    Image labeled_image;
    
    
    if (!ReadImage(in_file, &labeled_image) ) {
        cout <<"Can't open file " << in_file << endl;
    }
    
    size_t rows = labeled_image.num_rows();
    size_t cols = labeled_image.num_columns();
    
    modeled_image.AllocateSpaceAndSetSize(rows, cols);
    modeled_image.SetNumberGrayLevels(255);
    
    for (int i = 1; i < rows-1; i++) {
        for (int j = 1; j < cols-1; j++) {
            modeled_image.SetPixel(i, j, labeled_image.GetPixel(i, j));
        }
    }
    
    for (int i = 1; i < rows-1; i++) {
        for (int j = 1; j < cols-1; j++) {
            int label = labeled_image.GetPixel(i, j);
            if (label != 0) {
                iter = find(labels.begin(), labels.end(), label);
                if (iter == labels.end()) {
                    labels.push_back(label);
                }
            }
        }//close for j
    }//close for i
    
    cout << "Number of objects " << labels.size() << endl;
    for (int t = 0; t < labels.size(); t++) {
        cout << labels[t] << " " << endl;
        long int area = 0;
        double x_bar = 0.0, y_bar = 0.0; //fist moment x and y
        double a = 0.0, b = 0.0, c = 0.0; //used to calculate the second moments
        double a_prime = 0.0, b_prime = 0.0, c_prime = 0.0; //used to calculate the second moments
        double theta = 0.0; //ratio between b and a-c which is tan(2*theta) = b/(a-c)
        double rho = 0.0; // rho = y*cos(theta) - x*sin(theta)
        double Emin = 0.0, Emax = 0.0;
        double sin2theta = 0.0, cos2theta = 0.0;

        
        for (int i = 1; i < rows-1; i++) {
            for (int j = 1; j < cols-1; j++) {
                //if a label is located
                if (labeled_image.GetPixel(i, j) == labels[t]) {
                    area++; //update the object's area counter
                    
                    x_bar = x_bar + i; //the sum of all i values that had a label
                    y_bar = y_bar + j; //the sum of all j values that had a label
                    
                    a_prime = a_prime + (i * i);
                    b_prime = b_prime + (2 * (i * j));
                    c_prime = c_prime + (j * j);
                }
            }//close for j
        }//close for i
        
        //calculate the first moment
        x_bar = x_bar/area; //gives the x coordinate of the object's position (center of area)
        y_bar = y_bar/area; //gives the y coordinate of the object's position (center of area)
        
        //calculate the second moment
        a = a_prime + (((x_bar * x_bar) * area) - (2 * (x_bar * x_bar) * area));
        b = b_prime - (2 * (x_bar * y_bar * area));
        c = c_prime + (((y_bar * y_bar) * area) - (2 * (y_bar * y_bar) * area));
        
        //calculate the line
        theta = (atan2(b, c-a))/2;
        rho = y_bar*cos(theta) - x_bar*sin(theta);
        
        sin2theta = b/sqrt((b * b) + ((a-c) * (a-c)));
        cos2theta = (a-c)/sqrt((b * b) + ((a-c) * (a-c)));
        Emin = ((a+c)/2) - (((a-c)/2)*cos2theta) - (b/2)*sin2theta;
        Emax = -1 * Emin;
        //roundness = emin/emax
        
        cout << "area " << area << endl;
        cout << "center " << x_bar << "," << y_bar << endl;
        cout << a << " " << b << " " << c << endl;
        
        int line_color = 0;
        //white dot for the position of the object
        if (labels.size() < 3) {
            modeled_image.SetPixel(x_bar, y_bar, 255);
            modeled_image.SetPixel(x_bar-1, y_bar-1, 255);
            modeled_image.SetPixel(x_bar-1, y_bar, 255);
            modeled_image.SetPixel(x_bar, y_bar-1, 255);
            modeled_image.SetPixel(x_bar+1, y_bar+1, 255);
            modeled_image.SetPixel(x_bar+1, y_bar, 255);
            modeled_image.SetPixel(x_bar, y_bar+1, 255);
            modeled_image.SetPixel(x_bar+1, y_bar-1, 255);
            modeled_image.SetPixel(x_bar-1, y_bar+1, 255);
            line_color = 255;
        }
        else if (labels.size() >= 3) {
            modeled_image.SetPixel(x_bar, y_bar, 0);
            modeled_image.SetPixel(x_bar-1, y_bar-1, 0);
            modeled_image.SetPixel(x_bar-1, y_bar, 0);
            modeled_image.SetPixel(x_bar, y_bar-1, 0);
            modeled_image.SetPixel(x_bar+1, y_bar+1, 0);
            modeled_image.SetPixel(x_bar+1, y_bar, 0);
            modeled_image.SetPixel(x_bar, y_bar+1, 0);
            modeled_image.SetPixel(x_bar+1, y_bar-1, 0);
            modeled_image.SetPixel(x_bar-1, y_bar+1, 0);
            line_color = 0;
        }
        //short line segment originating from object center
        int x, y = 0;
        x = x_bar + 30;
        y = ((x * sin(theta)) + rho)/cos(theta); //manipulate line function to get the y coordinate
        DrawLine(x_bar, y_bar, x, y, line_color, &modeled_image);

        
        string line;
        ostringstream stream;
        
        stream << labels[t] << " " << x_bar << " " << y_bar << " " << Emin << " " << theta << endl;
        line = stream.str();
        attributes.push_back(line);
    }
    

    
    ofstream outfile;
    outfile.open("database.txt");
    for (int i = 0; i < attributes.size(); ++i) {
        outfile << attributes[i];
    }
    outfile.close();
    
    if (!WriteImage(out_file, modeled_image)){
        cout << "Can't write to file " << out_file << endl;
    }
    
}

void ObjectRecognition(string in_file, string database, string out_file) {
    
}



int main(int argc, char **argv){
    
    if (argc!=5) {
        printf("Usage: %s file1 file2\n", argv[0]);
        return 0;
    }
    const string prog(argv[1]);
    const string arg1(argv[2]);
    const string arg2(argv[3]);
    const string arg3(argv[4]);
    
    cout << prog << endl;
    
    string input_file = arg1;
    int threshold = atoi(argv[3]); //140 is best
    string output_file = arg3;
    
    if (prog == "p1") {
        ConvertToBinary(input_file, output_file, threshold);
    }
    
    else if (prog == "p2") {
        LabelImage(input_file, arg2);
    }
    
    else if (prog == "p3") {
        ComputeAttributes(input_file, arg2, output_file);
    }
    
    /*
    Image orig_image;
    
    if (!ReadImage(input_file, &orig_image)) {
        cout <<"Can't open file " << input_file << endl;
        return 0;
    }
    
    for (int i = 0; i < orig_image.num_rows(); i++) {
        for (int j = 0; j < orig_image.num_columns(); j++) {
            if (orig_image.GetPixel(i, j) < threshold) {
                orig_image.SetPixel(i, j, 0); //make the pixel black
            }
            else { //if the pixel is > threshold
                orig_image.SetPixel(i, j, 255); //make the pixel white
            }
        }
    }
    orig_image.SetNumberGrayLevels(1);//set number of colors in PGM as 1 in the header
    
    
    // Draw a line from (0, 0) to (100, 240) having gray-value
    // 200.
    //DrawLine(0, 0, 100, 240, 0, &an_image);
    
    //DrawLine(0, 0, 50, 100, 0, &an_image);
    
    if (!WriteImage(output_file, orig_image)){
        cout << "Can't write to file " << output_file << endl;
        return 0;
    }
     */
    
    return 0;
}
