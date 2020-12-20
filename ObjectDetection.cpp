/*********************************************************************************
 * File Name           :     ObjectDetection.cpp
 * Created By          :     tiboiser
 * Creation Date       :     [2020-12-19 17:30]
 * Last Modified       :     [2020-12-20 18:15]
 * Description         :     Objects detection class
 **********************************************************************************/

#include "ObjectDetection.hpp"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;
using namespace objdetect;

/***********************************
 * Class : ObjectDetection
 **********************************/


/* Default Constructor of ObjectDetection class */
ObjectDetection::ObjectDetection(void) {
	_arrDetection = vector<Point>();
	_boundingBoxs = vector<Rect>();
}

/* Default Destructor of ObjectDetection class */
ObjectDetection::~ObjectDetection(void) {
}

/* Get _arrDetection attribute */
vector<Point> ObjectDetection::arrDetection(void) {
	return _arrDetection;
}

/* Get _arrDetection attribute */
vector<Rect> ObjectDetection::boundingBoxs(void) {
	return _boundingBoxs;
}


/***********************************
 * Class : LineDetection
 **********************************/

/* horizontal projection
 * Project the maximum continuous zeros on a line into the returned vector<int> */
vector<int> LineDetection::maxContinuousHorizontalProjection (const Mat& m) {
	vector<int> h_proj = vector<int>(m.rows);
	for (int i = 0; i < m.rows; i++) {
		int p = 0, pMax = 0;
		const int* mi = m.ptr<int>(i);
		for (int j = 0; j < m.cols; j++) {
			if (mi[j] == 0) {
				p++;
			} else {
				if (p > pMax) {
					pMax = p;
				}
				p = 0;
			}
		}
		h_proj[i] = pMax;
	}
	return h_proj;
}

/* line Detection
 * Detect horizontal lines and stores rows number into a vector<int>. */
vector<int> LineDetection::detect(vector<int>& proj) {
	vector<int> lines = vector<int>();

	for (int i = 0; i < proj.size(); i++) {
		if (proj[i] / proj.size() > detectionRatio()) {
			lines.push_back(i);
		}
	}

	return lines;
}

/* Default Constructor of LineDetection class */
LineDetection::LineDetection(void) : ObjectDetection::ObjectDetection() {
	_detectionRatio = 0.5f;
}

/* Constructor of LineDetection class setting the detection pixel ratio black/white in a line */
LineDetection::LineDetection(float detectionRatio) : ObjectDetection::ObjectDetection() {
	_detectionRatio = detectionRatio;
}

/* Default destructor of LineDetection class */
LineDetection::~LineDetection(void) {
}

/* Getter of _detectionRatio */
float LineDetection::detectionRatio(void) {
	return _detectionRatio;
}
