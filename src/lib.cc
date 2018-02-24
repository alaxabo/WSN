/*
 * lib.cc
 *
 *  Created on: Nov 19, 2015
 *      Author: Administrator
 */
#include "lib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>
#include <vector>
#include <cstring>
#include <string.h>
using namespace std;
lib::lib(){

}

lib::~lib(){

}

vector<string> lib::splitString_C(string input, string delimiter)
{
     vector<string> output;
     char *pch;

     char *str = strdup(input.c_str());

     pch = strtok (str, delimiter.c_str());

     while (pch != NULL)
     {
        output.push_back(pch);

        pch = strtok (NULL,  delimiter.c_str());
     }

     free(str);

     return output;
}
double lib::log2(double number){
    return log( number ) / log( 2 );
}

double lib::entropy(vector<double> arr){

    std::map<double,double> frequencies;
    double infocontent = 0;
    std::map<double, double>::iterator iter;
    for(int i = 0; i < arr.size(); i++){
        frequencies[ arr[i] ]++;
    }
    for(iter = frequencies.begin(); iter != frequencies.end(); iter++){
        double freq = static_cast<double>(iter->second)/arr.size();
        infocontent += freq * log2( freq );
    }

    infocontent *= -1;
    return infocontent;
}

double lib::jEntropy(vector<double> arr1, vector<double> arr2){
    std::map<std::pair<double,double>,int> frequencies;
    std::map<std::pair<double,double>,int>::iterator iter;
    std::map<std::pair<double,double>,int>::iterator iter2;
    double infocontent = 0;
    for(int i = 0; i < arr1.size(); i++){
        frequencies[ make_pair( arr1[i],arr2[i]) ]++;

    }
    for(iter = frequencies.begin(); iter != frequencies.end(); iter++){
        double freq = static_cast<double>(iter->second)/arr1.size();
        infocontent += freq * log2( freq );

    }
    infocontent *= -1;
    return infocontent;

}

double lib::EntropyCorrelationCoefficient(vector<double> arr1, vector<double> arr2){
    return 2-(2*jEntropy(arr1,arr2)/(entropy(arr1)+entropy(arr2)));
}

vector<double> lib::extractColumn(vector<vector<double>> data, int column){
  vector<double> result;

  for(vector<double> v : data){
    result.push_back(v[column]);
  }

  return result;
}

double lib::jEntropyGroup(vector<vector<double>> data){
  map<vector<double>, int> frequencies;
  if (data.empty()) return 0;
  double size = data[0].size();
  double result = 0;

  for(int i = 0; i < size; i++){
    vector<double> tmp = extractColumn(data, i);
    frequencies[tmp]++;
  }

  for (const auto& kv : frequencies) {
    /*for (auto i = kv.first.begin(); i != kv.first.end(); ++i)
      std::cout << *i << ' ';
    cout << " has value " << kv.second << endl;*/
    double freq = kv.second / size;
    cout << freq << endl;
    result += freq * log2( freq );
  }

  return -result;
}

