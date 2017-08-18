//
// Created by Oleksandr Padko on 8/16/17.
//

#ifndef TICTACTOERECOGNITION_GRIDEXTRACTOR_H
#define TICTACTOERECOGNITION_GRIDEXTRACTOR_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc.hpp"

class GridExtractor {
public:
    GridExtractor(const cv::Mat &image);

    void extractGrid();

    cv::Mat getGrid();

    cv::Mat getImage();

    bool hasFoundGrid() const;

private:
    cv::Mat mImage;
    cv::Mat mGrid;
    bool foundGrid = false;

    std::vector<cv::Vec4i> findLines();

    double getSlope(const cv::Vec4i &line) const;

    static bool compareLines(const cv::Vec4i &line1, const cv::Vec4i &line2) {
        double line1Length = pow(pow(line1[0] - line1[2], 2) + pow(line1[1] - line1[3], 2), 0.5);
        double line2Length = pow(pow(line2[0] - line2[2], 2) + pow(line2[1] - line2[3], 2), 0.5);
        return line1Length > line2Length;
    }

    static bool comparePointsClockwise(cv::Point a, cv::Point b, cv::Point center)
    {
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

    std::vector<cv::Vec4i> filterSimmilar(std::vector<cv::Vec4i> lines, double angleThresh, double lengthThresh) const;

    int orientation(const cv::Point &p, const cv::Point &q, const cv::Point &r) const;

    bool doIntersect(const cv::Point &p1, const cv::Point &q1, const cv::Point &p2, const cv::Point &q2) const;

    std::vector<cv::Vec4i> getIntersectingLines(const cv::Vec4i &line, const std::vector<cv::Vec4i> &lines) const;

    std::vector<cv::Vec4i> getGridLines(const std::vector<cv::Vec4i> &lines) const;

    cv::Point getIntesectionCoordinate(const cv::Vec4i &line1, const cv::Vec4i &line2) const;

    std::vector<cv::Point2f> getGridCoordinates(std::vector<cv::Vec4i> lines) const;
};


#endif //TICTACTOERECOGNITION_GRIDEXTRACTOR_H
