#include <iostream>
#include "main/solver.h"

int main() {
    std::cout << "Размер матрицы, до какого разложить (1 цифра): ";
    uint32_t m, ms;
    std::cin >> ms;
    std::cout << "Размер матрицы, которую вы будете раскладывать (1 цифра): ";
    std::cin >> m;
    std::vector<std::vector<std::string>> a(m, std::vector<std::string>(m));
    for (uint32_t i = 0; i < m; i++) {
        for (uint32_t j = 0; j < m; j++) {
            std::cin >> a[i][j];
        }
    }
    auto mat = Matrix<std::string>(m, m, a);
    std::cout << mat.repr() << '\n' << mat.repr_decomposition() << '\n';
    std::cout << mat.decomposition_tex(ms);
    return 0;
}