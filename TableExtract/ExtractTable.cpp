#include <cstdio>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "TableExtract/ExtractTable.h"
using namespace std;
using namespace cv;
using json = nlohmann::json;

ExtractTable::ExtractTable(string imagePath){
    this->imgPath = imagePath;

};

ExtractTable::~ExtractTable(){
    //nothing to do
}

void ExtractTable::runProcess(){
    
    this->image = imread(this->getImgPath(), IMREAD_COLOR);
    this->resizeImg = this->resizeImage(this->image); 

    Mat processedImg = this->extractBoundary( this->resizeImg );
    this->contourDetection(processedImg);

}

void ExtractTable::saveResults( Mat image ){

    json exportDict;
    exportDict["width"] = this->getImgWidth();
    exportDict["height"] = this->getImgHeight();
    exportDict["results"] = this->result;

    ofstream file( this->getFilename() + ".json");
    file << exportDict.dump(2);

    printf("[ExtractTable::saveResults]: Table Extracted: %s", this->getImgPath().c_str());
    
    imwrite( this->getFilename() + "_contour.png", image);
    imshow("Final Result", image);
    waitKey(0);
}

Mat ExtractTable::resizeImage( Mat image ){

    Mat resized;
    if(this->getImgWidth() < this->resizeRatio && this->getImgHeight() < this->resizeRatio){
        resize(image, resized, Size(this->resizeRatio, this->resizeRatio), INTER_CUBIC);
        this->resizeFlag = true;
        return resized;
    }
    return image;
}

bool ExtractTable::isBoldText( Mat image, vector<Point> contours){

    Rect boundRect;
    Mat contourImg;
    boundRect = boundingRect( contours );
    contourImg = image(boundRect);
    
    vector<unsigned char> imageData(contourImg.begin<unsigned char>(), contourImg.end<unsigned char>());
    int num_white = count(imageData.begin(), imageData.end(), 255);
    int num_black = count(imageData.begin(), imageData.end(), 0);

    if(num_black < num_white){
        return false;
    }
    return true;
    
}

Rect ExtractTable::calRect( vector<Point> contours ){

    Rect rect = boundingRect(contours);
    if(this->resizeFlag){

        float w_diff = float(this->getImgWidth()) / float(this->resizeRatio);
        float h_diff = float(this->getImgHeight()) / float(this->resizeRatio);
        rect.x = (int)(rect.x * w_diff);
        rect.y = (int)(rect.y * h_diff);
        rect.width = (int)(rect.width * w_diff);
        rect.height = (int)(rect.height * h_diff);
    }
    return rect;
}

void ExtractTable::contourDetection( Mat processedImg ){

    Mat exportImg = this->image;
    int minTableWidth = 5, minTableHeight = 5;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    map<string, int> rectDict;
    Rect boundRect;
    RNG rng(12345);

    findContours( processedImg, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );
    for( unsigned int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );

        if(contourArea(contours[i]) < 200 || this->isBoldText(processedImg, contours[i])){
            continue;
        }
    
        if(hierarchy[i][0] != -1 || hierarchy[i][1] != -1 || hierarchy[i][2] != -1 || hierarchy[i][3] != -1){
            boundRect = this->calRect( contours[i] );
            if( boundRect.width > minTableWidth && boundRect.height > minTableHeight){
                rectDict["x"] = boundRect.x;
                rectDict["y"] = boundRect.y;
                rectDict["width"] = boundRect.width;
                rectDict["height"] = boundRect.height;
                this->result.push_back(rectDict);
                rectangle( exportImg, boundRect.tl(), boundRect.br(), color, 2 );
            } 
        }
    }
    this->saveResults(exportImg);
}

Mat ExtractTable::extractBoundary( Mat image ){

    Mat gray, invertedImg, edges, kernel, verKernel, horKernel, verLine, horLine, emptyTable;
    
    cvtColor(image, gray, COLOR_BGR2GRAY);
    invertedImg = 255-gray;
    threshold( invertedImg, edges, 200, 255, THRESH_BINARY | THRESH_OTSU);
    
    kernel = getStructuringElement( MORPH_RECT, Size( 2, 2 ));

    verKernel = getStructuringElement( MORPH_RECT, Size( 1, this->getImgWidth()/100 ));
    erode( edges, verLine, verKernel, Point(-1, -1), 3 );
    dilate( verLine, verLine, verKernel , Point(-1, -1), 3);

    horKernel = getStructuringElement( MORPH_RECT, Size( this->getImgWidth()/100, 1 ));
    erode( edges, horLine, horKernel, Point(-1, -1), 3 );
    dilate( horLine, horLine, horKernel, Point(-1, -1), 3 );

    addWeighted(verLine, 0.5, horLine, 0.5, 0.0, emptyTable);
    erode(~emptyTable, emptyTable, kernel, Point(-1, -1), 2);
    threshold(emptyTable, emptyTable, 128, 255, THRESH_BINARY | THRESH_OTSU);
    
    return emptyTable;
}

