/*********************************************************************************
*     File Name           :     LineDetection.cpp
*     Created By          :     tiboiser
*     Creation Date       :     [2020-12-21 10:35]
*     Last Modified       :     [2020-12-21 11:09]
*     Description         :     LineDetection class cpp
**********************************************************************************/

#include "ProfileDetection.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace std;
using namespace cv;
using namespace objdetect;


/***********************************
 * Class : ObjectBoundingBoxDetection
 **********************************/

template <typename T>
std::vector<T> linspace(double start, double end, double num)
{
    std::vector<T> linspaced;

    if (0 != num)
    {
        if (1 == num)
        {
            linspaced.push_back(static_cast<T>(start));
        }
        else
        {
            double delta = (end - start) / (num - 1);

            for (auto i = 0; i < (num - 1); ++i)
            {
                linspaced.push_back(static_cast<T>(start + delta * i));
            }
            // ensure that start and end are exactly the same as the input
            linspaced.push_back(static_cast<T>(end));
        }
    }
    return linspaced;
}

string type2str(int type) {
    string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch (depth) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
    }

    r += "C";
    r += (chans + '0');

    return r;
}

int ProfileDetection::detect(Mat& m) {

	return 1;
}

vector<int> ProfileDetection::profile(int d, cv::Rect boundingBox, cv::Mat& img) {
	vector<int> leftProfiles = vector<int>();
    vector<int> rightProfiles = vector<int>();
	Mat subImg = img(boundingBox);
    imshow("dxighnsdign", subImg);
    //cout << subImg << endl << endl;
    //cout << type2str(subImg.type()) << endl;
    
    vector<int> indices = linspace<int>(1, boundingBox.height-1, d / 2);

    for (int i = 0; i < d / 2; i++) {
        //find
        const uchar* row = subImg.ptr<uchar>(indices[i]);
        bool foundLeft = false;
        bool foundRight = false;
        for (int col = 0; col < subImg.cols; col++) {
            //cout << indices[i] << endl;
            if (row[col] > 0 && !foundLeft) {
                leftProfiles.push_back(col);
                foundLeft = true;
            }
        }

        for (int col = subImg.cols-1; col > 0; --col) {
            if (row[col] > 0 && !foundRight) {
                rightProfiles.push_back(subImg.cols-1 - col);
                foundRight = true;
            }
        }
    }
    
    vector<int> profiles(leftProfiles);
    profiles.insert(profiles.end(), rightProfiles.begin(), rightProfiles.end()); //Concatenation des deux vecteurs
    
    return profiles;    
}

vector<int> ProfileDetection::profile(int d, cv::Mat& img) {
    vector<int> leftProfiles = vector<int>();
    vector<int> rightProfiles = vector<int>();
    //cout << subImg << endl << endl;
    //cout << type2str(subImg.type()) << endl;

    vector<int> indices = linspace<int>(0, img.rows-1, d / 2);

    for (int i = 0; i < d / 2; i++) {
        //find
        const uchar* row = img.ptr<uchar>(indices[i]);
        bool foundLeft = false;
        bool foundRight = false;
        for (int col = 0; col < img.cols; col++) {
            //cout << indices[i] << endl;
            if (row[col] > 0 && !foundLeft) {
                leftProfiles.push_back(col);
                foundLeft = true;
            }
        }

        for (int col = img.cols-1; col >= 0; --col) {
            if (row[col] > 0 && !foundRight) {
                rightProfiles.push_back(img.cols-1 - col);
                foundRight = true;
            }
        }
    }

    vector<int> profiles(leftProfiles);
    profiles.insert(profiles.end(), rightProfiles.begin(), rightProfiles.end()); //Concatenation des deux vecteurs

    return profiles;
}

vector<vector<int>> ProfileDetection::getTemplatesProfiles(int d) {
    vector<vector<int>> profiles = vector<vector<int>>();
    vector<Mat> templates = vector<Mat>();
    templates.push_back(imread("imgs/templates/barre.png"));
    templates.push_back(imread("imgs/templates/blanche_bas.png"));
    templates.push_back(imread("imgs/templates/blanche_haut.png"));
    templates.push_back(imread("imgs/templates/cle_sol.png"));
    templates.push_back(imread("imgs/templates/croche.png"));
    templates.push_back(imread("imgs/templates/dieze_armature.png"));
    templates.push_back(imread("imgs/templates/noire_bas.png"));
    templates.push_back(imread("imgs/templates/noire_haut.png"));
    templates.push_back(imread("imgs/templates/quatre.png"));
    templates.push_back(imread("imgs/templates/ronde.png"));
    templates.push_back(imread("imgs/templates/silence.png"));

    for (int i = 0; i < templates.size(); i++) {
        cvtColor(templates[i], templates[i], cv::COLOR_BGR2GRAY);
        templates[i] = 255 - templates[i];
        //cout << templates[i] << endl;
        profiles.push_back(profile(d, templates[i]));
    }
    return profiles;
}

void ProfileDetection::profileClassification(int d, Rect line, vector<Rect> boundingBoxes, Mat& img) {
    Mat kernel = getStructuringElement(MORPH_RECT, Size(1, 3), Point(0, 1));
    Mat erodedImg;
    erode(img, erodedImg, kernel);
    Mat lineImg = erodedImg(line);
    vector<vector<int>> profiles = vector<vector<int>>();
    for (int i = 0; i < boundingBoxes.size(); i++) {
        profiles.push_back(profile(d, boundingBoxes[i], lineImg));
    }

    for (int i = 0; i < profiles.size(); i++) {
        for (int j = 0; j < profiles[i].size(); j++) {
            cout << profiles[i][j] << ' ';
        }
        cout << endl;
    }
    cout << endl;

    vector<vector<int>> templatesProfiles = getTemplatesProfiles(d);
    for (int i = 0; i < templatesProfiles.size(); i++) {
        for (int j = 0; j < templatesProfiles[i].size(); j++) {
            cout << templatesProfiles[i][j] << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

/* Default Constructor of LineDetection class */
ProfileDetection::ProfileDetection(void) : ObjectDetection::ObjectDetection() {

}

/* Default destructor of LineDetection class */
ProfileDetection::~ProfileDetection(void) {

}
