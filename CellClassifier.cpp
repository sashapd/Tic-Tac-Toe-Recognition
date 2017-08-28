//
// Created by Oleksandr Padko on 8/20/17.
//

#include <opencv2/imgproc.hpp>
#include <opencv/cv.hpp>
#include "CellClassifier.h"
#include "GeometricUtilities.h"

CellClassifier::CellClassifier(cv::Mat cellImage) {
    cv::Mat foreground;
    int morphSize = 5;
    cv::Mat morphElement = cv::getStructuringElement(cv::MORPH_RECT,
                                                     cv::Size(2 * morphSize + 1, 2 * morphSize + 1),
                                                     cv::Point(morphSize, morphSize));
    cv::morphologyEx(cellImage, foreground, cv::MORPH_CLOSE, morphElement);
    cv::Mat white(cellImage.size(), cellImage.type());
    white = cv::Scalar(255, 255, 255);

    mReflectionless = white - (foreground - cellImage);
}

Cell CellClassifier::getCellValue() {
    Cell value = NONE;
    if (isCross()) {
        value = X;
    } else if (isCircle()) {
        value = O;
    }
    return value;
}

bool CellClassifier::isCircle() {
    const int minRadius = mReflectionless.cols / 5;

    cv::Mat grayImage;
    cv::cvtColor(mReflectionless, grayImage, cv::COLOR_BGR2GRAY);

    int morphSize = 6;
    cv::Mat morphElement = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                                     cv::Size(2 * morphSize + 1, 2 * morphSize + 1),
                                                     cv::Point(morphSize, morphSize));
    cv::Mat eroded, dilated;
    cv::erode(grayImage, eroded, morphElement);
    cv::dilate(eroded, dilated, morphElement);

    cv::Mat blured;
    cv::GaussianBlur(dilated, blured, cv::Size(9, 9), 2, 2);

    std::vector<cv::Vec3f> circles;

    cv::HoughCircles(blured, circles, cv::HOUGH_GRADIENT, 1, grayImage.rows / 16, 40, 25);

    for (auto &&circle : circles) {
        if (circle[2] > minRadius) {
            return true;
        }
    }

    return false;
}

bool CellClassifier::isCross() {
    const double minLength = mReflectionless.cols / 3;

    int offset = int(mReflectionless.cols * 0.15);

    cv::Rect roi(offset, offset,
                 mReflectionless.cols - offset * 2, mReflectionless.rows - offset * 2);
    cv::Mat roiImg = mReflectionless(roi);

    cv::Mat dst;
    cv::Canny(roiImg, dst, 25, 75, 3);

    int morphSize = 4;
    cv::Mat morphElement = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                                     cv::Size(2 * morphSize + 1, 2 * morphSize + 1),
                                                     cv::Point(morphSize, morphSize));
    cv::Mat dilated;
    cv::dilate(dst, dilated, morphElement);

    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(dilated, lines, 1, CV_PI / 180, 25, 50, 2);

    std::vector<cv::Vec4i> filteredLines;
    for (auto &&line : lines) {
        double length = pow(pow(line[0] - line[2], 2) + pow(line[1] - line[3], 2), 0.5);
        if (length >= minLength) {
            filteredLines.push_back(line);
        }
    }

    //check proper intersection
    bool areIntersecting = false;
    for (auto &&line1 : filteredLines) {
        std::vector<cv::Vec4i> intersecting = GeometricUtilities::getIntersectingLines(line1, filteredLines);
        for (auto &&line2 : intersecting) {
            cv::Point p1(line1[0], line1[1]), p2(line1[2], line1[3]);
            cv::Point p3(line2[0], line2[1]), p4(line2[2], line2[3]);
            double dotProduct = (p1 - p2).dot(p3 - p4);
            double length1 = cv::norm(p1 - p2);
            double length2 = cv::norm(p3 - p4);
            if (fabs((dotProduct / (length1 * length2))) < cos(CV_PI / 2 - 0.785398) &&
                GeometricUtilities::distToLine(p1, line2) > length2 / 2.5 &&
                GeometricUtilities::distToLine(p2, line2) > length2 / 2.5) { // 0.785398 in radians = 45 degrees
                areIntersecting = true;
            }
        }
    }

    return areIntersecting;
}