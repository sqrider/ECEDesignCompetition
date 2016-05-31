/*@file CannyDetector_Demo.cpp
 * @brief Sample code showing how to detect edges using the Canny Detector
 * @author OpenCV team
 */

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

/// Global variables

Mat src, src_gray;
Mat dst, detected_edges;
Mat roi, roi_gray;

int edgeThresh = 1;
int lowThreshold = 80;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
const char* window_name = "thing";

int u, d, l, r;

int main( int argc, char** argv ) {

    /// Load an image
    src = imread( argv[1] );

    if( src.empty() ) {
        std::cout << "Usage: ./final imgfile" << std::endl;
        return -1; 
    }

    /// Create a matrix of the same type and size as src (for dst)
    dst.create( src.size(), src.type() );

    /// Convert the image to grayscale
    cvtColor( src, src_gray, COLOR_BGR2GRAY );

    /// Create a window
    namedWindow( window_name, WINDOW_NORMAL );
    namedWindow( "input" , WINDOW_NORMAL );
    namedWindow( "mid" , WINDOW_NORMAL );

    /// Reduce noise with a kernel 3x3
    blur( src_gray, detected_edges, Size(6,6) );

    /// Canny detector
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

    /// Using Canny's output as a mask, we display our result
    dst = Scalar::all(0);

    src.copyTo( dst, detected_edges);
    bitwise_not (dst, dst);

    imwrite("temp.JPG", dst);
    // get bounds
    Size s = dst.size();
    u = s.height;
    l = s.width;
    d = 0;
    r = 0;

    for (int row = 0; row < s.width; row++) {
        for (int col = 0; col < s.height; col++) {
            unsigned char val = detected_edges.at<uchar>(row, col);
            if (val == 255) {
                if (col < u) u = col;
                if (row < l) l = row;
                if (col > d) d = col;
                if (row > r) r = row;
            }
        }
    }

    Point p1(u, l);
    Point p2(d, r);
    //rectangle(src, p1, p2, Scalar(0, 0, 255), 5);

    /* PROCESS ROI */
    Rect region = Rect(u, l, d-u, r-l);
    roi = src(region);

    cvtColor( roi, roi_gray, COLOR_BGR2GRAY );
    threshold(roi_gray, roi_gray, 125, 255, 0);

    imshow("input", src);
    imshow( window_name, roi_gray );

    if (argc == 3) imwrite(argv[2], roi_gray);

    /// Wait until user exit program by pressing a key
    waitKey(0);

    return 0;
}
