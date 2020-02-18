#include "main/fraction.h"
#include "main/matrix.h"
#include "main/utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace Vahe;

string outputFile = "LaTexInfo.txt";

void readFrac(Fraction &frac, const string &num) {
  if (num.find('/') == string::npos) {
    frac.Num = stoi(num);
  } else {
    string str;
    for (char l : num)
      if (l != '/') {
        str += l;
      } else {
        frac.Num = stoi(str);
        str = "";
      }
    frac.Den = stoi(str);
  }
}

vector<Fraction> split(const string &line) {
  string num;
  vector<Fraction> nums;
  Fraction frac;
  for (char x : line)
    if (isspace(x)) {
      readFrac(frac, num);
      nums.push_back(frac);
      num = "";
      frac.Den = 1;
    } else {
      num += x;
    }
  readFrac(frac, num);
  nums.push_back(frac);
  return nums;
}

Matrix readMatrix(const string &filename) {
  ifstream myFile;
  myFile.open(filename);
  string line;
  vector<vector<Fraction>> matrix;
  while (getline(myFile, line))
    matrix.push_back(split(line));
  return Matrix(matrix);
}

int main() {
  string filename = "matrix.txt";
  ofstream outFile(outputFile);
  auto matrix = readMatrix(filename);
//  matrix.setRightPartSize(1);
  outFile << matrix.tex();
  matrix.analyzeMatrix(outFile);
  matrix.print();
  return 0;
}
