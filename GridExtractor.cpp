//
// Created by Oleksandr Padko on 8/16/17.
//
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "GridExtractor.h"
#include <functional>


GridExtractor::GridExtractor(const cv::Mat &image) {
    mImage = image;
}

void GridExtractor::extractGrid() {
    std::vector<cv::Vec4i> lines = findLines();

    std::vector<cv::Vec4i> gridLines = getGridLines(lines);

    if (gridLines.size() == 4) {
        foundGrid = true;

        gridCoordinates = getGridInnerCoordinates(gridLines);

        cv::Mat gridImage(512, 512, CV_8UC3);
        cv::Point2f dstPoint[4] = {
                cv::Point2f(gridImage.cols * 2 / 3, gridImage.rows * 2 / 3),
                cv::Point2f(gridImage.cols * 2 / 3, gridImage.rows / 3),
                cv::Point2f(gridImage.cols / 3, gridImage.rows / 3),
                cv::Point2f(gridImage.cols / 3, gridImage.rows * 2 / 3)};

        cv::Mat transformMatr = cv::getPerspectiveTransform(gridCoordinates.data(), dstPoint);

        cv::warpPerspective(mImage, gridImage, transformMatr, gridImage.size());

        mGridImage = gridImage;
    }
}

Grid GridExtractor::getGrid() const {
    return Grid(mGridImage);
}

cv::Mat GridExtractor::getImage() const {
    return mImage;
}

bool GridExtractor::hasFoundGrid() const {
    return foundGrid;
}

void GridExtractor::putBackGrid(Grid grid) const {
    cv::Mat gridImage = grid.getImage();
    cv::Point2f srcPoints[4] = {
            cv::Point2f(gridImage.cols * 2 / 3, gridImage.rows * 2 / 3),
            cv::Point2f(gridImage.cols * 2 / 3, gridImage.rows / 3),
            cv::Point2f(gridImage.cols / 3, gridImage.rows / 3),
            cv::Point2f(gridImage.cols / 3, gridImage.rows * 2 / 3)};


    cv::Mat transformMatr = cv::getPerspectiveTransform(srcPoints, gridCoordinates.data());

    cv::Mat foreground(mImage.size(), mImage.type());

    cv::warpPerspective(gridImage, foreground, transformMatr, foreground.size());

    cv::Mat mask = foreground > 0;

    foreground.copyTo(mImage, mask);
}

std::vector<cv::Vec4i> GridExtractor::findLines() {
    cv::Mat foreground;
    int morphSize = 10;
    cv::Mat morphElement = cv::getStructuringElement(cv::MORPH_RECT,
                                                     cv::Size(2 * morphSize + 1, 2 * morphSize + 1),
                                                     cv::Point(morphSize, morphSize));

    cv::morphologyEx(mImage, foreground, cv::MORPH_CLOSE, morphElement);

    cv::Mat reflectionless = foreground - mImage;

    cv::Mat canny_output;
    cv::Canny(reflectionless, canny_output, 50, 75, 3);

    int dialationSize = 1;
    cv::Mat dialationElement = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                                         cv::Size(2 * dialationSize + 1, 2 * dialationSize + 1),
                                                         cv::Point(dialationSize, dialationSize));
    cv::Mat dilated;
    cv::dilate(canny_output, dilated, dialationElement);

    cv::imshow("r", dilated);

    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(dilated, lines, 1, CV_PI / 180, 100, 50, 10);

    lines = filterSimmilar(lines);

    return lines;
}

double GridExtractor::getSlope(const cv::Vec4i &line) {
    return (double) (line[1] - line[3]) / (line[0] - line[2] + 10e-8);
}

bool GridExtractor::compareLines(const cv::Vec4i &line1, const cv::Vec4i &line2) {
    double line1Length = pow(pow(line1[0] - line1[2], 2) + pow(line1[1] - line1[3], 2), 0.5);
    double line2Length = pow(pow(line2[0] - line2[2], 2) + pow(line2[1] - line2[3], 2), 0.5);
    return line1Length > line2Length;
}

