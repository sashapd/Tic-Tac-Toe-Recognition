//
// Created by Oleksandr Padko on 8/21/17.
//

#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv/cv.hpp>
#include "GridDrawer.h"

GridDrawer::GridDrawer(Grid grid): crossColor(255, 0, 0), circleColor(0, 0, 255) {
    mGrid = grid;
}

void GridDrawer::drawCircle(int x, int y) {
    cv::Mat cellImg = mGrid.getCellImage(x, y);
    int radius = cellImg.cols * 0.35;
    cv::Point center(cellImg.cols / 2, cellImg.rows / 2);
    cv::circle(cellImg, center, radius, circleColor, 3);
}

void GridDrawer::drawCross(int x, int y) {
    cv::Mat cellImg = mGrid.getCellImage(x, y);
    int offset = cellImg.cols * 0.2;
    cv::Point p1(offset, offset), p2(cellImg.cols - offset, cellImg.rows - offset);
    cv::Point p3(offset, cellImg.rows - offset), p4(cellImg.cols - offset, offset);
    cv::line(cellImg, p1, p2, crossColor, 3);
    cv::line(cellImg, p3, p4, crossColor, 3);
}

void GridDrawer::drawWinnerLine(int x1, int y1, int x2, int y2, Cell winner) {
    cv::Mat gridImage = mGrid.getImage();
    int cellWidth = gridImage.cols / 3;
    int cellHeigh = gridImage.rows / 3;
    cv::Point p1(x1 * cellWidth + cellWidth/2, y1 * cellHeigh + cellHeigh/2);
    cv::Point p2(x2 * cellWidth + cellWidth/2, y2 * cellHeigh + cellHeigh/2);
    cv::Scalar color;
    if(winner == X) {
        color = crossColor;
    } else {
        color = circleColor;
    }
    cv::line(gridImage, p1, p2, color, 3);
}