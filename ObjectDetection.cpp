/*********************************************************************************
 * File Name           :     ObjectDetection.cpp
 * Created By          :     tiboiser
 * Creation Date       :     [2020-12-19 17:30]
 * Last Modified       :     [2020-12-21 10:39]
 * Description         :     Objects detection class
 **********************************************************************************/

#include <iostream>
#include <fstream>

#include "ObjectDetection.hpp"

using namespace std;
using namespace cv;
using namespace objdetect;

/***********************************
 * Class : ObjectDetection
 **********************************/


/* Default Constructor of ObjectDetection class */
ObjectDetection::ObjectDetection(void) {
	_positions = vector<Point>();
	_boundingBoxs = vector<Rect>();
}

/* Default Destructor of ObjectDetection class */
ObjectDetection::~ObjectDetection(void) {
}

/* Get _arrDetection attribute */
vector<Point> ObjectDetection::positions(void) {
	return _positions;
}

/* Get _arrDetection attribute */
vector<Rect> ObjectDetection::boundingBoxs(void) {
	return _boundingBoxs;
}


