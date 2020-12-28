#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "ProfileDetection.hpp"
#include "ObjectBoundingBoxDetection.hpp"
#include "LineDetection.hpp"

using namespace std;
using namespace cv;
using namespace objdetect;

void writeCSV(string, Mat);

int main()
{
	Mat img = imread("C:/Users/nigro/source/repos/Project IN5x/Project IN5x/imgs/mary.jpg");
	Mat imgGray;
	cvtColor(img,imgGray, cv::COLOR_BGR2GRAY);
	Mat imgBin;
	threshold(imgGray, imgBin, 175, 255, THRESH_BINARY_INV);

	LineDetection lD = LineDetection(0.6f); // Calls constructor
	lD.detect(imgBin); // Detect lines and sublines
	
	for (int i = 0; i < lD.boundingBoxs().size(); i++) {
		Rect _rect = lD.boundingBoxs()[i];
		//cout << _rect.x << ' ' << _rect.y << ' ' << _rect.width << ' ' << _rect.height << endl;
		cout << "New Line" << endl;
		ObjectBoundingBoxDetection oD = ObjectBoundingBoxDetection();
		oD.searchBoundingBoxes(imgBin, lD.boundingBoxs()[i]);
		
		//Affichage des rectangles
		/*for (int j = 0; j < oD.boundingBoxs().size(); j++) {
			Rect rect = oD.boundingBoxs()[j];
			cout << rect.x << ' ' << rect.y << ' ' << rect.width << ' ' << rect.height << endl;			
		}*/

		ProfileDetection profileDetection = ProfileDetection();
		profileDetection.profileClassification(10, lD.boundingBoxs()[i], oD.boundingBoxs(), imgBin);
		cout << endl;		
	}

	waitKey(0);
	return 0;
}

void writeCSV(string filename, Mat m)
{
	ofstream myfile;
	myfile.open(filename.c_str());
	myfile << cv::format(m, cv::Formatter::FMT_CSV) << std::endl;
	myfile.close();
}
