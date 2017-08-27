//
// Created by Oleksandr Padko on 8/27/17.
//

#ifndef TICTACTOERECOGNITION_GEOMETRICUTILITIES_H
#define TICTACTOERECOGNITION_GEOMETRICUTILITIES_H


#include <opencv2/core/matx.hpp>
#include <opencv2/core/types.hpp>

class GeometricUtilities {
public:
    static int orientation(const cv::Point &p, const cv::Point &q, const cv::Point &r);

    static bool doIntersect(const cv::Point &p1, const cv::Point &q1, const cv::Point &p2, const cv::Point &q2);

    static std::vector<cv::Vec4i> getIntersectingLines(const cv::Vec4i &line, const std::vector<cv::Vec4i> &lines);

    static double getLineAngle(const cv::Vec4i &line);

    static double getSlope(const cv::Vec4i &line);

    static cv::Point2f getIntesectionCoordinate(const cv::Vec4i &line1, const cv::Vec4i &line2);

    static double distToLine(const cv::Point &point, cv::Vec4i line);
};


#endif //TICTACTOERECOGNITION_GEOMETRICUTILITIES_H
