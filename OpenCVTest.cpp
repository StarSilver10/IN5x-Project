#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "ProfileDetection.hpp"
#include "ObjectBoundingBoxDetection.hpp"
#include "LineDetection.hpp"
#include "MidiConversion.hpp"
#include "MusicNote.hpp"

using namespace std;
using namespace cv;
using namespace objdetect;

int main()
{
	Mat img = imread("imgs/mary.jpg");
	Mat imgGray;
	cvtColor(img,imgGray, cv::COLOR_BGR2GRAY);
	Mat imgBin;
	threshold(imgGray, imgBin, 175, 255, THRESH_BINARY_INV);

	LineDetection lD = LineDetection(0.5f); // Calls constructor
	lD.detect(imgBin); // On détecte les lignes et sous-lignes de la partition
	
	vector<vector<objdetect::MusicNote>> musicNotesGlobal = vector<vector<objdetect::MusicNote>>();

	
	//Pour chaque ligne de la partition
	for (int i = 0; i < lD.boundingBoxs().size(); i++) {
		Rect _rect = lD.boundingBoxs()[i];
		//cout << _rect.x << ' ' << _rect.y << ' ' << _rect.width << ' ' << _rect.height << endl;
		cout << "New Line" << endl;

		//On détecte les boudingBoxs des objets dans cette ligne
		ObjectBoundingBoxDetection oD = ObjectBoundingBoxDetection(0.00714, i);
		oD.searchBoundingBoxes(imgBin, lD.boundingBoxs()[i]);
		
		//Affichage des rectangles
		/*for (int j = 0; j < oD.boundingBoxs().size(); j++) {
			Rect rect = oD.boundingBoxs()[j];
			cout << rect.x << ' ' << rect.y << ' ' << rect.width << ' ' << rect.height << endl;			
		}
		cout << endl;*/

		//On effectue une classification par profil de chacun de ces objets
		ProfileDetection profileDetection = ProfileDetection();
		vector<noteType> resultats = profileDetection.profileClassification(20, lD.boundingBoxs()[i], oD.boundingBoxs(), imgBin);

		//On convertit certte classification en MusicNotes qui vont être converties en MIDI
		musicNotesGlobal.push_back(profileDetection.getMusicNotesFromClassification(resultats, lD.boundingBoxs()[i], oD.boundingBoxs(), lD.subLines()[i]));
	}

	//On concatène toutes les notes de chaque ligne
	vector<objdetect::MusicNote> allNotes = musicNotesGlobal[0];
	if (musicNotesGlobal.size() > 1) {
		for (int i = 1; i < musicNotesGlobal.size(); i++) {
			allNotes.insert(allNotes.end(), musicNotesGlobal[i].begin(), musicNotesGlobal[i].end());
		}
	}

	//On convertit les notes en MIDI
	MidiConversion::notesToMidi("mary.midi", allNotes, 100);

	waitKey(0);
	return 0;
}
