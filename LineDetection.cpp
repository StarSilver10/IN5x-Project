/*********************************************************************************
*     File Name           :     LineDetection.cpp
*     Created By          :     tiboiser
*     Creation Date       :     [2020-12-21 10:35]
*     Last Modified       :     [2020-12-21 11:09]
*     Description         :     LineDetection class cpp
**********************************************************************************/

#include "LineDetection.hpp"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;
using namespace objdetect;

/***********************************
 * Class : LineDetection
 **********************************/

/* horizontal projection
 * Project the maximum continuous zeros on a line into the returned vector<int> */
vector<int> LineDetection::maxContinuousHorizontalProjection (const Mat& m) {
	vector<int> h_proj = vector<int>(m.rows);
	for (int i = 0; i < m.rows; i++) {
		int p = 0, pMax = 0;
		const unsigned char* mi = m.ptr<unsigned char>(i);
		for (int j = 0; j < m.cols; j++) {
			if (mi[j] != 0) {
				p++;
			} else {
				if (p > pMax) {
					pMax = p;
				}
				p = 0;
			}
		}
		if (p > pMax) {
			pMax = p;
		}
		h_proj[i] = pMax;
	}
	return h_proj;
}

/* Select wide black lines among the projection proj and merge continuous lines
 * Detect horizontal sublines and stores rows number into a vector<int>. */
vector<int> LineDetection::selectSubLines(vector<int>& proj, int cols) {
	vector<int> lines = vector<int>();

	int found = 0;
	for (int i = 0; i < proj.size(); i++) {
		if (found == 0 && ((float)proj[i]) / cols > detectionRatio()) {
			lines.push_back(i);
			found = 1;
		} else {
			found = 0;
		}
	}

	return lines;
}

int LineDetection::detect(Mat& m) {
	vector<int> proj = maxContinuousHorizontalProjection(m);

	vector<int> subLines = selectSubLines(proj, m.cols);
	if (subLines.size() % 5 != 0) {
		cout << "ERROR line dection, missed one or more lines" << endl;
		return 0;
	}

	int i = 0;
	while (i < ((double) subLines.size()) / 5) {
		vector<int> v = vector<int>();
		for (int j = 5 * i; j < 5 * i + 5; j++) {
			v.push_back(subLines[j]);
		}
		_subLines.push_back(v);

		// Mean of the 2 and and 3 subline row.
		_positions.push_back(Point(
					((double) subLines[5 * i + 2] + subLines[5 * i + 3]) / 2.0,
					0));
		i++;
	}

	generateBoundingBoxs(m, proj);

	return 1;
}

/* Generate bounding boxs around lines
 * Use the maxContinuousHorizontalProjection output and the lines positions.
 * Return 1 if successful, 0 otherwise */
int LineDetection::generateBoundingBoxs(Mat& m, vector<int>& proj) {
	for (int i = 0; i < _positions.size(); i++) {
		// On rows
		int k = 0;
		while (proj[_positions[i].x + k] != 0) {
			k--;
		}
		int rowStart = _positions[i].x + k + 1;

		k = 0;
		while (proj[_positions[i].x + k] != 0) {
			k++;
		}
		int height = _positions[i].x + k - 1 - rowStart;

		// On cols
		k = 0;
		int emptyRowSection = 0;
		while (!emptyRowSection) {
			for (int j = rowStart; j < rowStart + height; j++) {
				if (m.at<unsigned char>(j,k) != 0) {
					emptyRowSection = 1;
				}
			}
			k++;
		}
		int colStart = k - 1;

		k = m.cols - 1;
		emptyRowSection = 0;
		while (!emptyRowSection) {
			for (int j = rowStart; j < rowStart + height; j++) {
				if (m.at<unsigned char>(j,k) != 0) {
					emptyRowSection = 1;
				}
			}
			k--;
		}
		int width = k + 1 - colStart;

		_boundingBoxs.push_back(Rect(colStart, rowStart, width, height));
	}

	return 1;
}

/* Default Constructor of LineDetection class */
LineDetection::LineDetection(void) : ObjectDetection::ObjectDetection() {
	_detectionRatio = 0.5f;
	_subLines = vector<vector<int>>();
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

vector<vector<int>> LineDetection::subLines(void) {
	return _subLines;
}
