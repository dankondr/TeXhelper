#ifndef TEXHELPER_SOLVER_H
#define TEXHELPER_SOLVER_H

#include <vector>
#include <string>
#include <iostream>
#include "tex.h"

template<typename t>
class Matrix {
private:
    std::vector<std::vector<t>> mat;
    int n{};
    int m{};
    std::vector<std::pair<t, Matrix>> decomposition;

public:
    Matrix(int m, int n, std::vector<std::vector<t>> mat, bool decompose = true) : m(m), n(n), mat(mat) {
        if (decompose) decomposition = this->decompose();
    }

    Matrix(int m, int n) : m(m), n(n) {
        std::vector<std::vector<t>> temp_mat(m, std::vector<t>(n));
        this->mat = temp_mat;
    }

    int getM() { return m; }

    int getN() { return n; }

    std::string repr() {
        std::string result;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++)
                result += mat[i][j] + ' ';
            result += '\n';
        }
        return result;
    }

    std::string reprTexDet() {
        return TexConverter<t>::representMatrixDet(mat);
    }

    std::string reprDecomposition() {
        std::string result;
        for (auto &el : decomposition) {
            result += el.first + '\n';
            auto tm = el.second;
            result += tm.repr() + '\n';
        }
        return result;
    }

    std::string reprDecompositionTex(int min_size) {
        std::string result;
        bool first = true;
        for (auto pair : decomposition) {
            if (pair.first[0] == '0' || pair.first[1] == '0') continue;
            if (pair.first[0] != '-' && !first)
                result += '+';
            result += pair.first + "\\cdot" + '\n';
            if (pair.second.getM() > min_size) {
                result += "\\left(";
                result += pair.second.reprDecompositionTex(min_size);
                result += "\\right)";
            } else {
                result += pair.second.reprTexDet();
            }
            if (first) first = false;
        }
        return result;
    }

    const std::vector<std::pair<t, Matrix>> &getDecomposition() const {
        return decomposition;
    }

private:
    std::vector<std::pair<t, Matrix>> decompose() {
        // Finding string with maximum zeros
        int i;
        int max_ind = 0;
        int max_zeros = 0;
        int zero_counter = 0;
        for (int str = 0; str < m; str++) {
            for (t elem : mat[str]) {
                if (elem[0] == '0') {
                    zero_counter++;
                }
            }
            if (zero_counter > max_zeros) {
                max_ind = str;
                max_zeros = zero_counter;
            }
            zero_counter = 0;
        }
        i = max_ind;

        std::vector<std::pair<t, Matrix>> result;
        for (int el = 0; el < n; el++) {
            std::vector<std::vector<t>> temp_mat(m - 1);
            uint8_t was_i = 0;
            for (int str = 0; str < m; str++) {
                for (int col = 0; col < n; col++) {
                    if (str == i) {
                        was_i = 1;
                        break;
                    }
                    if (col == el)
                        continue;
                    temp_mat[str - was_i].push_back(mat[str][col]);
                }
            }
            Matrix matrix_to_insert = Matrix(m - 1, n - 1, temp_mat);
            t coef = mat[i][el];
            if (mat[i][el][0] == '-' and (i + el) % 2 != 0)
                coef = '+' + mat[i][el].substr(1);
            else if ((i + el) % 2 != 0)
                coef = '-' + mat[i][el];
            std::pair<t, Matrix> temp_pair(coef, matrix_to_insert);
            result.push_back(temp_pair);
        }
        return result;
    }
};

#endif //TEXHELPER_SOLVER_H
