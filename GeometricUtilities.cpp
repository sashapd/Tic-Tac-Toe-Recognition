//
// Created by Oleksandr Padko on 8/27/17.
//

#include "GeometricUtilities.h"

int GeometricUtilities::orientation(const cv::Point &p, const cv::Point &q, const cv::Point &r) {
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool
GeometricUtilities::doIntersect(const cv::Point &p1, const cv::Point &q1, const cv::Point &p2, const cv::Point &q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    return (o1 != o2 && o3 != o4);
}

std::vector<cv::Vec4i>
GeometricUtilities::getIntersectingLines(const cv::Vec4i &line, const std::vector<cv::Vec4i> &lines) {
    std::vector<cv::Vec4i> intersecting;
    cv::Point p2(line[0], line[1]), q2(line[2], line[3]);
    for (auto &&l : lines) {
        cv::Point p1(l[0], l[1]), q1(l[2], l[3]);
        if (doIntersect(p1, q1, p2, q2)) {
            intersecting.push_back(l);
        }
    }
    return intersecting;
}

double GeometricUtilities::getLineAngle(const cv::Vec4i &line) {
    return atan(getSlope(line));
}

double GeometricUtilities::getSlope(const cv::Vec4i &line) {
    return (double) (line[1] - line[3]) / (line[0] - line[2] + 10e-8);
}

cv::Point2f GeometricUtilities::getIntesectionCoordinate(const cv::Vec4i &line1, const cv::Vec4i &line2) {
    cv::Point2f p1(line1[0], line1[1]), p2(line1[2], line1[3]);
    cv::Point2f p3(line2[0], line2[1]), p4(line2[2], line2[3]);
    cv::Point2f intersection;
    intersection.x = ((p1.x * p2.y - p1.y * p2.x) * (p3.x - p4.x) - (p1.x - p2.x) * (p3.x * p4.y - p3.y * p4.x)) /
                     ((p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x));
    intersection.y = ((p1.x * p2.y - p1.y * p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x * p4.y - p3.y * p4.x)) /
                     ((p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x));
    return intersection;
}

double GeometricUtilities::distToLine(const cv::Point &point, cv::Vec4i line) {
    double dist = fabs(
            (line[3] - line[1]) * point.x - (line[2] - line[0]) * point.y + line[2] * line[1] - line[3] * line[0])
                  / sqrt(pow(line[3] - line[1], 2) + pow(line[2] - line[0], 2));
    return dist;
}