bool GridExtractor::comparePointsClockwise(cv::Point a, cv::Point b, cv::Point center) {
    if (a.x - center.x >= 0 && b.x - center.x < 0)
        return true;
    if (a.x - center.x < 0 && b.x - center.x >= 0)
        return false;
    if (a.x - center.x == 0 && b.x - center.x == 0) {
        if (a.y - center.y >= 0 || b.y - center.y >= 0)
            return a.y > b.y;
        return b.y > a.y;
    }

    // compute the cross product of vectors (center -> a) x (center -> b)
    int det = (a.x - center.x) * (b.y - center.y) - (b.x - center.x) * (a.y - center.y);
    if (det < 0)
        return true;
    if (det > 0)
        return false;

    // points a and b are on the same line from the center
    // check which point is closer to the center
    int d1 = (a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y);
    int d2 = (b.x - center.x) * (b.x - center.x) + (b.y - center.y) * (b.y - center.y);
    return d1 > d2;
}

double GridExtractor::distToLine(const cv::Point &point, cv::Vec4i line) {
    double dist = fabs(
            (line[3] - line[1]) * point.x - (line[2] - line[0]) * point.y + line[2] * line[1] - line[3] * line[0])
                  / sqrt(pow(line[3] - line[1], 2) + pow(line[2] - line[0], 2));
    return dist;
}

bool GridExtractor::areSimmilar(cv::Vec4i line1, cv::Vec4i line2) {
    cv::Point p1(line1[0], line1[1]), p2(line1[2], line1[3]), p3(line2[0], line2[1]), p4(line2[2], line2[3]);
    double length1 = cv::norm(p1 - p2);
    double length2 = cv::norm(p3 - p4);
    double dotProduct = (p2 - p1).dot(p4 - p3);

    if (fabs(dotProduct / (length1 * length2)) < cos(CV_PI / 10))
        return false;

    const double distThresh = 15;
    const double lengthThresh = 25;

    if (fabs(length1 - (cv::norm(p1 - p3) + cv::norm(p2 - p3))) < lengthThresh ||
        fabs(length1 - (cv::norm(p1 - p4) + cv::norm(p2 - p4))) < lengthThresh ||
        fabs(length2 - (cv::norm(p3 - p1) + cv::norm(p4 - p1))) < lengthThresh ||
        fabs(length2 - (cv::norm(p3 - p2) + cv::norm(p4 - p2))) < lengthThresh) {

        if (distToLine(p1, line2) < distThresh || distToLine(p2, line2) < distThresh ||
            distToLine(p3, line1) < distThresh || distToLine(p4, line1) < distThresh) {
            return true;
        }

    }
    return false;
}

cv::Vec4i GridExtractor::mergeLines(const std::vector<cv::Vec4i> &lines) const {
    std::vector<cv::Point2i> linePoints;
    for (auto &&line : lines) {
        linePoints.emplace_back(line[0], line[1]);
        linePoints.emplace_back(line[2], line[3]);
    }
    std::vector<cv::Point2i> convexPoints;
    cv::convexHull(linePoints, convexPoints);
    double maxDist = 0;
    cv::Vec4i maxLine;
    for (auto &&point1 : convexPoints) {
        for (auto &&point2 : convexPoints) {
            double dist = cv::norm(point1 - point2);
            if (dist > maxDist) {
                maxDist = dist;
                maxLine = cv::Vec4i(point1.x, point1.y, point2.x, point2.y);
            }
        }
    }
    return maxLine;
}

std::vector<cv::Vec4i>
GridExtractor::filterSimmilar(std::vector<cv::Vec4i> lines) const {
    std::vector<int> labels;
    int numberOfLines = cv::partition(lines, labels, areSimmilar);

    std::vector<std::vector<cv::Vec4i>> linesToMerge(numberOfLines);
    for (int i = 0; i < lines.size(); i++) {
        linesToMerge[labels[i]].push_back(lines[i]);
    }

    std::vector<cv::Vec4i> mergedLines;
    for (int i = 0; i < linesToMerge.size(); i++) {
        cv::Vec4i line = mergeLines(linesToMerge[i]);
        mergedLines.push_back(line);
    }
    return mergedLines;
}

