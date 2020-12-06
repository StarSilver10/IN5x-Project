#include <iostream>
#include <opencv2/opencv.hpp>
#include "plot.h"
#include <fstream>

using namespace std;
using namespace cv;



int main()
{
	Mat img = imread("C:/Users/nigro/source/repos/OpenCVTest/OpenCVTest/src/Mary.jpg");
	namedWindow("image", WINDOW_NORMAL);
	imshow("image", img);

	Mat imgBin;
	threshold(img, imgBin, 127, 255, THRESH_BINARY_INV); // Binarisation
	namedWindow("binarised image", WINDOW_NORMAL);
	imshow("binarised image", imgBin);
	
	Mat horizProj;
	reduce(imgBin, horizProj, 1, REDUCE_SUM, CV_64F); //Projection verticale
	cout << "horizProj = " << endl << " " << horizProj << endl << endl;//Affichage du résultat
	writeCSV("projec.csv", horizProj); //Export en csv pour affichage
	
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