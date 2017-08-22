//
// Created by Oleksandr Padko on 8/20/17.
//

#include <opencv2/imgproc.hpp>
#include <MacTypes.h>
#include <opencv/cv.hpp>
#include "CellClassifier.h"

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
    if (isCircle()) {
        value = O;
    } else if (isCross()) {
        value = X;
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

    cv::HoughCircles(blured, circles, cv::HOUGH_GRADIENT, 1, grayImage.rows / 16, 40, 30);

    for (auto &&circle : circles) {
        if (circle[2] > minRadius) {
            return true;
        }
    }

    return false;
}

int CellClassifier::orientation(const cv::Point &p, const cv::Point &q, const cv::Point &r) const {
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool CellClassifier::doIntersect(const cv::Point &p1, const cv::Point &q1, const cv::Point &p2, const cv::Point &q2) const {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    return (o1 != o2 && o3 != o4);
}

bool CellClassifier::isCross() {
    const double minLength = mReflectionless.cols / 3;

    int offset = int(mReflectionless.cols * 0.15);

    cv::Rect roi(offset, offset,
                 mReflectionless.cols - offset * 2, mReflectionless.rows - offset * 2);
    cv::Mat roiImg = mReflectionless(roi);

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

    //check proper intersection
    bool areIntersecting = false;
    for (auto &&line1 : lines) {
        for (auto &&line2 : lines) {
            cv::Point p1(line1[0], line1[1]), p2(line1[2], line1[3]);
            cv::Point p3(line2[0], line2[1]), p4(line2[2], line2[3]);
            double angle1 = atan((line1[1] - line1[3]) / (line1[0] - line1[2]));
            double angle2 = atan((line2[1] - line2[3]) / (line2[0] - line2[2]));
            if(doIntersect(p1, p2, p3, p4) && fabs(angle1 - angle2) > 0.785398) { // 0.785398 in radians = 45 degrees
                areIntersecting = true;
            }
        }
    }

    return areIntersecting;
}