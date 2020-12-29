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
#include <numeric>
#include <vector>

using namespace std;
using namespace cv;
using namespace objdetect;

int sequencing = 0;


/***********************************
 * Class : ObjectBoundingBoxDetection
 **********************************/

//Renvoie un vecteur de num indices également répartis entre start et end
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

noteType objectType(int type) {
    noteType _type = noteType::barre;
    switch (type) {
    case 0: _type = barre;
        break;
    case 1: _type = blanche_bas;
        break;
    case 2: _type = blanche_haut;
        break;
    case 3: _type = cle_sol;
        break;
    case 4: _type = croche;
        break;
    case 5: _type = dieze_armature;
        break;
    case 6: _type = noire_bas;
        break;
    case 7: _type = noire_haut;
        break;
    case 8: _type = quatre;
        break;
    case 9: _type = ronde;
        break;
    case 10: _type = silence;
        break;
    case 11: _type = noire_pointee_bas;
        break;
    case 12: _type = barre_fin;
        break;
    }
    return _type;
}

int getMostCloseLine(vector<int> lines, int y) {
    int minDistance = abs(y - lines[0]);
    int minIndex = 0;
    for (int i = 1; i < lines.size(); i++) {
        int newDistance = abs(y - lines[i]);
        if (newDistance < minDistance) {
            minDistance = newDistance;
            minIndex = i;
        }
    }
    return minIndex;
}

float lineToNote(int line) {
    switch (line) {
    case -1: return 5.5f;
    case 0: return 5.0f;
    case 1: return 4.5f;
    case 2: return 4.0f;
    case 3: return 3.5f;
    case 4: return 3.0f;
    case 5: return 2.5f;
    case 6: return 2.0f;
    case 7: return 1.5f;
    case 8: return 1.0f;

    }
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
    templates.push_back(imread("imgs/templates/noire_pointee_bas.png"));
    templates.push_back(imread("imgs/templates/barre_fin.png"));

    for (int i = 0; i < templates.size(); i++) {
        cvtColor(templates[i], templates[i], cv::COLOR_BGR2GRAY);
        templates[i] = 255 - templates[i];
        //cout << templates[i] << endl;
        profiles.push_back(profile(d, templates[i]));
    }
    return profiles;
}

vector<noteType> ProfileDetection::profileClassification(int d, Rect line, vector<Rect> boundingBoxes, Mat& img) {
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

    vector<vector<int>> probas = vector<vector<int>>(profiles.size());    

    for (int i = 0; i < profiles.size(); i++) {

        probas[i] = vector<int>(templatesProfiles.size());

        for (int j = 0; j < templatesProfiles.size(); j++) {
            vector<int> diff = vector<int>(d);
            //Calcul des différences entre le profil de l'objet et le profil de la classe
            for (int k = 0; k < d; k++) {
                diff[k] = abs(profiles[i][k] - templatesProfiles[j][k]);
            }
            
            int mean = std::accumulate(diff.begin(), diff.end(), 0.0)/diff.size();
            probas[i][j] = mean;
        }
    }

    for (int i = 0; i < probas.size(); i++) {
        for (int j = 0; j < probas[i].size(); j++) {
            cout << probas[i][j] << ' ';
        }
        cout << endl;
    }
    cout << endl;

    vector<noteType> resultats = vector<noteType>();
    for (int i = 0; i < probas.size(); i++) {
        vector<int>::iterator iter = probas[i].begin();
        //int min = min_element((vector<int>)(probas[i]).begin(), probas[i].end());
        int min = probas[i][0];
        int minIndex = 0;
        for (int j = 1; j < probas[i].size(); j++) {
            if (min > probas[i][j]) {
                min = probas[i][j];
                minIndex = j;
            }
        }
        resultats.push_back(objectType(minIndex));

        

        /*while ((iter = find_if(iter, probas[i].end(), isOne)) != probas[i].end())
        {
            // Do something with iter
            int idx = distance(probas[i].begin(), iter);
            resultats.push_back(objectType(idx));
            iter++;
        }*/
    }

    static const char* typeNames[] = { "barre", "blanche_bas", "blanche_haut" , "cle_sol","croche","dieze_armature","noire_bas","noire_haut","quatre","ronde","silence","noire_pointee_bas","barre_fin"};

    for (int j = 0; j < resultats.size(); j++) {
        cout << typeNames[resultats[j]] << endl;
    }
    cout << endl;

    return resultats;

}

vector<MusicNote> ProfileDetection::getMusicNotesFromClassification(vector<noteType> classification, Rect line, vector<Rect> boundingBoxes, vector<int> lines) {
    vector<MusicNote> notes = vector<MusicNote>();
    
    vector<int> linesComplete = vector<int>();
    for (int i = 0; i < lines.size()-1; i++) {
        linesComplete.push_back(lines[i]);
        linesComplete.push_back((lines[i] + lines[i + 1]) / 2);
    }
    linesComplete.push_back(lines[lines.size()-1]);
    for (int i = 0; i < boundingBoxes.size(); i++) {
        Rect boundingBox = boundingBoxes[i];
        int duration = 0;
        float note = -1.0f;
        int subline = 0;
        int y = 0;
        switch (classification[i]) {
        case blanche_bas:
            duration = 2;
            y = line.y + boundingBox.y;
            subline = getMostCloseLine(linesComplete, y) + 1;
            note = lineToNote(subline);
            notes.push_back(MusicNote(sequencing, duration, note));
            sequencing++;
            break;
        case blanche_haut:
            duration = 2;
            y = line.y + boundingBox.y + boundingBox.height;
            subline = getMostCloseLine(linesComplete, y) - 1;
            note = lineToNote(subline);
            notes.push_back(MusicNote(sequencing, duration, note));
            sequencing++;
            break;
        case noire_bas:
            duration = 1;
            y = line.y + boundingBox.y;
            subline = getMostCloseLine(linesComplete, y) + 1;
            note = lineToNote(subline);
            notes.push_back(MusicNote(sequencing, duration, note));
            sequencing++;
            break;
        case noire_haut:
            duration = 1;
            y = line.y + boundingBox.y + boundingBox.height;
            subline = getMostCloseLine(linesComplete, y) - 1;
            note = lineToNote(subline);
            notes.push_back(MusicNote(sequencing, duration, note));
            sequencing++;
            break;
        case croche:
            duration = 0.5;
            y = line.y + boundingBox.y + boundingBox.height;
            subline = getMostCloseLine(linesComplete, y) - 1;
            note = lineToNote(subline);
            notes.push_back(MusicNote(sequencing, duration, note));
            sequencing++;
            break;
        case ronde:
            duration = 4;
            y = line.y + boundingBox.y + boundingBox.height;
            subline = getMostCloseLine(linesComplete, y) - 1;
            note = lineToNote(subline);
            notes.push_back(MusicNote(sequencing, duration, note));
            sequencing++;
            break;
        case noire_pointee_bas:
            duration = 1.5;
            y = line.y + boundingBox.y + (0.06 * boundingBox.height);
            subline = getMostCloseLine(linesComplete, y) + 1;
            note = lineToNote(subline);
            notes.push_back(MusicNote(sequencing, duration, note));
            sequencing++;
            break;
        case silence:
            duration = 1;       
            note = lineToNote(subline);
            notes.push_back(MusicNote(sequencing, duration, note));
            sequencing++;
            break;
        }
       
    }
    return notes;
}



/* Default Constructor of LineDetection class */
ProfileDetection::ProfileDetection(void) : ObjectDetection::ObjectDetection() {

}

/* Default destructor of LineDetection class */
ProfileDetection::~ProfileDetection(void) {

}
