# 2D-Object-Recognition

This program is a vision system that performs 2D object recognition in gray-level .pgm images. Given an input image, the program will compute the attributes (position and orientation) of the objects. When given a second image, the program will use those attributes to determine if the objects from the first image are present.

## Table of Contents
 - [Setup](#setup)
 - [Running the Program](#running-the-program)
   - [Part 1](#part-1)
   - [Part 2](#part-2) 
   - [Part 3](#part-3)
   - [Part 4](#part-4)
 - [File List](#file-list)
   
## Setup

Before running this program, run the following commands in a terminal window:

  ```none
  make clean
  make all
  ```
  
*************************************************************************			    

## Running the Program

This program is separated into four parts:

### Part 1

Converts a gray-level image to a binary image using a threshold value.

To run this program, enter the following in a terminal window:

```./p1 <input gray-level image> <threshold value> binary_output.pgm```

**The threshold value that works best for all three input files
(two_objects.pgm, many_objects_1.pgm, and many_objects_2.pgm) is 115**

The input file is ```two_objects.pgm```.
The filename for the output binary image is binary_output.pgm.

*************************************************************************

### Part 2

Takes a binary image as input and segments the image into several connected regions using the sequential labeling algorithm. The output image should have each object be a single, distinct gray-level based on the label they were given during the raster scan. 

To run this program, enter the following on the command line:<br />

```./p2 binary_output.pgm labeled_output.pgm```

The filename for the input binary image is ```binary_output.pgm```.

The filename for the output labeled image is ```labeled_output.pgm```.

**Note: Some of the object gray-levels may look similar, but the labels 
are distinct.**

*************************************************************************

### Part 3

Takes a labeled image as input and computes the attributes of the objects and generates a database that includes the object’s label, area, roundness, row position of its center, column position of its center, its minimum moment of inertia, and its orientation.

To run this program, enter the following in a terminal window:

```./p3 labeled_output.pgm database.txt attributes.pgm```

The filename for the input labeled image is ```labeled_output.pgm```
The filename for the output database is ```database.txt```
The filename for the output image is ```attributes.pgm```

*************************************************************************

### Part 4

Takes a labeled image and a database as input and recognizes objects in the image from the database. It outputs an image that shows the position and orientation (dot and line segment) of the matched object(s) from the database.

To run this program, enter the following in a terminal window:

```./p4 labeled_output.pgm database.txt matched_objects.pgm```

The filename for the input labeled image is ```labeled_output.pgm```.
The filename for the input database is ```database.txt```.
The filename for the output image is ```matched_objects.pgm```.

*************************************************************************

## File List

| File Name          | Purpose               |
| ------------------- |:---------------------:|
| Makefile     | Makefile with ‘make clean’ and ‘make all’ commands |
| p1.cpp    | Converts a gray-level image to a binary image       |
| p2.cpp       | Segments a binary image into connected components          |
| p3.cpp       | Computes attributes of a sequentially labeled image          |
| p4.cpp       | Performs object recognition using an attributes database          |
| Image.h       | Header for a class representing a 2D gray-scale image          |
| Image.cpp       | Implementation of representing 2D gray-scale image          |
| DisjSets.h       | Header for the Disjoint Sets data structure          |
| DisjSets.cpp       | Implementation of the Disjoint Sets data structure          |

