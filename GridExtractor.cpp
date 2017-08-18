//
// Created by Oleksandr Padko on 8/16/17.
//
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include "GridExtractor.h"


GridExtractor::GridExtractor(const cv::Mat &image) {
    mImage = image;
}

void GridExtractor::extractGrid() {
    std::vector<cv::Vec4i> lines = findLines();

    std::vector<cv::Vec4i> gridLines = getGridLines(lines);

    for (size_t i = 0; i < gridLines.size(); i++) {
        cv::Vec4i l = gridLines[i];
        cv::line(mImage, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
    }

    if (gridLines.size() == 4) {
        foundGrid = true;

        std::vector<cv::Point2f> gridCoordinates = getGridCoordinates(gridLines);

        cv::Mat gridImage(512, 512, CV_8UC3);
        cv::Point2f dstPoint[4] = {cv::Point2f(0, 0), cv::Point2f(0, gridImage.rows),
                                   cv::Point2f(gridImage.cols, gridImage.rows), cv::Point2f(gridImage.cols, 0)};

        cv::Mat transformMatr = cv::getPerspectiveTransform(gridCoordinates.data(), dstPoint);

        cv::warpPerspective(mImage, gridImage, transformMatr, gridImage.size());

        mGrid = gridImage;
    }
}

cv::Mat GridExtractor::getGrid() {
    return mGrid;
}

cv::Mat GridExtractor::getImage() {
    return mImage;
}

bool GridExtractor::hasFoundGrid() const {
    return foundGrid;
}

std::vector<cv::Vec4i> GridExtractor::findLines() {
    cv::Mat canny_output;
    cv::Canny(mImage, canny_output, 50, 200, 3);

    int dialationSize = 6;
    cv::Mat dialationElement = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                                         cv::Size(2 * dialationSize + 1, 2 * dialationSize + 1),
                                                         cv::Point(dialationSize, dialationSize));
    cv::Mat dilated;
    cv::dilate(canny_output, dilated, dialationElement);

    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(dilated, lines, 5, CV_PI / 90, 100, 100, 3);

    lines = filterSimmilar(lines, 0.349066, 10);

    return lines;
}

double GridExtractor::getSlope(const cv::Vec4i &line) const {
    return (double) (line[1] - line[3]) / (line[0] - line[2]);
}

std::vector<cv::Vec4i>
GridExtractor::filterSimmilar(std::vector<cv::Vec4i> lines, double angleThresh, double lengthThresh) const {
    std::vector<cv::Vec4i> filtered;
    std::sort(lines.begin(), lines.end(), compareLines);

    for (const auto &line : lines) {
        cv::Point point1(line[0], line[1]), point2(line[2], line[3]);
        double slope = getSlope(line);
        double angle = fabs(atan(slope));
        bool isSimmilar = false;
        for (const auto &filteredLine : filtered) {
            cv::Point p1(filteredLine[0], filteredLine[1]), p2(filteredLine[2], filteredLine[3]);
            double filteredSlope = getSlope(filteredLine);
            double filteredAngle = fabs(atan(filteredSlope));
            double filteredLineLenth = cv::norm(p1 - p2);

            if (fabs(angle - filteredAngle) < angleThresh &&
                (fabs(filteredLineLenth - (cv::norm(p1 - point1) + cv::norm(p2 - point1))) < lengthThresh ||
                 fabs(filteredLineLenth - (cv::norm(p1 - point2) + cv::norm(p2 - point2))) < lengthThresh)) {
                isSimmilar = true;
                break;
            }
        }
        if (!isSimmilar) {
            filtered.push_back(line);
        }
    }
    return filtered;
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

std::vector<cv::Vec4i> GridExtractor::getGridLines(const std::vector<cv::Vec4i> &lines) const {
    for (auto &&line : lines) {
        std::vector<cv::Vec4i> intersecting = getIntersectingLines(line, lines);
        if (intersecting.size() == 2) {
            cv::Vec4i line1 = intersecting[0];
            cv::Vec4i line2 = intersecting[1];
            std::vector<cv::Vec4i> line1Intersecting = getIntersectingLines(line1, lines);
            std::vector<cv::Vec4i> line2Intersecting = getIntersectingLines(line2, lines);
            std::sort(line1Intersecting.begin(), line1Intersecting.end(), compareLines);
            std::sort(line2Intersecting.begin(), line2Intersecting.end(), compareLines);
            if (line1Intersecting.size() == 2 && line1Intersecting == line2Intersecting) {
                return std::vector<cv::Vec4i> {line1, line2, line1Intersecting[0], line1Intersecting[1]};
            }
        }
    }
    return std::vector<cv::Vec4i>();
}

cv::Point GridExtractor::getIntesectionCoordinate(const cv::Vec4i &line1, const cv::Vec4i &line2) const {
    cv::Point p1(line1[0], line1[1]), p2(line1[2], line1[3]);
    cv::Point p3(line2[0], line2[1]), p4(line2[2], line2[3]);
    cv::Point intersection;
    intersection.x = (p1.x*p2.y - p1.y*p2.x) * (p3.x - p4.x) - (p1.x - p2.x) * (p3.x*p4.y - p3.y*p4.x) /
                     (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);
    intersection.y = (p1.x*p2.y - p1.y*p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x*p4.y - p3.y*p4.x) /
                     (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);
    return intersection;
}

std::vector<cv::Point2f> GridExtractor::getGridCoordinates(std::vector<cv::Vec4i> lines) const {
    int minX = INT_MAX, minY = INT_MAX, maxX = 0, maxY = 0;
    for (auto &&line : lines) {
        minX = std::min(line[0], minX);
        minX = std::min(line[2], minX);

        minY = std::min(line[1], minY);
        minY = std::min(line[3], minY);

        maxX = std::max(line[0], maxX);
        maxX = std::max(line[2], maxX);

        maxY = std::max(line[1], maxY);
        maxY = std::max(line[3], maxY);
    }
    return std::vector<cv::Point2f> {cv::Point2f(minX, minY), cv::Point2f(minX, maxY),
                                     cv::Point2f(maxX, maxY), cv::Point2f(maxX, minY)};
}
