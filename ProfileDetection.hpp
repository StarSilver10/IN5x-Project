/*********************************************************************************
* File Name           :     LineDetection.hpp
* Created By          :     tiboiser
* Creation Date       :     [2020-12-20 13:00]
* Last Modified       :     [2020-12-21 10:44]
* Description         :     Objects detection header
**********************************************************************************/

#ifndef PROFDETEC_H
#define PROFDETEC_H

#include "ObjectDetection.hpp"
#include "MusicNote.hpp"

namespace objdetect {

	
	/***********************************
	 * Class : ObjectDetection
	 **********************************/
	class ProfileDetection : public virtual ObjectDetection {

		/* ---- Attributes ---- */


		/* ---- Methods ---- */
	public:
		/* -- Constructors / Destructors-- */

		/* Default Constructor of ObjectDetection class */
		ProfileDetection(void);

		/* Virtual destructor of ObjectDetection class */
		~ProfileDetection(void);

		/* -- Others Methods -- */

		/* Computes the recognition and stores it in class attributes
		 * Return 1 if successful, 0 otherwise. */
		int detect(cv::Mat& m);

		/*
		* Calcule le profil d'une sous image de dimension égal à boundingBox, avec un pas égal à d
		* Retourne le vecteur de distance des profils (profils gauches puis droit)
		*/
		std::vector<int> profile(int d, cv::Rect boundingBox, cv::Mat& img);

		/*
		* Calcule le profil d'une image img, avec un pas égal à d
		* Retourne le vecteur de distance des profils (profils gauches puis droit)
		*/
		std::vector<int> profile(int d, cv::Mat& img);

		/*
		* Retourne un vecteur contenant les vecteurs de profil de chaque image template
		*/
		std::vector<std::vector<int>> getTemplatesProfiles(int d);

		/*
		* Compare les profils de chaque objet issu de boundingBoxes dans la ligne line, avec les profils des templates
		* Attribue ensuite à chaque objet une classe selon la distance de son profil avec le profil des différents templates
		*/
		std::vector<objdetect::noteType> profileClassification(int d, cv::Rect line, std::vector<cv::Rect> boundingBoxes, cv::Mat& img);

		std::vector<objdetect::MusicNote> getMusicNotesFromClassification(std::vector<objdetect::noteType> classification, cv::Rect line, std::vector<cv::Rect> boundingBoxes, std::vector<int> lines);
	};
}

#endif