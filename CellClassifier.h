//
// Created by Oleksandr Padko on 8/20/17.
//

#ifndef TICTACTOERECOGNITION_CELLCLASSIFIER_H
#define TICTACTOERECOGNITION_CELLCLASSIFIER_H


#include <opencv2/core/mat.hpp>
#include "Cell.h"

class CellClassifier {
public:
    CellClassifier(cv::Mat cellImage);

    Cell getCellValue();

private:
    cv::Mat mCellImage;

    bool isCircle();

    bool isCross();
};


#endif //TICTACTOERECOGNITION_CELLCLASSIFIER_H
