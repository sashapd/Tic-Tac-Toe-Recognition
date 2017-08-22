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
    cv::Mat mReflectionless;

    bool isCircle();

    int orientation(const cv::Point &p, const cv::Point &q, const cv::Point &r) const;

    bool doIntersect(const cv::Point &p1, const cv::Point &q1, const cv::Point &p2, const cv::Point &q2) const;

    bool isCross();
};


#endif //TICTACTOERECOGNITION_CELLCLASSIFIER_H
