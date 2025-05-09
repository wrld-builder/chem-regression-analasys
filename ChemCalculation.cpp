/**
 * \file ChemCalculation.cpp
 * \brief Реализация класса ChemCalculation для расчёта параметров химической реакции.
 *
 * Данный файл содержит реализацию метода \c Calculate, который выполняет расчет параметров химической реакции,
 * используя экспериментальные данные (значения Ca и Tm) и начальные концентрации Cb и Cc.
 *
 * \author Шунин Михаил Дмитриевич
 * \date Февраль 2025
 */

#include "ChemCalculation.h"

#include <cmath>
#include <sstream>
#include <stdexcept>

#include "Utils.h"

/**
 * \brief Выполняет расчёт параметров химической реакции.
 *
 * Функция принимает экспериментальные данные, представленные в виде двух векторов:
 * - \a Ca содержит значения концентрации A,
 * - \a Tm содержит соответствующие значения времени.
 *
 * Дополнительно задаются начальные концентрации:
 * - \a Cb для вещества B,
 * - \a Cc для вещества C.
 *
 * Перед началом расчёта функция проверяет корректность входных данных:
 * - Начальные концентрации \a Cb и \a Cc не могут быть отрицательными.
 * - Все значения вектора \a Ca должны быть неотрицательными.
 * - Значения вектора \a Tm должны строго возрастать (t[i+1] > t[i]).
 * - Для расчёта требуется не менее двух точек.
 *
 * Затем выполняется регрессионный анализ методом наименьших квадратов:
 * - Вычисляются суммарные коэффициенты (s2, s3, s4, s5, s6) на основе логарифмов скорости реакции и концентрации.
 * - Из них рассчитывается порядок реакции \a n и константа скорости \a k.
 * - Коэффициент корреляции \a r также определяется на основе полученных сумм.
 *
 * Далее функция вычисляет дисперсию (\a disp) методом интегрирования с использованием подшагов:
 * - Интегрирование производится по интервалам между экспериментальными точками.
 * - Для каждого интервала вычисляется разница между экспериментальным значением и смоделированным,
 *   затем суммируются квадраты этих разниц.
 *
 * \param Ca Вектор экспериментальных значений концентрации A.
 * \param Tm Вектор экспериментальных значений времени.
 * \param Cb Начальная концентрация вещества B.
 * \param Cc Начальная концентрация вещества C.
 *
 * \return Структура \c CalculationResult, содержащая рассчитанные параметры:
 * - \a n: Порядок реакции.
 * - \a k: Константа скорости.
 * - \a r: Коэффициент корреляции.
 * - \a disp: Дисперсия.
 *
 * \throw std::runtime_error Если входные данные некорректны:
 * - Если \a Cb или \a Cc отрицательны.
 * - Если какое-либо значение в \a Ca отрицательно.
 * - Если значения в \a Tm не возрастают.
 * - Если количество точек меньше двух.
 * - Если возникает деление на ноль при вычислении коэффициентов регрессионного анализа.
 */
