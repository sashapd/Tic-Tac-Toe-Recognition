//
// Created by Oleksandr Padko on 8/16/17.
//

#ifndef TICTACTOERECOGNITION_GRIDEXTRACTOR_H
#define TICTACTOERECOGNITION_GRIDEXTRACTOR_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/core/mat.hpp>

class GridExtractor {
public:
    GridExtractor(const cv::Mat &image);

    void extractGrid();

    cv::Mat getGrid();

    cv::Mat getImage();

private:
    cv::Mat mImage;
    cv::Mat mGrid;

    std::vector<cv::Vec4i> findLines();

    double getSlope(const cv::Vec4i &line) const;

    //bool compareLines(const cv::Vec4i &line1, const cv::Vec4i &line2) const;

    std::vector<cv::Vec4i> filterSimmilar(std::vector<cv::Vec4i> &lines, double angleThresh, double lengthThresh) const;

    int orientation(const cv::Point &p, const cv::Point &q, const cv::Point &r) const;

    bool doIntersect(const cv::Point &p1, const cv::Point &q1, const cv::Point &p2, const cv::Point &q2) const;

    std::vector<cv::Vec4i> getIntersectingLines(const cv::Vec4i &line, const std::vector<cv::Vec4i> &lines) const;

    std::vector<cv::Vec4i> getGridLines(const std::vector<cv::Vec4i> &lines) const;

    std::vector<cv::Point2f> getGridCoordinates(const std::vector<cv::Vec4i> &lines) const;
};


#endif //TICTACTOERECOGNITION_GRIDEXTRACTOR_H
