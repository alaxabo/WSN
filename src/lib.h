/*
 * lib.h
 *
 *  Created on: Nov 19, 2015
 *      Author: Administrator
 */

#ifndef LIB_H_
#define LIB_H_
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>
#include <vector>
#include <cstring>
#include <string.h>
#include <map>
#include <algorithm>
#include <cmath>
using namespace std;

class lib {
    public:
        lib();
        virtual ~lib();
        vector<string> splitString_C(string input, string delimiter);
        double entropy(vector<double> arr);
        double log2(double number);
        double findK(double b, double n);
        double jEntropy(vector<double> arr1, vector<double> arr2);
        double EntropyCorrelationCoefficient(vector<double> arr1, vector<double> arr2);
        vector<double> extractColumn(vector<vector<double>> data, int column);
        double jEntropyGroup(vector<vector<double>> data);
};


#endif /* LIB_H_ */
