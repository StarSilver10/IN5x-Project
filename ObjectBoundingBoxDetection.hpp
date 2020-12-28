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
		

		/* ---- Methods ---- */
	private:

	public:
		/* -- Constructors / Destructors-- */

		/* Default Constructor of LineDetection class */
		ObjectBoundingBoxDetection(void);	

		/* Default destructor of LineDetection class */
		~ObjectBoundingBoxDetection(void);

		/* -- Others Methods -- */
		std::vector<cv::Rect> searchBoundingBoxes(cv::Mat& m, cv::Rect boundingBox);
		/* line Detection
		 * Detects boundingBoxes of objects in a bar */
		int detect(cv::Mat& m);
	};
}

#endif