int GridExtractor::orientation(const cv::Point &p, const cv::Point &q, const cv::Point &r) const {
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool
GridExtractor::doIntersect(const cv::Point &p1, const cv::Point &q1, const cv::Point &p2, const cv::Point &q2) const {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    return (o1 != o2 && o3 != o4);
}

std::vector<cv::Vec4i>
GridExtractor::getIntersectingLines(const cv::Vec4i &line, const std::vector<cv::Vec4i> &lines) const {
    std::vector<cv::Vec4i> intersecting;
    cv::Point p2(line[0], line[1]), q2(line[2], line[3]);
    for (auto &&l : lines) {
        cv::Point p1(l[0], l[1]), q1(l[2], l[3]);
        if (doIntersect(p1, q1, p2, q2)) {
            intersecting.push_back(l);
        }
    }
    return intersecting;
}

double GridExtractor::getLineAngle(const cv::Vec4i &line) const {
    return atan(getSlope(line));
}

std::vector<cv::Vec4i> GridExtractor::getGridLines(const std::vector<cv::Vec4i> &lines) const {
    for (auto &&line : lines) {
        std::vector<cv::Vec4i> intersecting = getIntersectingLines(line, lines);

        if (intersecting.size() >= 2) {
            for (int i = 0; i < intersecting.size() - 1; i++) {
                cv::Vec4i line1 = intersecting[i];
                cv::Vec4i line2 = intersecting[i+1];

                std::vector<cv::Vec4i> line1Intersecting = getIntersectingLines(line1, lines);
                std::vector<cv::Vec4i> line2Intersecting = getIntersectingLines(line2, lines);

                //std::sort(line1Intersecting.begin(), line1Intersecting.end(), compareLines);
                //std::sort(line2Intersecting.begin(), line2Intersecting.end(), compareLines);

                std::vector<cv::Vec4i> common;
                //getting common intersection lines
                std::set_intersection(line1Intersecting.begin(), line1Intersecting.end(), line2Intersecting.begin(),
                                      line2Intersecting.end(), std::back_inserter(common), compareLines);
                if (common.size() >= 2) {
                    //found four grid like intersecting lines lines
                    //check if they are pairs of relatively parallel lines
                    for (int j = 0; j < common.size() - 1; ++j) {
                        const double angleDiffThreshold = 0.610865; // 35 degrees
                        double angle1 = getLineAngle(line1);
                        double angle2 = getLineAngle(line2);
                        double angle3 = getLineAngle(common[j]);
                        double angle4 = getLineAngle(common[j+1]);
                        double line12Diff = fabs(angle1 - angle2);
                        double line34Diff = fabs(angle3 - angle4);
                        if ((line12Diff < angleDiffThreshold || line12Diff > CV_PI - angleDiffThreshold) &&
                            (line34Diff < angleDiffThreshold || line34Diff > CV_PI - angleDiffThreshold)) {
                            return std::vector<cv::Vec4i> {line1, common[0], line2, common[1]};
                        }
                    }
                }
            }
        }
    }
    return std::vector<cv::Vec4i>();
}

cv::Point2f GridExtractor::getIntesectionCoordinate(const cv::Vec4i &line1, const cv::Vec4i &line2) const {
    cv::Point2f p1(line1[0], line1[1]), p2(line1[2], line1[3]);
    cv::Point2f p3(line2[0], line2[1]), p4(line2[2], line2[3]);
    cv::Point2f intersection;
    intersection.x = ((p1.x * p2.y - p1.y * p2.x) * (p3.x - p4.x) - (p1.x - p2.x) * (p3.x * p4.y - p3.y * p4.x)) /
                     ((p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x));
    intersection.y = ((p1.x * p2.y - p1.y * p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x * p4.y - p3.y * p4.x)) /
                     ((p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x));
    return intersection;
}

std::vector<cv::Point2f> GridExtractor::getGridInnerCoordinates(std::vector<cv::Vec4i> lines) const {
    std::vector<cv::Point2f> innerPoints;

    for (int i = 0; i < lines.size() - 1; i++) {
        innerPoints.push_back(getIntesectionCoordinate(lines[i], lines[i + 1]));
    }
    innerPoints.push_back(getIntesectionCoordinate(lines[0], lines.back()));
    cv::Point2f center(0, 0);
    for (auto &&point  : innerPoints) {
        center += point;
    }
    center.x /= innerPoints.size();
    center.y /= innerPoints.size();

    //sorting points clockwise from center staring at "12 o clock"
    std::sort(innerPoints.begin(), innerPoints.end(),
              std::bind(comparePointsClockwise, std::placeholders::_1, std::placeholders::_2, center));
    return innerPoints;
}
