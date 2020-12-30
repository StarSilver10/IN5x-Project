/*********************************************************************************
*     File Name           :     LineDetection.hpp
*     Created By          :     tiboiser
*     Creation Date       :     [2020-12-21 10:37]
*     Last Modified       :     [2020-12-21 10:46]
*     Description         :     LineDetection class header
**********************************************************************************/

#ifndef OBJDETECBB_H
#define OBJDETECBB_H

#include "ObjectDetection.hpp"

namespace objdetect {

	/***********************************
	 * Class : LineDetection
	 **********************************/
	class ObjectBoundingBoxDetection : public virtual ObjectDetection {

		/* ---- Attributes ---- */
	private:
		double mergeRectanglesThreshold;
		int lineNumber;
		/* ---- Methods ---- */
	private:

	public:
		/* -- Constructors / Destructors-- */

		/* Default Constructor of LineDetection class */
		ObjectBoundingBoxDetection(double mergeRectanglesThreshold, int lineNumber);

		/* Default destructor of LineDetection class */
		~ObjectBoundingBoxDetection(void);

		/* -- Others Methods -- */

		/* Détecte les boundingBoxs dans l'image m et dans la ligne délimitée par boundingBox */		
		std::vector<cv::Rect> searchBoundingBoxes(cv::Mat& m, cv::Rect boundingBox);
		
		int detect(cv::Mat& m);
	};
}

#endif