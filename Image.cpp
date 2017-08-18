//
//  Image.cpp
//  VisionProject2
//
//  Created by Elizabeth Kelly on 9/12/16.
//  Copyright © 2016 Elizabeth Kelly. All rights reserved.
//
// Class for representing a 2D gray-scale image,
// with support for reading/writing pgm images.
// To be used in Computer Vision class.

#include "Image.h"
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

namespace ComputerVisionProjects {
    
    Image::Image(const Image &an_image){
        AllocateSpaceAndSetSize(an_image.num_rows(), an_image.num_columns());
        SetNumberGrayLevels(an_image.num_gray_levels());
        
        for (size_t i = 0; i < num_rows(); ++i)
            for (size_t j = 0; j < num_columns(); ++j){
                SetPixel(i,j, an_image.GetPixel(i,j));
            }
    }
    
    Image::~Image(){
        DeallocateSpace();
    }
    
    void Image::AllocateSpaceAndSetSize(size_t num_rows, size_t num_columns) {
        if (pixels_ != nullptr) DeallocateSpace();
        pixels_ = new int*[num_rows];
        for (size_t i = 0; i < num_rows; ++i)
            pixels_[i] = new int[num_columns];
        
        num_rows_ = num_rows;
        num_columns_ = num_columns;
    }
    
    void
    Image::DeallocateSpace() {
        for (size_t i = 0; i < num_rows_; i++)
            delete pixels_[i];
        delete pixels_;
        pixels_ = nullptr;
        num_rows_ = 0;
        num_columns_ = 0;
    }
    
    bool ReadImage(const string &filename, Image *an_image) {
        if (an_image == nullptr) abort();
        FILE *input = fopen(filename.c_str(),"rb");
        if (input == 0) {
            cout << "ReadImage: Cannot open file" << endl;
            return false;
        }
        
        // Check for the right "magic number".
        char line[1024];
        if (fread(line, 1, 3, input) != 3 || strncmp(line,"P5\n",3)) {
            fclose(input);
            cout << "ReadImage: Expected .pgm file" << endl;
            return false;
        }
        
        // Skip comments.
        do
            fgets(line, sizeof line, input);
        while(*line == '#');
        
        // Read the width and height.
        int num_columns,num_rows;
        sscanf(line,"%d %d\n", &num_columns, &num_rows);
        an_image->AllocateSpaceAndSetSize(num_rows, num_columns);
        
        
        // Read # of gray levels.
        fgets(line, sizeof line, input);
        int levels;
        sscanf(line,"%d\n", &levels);
        an_image->SetNumberGrayLevels(levels);
        
        // read pixel row by row.
        for (int i = 0; i < num_rows; ++i) {
            for (int j = 0;j < num_columns; ++j) {
                const int byte=fgetc(input);
                if (byte == EOF) {
                    fclose(input);
                    cout << "ReadImage: short file" << endl;
                    return false;
                }
                an_image->SetPixel(i, j, byte);
            }
        }
        
        fclose(input);
        return true;
    }
    
    bool WriteImage(const string &filename, const Image &an_image) {
        FILE *output = fopen(filename.c_str(), "w");
        if (output == 0) {
            cout << "WriteImage: cannot open file" << endl;
            return false;
        }
        const size_t num_rows = an_image.num_rows();
        const size_t num_columns = an_image.num_columns();
        const size_t colors = an_image.num_gray_levels();
        
        // Write the header.
        fprintf(output, "P5\n"); // Magic number.
        fprintf(output, "#\n");  // Empty comment.
        fprintf(output, "%zu %zu\n%03zu\n", num_columns, num_rows, colors);
        //fprintf(output, "%d %d\n%03d\n", num_columns, num_rows, colors);

        
        for (int i = 0; i < num_rows; ++i) {
            for (int j = 0; j < num_columns; ++j) {
                const int byte = an_image.GetPixel(i , j);
                if (fputc(byte,output) == EOF) {
                    fclose(output);
                    cout << "WriteImage: could not write" << endl;
                    return false;
                }
            }
        }
        
        fclose(output);
        return true;
    }
    
