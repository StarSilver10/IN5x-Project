/*********************************************************************************
*     File Name           :     LineDetection.cpp
*     Created By          :     tiboiser
*     Creation Date       :     [2020-12-21 10:35]
*     Last Modified       :     [2020-12-21 11:09]
*     Description         :     LineDetection class cpp
**********************************************************************************/

#include "ObjectBoundingBoxDetection.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace std;
using namespace cv;
using namespace objdetect;


void _writeCSV(string filename, Mat m)
{
	ofstream myfile;
	myfile.open(filename.c_str());
	myfile << cv::format(m, cv::Formatter::FMT_CSV) << std::endl;
	myfile.close();
}

bool isOne(int x) {
	return x == 1;
}

//Draws a line on image
void MyLine(Mat img, Point start, Point end)
{
	int thickness = 1;
	int lineType = LINE_8;
	line(img,
		start,
		end,
		Scalar(255, 0, 0),
		thickness,
		lineType);
}

/***********************************
 * Class : ObjectBoundingBoxDetection
 **********************************/

int ObjectBoundingBoxDetection::detect(Mat& m) {
	
	return 1;
}

vector<Rect> ObjectBoundingBoxDetection::searchBoundingBoxes(Mat& m, Rect boundingBox){
	vector<Rect> boundingBoxes = vector<Rect>();

	//Erosion de l'image pour supprimer les lignes
	Mat kernel = getStructuringElement(MORPH_RECT, Size(1, 3), Point(0, 1));
	Mat erodedImg;
	erode(m, erodedImg, kernel);
	
	//On r�cup�re la sous-image � partir du rectangle
	Mat subImg = erodedImg(boundingBox);
	
	//Projection horizontale de l'image
	Mat horizProj;
	reduce(subImg, horizProj, 0, REDUCE_SUM, CV_32SC1); 
	
	vector<int> lines = vector<int>(horizProj.cols);
	//lines va contenir 1 si le pixel de horizProj est sup�rieur � 1
	int count = 0;
	for (int col = 0; col < horizProj.cols; ++col) {			
		if (horizProj.at<int>(0, col) > 0) {
			lines[col] = 1;
		}
	}

	vector<int> diff = vector<int>(horizProj.cols);
	//diff va contenir la valeur absolue de la diff�rence entre toutes les valeurs adjacentes dans lines
	for (int col = 0; col < lines.size()-1; ++col) {
		diff[col] = abs(lines[col + 1] - lines[col]);
	}
	
	vector<int> find = vector<int>();
	//Find va contenir les indices o� diff est �gal � 1
	vector<int>::iterator iter = diff.begin();
	while ((iter = find_if(iter, diff.end(), isOne)) != diff.end())
	{
		// Do something with iter
		int idx = distance(diff.begin(), iter);
		find.push_back(idx);
		iter++;
	}

	find.push_back(subImg.cols);
	
	//On parcourt les indices trouv�s
	for (int i = 0; i < find.size(); i+=2) {		
		//Sous image � partir des nouveaux indices
		Mat newSubImg = subImg(Rect(find[i], 0 , find[i + 1] - find[i], subImg.rows));			

		//Projection verticale afin de trouver les indices de lignes des objets
		Mat vertProj;
		reduce(newSubImg, vertProj, 1, REDUCE_SUM, CV_32SC1);

		vector<int> cols = vector<int>(vertProj.rows);
		//cols va contenir 1 si le pixel de vertProj est sup�rieur � 1
		for (int row = 0; row < vertProj.rows; ++row) {
			if (vertProj.at<int>(row, 0) > 0) {
				cols[row] = 1;
			}
		}

		vector<int> _diff = vector<int>(vertProj.rows);
		//diff va contenir la valeur absolue de la diff�rence entre toutes les valeurs adjacentes dans cols
		for (int row = 0; row < cols.size() - 1; ++row) {
			_diff[row] = abs(cols[row + 1] - cols[row]);
		}

		vector<int> _find = vector<int>();
		//Find va contenir les indices o� _diff est �gal � 1
		vector<int>::iterator _iter = _diff.begin();
		while ((_iter = find_if(_iter, _diff.end(), isOne)) != _diff.end())
		{
			int idx = distance(_diff.begin(), _iter);
			_find.push_back(idx);
			_iter++;
		}

		if (_find.empty()) {
			_find.push_back(0);
			_find.push_back(cols.size());
		}
		else {
			if (_find.size() == 1) {
				_find.push_back(cols.size());
			}
		}
		
		//Nouveau rectangle
		Rect newRect = Rect(find[i], _find[0], find[i + 1] - find[i], _find[1] - _find[0]);
		
		boundingBoxes.push_back(newRect);

		//Test si le nouveau rectangle est proche du pr�c�dent
		if (boundingBoxes.size() >= 2) {
			Rect oldRect = boundingBoxes[boundingBoxes.size() - 2];
			if (abs(oldRect.x + oldRect.width - newRect.x) < (mergeRectanglesThreshold*boundingBox.width)) {
				//Si oui on fusionne les deux rectangles

				if (oldRect.y <= newRect.y) {
					Rect mergedRectangle = Rect(oldRect.x, oldRect.y, newRect.x - oldRect.x + newRect.width, newRect.y - oldRect.y + newRect.height);
					boundingBoxes.pop_back();
					boundingBoxes.pop_back();
					boundingBoxes.push_back(mergedRectangle);

					rectangle(subImg,
						Point(mergedRectangle.x, mergedRectangle.y),
						Point(mergedRectangle.x + mergedRectangle.width, mergedRectangle.y + mergedRectangle.height),
						Scalar(255, 0, 0),
						LINE_4);
				}
				else {
					Rect mergedRectangle = Rect(oldRect.x, newRect.y, newRect.x - oldRect.x + newRect.width, oldRect.y - newRect.y + oldRect.height);
					//Rect mergedRectangle = Rect(oldRect.x, oldRect.y, 50, 50);
					boundingBoxes.pop_back();
					boundingBoxes.pop_back();
					boundingBoxes.push_back(mergedRectangle);

					rectangle(subImg,
						Point(mergedRectangle.x, mergedRectangle.y),
						Point(mergedRectangle.x + mergedRectangle.width, mergedRectangle.y + mergedRectangle.height),
						Scalar(255, 0, 0),
						LINE_4);
				}

			}
			else {

				rectangle(subImg,
					Point(find[i], _find[0]),
					Point(find[i + 1], _find[1]),
					Scalar(255, 0, 0),
					LINE_4);
			}
		}
		else {

			rectangle(subImg,
				Point(find[i], _find[0]),
				Point(find[i + 1], _find[1]),
				Scalar(255, 0, 0),
				LINE_4);
		}
		
		//Pour le rythme il y a deux rectangles (4/4,3/4), donc 4 indices
		if (_find.size() == 4) {
			Rect _newRect = Rect(find[i], _find[2], find[i + 1] - find[i], _find[3] - _find[2]);
			boundingBoxes.push_back(_newRect);

			rectangle(subImg,
				Point(find[i], _find[2]),
				Point(find[i + 1], _find[3]),
				Scalar(255, 0, 0),
				LINE_4);
		}
	}
	imshow("Sub image with rectangles "+lineNumber, subImg);
	_boundingBoxs = boundingBoxes;
	return boundingBoxes;
}

/* Default Constructor of LineDetection class */
ObjectBoundingBoxDetection::ObjectBoundingBoxDetection(double _mergeRectanglesThreshold , int _lineNumber) : ObjectDetection::ObjectDetection() {
	mergeRectanglesThreshold = _mergeRectanglesThreshold;
	lineNumber = _lineNumber;
}

/* Default destructor of LineDetection class */
ObjectBoundingBoxDetection::~ObjectBoundingBoxDetection(void) {

}
