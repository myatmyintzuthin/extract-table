

#include <iostream>
#include <opencv2/opencv.hpp>
#include "TableExtract/ExtractTable.h"

using namespace std;

int main(int argc, char** argv)
{
    string tablePath = argv[1];

    ExtractTable myTable(tablePath);
    myTable.runProcess();

    return 0;
}
