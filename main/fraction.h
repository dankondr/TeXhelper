#ifndef TEXHELPER_FRACTION_H
#define TEXHELPER_FRACTION_H

#include "utils.h"
#include <iostream>
#include <string>

namespace Vahe {
class Fraction {
public:
  int Num = 0, Den = 1;

  static void reduce(Fraction &num) {
    int multiply = gfc(num.Num, num.Den);
    num.Num /= multiply;
    num.Den /= multiply;
    if (num.Den < 0) {
      num.Den *= -1;
      num.Num *= 1;
    }
  }

  Fraction operator+(Fraction &number) const {
    Fraction newNumber;
    newNumber.Den = Den * number.Den / gfc(Den, number.Den);
    newNumber.Num =
        newNumber.Den / Den * Num + newNumber.Den / number.Den * number.Num;
    reduce(newNumber);
    return newNumber;
  }

  Fraction operator-(Fraction &number) const {
    Fraction newNumber;
    newNumber.Den = Den * number.Den / gfc(Den, number.Den);
    newNumber.Num =
        newNumber.Den / Den * Num - newNumber.Den / number.Den * number.Num;
    reduce(newNumber);
    return newNumber;
  }

  Fraction operator*(Fraction &number) const {
    Fraction newNumber;
    newNumber.Num = Num * number.Num;
    newNumber.Den = Den * number.Den;
    reduce(newNumber);
    return newNumber;
  }

  Fraction operator/(Fraction &number) const {
    Fraction newNumber;
    newNumber.Num = Num * number.Den;
    newNumber.Den = Den * number.Num;
    reduce(newNumber);
    return newNumber;
  }

  bool operator<(Fraction &number) const {
    int gdc = Den * number.Den / gfc(Den, number.Den);
    return abs(Den / gdc * Num) < abs(number.Den / gdc * number.Num);
  }

  void print() const {
    if (Den == 1)
      std::cout << Num;
    else
      std::cout << Num << "/" << Den;
  }

  std::string tex() {
    std::string k;
    if (Den == 1)
      k = std::to_string(Num);
    else if (Den == -1)
      k = std::to_string(Num * -1);
    else
      k = "\\frac{" + std::to_string(Num) + "}{" + std::to_string(Den) + "}";
    return k;
  }
};
} // namespace Vahe

#endif // TEXHELPER_FRACTION_H
