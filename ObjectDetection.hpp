/*********************************************************************************
* File Name           :     LineDetection.hpp
* Created By          :     tiboiser
* Creation Date       :     [2020-12-20 13:00]
* Last Modified       :     [2020-12-21 10:44]
* Description         :     Objects detection header
**********************************************************************************/


#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

namespace objdetect {

	/***********************************
	 * Class : ObjectDetection
	 **********************************/
	class ObjectDetection {

		/* ---- Attributes ---- */
		protected:
			// Contains each position of object identification
			// Vector of <rows, cols>
			std::vector<cv::Point> _positions;

			// Contains bounding box around each recognized object.
			// Vector of <rows, cols, height, width>
			std::vector<cv::Rect> _boundingBoxs;


		/* ---- Methods ---- */
		public:
			/* -- Constructors / Destructors-- */

			/* Default Constructor of ObjectDetection class */
			ObjectDetection(void);

			/* Virtual destructor of ObjectDetection class */
			virtual ~ObjectDetection(void) = 0;

			/* -- Others Methods -- */

			/* Get _arrDetection attribute */
			std::vector<cv::Point> positions(void);

			/* Get _arrDetection attribute */
			std::vector<cv::Rect> boundingBoxs(void);

			/* Computes the recognition and stores it in class attributes
			 * Return 1 if successful, 0 otherwise. */
			virtual int detect(cv::Mat& m) = 0;
	};
}