    // Implements the Bresenham's incremental midpoint algorithm;
    // (adapted from J.D.Foley, A. van Dam, S.K.Feiner, J.F.Hughes
    // "Computer Graphics. Principles and practice",
    // 2nd ed., 1990, section 3.2.2);
    void
    DrawLine(int x0, int y0, int x1, int y1, int color,
             Image *an_image) {
        if (an_image == nullptr) abort();
        
#ifdef SWAP
#undef SWAP
#endif
#define SWAP(a,b) {a^=b; b^=a; a^=b;}
        
        const int DIR_X = 0;
        const int DIR_Y = 1;
        
        // Increments: East, North-East, South, South-East, North.
        int incrE, incrNE,incrS,incrSE,incrN;
        int d;         /* the D */
        int x,y;       /* running coordinates */
        int mpCase;    /* midpoint algorithm's case */
        int done;      /* set to 1 when done */
        
        int xmin = x0;
        int xmax = x1;
        int ymin = y0;
        int ymax = y1;
        
        int dx = xmax - xmin;
        int dy = ymax - ymin;
        int dir;
        
        if (dx * dx > dy * dy) {  // Horizontal scan.
            dir=DIR_X;
            if (xmax < xmin) {
                SWAP(xmin, xmax);
                SWAP(ymin , ymax);
            }
            dx = xmax - xmin;
            dy = ymax - ymin;
            
            if (dy >= 0) {
                mpCase = 1;
                d = 2 * dy - dx;
            } else {
                mpCase = 2;
                d = 2 * dy + dx;
            }
            
            incrNE = 2 * (dy - dx);
            incrE = 2 * dy;
            incrSE = 2 * (dy + dx);
        } else {// vertical scan.
            dir = DIR_Y;
            if (ymax < ymin) {
                SWAP(xmin, xmax);
                SWAP(ymin, ymax);
            }
            dx = xmax - xmin;
            dy = ymax-ymin;
            
            if (dx >=0 ) {
                mpCase = 1;
                d = 2 * dx - dy;
            } else {
                mpCase = 2;
                d = 2 * dx + dy;      
            }
            
            incrNE = 2 * (dx - dy);
            incrE = 2 * dx;
            incrSE = 2 * (dx + dy);
        }
        
        /// Start the scan.
        x = xmin;
        y = ymin;
        done = 0;
        
        while (!done) {
            an_image->SetPixel(x,y,color);
            
            // Move to the next point.
            switch(dir) {
                case DIR_X: 
                    if (x < xmax) {
                        switch(mpCase) {
                            case 1:
                                if (d <= 0) {
                                    d += incrE;  
                                    x++;
                                } else {
                                    d += incrNE; 
                                    x++; 
                                    y++;
                                }
                                break;
                                
                            case 2:
                                if (d <= 0) {
                                    d += incrSE; 
                                    x++; 
                                    y--;
                                } else {
                                    d += incrE;  
                                    x++;
                                }
                                break;
                        } 
                    } else {
                        done=1;
                    }     
                    break;
                    
                case DIR_Y: 
                    if (y < ymax) {
                        switch(mpCase) {
                            case 1:
                                if (d <= 0) {
                                    d += incrE;  
                                    y++;
                                } else {
                                    d += incrNE; 
                                    y++; 
                                    x++;
                                }
                                break;
                                
                            case 2:
                                if (d <= 0) {
                                    d += incrSE; 
                                    y++; 
                                    x--;
                                } else {
                                    d += incrE;  
                                    y++;
                                }
                                break;
                        } // mpCase
                    } // y < ymin 
                    else {
                        done=1;
                    }
                    break;    
            }
        }
    }
    
}  // namespace ComputerVisionProjects