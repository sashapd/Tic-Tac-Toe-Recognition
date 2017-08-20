#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/core/mat.hpp>
#include "GridExtractor.h"

int main() {
    cv::Mat im = cv::imread("hoax4.jpg");
    cv::Mat image;
    cv::resize(im, image, cv::Size(), 0.5, 0.5);

    GridExtractor extractor(image);
    extractor.extractGrid();

    if (!extractor.hasFoundGrid()) {
        std::cout << "Grid not found" << std::endl;
    } else {
        Grid grid = extractor.getGrid();
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                Cell c = grid.getCellValue(j, i);
                if(c == O) {
                    std::cout << "O" << " ";
                } else if(c == X) {
                    std::cout << "X" << " ";
                } else {
                    std::cout << " " << " ";
                }
            }
            std::cout << std::endl;
        }
        cv::imshow("image", grid.getImage());
        cv::waitKey(0);
    }

    return 0;
}