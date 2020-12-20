/*********************************************************************************
* File Name           :     LineDetection.hpp
* Created By          :     tiboiser
* Creation Date       :     [2020-12-20 13:00]
* Last Modified       :     [2020-12-20 18:10]
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
			std::vector<cv::Point> _arrDetection;

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

			/* Get _arrDetection attribute */
			std::vector<cv::Point> arrDetection(void);

			/* Get _arrDetection attribute */
			std::vector<cv::Rect> boundingBoxs(void);
	};

	/***********************************
	 * Class : LineDetection
	 **********************************/
	class LineDetection : public virtual ObjectDetection {

		/* ---- Attributes ---- */
		private:
			float _detectionRatio; // Ratio black/white pixel on a single line. (It's [max length of continuous black pixel] / total_columns)

		/* ---- Methods ---- */
		private:
			/* horizontal projection
			 * Project the maximum continuous zeros on a line into the returned vector<int> */
			std::vector<int> maxContinuousHorizontalProjection (const cv::Mat& m);

		public :
			/* -- Constructors / Destructors-- */

			/* Default Constructor of LineDetection class */
			LineDetection(void);

			/* Constructor of LineDetection class setting the detection pixel ratio black/white in a line */
			LineDetection(float detectRatio);

			/* Default destructor of LineDetection class */
			~LineDetection(void);

			/* -- Others Methods -- */

			/* line Detection
			 * Detect horizontal lines and stores rows number into a vector<int>. */
			std::vector<int> detect(std::vector<int>& proj);

			/* Getter of _detectionRatio */
			float detectionRatio (void) __attribute__ ((hot));
	};
}
