//
// Created by Oleksandr Padko on 8/20/17.
//

#ifndef TICTACTOERECOGNITION_GRID_H
#define TICTACTOERECOGNITION_GRID_H


#include <opencv2/core/mat.hpp>

class Grid {
public:
    Grid(cv::Mat gridImage);

private:
    cv::Mat mGridImage;
};


#endif //TICTACTOERECOGNITION_GRID_H
