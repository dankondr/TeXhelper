#ifndef TEXHELPER_MATRIX_H
#define TEXHELPER_MATRIX_H

#include "fraction.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

namespace Vahe {
class Matrix {
private:
  std::vector<std::vector<Vahe::Fraction>> matrix;
  int rightPartSize = 0;
  int m{}, n{};

  std::vector<std::pair<int, Fraction>> getColumns(int i, int j) {
    std::vector<std::pair<int, Fraction>> columns;
    for (size_t k = i; k < matrix.size(); ++k)
      columns.emplace_back(k, matrix[k][j]);
    sort(begin(columns), end(columns),
         [](std::pair<int, Fraction> &pair1, std::pair<int, Fraction> &pair2) {
           return pair1.second < pair2.second;
         });
    return columns;
  }

  void transform1(int i, int j, Fraction &alpha) {
    for (size_t k = 0; k < n; ++k) {
      Fraction x = matrix[j][k] * alpha;
      matrix[i][k] = matrix[i][k] + x;
    }
  }

  void transform2(int i, int j) {
    for (size_t k = 0; k < n; ++k) {
      std::swap(matrix[i][k], matrix[j][k]);
    }
  }

  void transform3(int i, Fraction &alpha) {
    for (size_t k = 0; k < n; ++k) {
      matrix[i][k] = matrix[i][k] * alpha;
    }
  }

  bool isZero(int i) {
    for (const auto &v : matrix[i])
      if (v.Num)
        return false;
    return true;
  }

  void cutBack() {
    int myGfc = 0;
    for (size_t i = 0; i < matrix.size(); ++i) {
      myGfc = matrix[i][0].Num;
      for (size_t j = 1; j < matrix[i].size(); ++j)
        myGfc = gfc(std::min(myGfc, matrix[i][j].Num),
                    std::max(myGfc, matrix[i][j].Num));
      Fraction alpha{1, myGfc};
      if (myGfc != 0 && myGfc != 1)
        transform3(i, alpha);
    }
  }

  void annihilate(int i, int j) {
    Fraction alpha{-1};
    if (matrix[i][j].Num < 0 || matrix[i][j].Den < 0)
      transform3(i, alpha);
    for (size_t k = i + 1; k < matrix.size(); ++k) {
      Fraction myAlpha = matrix[k][j] / matrix[i][j] * alpha;
      transform1(k, i, myAlpha);
    }
    for (size_t k = 0; k < matrix.size(); ++k)
      if (isZero(k)) {
        size_t t = k + 1;
        while (t < matrix.size() && !isZero(t)) {
          transform2(k, t);
          ++t;
        }
      }
    cutBack();
  }

  void annihilateUp(int i) {
    Fraction negative{-1};
    size_t l = 0;
    Fraction mainElem;
    bool flag = false;
    for (size_t j = 0; j < matrix[i].size(); ++j)
      if (matrix[i][j].Num != 0) {
        mainElem = matrix[i][j];
        l = j;
        flag = true;
        break;
      }
    if (flag)
      for (int k = i - 1; k >= 0; --k) {
        Fraction alpha = matrix[k][l] / mainElem * negative;
        transform1(k, i, alpha);
      }
    cutBack();
  }

  void format(int line, int column) {
    std::vector<std::pair<int, Fraction>> columns = getColumns(line, column);
    bool flag = false;
    for (size_t i = 0; i < columns.size(); ++i) {
      for (size_t j = i + 1; j < columns.size(); ++j) {
        if (columns[i].second.Den == 1 && columns[j].second.Den == 1 &&
            gfc(abs(columns[i].second.Num), abs(columns[j].second.Num)) == 1) {
          flag = true;
          int x, y, a = columns[i].second.Num, b = columns[j].second.Num;
          euclid(a, b, x, y);
          Fraction alpha1{x}, alpha2{y}, alpha3{-1};
          //                printMatrix(matrix);
          //                std::cout << a << " (" << x << ") --- " << b << "("
          //                << y <<
          //                ")\n\n";
          if (x != 0) {
            transform3(columns[i].first, alpha1);
          }
          transform1(columns[i].first, columns[j].first, alpha2);
          transform2(columns[i].first, line);
          annihilate(line, column);
          break;
        }
      }
      if (flag)
        break;
    }
    if (!flag) {
      columns = getColumns(line, column);
      int l = 0;
      Fraction mainElem;
      bool flag2 = false;
      for (const auto &x : columns)
        if (x.second.Num != 0) {
          flag2 = true;
          mainElem = x.second;
          l = x.first;
          break;
        }
      if (flag2) {
        Fraction alpha{mainElem.Den, mainElem.Num};
        transform3(l, alpha);
        transform2(l, line);
        annihilate(line, column);
      }
    }
  }

  std::string getMap() {
    std::string map = "[";
    for (int i = 0; i < n - rightPartSize; ++i) {
      map += 'c';
    }
    map += '|';
    for (int i = 0; i < rightPartSize; ++i) {
      map += 'c';
    }
    map += ']';
    return map;
  }

public:
  void print() const {
    for (const auto &line : matrix) {
      for (const auto &num : line) {
        num.print();
        std::cout << "\t";
      }
      std::cout << "\n";
    }
  }

  std::string tex() {
    std::string latex;
    latex += "\\implies\n\\begin{pmatrix}";
    if (rightPartSize > 0) {
      latex += getMap();
    }
    latex += '\n';
    for (size_t i = 0; i < matrix.size(); ++i) {
      bool flag = false;
      for (size_t j = 0; j < matrix[i].size(); ++j) {
        std::string fr = matrix[i][j].tex();
        latex += fr;
        if (fr.find("\\frac") != std::string::npos)
          flag = true;
        if (j != matrix[i].size() - 1)
          latex += " & ";
      }
      if (i != matrix.size() - 1) {
        latex += " \\\\";
        if (flag)
          latex += "[6pt]";
      }
      latex += "\n";
    }
    latex += "\\end{pmatrix}\n";
    return latex;
  }

  void analyzeMatrix(std::ofstream &file) {
    std::vector<int> main;
    for (size_t j = 0; j < m; ++j) {
      for (size_t i = 0; i < n; ++i) {
        if (!count(begin(main), end(main), i)) {
          this->print();
          std::cout << "\n";
          main.push_back(i);
          this->format(i, j);
          file << this->tex();
          break;
        }
      }
    }
    for (size_t i = main.size() - 1; i > 0; --i) {
      annihilateUp(main[i]);
      file << this->tex();
      this->print();
      std::cout << "\n";
    }
  }

  void setRightPartSize(int val) { rightPartSize = val; }

  explicit Matrix(std::vector<std::vector<Vahe::Fraction>> matrix)
      : matrix(std::move(matrix)) {
    m = this->matrix.size();
    n = this->matrix[0].size();
  }
};
} // namespace Vahe

#endif // TEXHELPER_MATRIX_H
