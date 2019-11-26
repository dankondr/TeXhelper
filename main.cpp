#include <iostream>
#include "main/solver.h"
#include <fstream>

int main() {
    int m;
    std::cout << "Размер матрицы, которую вы будете раскладывать (1 цифра): ";
    std::cin >> m;
    std::vector<std::vector<std::string>> a(m, std::vector<std::string>(m));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            std::cin >> a[i][j];
        }
    }
    auto mat = Matrix<std::string>(m, m, a);
    std::ofstream f;
    f.open("output.txt");
    for (int i = m - 1; i >= 1; i--) {
        f << mat.reprDecompositionTex(i) << "=\n";
    }
    f.close();
    return 0;
}