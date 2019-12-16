#include "classifier.h"

/**
 * @brief Classifier::Classifier
 * @param modelConfFilename - file with yolov3 configs
 * @param modelWeightsFilename - file with yolov3 weights
 * @param modelClassesFilename - file with names of classes to classify
 */
Classifier::Classifier(std::string modelConfFilename, std::string modelWeightsFilename,
                       std::string modelClassesFilename)
    : modelConfFilename_(modelConfFilename),
      modelWeightsFilename_(modelWeightsFilename),
      modelClassesFilename_(modelClassesFilename) {
    init();
}

/**
 * @brief Classifier::classify - returns vector of matrices with results of classify
 * @param img - image to classify
 * @return returns vector of matrices with results of classify:
 * each matrix in vector includes a result for each network layer
 * each row in matrix includes a rectangle (first 4 elements) and confidences for each class
 * (elements after 4th element)
 */
std::vector<cv::Mat> Classifier::classify(cv::Mat img) {
    cv::Mat blob;
    cv::dnn::blobFromImage(img, blob, 1 / 255.0, cv::Size(inpWidth_, inpHeight_),
                           cv::Scalar(0, 0, 0), true, false);
    // Sets the input to the network
    net_.setInput(blob);

    std::vector<cv::Mat> outs;
    net_.forward(outs, net_.getLayerNames());
    return outs;
}

/**
 * @brief Classifier::init - load all necessary files passed in constructor and runs darknetNetwork
 */
void Classifier::init() {
    std::ifstream ifs(modelClassesFilename_);  // Names of classes
    for (std::string line; getline(ifs, line);) {
        classes_.push_back(line);
    }

    // Load the network
    net_ = cv::dnn::readNetFromDarknet(modelConfFilename_, modelWeightsFilename_);
}

/**
 * @brief Classifier::parseClassified do NMS supression and delete detected objects with confidence
 * less then confTreshold_
 * @param outs
 * @return
 */
ClassifiedImageInfo Classifier::parseClassified(std::vector<cv::Mat> outs, int imgWidth,
                                                int imgHeight) {
    std::vector<int> resClassIds;
    std::vector<float> resConfidences;
    std::vector<cv::Rect> resBoxes;

    for (auto curLayerOuts : outs) {
        for (int i = 0; i < curLayerOuts.rows; ++i) {
            auto curObjInfo = curLayerOuts.row(i);
            cv::Mat curObjConfidences = curObjInfo.colRange(5, curLayerOuts.cols);

            // Find best confidence
            cv::Point classIdPoint;
            double bestConfidence;
            minMaxLoc(curObjConfidences, nullptr, &bestConfidence, nullptr, &classIdPoint);

            // If best confidence is not less than threshold
            // then add this class in results
            if (bestConfidence > confThreshold_) {
                std::vector<float> curObjRect = curObjInfo.colRange(0, 4);
                int centerX = static_cast<int>(curObjRect[0] * imgWidth);
                int centerY = static_cast<int>(curObjRect[1] * imgHeight);
                int width = static_cast<int>(curObjRect[2] * imgWidth);
                int height = static_cast<int>(curObjRect[3] * imgHeight);
                int left = centerX - width / 2;
                int top = centerY - height / 2;

                resClassIds.push_back(classIdPoint.x);
                resConfidences.push_back(static_cast<float>(bestConfidence));
                resBoxes.push_back(cv::Rect(left, top, width, height));
            }
        }
    }

    // Perform non maximum suppression to eliminate redundant overlapping boxes
    // with lower confidences
    ClassifiedImageInfo imgInfo;
    std::vector<int> nmsIndices;
    cv::dnn::NMSBoxes(resBoxes, resConfidences, confThreshold_, nmsThreshold_, nmsIndices);
    for (size_t i = 0; i < nmsIndices.size(); ++i) {
        ObjectInfo objInfo;
        int curObjectIndex = nmsIndices[i];

        auto& r = resBoxes[curObjectIndex];
        objInfo.rect = QRect(r.x, r.y, r.width, r.height);
        objInfo.classId = resClassIds[curObjectIndex];
        objInfo.className = QString(classes_[objInfo.classId].c_str());
        objInfo.confidence = resConfidences[curObjectIndex];

        imgInfo.objects.push_back(std::move(objInfo));
    }

    return imgInfo;
}
