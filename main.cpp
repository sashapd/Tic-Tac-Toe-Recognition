#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/core/mat.hpp>
#include "GridExtractor.h"

int main() {
    cv::Mat im = cv::imread("tilt1.jpg");
    cv::Mat image;
    cv::resize(im, image, cv::Size(), 0.5, 0.5);

    GridExtractor extractor(image);
    extractor.extractGrid();

    cv::imshow("image", extractor.getGrid());
    cv::waitKey(0);
    return 0;
}