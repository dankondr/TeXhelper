//
// Created by Даниил Кондратьев on 2019-11-12.
//

#ifndef TEXHELPER_TEX_H
#define TEXHELPER_TEX_H

#include "exception.h"

template<typename t>
class TexConverter {
public:
    static std::string representMatrixDet(std::vector<std::vector<t>> mat) {
        int m = mat.size();
        if (m == 0) throw Exception("Matrix can't be empty.");
        int n = mat[0].size();
        if (n == 0) throw Exception("Matrix can't be empty.");

        std::string result = "";
        if (m > 1 || n > 1) result += "\\begin{vmatrix}\n";
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                result += mat[i][j];
                if (j != n - 1)
                    result += '&';
                else
                    result += ' ';
            }
            if (m > 1 || n > 1) result += "\\\\\n";
        }
        if (m > 1 || n > 1) result += "\\end{vmatrix}\n";
        return result;
    }

    static std::string representMatrix(std::vector<std::vector<t>> mat) {
        int m = mat.size();
        if (m == 0) throw Exception("Matrix can't be empty.");
        int n = mat[0].size();
        if (n == 0) throw Exception("Matrix can't be empty.");

        std::string result = "";
        if (m > 1 || n > 1) result += "\\begin{pmatrix}\n";
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                result += mat[i][j];
                if (j != n - 1)
                    result += '&';
                else
                    result += ' ';
            }
            if (m > 1 || n > 1) result += "\\\\\n";
        }
        if (m > 1 || n > 1) result += "\\end{pmatrix}\n";
        return result;
    }

};

#endif //TEXHELPER_TEX_H
