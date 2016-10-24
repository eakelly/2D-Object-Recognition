//
//  p3.cpp
//  VisionProject2
//
//  Created by Elizabeth Kelly on 9/20/16.
//  Copyright © 2016 Elizabeth Kelly. All rights reserved.
//

#include "Image.h"
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
 * Function that computes the attributes of the objects in a labeled image.
 * in_file is the labeled .pgm file to scan.
 * database is the file that stores the necessary object attributes for recognition.
 * out_file is the image with computed object attributes (has dots and lines).
 * Returns true if the image was read, processed, and written.
 **/
void ComputeAttributes(string in_file, string database, string out_file) {
    //Takes a labeled image as input, and computes the following:
    //1) Object's label
    //2) Row position of the center
    //3) Column position of the center
    //4) Minimum moment of inertia
    //5) Orientation (angle in degrees b/t the axis of minimum inertia and the vertical axis
    
    vector<int> labels; //holds the distinct labels from the labeled image
    vector<int>::const_iterator iter; //
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
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            modeled_image.SetPixel(i, j, labeled_image.GetPixel(i, j));
        }
    }
    //gather all the different labels in the labeled image
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int label = labeled_image.GetPixel(i, j);
            if (label != 0) {
                iter = find(labels.begin(), labels.end(), label);
                if (iter == labels.end()) {
                    labels.push_back(label);
                }
            }
        }//close for j
    }//close for i
    
    for (int t = 0; t < labels.size(); t++) {
        double area = 0;
        double x_bar = 0.0, y_bar = 0.0; //fist moment x and y
        double a = 0.0, b = 0.0, c = 0.0; //used to calculate the second moments
        double a_prime = 0.0, b_prime = 0.0, c_prime = 0.0; //used to calculate the second moments
        double theta = 0.0; //ratio between b and a-c which is tan(2*theta) = b/(a-c)
        double rho = 0.0; // rho = y*cos(theta) - x*sin(theta)
        double Emin = 0.0;
        double Emax = 0.0;
        double sin_theta = 0.0, cos_theta = 0.0;
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                
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
        a = a_prime - ((x_bar * x_bar) * area);
        b = b_prime - (2 * (x_bar * y_bar * area));
        c = c_prime - ((y_bar * y_bar) * area);
        
        //calculate the line
        theta = (atan2(b, a-c))/2;
        double theta_degree = (theta * 180)/3.141592;
        double theta_max = theta + (3.141592/2);
        double sin_theta_max = sin(theta_max);
        double cos_theta_max = cos(theta_max);
        sin_theta = sin(theta);
        cos_theta = cos(theta);
        rho = y_bar*cos(theta) - x_bar*sin(theta);
        
        
        Emin = (a*(sin_theta*sin_theta)) - (b*sin_theta*cos_theta) + (c*(cos_theta*cos_theta));
        Emax = (a*(sin_theta_max*sin_theta_max)) - (b*sin_theta_max*cos_theta_max) + (c*(cos_theta_max*cos_theta_max));
        
        double roundness = Emin/Emax;
        
        int line_color = 0;
        
        for (int r = x_bar-1; r < x_bar+1; r++) {
            for (int c = y_bar-1; c < y_bar+1; c++) {
                if (labels[t] <= 180) {
                    //white dot and line for the position of the object
                    modeled_image.SetPixel(r, c, 255);
                    line_color = 255;
                }
                else {
                    //black dot for the position of the object
                    modeled_image.SetPixel(r, c, 0);
                    line_color = 0;
                }
            }
        }
        
        //short line segment originating from object center
        DrawLine(x_bar, y_bar, (x_bar+(50*cos_theta)), (y_bar+(50*sin_theta)), line_color, &modeled_image);
        
        
        string line;
        ostringstream stream;
        
        stream << labels[t] << " " << area << " " << roundness << " " << x_bar << " " << y_bar << " " << Emin << " " << theta_degree << endl;
        line = stream.str();
        attributes.push_back(line);
    }
    
    ofstream outfile;
    outfile.open(database);
    for (int i = 0; i < attributes.size(); ++i) {
        outfile << attributes[i];
    }
    outfile.close();
    
    if (!WriteImage(out_file, modeled_image)){
        cout << "Can't write to file " << out_file << endl;
    }
}//end ComputeAttributes


int main(int argc, char **argv){
    
    if (argc!=4) {
        cout << "Usage: <input labeled image> <output database> <output image> " << endl;
        return 0;
    }
    const string input_file(argv[1]);
    const string database(argv[2]);
    const string output_file(argv[3]);
    
    ComputeAttributes(input_file, database, output_file);
    
    return 0;
    
}