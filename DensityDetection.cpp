/*********************************************************************************
*     File Name           :     DensityDetection.cpp
*     Created By          :     rixte
*     Creation Date       :     [2020-12-21 10:35]
*     Last Modified       :     [2020-12-21 11:09]
*     Description         :     DensityDetection class cpp
**********************************************************************************/

#include "DensityDetection.hpp"
#include "ObjectBoundingBoxDetection.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;
using namespace cv;
using namespace objdetect;



//Permet de récupérer les indices Lorsque l'on souhaite diviser un tableau en un nombre de tailles égales
template <typename T>
vector<T> linspace(double start, double end, double num)
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


/***********************************
 * Class : DensityDetection
 **********************************/

DensityDetection::DensityDetection(){}

DensityDetection::~DensityDetection(){}

int DensityDetection::getMostCloseLine(vector<int> lines, int y) {
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

float DensityDetection::lineToNote(int line) {
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

void DensityDetection::Line(Mat img, Point start, Point end)
{
	int thickness = 1;
	int lineType = LINE_8;
	line(img,
		start,
		end,
		Scalar(0, 0, 255),
		thickness,
		lineType);
}

noteType DensityDetection::objectType(int type) {
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

int DensityDetection::detect(Mat& m) {

	return 1;
}

double DensityDetection::vectDist(vector<double> v1, vector<double> v2)
{
	double somme(0.f);
	for (int i = 0; i < v1.size(); i++)
	{
		double diff(pow(v1[i] - v2[i], 2));
		somme += diff;
	}
	double dist(sqrt(somme));
	return dist;
}

int DensityDetection::getMaxIndicesRow(vector<int> indicesType, int n, Mat img) {
	int soustraction(0);
	vector<int> indicesTemp;
	//Calcul de la distance entre chaque ligne
	for (int i = 0; i < indicesType.size(); i++) {
		if (i == indicesType.size() - 1) {
			soustraction = indicesType[i] - indicesType[i - 1];
		}
		else {
			soustraction = indicesType[i + 1] - indicesType[i];
		}
		indicesTemp.push_back(soustraction);
	}

	int max = *max_element(indicesTemp.begin(), indicesTemp.end());

	int check(n - 1);
	double test = static_cast<double>(img.rows) / static_cast<double>(max);

	if (test > check)
	{
		max++;
	}
	return max;
}

int DensityDetection::getMaxIndicesCol(std::vector<int> indicesType, int m, cv::Mat img)
{
	int soustraction(0);
	vector<int> indicesTemp;
	//Calcul de la distance entre chaque ligne
	for (int i = 0; i < indicesType.size(); i++) {
		if (i == indicesType.size() - 1) {
			soustraction = indicesType[i] - indicesType[i - 1];
		}
		else {
			soustraction = indicesType[i + 1] - indicesType[i];
		}
		indicesTemp.push_back(soustraction);
	}

	int max = *max_element(indicesTemp.begin(), indicesTemp.end());

	int check(m - 1);
	double test = static_cast<double>(img.cols) / static_cast<double>(max);

	if (test > check)
	{
		max++;
	}
	return max;
}

void DensityDetection::drawLine(int n, int m, Mat img, vector<int> indicesRows, vector<int> indicesCols)
{
	//Print zoning on the picture lines
	for (int i = 0; i < n; i++)
	{
		Point x(0.f, indicesRows[i]), y(img.cols, indicesRows[i]);
		Line(img, x, y);

		cout << "indices ligne " << indicesRows[i] << endl;
	}
	cout << endl;

	//Print zoning on the picture rows
	for (int i = 0; i < m; i++)
	{
		Point x(indicesCols[i], 0.f), y(indicesCols[i], img.rows);
		Line(img, x, y);

		cout << "indice colonne " << indicesCols[i] << endl;
	}

	imshow("Zoning image", img);

}

vector<double> DensityDetection::density(int n, int m, Rect boundingBox, Mat& img)
{
	//Création de la sous image qui correspond à un rectangke de l'image
	Mat subImg = img(boundingBox);
	//imshow("sous image", subImg);

	//Récupération des indices qui correspondent à la division du rectangle en un cadrillage (zoning)
	vector<int> indicesRows = linspace<int>(1, subImg.rows, n);
	vector<int> indicesCols = linspace<int>(1, subImg.cols, m);

	//écars entre les différents indices du cadrillage du zoning
	int N(getMaxIndicesRow(indicesRows, n, subImg));
	int M(getMaxIndicesCol(indicesCols, m, subImg));

	double somme(0.f);
	int compteur(0);

	vector<double> vectorDensity;
	//Parcours de chaque sous rectangle de la sous image
	for (int i = 0; i < subImg.rows; i += N)
	{
		for (int j = 0; j < subImg.cols; j += M)
		{
			somme = 0.f;
			for (int x = i; x < i + N; x++)
			{
				for (int y = j; y < j + M; y++)
				{
					if (x < subImg.rows && y < subImg.cols) // Permet de ne pas dépasser la taille de l'image
					{
						const uchar* rowShort = subImg.ptr<uchar>(x); // Pointeur sur les lignes du sous rectangle
						if (rowShort[y] > 0)
						{
							somme++;
						}
					}
				}
			}
			somme = somme / (N * M); // normalisation de la densité
			vectorDensity.push_back(somme);
			compteur++;
		}
	}

	//traitement des exceptions (barres dont le zoning en différentes parties n'est pas possible
	if (vectorDensity.size() != (n - 1) * (m - 1))
	{
		for (int i = 0; i < vectorDensity.size(); i++) 
		{
			vectorDensity[i] = 1; // Permet de créer un vecteur densité qui corres
		}
		while (vectorDensity.size() != (n - 1) * (m - 1))
		{
			vectorDensity.push_back(0); // Permet au vecteur d'avoir la même taille que les autres vecteurs densités
		}
	}
	return vectorDensity;
}

vector<double> DensityDetection::density(int n, int m, Mat& img) 
{
	//Récupération des indices qui correspondent à la division du rectangle en un cadrillage (zoning)
	vector<int> indicesRows = linspace<int>(0, img.rows, n);
	vector<int> indicesCols = linspace<int>(0, img.cols, m);
	
	//écars entre les différents indices du cadrillage du zoning
	int N(getMaxIndicesRow(indicesRows, n, img));
	int M(getMaxIndicesCol(indicesCols, m, img));

	double somme(0.f);
	int compteur(0);

	vector<double> vectorDensity;

	//Parcours de chaque sous rectangle de la sous image
	for (int i = 0; i < img.rows; i += N)
	{
		for (int j = 0; j < img.cols; j += M)
		{
			somme = 0.f;
			for (int x = i; x < i + N; x++)
			{
				for (int y = j; y < j + M; y++)
				{
					if (x < img.rows && y < img.cols) // Permet de ne pas dépasser la taille de l'image
					{
						const uchar* rowShort = img.ptr<uchar>(x); // Pointeur sur les lignes du sous rectangle
						if (rowShort[y] > 0)
						{
							somme++;
						}
					}
				}
			}
			somme = somme / (N * M); //normalisation de la densité
			vectorDensity.push_back(somme);
			compteur++;
		}
	}

	while (vectorDensity.size() != (n - 1) * (m - 1))
	{
		vectorDensity.push_back(0);
	}

	//drawLine(n, m, img, indicesRows, indicesCols);


	return vectorDensity;
}

vector<vector<double>> DensityDetection::getTemplatesDensity(int n, int m)
{
	vector<vector<double>> vectorDensity = vector<vector<double>>();

	//Création d'un tableau contenant toutes les images templates qui servent de modèle d'apprentissage
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

		cvtColor(templates[i], templates[i], cv::COLOR_BGR2GRAY);  //On convertis les images en niveau de gris
		templates[i] = 255 - templates[i]; //invertion des images
		vectorDensity.push_back(density(n, m, templates[i]));
	}
	return vectorDensity;
}

vector<noteType> DensityDetection::densityClassification(int n, int m, Rect line, vector<Rect> boundingBoxes, Mat& img)
{
	//On erode l'image pour enlever les sous-lignes de la partition
	Mat kernel = getStructuringElement(MORPH_RECT, Size(1, 3), Point(0, 1));
	Mat erodedImg;
	erode(img, erodedImg, kernel);
	//On récupère la sous image correspondant à la ligne que l'on veut classifier
	Mat lineImg = erodedImg(line);

	//On calcule les densités de chaque objet dans la ligne
	vector<vector<double>> densities = vector<vector<double>>();
	for (int i = 0; i < boundingBoxes.size(); i++) {
		densities.push_back(density(n, m, boundingBoxes[i], lineImg));
	}

	// //Affichage des densités
	//for (int i = 0; i < densities.size(); i++) {
	//	for (int j = 0; j < densities[i].size(); j++) {
	//		cout << densities[i][j] << ' ';
	//	}
	//	cout << endl;
	//}
	//cout << endl;

	//On calcule les densités des images templates
	vector<vector<double>> templatesDensities = getTemplatesDensity(n, m);

	////Affichage des densités
	//for (int i = 0; i < templatesDensities.size(); i++) {
	//	for (int j = 0; j < templatesDensities[i].size(); j++) {
	//		cout << templatesDensities[i][j] << ' ';
	//	}
	//	cout << endl;
	//}
	//cout << endl;

	vector<vector<double>> probas;

	int compteur(0);
	vector<double> vectorDist;
	//Calcule de la distance entre les sous images testés et tous les vecteurs d'apprentissages des images templates
	for (int i = 0; i < densities.size(); i++) {
		
			for (int j = 0; j < templatesDensities.size(); j++) {
				double dist = vectDist(densities[i], templatesDensities[j]);
				vectorDist.push_back(dist);
			}
			probas.push_back(vectorDist);

			for (int k = 0; k < templatesDensities.size(); k++)
			{
				vectorDist.pop_back();
			}
			compteur++;
	}

	////Affichage des distances
	//for (int i = 0; i < probas.size(); i++) {
	//	for (int j = 0; j < probas[i].size(); j++) {
	//		cout << probas[i][j] << ' ';
	//	}
	//	cout << endl;
	//}
	//cout << endl;


	//On calcule la classe des objets en récupérant la classe qui a la plus forte proba pour chaque objet
	vector<noteType> resultats = vector<noteType>();
	compteur = 0;
	for (int i = 0; i < probas.size(); i++)
	{
		int minElementIndex = std::min_element(probas[i].begin(), probas[i].end()) - probas[i].begin();
		resultats.push_back(objectType(minElementIndex));
	}

	 //Affichage des résulats de la classification
	static const char* typeNames[] = { "barre", "blanche_bas", "blanche_haut" , "cle_sol","croche","dieze_armature","noire_bas","noire_haut","quatre","ronde","silence","noire_pointee_bas","barre_fin"};
	for (int j = 0; j < resultats.size(); j++) {
		cout << typeNames[resultats[j]] << endl;
	}
	cout << endl;

	return resultats;
}

vector<MusicNote> DensityDetection::getMusicNotesFromDensityClassification(vector<noteType> classification, Rect line, vector<Rect> boundingBoxes, vector<int> lines) {
	vector<MusicNote> notes = vector<MusicNote>();

	//J'ajoute aux sous-lignes les lignes intermédiaire car une note peut se trouver entre deux sous-lignes
	vector<int> linesComplete = vector<int>();
	for (int i = 0; i < lines.size() - 1; i++) {
		linesComplete.push_back(lines[i]);
		linesComplete.push_back((lines[i] + lines[i + 1]) / 2);
	}
	linesComplete.push_back(lines[lines.size() - 1]);

	//Pour chaque type d'objet, je calcule la sous-ligne sur laquelle il se trouve
	//ainsi que sa durée.
	//Si une note est vers le bas, je vais calculer sa position par rapport au y en haut de sa bounding box
	//Si une note est vers le haut, je vais calculer sa position par rapport au y en bas de sa bounding box
	for (int i = 0; i < boundingBoxes.size(); i++) {
		Rect boundingBox = boundingBoxes[i];
		float duration = 0;
		float note = -1.0f;
		int subline = 0;
		int y = 0;
		switch (classification[i]) {
		case blanche_bas:
			duration = 1;
			y = line.y + boundingBox.y;
			//+1 car la vraie position de la note est une sous-ligne plus bas
			subline = getMostCloseLine(linesComplete, y) + 1;
			note = lineToNote(subline);
			notes.push_back(MusicNote(0, duration, note, false));
			break;
		case blanche_haut:
			duration = 1;
			y = line.y + boundingBox.y + boundingBox.height;
			//-1 car la vraie position de la note est une sous-ligne plus haut
			subline = getMostCloseLine(linesComplete, y) - 1;
			note = lineToNote(subline);
			notes.push_back(MusicNote(0, duration, note, false));
			break;
		case noire_bas:
			duration = 1;
			y = line.y + boundingBox.y;
			subline = getMostCloseLine(linesComplete, y) + 1;
			note = lineToNote(subline);
			notes.push_back(MusicNote(0, duration, note, false));
			break;
		case noire_haut:
			duration = 1;
			y = line.y + boundingBox.y + boundingBox.height;
			subline = getMostCloseLine(linesComplete, y) - 1;
			note = lineToNote(subline);
			notes.push_back(MusicNote(0, duration, note, false));
			break;
		case croche:
			duration = 0.5;
			y = line.y + boundingBox.y + boundingBox.height;
			subline = getMostCloseLine(linesComplete, y) - 1;
			note = lineToNote(subline);
			notes.push_back(MusicNote(0, duration, note, false));
			break;
		case ronde:
			duration = 4;
			y = line.y + boundingBox.y + boundingBox.height;
			subline = getMostCloseLine(linesComplete, y) - 1;
			note = lineToNote(subline);
			notes.push_back(MusicNote(0, duration, note, false));
			break;
		case noire_pointee_bas:
			duration = 1.5;
			y = line.y + boundingBox.y + (0.06 * boundingBox.height); //Ajustement pour le point de la noire pointée
			subline = getMostCloseLine(linesComplete, y) + 1;
			note = lineToNote(subline);
			notes.push_back(MusicNote(0, duration, note, false));
			break;
		case silence:
			duration = 1;
			note = lineToNote(subline);
			notes.push_back(MusicNote(0, duration, note, true));
			break;
		}

	}
	return notes;
}