//
//  p4.cpp
//  VisionProject2
//
//  Created by Elizabeth Kelly on 9/20/16.
//  Copyright © 2016 Elizabeth Kelly. All rights reserved.
//

#include "Image.h"
#include <cstdio>
#include <cstdlib>
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
 * Structure that stores the attributes of an object in the image.
 **/
struct Object {
    int label;
    double area, roundness, x_bar, y_bar, Emin, theta;
    /**
     * Funtion that prints the attributes of an object in the image. Used for debugging.
     **/
    void printInfo()  {
        cout << "label " << label << endl;
        cout << "area " << area << endl;
        cout << "roundness " << roundness << endl;
        cout << "x_bar " << x_bar << endl;
        cout << "y_bar " << y_bar << endl;
        cout << "Emin " << Emin << endl;
        cout << "theta " << theta << endl;
        cout << endl;
    }
};
/**
 * Function that recognizes objects in an image given attributes from a database.
 * in_file is the labeled .pgm file to scan.
 * database is the file that contains the attributes of the objects to be recognized.
 * out_file is the image with the attributes computed for the recognized objects.
 * Returns true if the image was read, processed, and written.
 **/
void ObjectRecognition(string in_file, string database, string out_file) {
    
    Image labeled_image;
    Image output_image;
    
    vector<int> labels;
    vector<int>::const_iterator iter;
    vector<string> attributes;
    
    int objects_matched = 0;
    
    if (!ReadImage(in_file, &labeled_image) ) {
        cout <<"Can't open file " << in_file << endl;
    }
    
    size_t rows = labeled_image.num_rows();
    size_t cols = labeled_image.num_columns();
    
    output_image.AllocateSpaceAndSetSize(rows, cols);
    output_image.SetNumberGrayLevels(255);
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            output_image.SetPixel(i, j, labeled_image.GetPixel(i, j));
        }
    }
    
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
    
    ifstream db;
    vector<Object> objectsToRecognize;
    Object object;
    db.open(database);
    while (db >> object.label >> object.area >> object.roundness >> object.x_bar >> object.y_bar >> object.Emin >> object.theta) {
        objectsToRecognize.push_back(object);
    }
    db.close();
    
    for (int t = 0; t < labels.size(); t++) {
        double area = 0;
        float x_bar = 0.0, y_bar = 0.0; //fist moment x and y
        double a = 0.0, b = 0.0, c = 0.0; //used to calculate the second moments
        double a_prime = 0.0, b_prime = 0.0, c_prime = 0.0; //used to calculate the second moments
        double theta = 0.0; //ratio between b and a-c which is tan(2*theta) = b/(a-c)
        double theta_max = 0.0; //used to calculated Emax; is = theta + (pi/2)
        double rho = 0.0; // rho = y*cos(theta) - x*sin(theta)
        double Emin = 0.0, Emax = 0.0;
        double roundness = 0.0;
        double sin_theta = 0.0, cos_theta = 0.0; //used to compute Emin
        double sin_theta_max = 0.0, cos_theta_max = 0.0; //used to compute Emax
        
        
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
        theta_max = theta + (3.141592/2);
        sin_theta_max = sin(theta_max);
        cos_theta_max = cos(theta_max);
        sin_theta = sin(theta);
        cos_theta = cos(theta);
        rho = y_bar*cos(theta) - x_bar*sin(theta);
        
        
        Emin = (a*(sin_theta*sin_theta)) - (b*sin_theta*cos_theta) + (c*(cos_theta*cos_theta));
        Emax = (a*(sin_theta_max*sin_theta_max)) - (b*sin_theta_max*cos_theta_max) + (c*(cos_theta_max*cos_theta_max));
        
        roundness = Emin/Emax;
        
        for (int r = 0; r < objectsToRecognize.size(); r++) {
            double area_diff = abs(objectsToRecognize[r].area - area); //difference in area
            double roundness_diff = abs(objectsToRecognize[r].roundness - roundness); //diff. in roundness
            double Emin_diff = abs(objectsToRecognize[r].Emin - Emin);
            
            if (area_diff < 260 && roundness_diff < 0.017 && Emin_diff < 265000) {
                
                int line_color = 0;
                for (int r = x_bar-1; r < x_bar+1; r++) {
                    for (int c = y_bar-1; c < y_bar+1; c++) {
                        if (labels[t] <= 180) {
                            //white dot and line for the position of the object
                            output_image.SetPixel(r, c, 255);
                            line_color = 255;
                        }
                        else {
                            //black dot for the position of the object
                            output_image.SetPixel(r, c, 0);
                            line_color = 0;
                        }
                    }
                }
                //short line segment originating from object center in the matched objects
                DrawLine(x_bar, y_bar, (x_bar+(50*cos_theta)), (y_bar+(50*sin_theta)), line_color, &output_image);
                
                objects_matched++;
                
            }
        }
        
    }
    cout << "There is/are " << objects_matched << " match(es)." << endl;
    
    if (!WriteImage(out_file, output_image)){
        cout << "Can't write to file " << out_file << endl;
    }
}

int main(int argc, char **argv){
    
    if (argc!=4) {
        cout << "Usage: <input labeled image> <input database> <output image> " << endl;
        return 0;
    }
    const string input_file(argv[1]);
    const string database(argv[2]);
    const string output_file(argv[3]);
    
    ObjectRecognition(input_file, database, output_file);
    
    return 0;
}