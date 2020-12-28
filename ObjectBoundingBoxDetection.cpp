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
	//On récupère la sous-image à partir du rectangle
	Mat subImg = erodedImg(boundingBox);
	imshow("subimg", subImg);
	//Projection horizontale de l'image
	Mat horizProj;
	reduce(subImg, horizProj, 0, REDUCE_SUM, CV_32SC1); 
	
	vector<int> lines = vector<int>(horizProj.cols);
	//lines va contenir 1 si le pixel de horizProj est supérieur à 1
	int count = 0;
	for (int col = 0; col < horizProj.cols; ++col) {			
		if (horizProj.at<int>(0, col) > 0) {
			lines[col] = 1;
		}
	}

	vector<int> diff = vector<int>(horizProj.cols);
	//diff va contenir la valeur absolue de la différence entre toutes les valeurs adjacentes dans lines
	for (int col = 0; col < lines.size()-1; ++col) {
		diff[col] = abs(lines[col + 1] - lines[col]);
	}
	
	vector<int> find = vector<int>();
	//Find va contenir les indices où diff est égal à 1
	vector<int>::iterator iter = diff.begin();
	while ((iter = find_if(iter, diff.end(), isOne)) != diff.end())
	{
		// Do something with iter
		int idx = distance(diff.begin(), iter);
		find.push_back(idx);
		iter++;
	}

	find.push_back(subImg.cols);
	
	for (int i = 0; i < find.size(); i+=2) {		
		
		Mat newSubImg = subImg(Rect(find[i], 0 , find[i + 1] - find[i], subImg.rows));

		Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 1), Point(1, 0));
		Mat newSubImgEroded = newSubImg;
		//erode(newSubImg, newSubImgEroded, kernel);
		//imshow("eroded2"+i, newSubImgEroded);				

		Mat vertProj;
		reduce(newSubImgEroded, vertProj, 1, REDUCE_SUM, CV_32SC1);

		vector<int> cols = vector<int>(vertProj.rows);
		//lines va contenir 1 si le pixel de vertProj est supérieur à 1
		for (int row = 0; row < vertProj.rows; ++row) {
			if (vertProj.at<int>(row, 0) > 0) {
				cols[row] = 1;
			}
		}

		vector<int> _diff = vector<int>(vertProj.rows);
		//diff va contenir la valeur absolue de la différence entre toutes les valeurs adjacentes dans cols
		for (int row = 0; row < cols.size() - 1; ++row) {
			_diff[row] = abs(cols[row + 1] - cols[row]);
		}

		vector<int> _find = vector<int>();
		//Find va contenir les indices où _diff est égal à 1
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

		//for (int i = 0; i < _find.size(); i++) cout << _find[i] << endl;
		//cout << endl;
		
		Rect newRect = Rect(find[i], _find[0], find[i + 1] - find[i], _find[1] - _find[0]);
		//cout << newRect.x << ' ' << newRect.y << ' ' << newRect.width << ' ' << newRect.height << endl;
		boundingBoxes.push_back(newRect);
		rectangle(subImg,
			Point(find[i], _find[0]),
			Point(find[i + 1], _find[1]),
			Scalar(255, 0, 0),
			LINE_4);
		
		//Pour le rythme il y a deux rectangles (4/4,3/4), donc 4 indices
		if (_find.size() == 4) {
			Rect _newRect = Rect(find[i], _find[2], find[i + 1] - find[i], _find[3] - _find[2]);
			//cout << _newRect.x << ' ' << _newRect.y << ' ' << _newRect.width << ' ' << _newRect.height << endl;
			boundingBoxes.push_back(_newRect);

			rectangle(subImg,
				Point(find[i], _find[2]),
				Point(find[i + 1], _find[3]),
				Scalar(255, 0, 0),
				LINE_4);
		}
	}
	imshow("Sub image with rectangles"+rand(), subImg);
	_boundingBoxs = boundingBoxes;
	return boundingBoxes;
}

/* Default Constructor of LineDetection class */
ObjectBoundingBoxDetection::ObjectBoundingBoxDetection(void) : ObjectDetection::ObjectDetection() {
	
}

/* Default destructor of LineDetection class */
ObjectBoundingBoxDetection::~ObjectBoundingBoxDetection(void) {

}
