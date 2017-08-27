//
// Created by Oleksandr Padko on 8/16/17.
//

#ifndef TICTACTOERECOGNITION_GRIDEXTRACTOR_H
#define TICTACTOERECOGNITION_GRIDEXTRACTOR_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include "Grid.h"

class GridExtractor {
public:
    GridExtractor(const cv::Mat &image);

    void extractGrid();

    Grid getGrid() const;

    cv::Mat getImage() const;

    bool hasFoundGrid() const;

    void putBackGrid(Grid grid) const;

private:
    cv::Mat mImage;
    cv::Mat mGridImage;
    std::vector<cv::Point2f> gridCoordinates;
    bool foundGrid = false;

    std::vector<cv::Vec4i> findLines();

    static bool compareLines(const cv::Vec4i &line1, const cv::Vec4i &line2);

    static bool comparePointsClockwise(cv::Point a, cv::Point b, cv::Point center);

    static bool areSimmilar(cv::Vec4i line1, cv::Vec4i line2);

    cv::Vec4i mergeLines(const std::vector<cv::Vec4i> &lines) const;

    std::vector<cv::Vec4i> filterSimmilar(std::vector<cv::Vec4i> lines) const;

    std::vector<cv::Vec4i> getGridLines(const std::vector<cv::Vec4i> &lines) const;

    std::vector<cv::Point2f> getGridInnerCoordinates(std::vector<cv::Vec4i> lines) const;
};


#endif //TICTACTOERECOGNITION_GRIDEXTRACTOR_H
