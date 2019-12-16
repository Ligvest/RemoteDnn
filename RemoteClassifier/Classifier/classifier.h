#pragma once

#include <QRect>
#include <QString>
#include <fstream>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include "classifiedimageinfo.h"

/**
 * @brief Classifier - wrapper over cv::dnn::Net to simplify classify objects by Darknet model
 */
class Classifier {
   public:
    Classifier(std::string modelConfFilename, std::string modelWeightsFilename,
               std::string modelClassesFilename);

    std::vector<cv::Mat> classify(cv::Mat img);
    void init();
    ClassifiedImageInfo parseClassified(std::vector<cv::Mat>, int imgWidth, int imgHeight);

   private:
    // Neural network
    cv::dnn::Net net_;

    // Initialize the parameters
    float confThreshold_ = 0.5f;  // Confidence threshold
    float nmsThreshold_ = 0.4f;   // Non-maximum suppression threshold
    int inpWidth_ = 416;          // Width of network's input image
    int inpHeight_ = 416;         // Height of network's input image
    std::vector<std::string> classes_;

    // Filenames
    std::string modelConfFilename_;
    std::string modelWeightsFilename_;
    std::string modelClassesFilename_;
};
