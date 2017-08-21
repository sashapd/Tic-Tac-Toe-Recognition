//
// Created by Oleksandr Padko on 8/20/17.
//

#include "Grid.h"
#include "CellClassifier.h"

Grid::Grid(){}

Grid::Grid(cv::Mat gridImage) {
    mGridImage = gridImage;
}

Grid::Grid(const Grid &grid) {
    mGridImage = grid.mGridImage;
}

Grid &Grid::operator=(const Grid &grid) {
    mGridImage = grid.mGridImage;
    return *this;
}

cv::Mat Grid::getImage() {
    return mGridImage;
}

Cell Grid::getCellValue(int x, int y) {
    cv::Mat cellImg = getCellImage(x, y);

    CellClassifier classifier(cellImg);

    Cell value = classifier.getCellValue();

    return value;
}

cv::Mat Grid::getCellImage(int x, int y) {
    int cellHeigh = mGridImage.rows / 3;
    int cellWidth = mGridImage.cols / 3;

    cv::Rect roiRect(x * cellWidth, y * cellHeigh, cellWidth, cellHeigh);
    cv::Mat cellRoi = mGridImage(roiRect);

    return cellRoi;
}