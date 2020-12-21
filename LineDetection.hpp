/*********************************************************************************
*     File Name           :     LineDetection.hpp
*     Created By          :     tiboiser
*     Creation Date       :     [2020-12-21 10:37]
*     Last Modified       :     [2020-12-21 10:46]
*     Description         :     LineDetection class header
**********************************************************************************/


#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "ObjectDetection.hpp"

namespace objdetect {

	/***********************************
	 * Class : LineDetection
	 **********************************/
	class LineDetection : public virtual ObjectDetection {

		/* ---- Attributes ---- */
		private:
			/* Ratio black/white pixel on a single line. (It's [max length of continuous black pixel] / total_columns) */
			float _detectionRatio;

			/* Vector of lines, containing a vector of sublines per lines. */
			std::vector<std::vector<int>> _subLines;


		/* ---- Methods ---- */
		private:
			/* horizontal projection
			 * Project the maximum continuous zeros on a line into the returned vector<int> */
			std::vector<int> maxContinuousHorizontalProjection (const cv::Mat& m);

			/* Select wide black lines among the projection proj and merge continuous lines
			 * Detect horizontal sublines and stores rows number into a vector<int>. */
			std::vector<int> selectSubLines(std::vector<int>& proj, int cols);

			/* Generate bounding boxs around lines
			 * Use the maxContinuousHorizontalProjection output and the lines positions.
			 * Return 1 if successful, 0 otherwise */
			int generateBoundingBoxs(cv::Mat& m, std::vector<int>& proj);

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
			int detect(cv::Mat& m);


			/* Getter of _detectionRatio */
			float detectionRatio(void);


			/* Getter of _subLines */
			std::vector<std::vector<int>> subLines(void);
	};
}
