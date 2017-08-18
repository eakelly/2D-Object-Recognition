//
//  Image.hpp
//  VisionProject2
//
//  Created by Elizabeth Kelly on 9/12/16.
//  Copyright © 2016 Elizabeth Kelly. All rights reserved.
//
// Class for representing a 2D gray-scale image,
// with support for reading/writing pgm images.
// To be used in Computer Vision class.

#ifndef COMPUTER_VISION_IMAGE_H_
#define COMPUTER_VISION_IMAGE_H_

#include <cstdlib>
#include <string>
#include <stdio.h>
#include <cstring>


namespace ComputerVisionProjects {
    
    // Class for representing a gray-scale image.
    // Sample usage:
    //   Image one_image;
    //   one_image.AllocateSpaceAndSetSize(100, 200);
    //   one_image.SetNumberGrayLevels(255);
    //   // Creates and image such that each pixel is 150.
    //   for (int i = 0; i < 100; ++i)
    //     for (int j = 0; j < 200; ++j)
    //       one_image.SetPixel(i, j, 150);
    //   WriteImage("output_file.pgm", an_image);
    //   // See image_demo.cc for read/write image.
    class Image {
    public:
        Image(): num_rows_(0), num_columns_(0), num_gray_levels_(0), pixels_(nullptr) { };
        
        Image(const Image &an_image);
        Image& operator=(const Image &an_image) = delete;
        
        ~Image();
        
        // Sets the size of the image to the given
        // height (num_rows) and columns (num_columns).
        void AllocateSpaceAndSetSize(size_t num_rows, size_t num_columns);
        
        size_t num_rows() const { return num_rows_; }
        size_t num_columns() const { return num_columns_; }
        size_t num_gray_levels() const { return num_gray_levels_; }
        void SetNumberGrayLevels(size_t gray_levels) {
            num_gray_levels_ = gray_levels;
        }
        
        // Sets the pixel in the image at row i and column j
        // to a particular gray_level.
        void SetPixel(size_t i, size_t j, int gray_level) {
            if (i >= num_rows_ || j >= num_columns_) abort();
            pixels_[i][j] = gray_level;
        }
        
        int GetPixel(size_t i, size_t j) const {
            if (i >= num_rows_ || j >= num_columns_) abort();
            return pixels_[i][j];
        }
        
    private:
        void DeallocateSpace();
        
        size_t num_rows_;
        size_t num_columns_;
        size_t num_gray_levels_;
        int **pixels_;
    };
    
    // Reads a pgm image from file input_filename.
    // an_image is the resulting image.
    // Returns true if  everyhing is OK, false otherwise.
    bool ReadImage(const std::string &input_filename, Image *an_image);
    
    // Writes image an_iamge into the pgm file output_filename.
    // Returns true if  everyhing is OK, false otherwise.
    bool WriteImage(const std::string &output_filename, const Image &an_image);
    
    //  Draws a line of given gray-level color from (x0,y0) to (x1,y1);
    //  an_image is the output_image.
    // IMPORTANT: (x0,y0) and (x1,y1) can lie outside the image 
    //   boundaries, so SetPixel() should check the coordinates passed to it.
    void DrawLine(int x0, int y0, int x1, int y1, int color,
                  Image *an_image);
    
}  // namespace ComputerVisionProjects

#endif  // COMPUTER_VISION_IMAGE_H_
