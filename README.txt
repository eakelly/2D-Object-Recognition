*******************************
* Elizabeth Kelly		*	
* Computational Vision HW 2	*			
* CSCI 493			*	
* Professor Stamos		*
* September 23, 2016		*
*******************************

*************************************************************************
This program is a vision system that performs 2D object recognition in   
gray-level .pgm images. Given an input image, the program will compute  
the attributes (position and orientation) of the objects. When given a  
second image, the program will use those attributes to determine if the 
objects from the first image are present.				    

i. All parts of the assignment were completed.
ii. No bugs were encountered.
iii. & iv. Complete instructions of how to run this program and the 
needed input and output files are listed below.

Before running this program, please issue the following in the command 
line:

1) make clean
2) make all

*************************************************************************
This program is separated into four parts:
*************************************************************************
Program 1: Converts a gray-level image to a binary image using a 
threshold value.

To run this program, enter the following on the command line:

./p1 <input gray-level image> <threshold number> <output binary image>

** The threshold value that works best for all three input files
(two_objects.pgm, many_objects_1.pgm, and many_objects_2.pgm) is 115.

The filename for the output binary image is binary_output.pgm

*************************************************************************
Program 2: Takes a binary image as input and segments the image into 
several connected regions using the sequential labeling algorithm. The
output image should have each object be a single, distinct gray-level 
based on the label they were given during the raster scan. 

** Note: Some of the object gray-levels may look similar, but the labels 
are, in fact, distinct.

To run this program, enter the following on the command line:

./p2 <input binary image> <output labeled image>

The filename for the input binary image is binary_output.pgm
The filename for the output labeled image is labeled_output.pgm

*************************************************************************
Program 3: Takes a labeled image as input and computes the attributes of
the objects and generates a database that includes the object’s label, 
area, roundness, row position of its center, column position of its
center, its minimum moment of inertia, and its orientation.

To run this program, enter the following on the command line:

./p3 <input labeled image> <output database> <output image>

The filename for the input labeled image is labeled_output.pgm
The filename for the output database is database.txt
The filename for the output image is attributes.pgm

*************************************************************************
Program 4: Takes a labeled image and a database as input and recognizes
objects in the image from the database. It outputs an image that shows
the position and orientation (dot and line segment) of the matched
object(s) from the database.

To run this program, enter the following on the command line:

./p4 <input labeled image> <input database> <output image>

The filename for the input labeled image is labeled_output.pgm
The filename for the input database is database.txt
The filename for the output image is matched_objects.pgm

*************************************************************************
File list:
*************************************************************************
Makefile	Makefile with ‘make clean’ and ‘make all’ commands
p1.cpp		Converts a gray-level image to a binary image.
p2.cpp		Segments a binary image into connected components.
p3.cpp		Computes attributes of a sequentially labeled image.
p4.cpp		Performs object recognition using an attributes database.
Image.h		Header for a class representing a 2D gray-scale image.
Image.cpp 	Implementation of representing 2D gray-scale image.
DisjSets.h	Header for the Disjoint Sets data structure.
DisjSets.cpp	Implementation of the Disjoint Sets data structure.
*************************************************************************