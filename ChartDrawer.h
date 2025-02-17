#ifndef CHARTDRAWER_H
#define CHARTDRAWER_H

#include <windows.h>

#include <vector>

/// \brief Функция для рисования графика экспериментальных данных и
/// аппроксимации.
///
/// \param hdc Контекст устройства для рисования.
/// \param rcClient Прямоугольник клиентской области, где рисуем график.
/// \param Ca Вектор экспериментальных значений концентрации A.
/// \param Tm Вектор моментов времени.
/// \param Cb Начальная концентрация B.
/// \param Cc Начальная концентрация C.
/// \param k Расчитанная константа скорости.
/// \param n Расчитанный порядок реакции.
void DrawChart(HDC hdc, const RECT& rcClient, const std::vector<double>& Ca,
               const std::vector<double>& Tm, double Cb, double Cc, double k,
               double n);

#endif  // CHARTDRAWER_H
