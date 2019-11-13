//
// Created by Даниил Кондратьев on 2019-11-12.
//

#ifndef TEXHELPER_SOLVER_H
#define TEXHELPER_SOLVER_H

#include <vector>
#include <string>
#include <iostream>

template<typename t>
class Matrix {
private:
    std::vector<std::vector<t>> mat;
    uint32_t n{};
    uint32_t m{};
    std::vector<std::pair<t, Matrix>> decomposition;

public:
    Matrix(uint32_t m, uint32_t n, std::vector<std::vector<t>> mat, bool decompose = true) {
        this->m = m;
        this->n = n;
        this->mat = mat;

        if (decompose)
            decomposition = this->decompose();
    }

    Matrix(uint32_t m, uint32_t n) {
        this->m = m;
        this->n = n;
        std::vector<std::vector<t>> temp_mat(m, std::vector<t>(n, 0));
        this->mat = temp_mat;
    }

    uint32_t get_m() { return m; }

    uint32_t get_n() { return n; }

    std::string repr() {
        std::string result = "";
        for (uint32_t i = 0; i < m; i++) {
            for (uint32_t j = 0; j < n; j++)
                result += mat[i][j] + ' ';
            result += '\n';
        }
        return result;
    }

    std::string repr_tex_det() {
        std::string result = "\\begin{vmatrix}\n";
        for (uint32_t i = 0; i < m; i++) {
            for (uint32_t j = 0; j < n; j++) {
                result += mat[i][j];
                if (j != n - 1)
                    result += '&';
                else
                    result += ' ';
            }
            result += "\\\\\n";
        }
        result += "\\end{vmatrix}\n";
        return result;
    }

    std::string repr_decomposition() {
        std::string result = "";
        for (auto &el : decomposition) {
            result += el.first + '\n';
            auto tm = el.second;
            result += tm.repr() + '\n';
        }
        return result;
    }

    std::string decomposition_tex(uint32_t min_size) {
        std::string result = "";
        bool first = true;
        for (auto pair : decomposition) {
            if (pair.first[0] != '-' && !first)
                result += '+';
            result += pair.first + "\\cdot" + '\n';
            if (pair.second.get_m() > min_size) {
                result += '(';
                result += pair.second.decomposition_tex(min_size);
                result += ')';
            } else {
                result += pair.second.repr_tex_det();
            }
            if (first) first = false;
        }
        return result;
    }

private:
    std::vector<std::pair<t, Matrix>> decompose() {
        // Finding string with maximum zeros
        uint32_t i;
        uint32_t max_ind = 0;
        uint32_t max_zeros = 0;
        uint32_t zero_counter = 0;
        for (uint32_t str = 0; str < m; str++) {
            for (t elem : mat[str]) {
                if (elem == "0") {
                    zero_counter++;
                }
            }
            if (zero_counter > max_zeros) {
                max_ind = str;
                max_zeros = zero_counter;
            }
        }
        i = max_ind;

        std::vector<std::pair<t, Matrix>> result;
        for (uint32_t el = 0; el < n; el++) {
            std::vector<std::vector<t>> temp_mat(m - 1);
            uint8_t was_i = 0;
            for (uint32_t str = 0; str < m; str++) {
                for (uint32_t col = 0; col < n; col++) {
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
            else if((i + el) % 2 != 0)
                coef = '-' + mat[i][el];
            std::pair<t, Matrix> temp_pair(coef, matrix_to_insert);
            result.push_back(temp_pair);
        }
        return result;
    }
};

#endif //TEXHELPER_SOLVER_H
