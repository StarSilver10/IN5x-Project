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

		std::vector<int> profile(int d, cv::Rect boundingBox, cv::Mat& img);

		std::vector<int> profile(int d, cv::Mat& img);

		std::vector<std::vector<int>> getTemplatesProfiles(int d);

		void profileClassification(int d, cv::Rect line, std::vector<cv::Rect> boundingBoxes, cv::Mat& img);
	};
}

#endif