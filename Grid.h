//
// Created by Oleksandr Padko on 8/20/17.
//

#ifndef TICTACTOERECOGNITION_GRID_H
#define TICTACTOERECOGNITION_GRID_H


#include <opencv2/core/mat.hpp>
#include "Cell.h"
#include "GridDrawer.h"

class Grid {
public:
    Grid();

    Grid(cv::Mat gridImage);

    Grid(const Grid &grid);

    Grid &operator=(const Grid &grid);

    cv::Mat getImage();

    Cell getCellValue(int x, int y);

private:
    cv::Mat mGridImage;

    cv::Mat getCellImage(int x, int y);

    friend class GridDrawer;
};


#endif //TICTACTOERECOGNITION_GRID_H