CalculationResult ChemCalculation::Calculate(const std::vector<double>& Ca,
                                             const std::vector<double>& Tm,
                                             double Cb, double Cc) {
  CalculationResult result = {0.0, 0.0, 0.0, 0.0};
  const int nPoints = static_cast<int>(Ca.size());

  // 1. Проверка входных данных
  if (Cb < 0.0 || Cc < 0.0) {
    throw std::runtime_error(
        "Начальные концентрации Cb и Cc не могут быть отрицательными!");
  }
  for (size_t i = 0; i < Ca.size(); i++) {
    if (Ca[i] < 0.0) {
      throw std::runtime_error("Значение Ca не может быть отрицательным!");
    }
  }
  if (nPoints < 2) {
    throw std::runtime_error("Недостаточно точек для расчёта!");
  }
  for (int i = 0; i < nPoints - 1; i++) {
    if (Tm[i + 1] <= Tm[i]) {
      throw std::runtime_error(
          "Время должно строго возрастать (t[i+1] > t[i])!");
    }
  }

  // 2. Набор сумм для регрессионного анализа
  double s1 = static_cast<double>(nPoints - 1);
  double s2 = 0.0, s3 = 0.0, s4 = 0.0, s5 = 0.0, s6 = 0.0;

  for (int i = 0; i < nPoints - 1; i++) {
    double dC = Ca[i + 1] - Ca[i];
    double dt = Tm[i + 1] - Tm[i];
    // Защита от слишком маленького dt
    if (dt <= 1e-15) dt = 1e-15;

    // Вычисляем скорость
    double w = std::fabs(dC / dt);
    // Защита от w=0 => log(0) = -inf
    if (w < 1e-15) {
      w = 1e-15;
    }

    // Логарифмируем концентрацию (защита от нуля)
    double cVal = (Ca[i] < 1e-15) ? 1e-15 : Ca[i];
    double x = std::log(cVal);
    double y = std::log(w);

    s2 += x;
    s3 += y;
    s4 += x * x;
    s5 += x * y;
    s6 += y * y;
  }

  // 3. Вычисление порядка реакции (n) и ln(k) по формуле линейной регрессии
  double denom = (s1 * s4 - s2 * s2);
  if (std::fabs(denom) < 1e-15) {
    throw std::runtime_error(
        "Невозможно вычислить параметры (деление на ноль). Проверьте данные!");
  }

  result.n = (s1 * s5 - s2 * s3) / denom;    // slope
  double t_k = (s3 * s4 - s2 * s5) / denom;  // intercept
  result.k = std::exp(t_k);  // константа скорости

  // 4. Расчёт коэффициента корреляции r
  double denom_r_val = (s1 * s4 - s2 * s2) * (s1 * s6 - s3 * s3);
  if (denom_r_val < 1e-15) {
    // Данные вырождены (скорее всего все y одинаковы) -> r не определён
    result.r = 0.0;
  } else {
    double denom_r = std::sqrt(denom_r_val);
    result.r = (s1 * s5 - s2 * s3) / denom_r;
    // Зажать в диапазон [-1..1] на случай неточностей
    if (result.r > 1.0) result.r = 1.0;
    if (result.r < -1.0) result.r = -1.0;
  }

  // 5. Вычисление дисперсии методом пошагового интегрирования
  double sumSq = 0.0;
  double Acur = Ca[0];
  double tcur = Tm[0];

  for (int i = 1; i < nPoints; i++) {
    double dtFull = Tm[i] - tcur;
    if (dtFull < 1e-15) {
      dtFull = 1e-15;  // защита
    }
    const int subSteps = 70;  // Количество подшагов
    double dtSub = dtFull / subSteps;
    double Atemp = Acur;

    for (int s = 0; s < subSteps; s++) {
      // rate = k * A^n
      // Если Atemp < 0, то pow(Atemp, n) даст NaN при n < 1
      if (Atemp < 1e-15) {
        Atemp = 0.0;  // «зажимаем» отрицательные/слишком малые значения
      }
      double rate = result.k * std::pow(Atemp, result.n);
      Atemp -= rate * dtSub;
      if (Atemp < 0.0) {
        Atemp = 0.0;  // не даём уйти в минус
      }
    }

    double diff = Ca[i] - Atemp;
    // Округляем, чтобы уменьшить накопление ошибок double
    sumSq += std::round(diff * diff * 1e4) / 1e4;

    Acur = Atemp;
    tcur = Tm[i];
  }
  // Делим на (nPoints - 1) если nPoints > 1 (мы проверили выше)
  result.disp = sumSq / (nPoints - 1);

  // 6. Дополнительная проверка на NaN/inf
  // В C++17 можно использовать std::isfinite,
  // в MSVC до C++17 можно _finite(...) и т.п.
  if (!std::isfinite(result.n) || !std::isfinite(result.k) ||
      !std::isfinite(result.r) || !std::isfinite(result.disp)) {
    throw std::runtime_error(
        "Вычисленные параметры содержат NaN/inf! Проверьте данные.");
  }

  return result;
}
