# 2D-Object-Recognition

Elizabeth Kelly		
Computational Vision HW 2		
September 23, 2016		


*************************************************************************
This program is a vision system that performs 2D object recognition in   
gray-level .pgm images. Given an input image, the program will compute  
the attributes (position and orientation) of the objects. When given a  
second image, the program will use those attributes to determine if the  
objects from the first image are present.				    

i. All parts of the assignment were completed. <br />
ii. No bugs were encountered. <br />
iii. & iv. Complete instructions of how to run this program and the 
needed input and output files are listed below. <br />

Before running this program, please issue the following in the command 
line:

1) make clean <br />
2) make all

*************************************************************************
This program is separated into four parts:

Program 1: Converts a gray-level image to a binary image using a 
threshold value.

To run this program, enter the following on the command line:<br />

./p1 "input gray-level image" "threshold number" "output binary image"<br />

** The threshold value that works best for all three input files
(two_objects.pgm, many_objects_1.pgm, and many_objects_2.pgm) is 115.

The filename for the output binary image is binary_output.pgm<br />

*************************************************************************
Program 2: Takes a binary image as input and segments the image into 
several connected regions using the sequential labeling algorithm. The
output image should have each object be a single, distinct gray-level 
based on the label they were given during the raster scan. 

** Note: Some of the object gray-levels may look similar, but the labels 
are, in fact, distinct.

To run this program, enter the following on the command line:<br />

./p2 "input binary image" "output labeled image"<br />

The filename for the input binary image is binary_output.pgm<br />
The filename for the output labeled image is labeled_output.pgm<br />

*************************************************************************
Program 3: Takes a labeled image as input and computes the attributes of
the objects and generates a database that includes the object’s label, 
area, roundness, row position of its center, column position of its
center, its minimum moment of inertia, and its orientation.

To run this program, enter the following on the command line:<br />

./p3 "input labeled image" "output database" "output image"<br />

The filename for the input labeled image is labeled_output.pgm<br />
The filename for the output database is database.pgm<br />
The filename for the output image is attributes.pgm<br />

*************************************************************************
Program 4: Takes a labeled image and a database as input and recognizes
objects in the image from the database. It outputs an image that shows
the position and orientation (dot and line segment) of the matched
object(s) from the database.

To run this program, enter the following on the command line:<br />

./p4 "input labeled image" "input database" "output image"<br />

The filename for the input labeled image is labeled_output.pgm<br />
The filename for the input database is database.pgm<br />
The filename for the output image is matched_objects.pgm<br />

*************************************************************************
File list:

Makefile	Makefile with ‘make clean’ and ‘make all’ commands <br />
p1.cpp		Converts a gray-level image to a binary image.<br />
p2.cpp		Segments a binary image into connected components.<br />
p3.cpp		Computes attributes of a sequentially labeled image.<br />
p4.cpp		Performs object recognition using an attributes database.<br />
Image.h		Header for a class representing a 2D gray-scale image.<br />
Image.cpp 	Implementation of representing 2D gray-scale image.<br />
DisjSets.h	Header for the Disjoint Sets data structure.<br />
DisjSets.cpp	Implementation of the Disjoint Sets data structure.<br />
*************************************************************************