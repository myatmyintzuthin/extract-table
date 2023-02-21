
import cv2
import copy
import json
import os
import argparse
import numpy as np
from typing import Tuple

# -------------------------------------------------------------------
# Class Library
# -------------------------------------------------------------------


class ExtractTable:
    '''
        @brief table extraction class
    '''

    @property
    def imgPath(self) -> str:
        return self.__imgDir

    @property
    def fileName(self) -> str:
        return os.path.splitext(self.imgPath)[0]

    def __init__(self, imgDir: str) -> None:
        '''
        @brief Constructor
        @param imgDir: Target Image Path
        '''
        super(ExtractTable, self).__init__()
        self.__imgDir = imgDir

    def run(self) -> None:

        self.__resizeRatio = 640
        self.__resizeFlag = False
        self.__image = cv2.imread(self.__imgDir)
        self.__orgHeight, self.__orgWidth, _ = self.__image.shape
        self.__resize = self.resizeImg(self.__image)
        processedImg = self.extractBoundary(self.__resize)
        exportImg, exportDict = self.contourDetection(processedImg)
        self.saveResults(exportImg, exportDict)

    def saveResults(self, exportImg: cv2.Mat, exportDict: dict) -> None:
        '''
            @brief save results
            @param exportImg: image to be saved
                   exportDict: json data to be saved
        '''
        cv2.imwrite(self.fileName + '_contour.png', exportImg)
        with open(self.fileName + '.json', 'w') as writer:
            json.dump(exportDict, writer, indent=2)
        print('[ExtractTable::saveResults]: Table Extracted: %s' %
              self.__imgDir)

    def showImage(self, img: cv2.Mat, windowName: str) -> None:
        '''
            @brief show image
            @param img: cv2 read image
            @param windowName: window name
        '''
        cv2.imshow(windowName, img)
        key = cv2.waitKey(0)
        if key == 27:  # wait for ESC key to exit
            cv2.destroyWindow('visualize')

    def calRect(self, contours: np.ndarray) -> Tuple[int, int, int, int]:
        '''
            @brief calculate rectangle
            @param contours: contour area
            @return x, y, width, height
        '''
        x, y, w, h = cv2.boundingRect(contours)
        if self.__resizeFlag:
            w_diff = self.__orgWidth / self.__resizeRatio
            h_diff = self.__orgHeight / self.__resizeRatio
            x = int(x * w_diff)
            y = int(y * h_diff)
            w = int(w * w_diff)
            h = int(h * h_diff)
        return x, y, w, h

    def isBoldText(self, image: cv2.Mat, contour: np.ndarray) -> bool:
        '''
            @brief check whether contour is black text or not
            @param contour: contour area, image 
            @return isBold: True or False
        '''
        x, y, w, h = cv2.boundingRect(contour)
        ct_img = image[y:y+h, x:x+w]

        num_white = np.sum(ct_img == 255)
        num_black = np.sum(ct_img == 0)

        if num_black > num_white:
            return True
        else:
            return False

    def contourDetection(self, processedImg: cv2.Mat) -> Tuple[cv2.Mat, dict]:
        '''
            @brief detect contours on image
            @param processedImg: empty table without text image
            @return contour drew image, contour area dict
        '''
        min_width = 5
        min_height = 5
        exportImg = copy.deepcopy(self.__image)

        exportDict = {
            'width': self.__orgWidth,
            'height': self.__orgHeight,
            'results': []
        }
        contours, hierarchy = cv2.findContours(
            processedImg, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

        for i in range(len(contours)):
            color = np.random.randint(0, 255, 3).tolist()

            if cv2.contourArea(contours[i]) < 200 or self.isBoldText(processedImg, contours[i]):
                continue

            if hierarchy[0][i].any() != -1:
                x, y, w, h = self.calRect(contours[i])
                if w > min_width and h > min_height:
                    exportDict['results'].append(
                        {'x': x, 'y': y, 'width': w, 'height': h})
                    cv2.rectangle(exportImg, (x, y), (x+w, y+h), color, 2)

        self.showImage(exportImg, 'exportImg')
        return exportImg, exportDict

    def resizeImg(self, image: cv2.Mat) -> cv2.Mat:
        '''
            @brief resize image
            @param: cv2 read image, resize ratio
            @return: resized image, resize flag
        '''
        if self.__orgWidth < self.__resizeRatio and self.__orgHeight < self.__resizeRatio:
            image = cv2.resize(
                image, (self.__resizeRatio, self.__resizeRatio), interpolation=cv2.INTER_CUBIC)
            self.__resizeFlag = True
        return image

    def extractBoundary(self, image: cv2.Mat):
        '''
            @brief extract table boundaries by extracting verticle and horizontal lines
            @param image: cv2 read image
            @return empty table
        '''
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        
        invertImg = 255-gray
        _, edges = cv2.threshold(
            invertImg, 200, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)

        kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (2, 2))

        vertical_kernel = cv2.getStructuringElement(
            cv2.MORPH_RECT, (1, np.array(image).shape[1]//100))
        vertical_lines = cv2.erode(edges, vertical_kernel, iterations=3)
        vertical_lines = cv2.dilate(
            vertical_lines, vertical_kernel, iterations=3)

        hor_kernel = cv2.getStructuringElement(
            cv2.MORPH_RECT, (np.array(image).shape[1]//100, 1))
        horizontal_lines = cv2.erode(edges, hor_kernel, iterations=3)
        horizontal_lines = cv2.dilate(
            horizontal_lines, hor_kernel, iterations=3)

        empty_table = cv2.addWeighted(
            vertical_lines, 0.5, horizontal_lines, 0.5, 0.0)
        empty_table = cv2.erode(~empty_table, kernel, iterations=2)
        _, empty_table = cv2.threshold(
            empty_table, 128, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)
        # self.showImage(empty_table, 'table')
        return empty_table


# -------------------------------------------------------------------
# Main Routine
# -------------------------------------------------------------------
if __name__ == '__main__':

    parser = argparse.ArgumentParser(
        prog='extract table',
        description='extract table from image'
    )
    parser.add_argument(
        '-i', '--input', help='path to input image'
    )
    args = parser.parse_args()
    extractor = ExtractTable(args.input)
    extractor.run()
