cmake_minimum_required(VERSION 3.8)
project(TicTacToeRecognition)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package( OpenCV REQUIRED )

set(SOURCE_FILES main.cpp GridExtractor.cpp GridExtractor.h Grid.cpp Grid.h CellClassifier.cpp CellClassifier.h Cell.h GameAutomate.cpp GameAutomate.h GameState.cpp GameState.h GridDrawer.cpp GridDrawer.h TicTacToeActivity.cpp TicTacToeActivity.h GeometricUtilities.cpp GeometricUtilities.h)
add_executable(TicTacToeRecognition ${SOURCE_FILES})
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries( TicTacToeRecognition ${OpenCV_LIBS} )