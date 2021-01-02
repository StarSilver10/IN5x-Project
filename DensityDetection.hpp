/*********************************************************************************
* File Name           :     DensiteDetection.hpp
* Created By          :     rixte
* Creation Date       :     [2020-12-20 13:00]
* Last Modified       :     [2020-12-21 10:44]
* Description         :     Objects recognition header
**********************************************************************************/

#ifndef DENSITYDETECTION_HPP
#define DENSITYDETECTION_HPP

#include "ObjectDetection.hpp"
#include "MusicNote.hpp"

namespace objdetect {

	/***********************************
	 * Class : ObjectDetection
	 **********************************/
	class DensityDetection : public virtual ObjectDetection {

		/* ---- Attributes ---- */


		/* ---- Methods ---- */
	public:
		/* -- Constructors / Destructors-- */

		/* Default Constructor of ObjectDetection class */
		DensityDetection(void);

		/* Virtual destructor of ObjectDetection class */
		~DensityDetection(void);
	
		/* -- Others Methods -- */

		///* Computes the recognition and stores it in class attributes
		// * Return 1 if successful, 0 otherwise. */
		int detect(cv::Mat& m);

		//Permet d'obtenir tous les vecteurs densités à partir d'une sous image (rectangle)
		std::vector<double> density(int n, int m, cv::Rect boundingBox, cv::Mat& img);

		/*Permet d'obtenir tous les vecteurs densités à partir d'une image donnée*/
		std::vector<double> density(int n, int m, cv::Mat& img);

		//Permet de récupérer les vecteurs densités des images templates
		std::vector<std::vector<double>> getTemplatesDensity(int n, int m);

		//Retourne le type de symbole auquel correspond chaque boundingBax de l'image
		std::vector<objdetect::noteType> densityClassification(int n, int m, cv::Rect line, std::vector<cv::Rect> boundingBoxes, cv::Mat& img);

		//Convertis les résultats de la classification en MusicNotes convertibles en MIDI
		std::vector<objdetect::MusicNote> getMusicNotesFromDensityClassification(std::vector<objdetect::noteType> classification, cv::Rect line, std::vector<cv::Rect> boundingBoxes, std::vector<int> lines);
	private:

		//Permet de tracer une ligne sur une image donnée
		void Line(cv::Mat img, cv::Point start, cv::Point end);

		//Renvoie le type d'un objet de la partition 
		noteType objectType(int type);

		//Renvoie la ligne la plus proche d'un indice de ligne y
		int getMostCloseLine(std::vector<int> lines, int y);

		//Convertis la position d'un objet sur une ligne en note convertible en MIDI
		float lineToNote(int line);

		//Permet de calculer la distance entre 2 vecteurs
		double vectDist(std::vector<double> v1, std::vector<double> v2);

		//Permet d'obtenir l'indice qui correspond à l'écart entre deux lignes du zoning
		int getMaxIndicesRow(std::vector<int> indicesType, int n, cv::Mat img);

		//Permet d'obtenir l'indice qui correspond à l'écart entre deux colonnes du zoning
		int getMaxIndicesCol(std::vector<int> indicesType, int m, cv::Mat img);

		//Trace le cadrillage du zoning sur une image choisie
		void drawLine(int n, int m, cv::Mat img, std::vector<int> indicesRows, std::vector<int> indicesCols);
	};
}

#endif