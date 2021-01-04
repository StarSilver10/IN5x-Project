#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <cmath>
#include "ProfileDetection.hpp"
#include "ObjectBoundingBoxDetection.hpp"
#include "LineDetection.hpp"
#include "MidiConversion.hpp"
#include "MusicNote.hpp"
#include "DensityDetection.hpp"

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
	
	vector<vector<objdetect::MusicNote>> musicNotesGlobalProfile = vector<vector<objdetect::MusicNote>>();
	vector<vector<objdetect::MusicNote>> musicNotesGlobalDensity = vector<vector<objdetect::MusicNote>>();
	
	//Pour chaque ligne de la partition
	for (int i = 0; i < lD.boundingBoxs().size(); i++) {
		Rect _rect = lD.boundingBoxs()[i];
		//cout << _rect.x << ' ' << _rect.y << ' ' << _rect.width << ' ' << _rect.height << endl;
		cout << "New Line" << endl;

		//On détecte les boudingBoxs des objets dans cette ligne
		ObjectBoundingBoxDetection oD = ObjectBoundingBoxDetection(0.00714, i);
		oD.searchBoundingBoxes(imgBin, lD.boundingBoxs()[i]);
		
		////Affichage des rectangles
		//for (int j = 0; j < oD.boundingBoxs().size(); j++) {
		//	Rect rect = oD.boundingBoxs()[j];
		//	cout << rect.x << ' ' << rect.y << ' ' << rect.width << ' ' << rect.height << endl;			
		//}
		//cout << endl;

		//On effectue une classification par profil de chacun de ces objets
		ProfileDetection profileDetection = ProfileDetection();
		vector<noteType> resultatsProfile = profileDetection.profileClassification(20, lD.boundingBoxs()[i], oD.boundingBoxs(), imgBin);

		//On effectue une classification par densité de chacun de ces objets
		DensityDetection densityDetection = DensityDetection();
		vector<noteType> resultatsDensity = densityDetection.densityClassification(6, 6, lD.boundingBoxs()[i], oD.boundingBoxs(), imgBin);

		//On convertit cette classification par Profil en MusicNotes qui vont être converties en MIDI
		musicNotesGlobalProfile.push_back(profileDetection.getMusicNotesFromProfilClassification(resultatsProfile, lD.boundingBoxs()[i], oD.boundingBoxs(), lD.subLines()[i]));

		//On convertit cette classification par Densité en MusicNotes qui vont être converties en MIDI
		musicNotesGlobalDensity.push_back(densityDetection.getMusicNotesFromDensityClassification(resultatsDensity, lD.boundingBoxs()[i], oD.boundingBoxs(), lD.subLines()[i]));
	}

	////On concatène toutes les notes de chaque ligne (Profile)
	//vector<objdetect::MusicNote> allNotes = musicNotesGlobalProfile[0];
	//if (musicNotesGlobalProfile.size() > 1) {
	//	for (int i = 1; i < musicNotesGlobalProfile.size(); i++) {
	//		allNotes.insert(allNotes.end(), musicNotesGlobalProfile[i].begin(), musicNotesGlobalProfile[i].end());
	//	}
	//}

	//On concatène toutes les notes de chaque ligne (Densité)
	vector<objdetect::MusicNote> allNotes = musicNotesGlobalDensity[0];
	if (musicNotesGlobalDensity.size() > 1) {
		for (int i = 1; i < musicNotesGlobalDensity.size(); i++) {
	    	allNotes.insert(allNotes.end(), musicNotesGlobalDensity[i].begin(), musicNotesGlobalDensity[i].end());
		}
	}
	cout << "TAILLE " << allNotes.size() << endl;
	////Affichage des résulats
	//static const char* typeNames[] = { "barre", "blanche_bas", "blanche_haut" , "cle_sol","croche","dieze_armature","noire_bas","noire_haut","quatre","ronde","silence","noire_pointee_bas","barre_fin" };
	//for (int j = 0; j < resultats.size(); j++) {
	//	cout << typeNames[resultats[j]] << endl;
	//}
	//cout << endl;

	//On convertit les notes en MIDI
	MidiConversion::notesToMidi("mary5.midi", allNotes, 100);

	waitKey(0);
	return 0;
}
