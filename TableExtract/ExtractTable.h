
#include <map>
#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class ExtractTable{
    public:

        /**
         * @brief Get the Img Path object
         * 
         * @return string 
         */
        inline string getImgPath(){
            return this->imgPath;
        }

        /**
         * @brief Get the Filename object
         * 
         * @return string 
         */
        inline string getFilename(){
            const std::filesystem::path path = this->imgPath;
            return path.parent_path().string() + "/" + path.stem().string();
        }

        /**
         * @brief Get the Img Width object
         * 
         * @return int 
         */
        inline int getImgWidth(){
            return this->image.size[1];
        }

        /**
         * @brief Get the Img Height object
         * 
         * @return int 
         */
        inline int getImgHeight(){
            return this->image.size[0];
        }

        /**
         * @brief Construct a new Extract Table object
         * 
         * @param imgPath 
         */
        ExtractTable( string imagePath );

        /**
         * @brief Destroy the Extract Table object
         * 
         */
        ~ExtractTable();

        /**
         * @brief Run extract table process
         * 
         */
        void runProcess();
        
    private:

        /* Target image path */
        string imgPath;

        /* cv2 image */
        Mat image, resizeImg;

        /* Results dictionary */
        vector<map<string, int>> result;
        
        /* resize flag */
        bool resizeFlag = false;

        /* image resize ratio */
        int resizeRatio = 640;

        /**
         * @brief extract table boundary in image
         * @return extract table image
         */
        Mat extractBoundary( Mat image);

        /**
         * @brief contour detection
         * @param image 
         */
        void contourDetection( Mat image );

        /**
         * @brief Resize Image
         * 
         * @param image 
         * @return Mat 
         */
        Mat resizeImage( Mat image );

        /**
         * @brief check is bold text detected as table cell
         * @param image, contours
         * @return bool
         */
        bool isBoldText( Mat image, vector<Point> contour );

        /**
         * @brief recalculate rectangle
         * 
         * @param contour 
         * @return Rect 
         */
        Rect calRect( vector<Point> contour );

        /**
         * @brief save result image and json
         * 
         * @param image 
         */
        void saveResults( Mat image );

};