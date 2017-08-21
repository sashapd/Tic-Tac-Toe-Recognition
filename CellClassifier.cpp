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
    Cell value;
    if (isCircle()) {
        value = O;
    } else if (isCross()) {
        value = X;
    } else {
        value = NONE;
    }
    return value;
}

bool CellClassifier::isCircle() {
    const int minRadius = mCellImage.cols / 3;

    cv::Mat grayImage;
    cv::cvtColor(mCellImage, grayImage, cv::COLOR_BGR2GRAY);

    int morphSize = 6;
    cv::Mat morphElement = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                                     cv::Size(2 * morphSize + 1, 2 * morphSize + 1),
                                                     cv::Point(morphSize, morphSize));
    cv::Mat eroded, dilated;
    cv::erode(grayImage, eroded, morphElement);
    cv::dilate(eroded, dilated, morphElement);

    cv::Mat blured;
    cv::GaussianBlur(grayImage, blured, cv::Size(9, 9), 2, 2);

    std::vector<cv::Vec3f> circles;

    cv::HoughCircles(blured, circles, cv::HOUGH_GRADIENT, 1, grayImage.rows / 16, 40, 30);

    for (auto &&circle : circles) {
        if (circle[3] > minRadius) {
            return true;
        }
    }

    return false;
}

bool CellClassifier::isCross() {
    const double minLength = mCellImage.cols / 4;

    int offset = int(mCellImage.cols * 0.15);

    cv::Rect roi(offset, offset,
                 mCellImage.cols - offset * 2, mCellImage.rows - offset * 2);
    cv::Mat roiImg = mCellImage(roi);

    cv::Mat dst;
    cv::Canny(roiImg, dst, 50, 200, 3);

    int morphSize = 3;
    cv::Mat morphElement = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                                     cv::Size(2 * morphSize + 1, 2 * morphSize + 1),
                                                     cv::Point(morphSize, morphSize));
    cv::Mat dilated;
    cv::dilate(dst, dilated, morphElement);

    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(dilated, lines, 1, CV_PI / 180, 50, 50, 10);

    std::vector<cv::Vec4i> filteredLines;
    for (auto &&line : lines) {
        double length = pow(pow(line[0] - line[2], 2) + pow(line[1] - line[3], 2), 0.5);
        if (length >= minLength) {
            filteredLines.push_back(line);
        }
    }
    return filteredLines.size() >= 2;
}