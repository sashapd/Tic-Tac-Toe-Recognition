//
// Created by Oleksandr Padko on 8/20/17.
//

#include <opencv2/imgproc.hpp>
#include <MacTypes.h>
#include <opencv/cv.hpp>
#include "CellClassifier.h"

CellClassifier::CellClassifier(cv::Mat cellImage) {
    mCellImage = cellImage;
}

Cell CellClassifier::getCellValue() {
    if (isCircle()) {
        return O;
    } else if (isCross()) {
        return X;
    } else {
        return NONE;
    }
}

bool CellClassifier::isCircle() {
    const int minRadius = mCellImage.cols / 3;

    cv::Mat grayImage;
    cv::cvtColor(mCellImage, grayImage, cv::COLOR_BGR2GRAY);

    cv::Mat blured;
    cv::GaussianBlur(grayImage, blured, cv::Size(9, 9), 2, 2 );

    std::vector<cv::Vec3f> circles;

    cv::HoughCircles( blured, circles, cv::HOUGH_GRADIENT, 1, grayImage.rows/16, 100, 45);

    for (auto &&circle : circles) {
        if(circle[3] > minRadius) {
            return true;
        }
    }

    return false;
}

bool CellClassifier::isCross() {
    const double minLength = mCellImage.cols / 2;

    cv::Rect roi(mCellImage.cols / 10, mCellImage.rows / 10,
                 mCellImage.cols - mCellImage.cols / 10, mCellImage.rows - mCellImage.rows / 10);
    cv::Mat roiImg = mCellImage(roi);

    cv::Mat dst;
    cv::Canny(roiImg, dst, 50, 200, 3);

    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );

    std::vector<cv::Vec4i> filteredLines;
    for (auto &&line : lines) {
        double length = pow(pow(line[0] - line[2], 2) + pow(line[1] - line[3], 2), 0.5);
        if(length >= minLength) {
            filteredLines.push_back(line);
        }
    }
    return filteredLines.size() >= 2;
}