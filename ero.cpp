/*********************************************************************************
*     File Name           :     erosion.cpp
*     Created By          :     tiboiser
*     Creation Date       :     [2020-12-20 12:59]
*     Last Modified       :     [2020-12-20 20:28]
*     Description         :      
**********************************************************************************/


#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

Mat src, erosion_dst, dilation_dst;

int erosion_elem = 0;
int erosion_size_height = 0;
int erosion_size_width = 0;
int dilation_elem = 0;
int dilation_size_height = 0;
int dilation_size_width = 0;
int const max_elem = 3;
int const max_kernel_size_height = 21;
int const max_kernel_size_width = 21;

void Erosion( int, void* );
void Dilation( int, void* );


int xcgx( int argc, char** argv )
{
	//CommandLineParser parser( argc, argv, "{@input | LinuxLogo.jpg | input image}" );
	//src = imread( samples::findFile( parser.get<String>( "@input" ) ), IMREAD_COLOR );
	src = imread("C:/Users/nigro/source/repos/Project IN5x/Project IN5x/imgs/mary.jpg");
	if( src.empty() )
	{
		cout << "Could not open or find the image!\n" << endl;
		cout << "Usage: " << argv[0] << " <Input image>" << endl;
		return -1;
	}


	// for (int i = 0; i < v.size(); i++) {
	// 	cout << v(i) << endl;
	// }
	namedWindow("Dilation Demo", WINDOW_NORMAL);
	namedWindow( "Erosion Demo", WINDOW_NORMAL);
	
	//moveWindow( "Dilation Demo", src.cols, 0 );
	createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse \n 3: H-Line", "Erosion Demo",
			&erosion_elem, max_elem,
			Erosion );
	createTrackbar( "Kernel size height: \n 2n +1", "Erosion Demo",
			&erosion_size_height, max_kernel_size_height,
			Erosion );
	createTrackbar( "Kernel size width:\n 2n +1", "Erosion Demo",
			&erosion_size_width, max_kernel_size_width,
			Erosion );
	createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse \n 3: H-Line", "Dilation Demo",
			&dilation_elem, max_elem,
			Dilation );
	createTrackbar( "Kernel size height:\n 2n +1", "Dilation Demo",
			&dilation_size_height, max_kernel_size_height,
			Dilation );
	createTrackbar( "Kernel size width:\n 2n +1", "Dilation Demo",
			&dilation_size_width, max_kernel_size_width,
			Dilation );
	Dilation(0, 0);
	Erosion( 0, 0 );
	
	waitKey(0);
	return 0;
}
void Erosion( int, void* )
{
	int erosion_type = 0;
	if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
	else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
	else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }
	Mat element;
	if (erosion_elem == 3) {
	} else {
		element = getStructuringElement( erosion_type,
				Size( 2*erosion_size_width + 1, 2*erosion_size_height+1 ),
				Point( erosion_size_width, erosion_size_height ) );
	}

	erode( src, erosion_dst, element );
	imshow( "Erosion Demo", erosion_dst );
}

void Dilation( int, void* )
{
	int dilation_type = 0;
	if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
	else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
	else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }
	Mat element = getStructuringElement( dilation_type,
			Size( 2*dilation_size_width + 1, 2*dilation_size_height+1 ),
			Point( dilation_size_width, dilation_size_height ) );
	dilate( src, dilation_dst, element );
	imshow( "Dilation Demo", dilation_dst );
}
