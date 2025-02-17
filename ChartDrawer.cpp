#include "ChartDrawer.h"

#include <cmath>
#include <vector>

#include "Constants.h"
#include "Utils.h"

/// Функция для рисования графика, аналогичная функции DrawChart из оригинального кода.
void DrawChart(HDC hdc, const RECT& rcClient, const std::vector<double>& Ca,
               const std::vector<double>& Tm, double Cb, double Cc, double k,
               double n) {
  const int nPoints = static_cast<int>(Ca.size());
  int left = rcClient.left;
  int top = rcClient.top;
  int width = rcClient.right - rcClient.left;
  int height = rcClient.bottom - rcClient.top;

  // Рисуем оси
  HPEN hPenAxis = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
  HPEN hPenOld = (HPEN)SelectObject(hdc, hPenAxis);

  // Ось X
  MoveToEx(hdc, left + 40, top + height - 40, nullptr);
  LineTo(hdc, left + width - 10, top + height - 40);

  // Ось Y
  MoveToEx(hdc, left + 40, top + height - 40, nullptr);
  LineTo(hdc, left + 40, top + 10);

  // Подписи осей
  TextOut(hdc, left + width - 60, top + height - 50, L"Time", 4);
  TextOut(hdc, left + 50, top + 10, L"Concentration", 13);

  // Определение границ данных
  double Tmin = 1e9, Tmax = -1e9, Cmin = 1e9, Cmax = -1e9;
  for (int i = 0; i < nPoints; i++) {
    if (Tm[i] < Tmin) Tmin = Tm[i];
    if (Tm[i] > Tmax) Tmax = Tm[i];
    if (Ca[i] < Cmin) Cmin = Ca[i];
    if (Ca[i] > Cmax) Cmax = Ca[i];
  }
  // Учитываем начальные концентрации B и C
  if (Cb < Cmin) Cmin = Cb;
  if (Cb > Cmax) Cmax = Cb;
  if (Cc < Cmin) Cmin = Cc;
  if (Cc > Cmax) Cmax = Cc;

  if (Tmin > 1e8) {
    Tmin = 0;
    Tmax = 1;
  }
  if (fabs(Tmax - Tmin) < 1e-6) {
    Tmax = Tmin + 1;
  }
  if (fabs(Cmax - Cmin) < 1e-6) {
    Cmax = Cmin + 1;
  }
  if (Cmin < 0) Cmin = 0;
  if (Tmin < 0) Tmin = 0;

  double offsetX = 40.0, offsetY = 40.0;
  double plotW = static_cast<double>(width - 50);
  double plotH = static_cast<double>(height - 50);

  // Функции для преобразования координат из модели в экранные пиксели
  auto fx = [=](double t) -> int {
    return static_cast<int>(left + offsetX +
                            (t - Tmin) / (Tmax - Tmin) * (plotW - 20));
  };
  auto fy = [=](double c) -> int {
    return static_cast<int>(top + plotH -
                            (c - Cmin) / (Cmax - Cmin) * (plotH - offsetY));
  };

  // Рисуем деления на осях
  int nTicks = 5;
  for (int i = 0; i <= nTicks; i++) {
    double tx = Tmin + (Tmax - Tmin) * i / nTicks;
    int X = fx(tx);
    MoveToEx(hdc, X, top + height - 40, nullptr);
    LineTo(hdc, X, top + height - 35);

    wchar_t buf[32];
    swprintf_s(buf, L"%.1f", tx);
    TextOut(hdc, X - 10, top + height - 30, buf, lstrlen(buf));
  }
  for (int i = 0; i <= nTicks; i++) {
    double cc = Cmin + (Cmax - Cmin) * i / nTicks;
    int Y = fy(cc);
    MoveToEx(hdc, left + 40, Y, nullptr);
    LineTo(hdc, left + 45, Y);

    wchar_t buf[32];
    swprintf_s(buf, L"%.1f", cc);
    TextOut(hdc, left + 5, Y - 8, buf, lstrlen(buf));
  }

  // Рисуем экспериментальные точки (красный крест)
  HPEN hPenExp = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
  SelectObject(hdc, hPenExp);
  for (int i = 0; i < nPoints; i++) {
    int X = fx(Tm[i]);
    int Y = fy(Ca[i]);
    MoveToEx(hdc, X - 3, Y, nullptr);
    LineTo(hdc, X + 3, Y);
    MoveToEx(hdc, X, Y - 3, nullptr);
    LineTo(hdc, X, Y + 3);
  }

  // Построение кривых A(t), B(t), C(t) через интегрирование
  if (nPoints >= 2) {
    std::vector<POINT> arrA;
    std::vector<POINT> arrB;
    std::vector<POINT> arrC;

    double Acur = Ca[0];
    double Bcur = Cb;
    double Ccur = Cc;
    double tcur = Tm[0];

    arrA.push_back({fx(tcur), fy(Acur)});
    arrB.push_back({fx(tcur), fy(Bcur)});
    arrC.push_back({fx(tcur), fy(Ccur)});

    for (int i = 1; i < nPoints; i++) {
      double dtFull = Tm[i] - Tm[i - 1];
      if (dtFull < 0) dtFull = 0;
      int subSteps = 20;
      double dtSub = dtFull / subSteps;

      for (int s = 0; s < subSteps; s++) {
        double rate = k * pow(Acur, n);
        Acur -= rate * dtSub;
        Bcur += (7.0 / 3.0) * rate * dtSub;
        Ccur += rate * dtSub;
        tcur += dtSub;

        arrA.push_back({fx(tcur), fy(Acur)});
        arrB.push_back({fx(tcur), fy(Bcur)});
        arrC.push_back({fx(tcur), fy(Ccur)});
      }
    }

    // Рисуем кривые
    HPEN hPenA = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
    SelectObject(hdc, hPenA);
    Polyline(hdc, arrA.data(), static_cast<int>(arrA.size()));
    DeleteObject(hPenA);

    HPEN hPenB = CreatePen(PS_SOLID, 1, RGB(0, 128, 0));
    SelectObject(hdc, hPenB);
    Polyline(hdc, arrB.data(), static_cast<int>(arrB.size()));
    DeleteObject(hPenB);

    HPEN hPenC = CreatePen(PS_SOLID, 1, RGB(255, 128, 0));
    SelectObject(hdc, hPenC);
    Polyline(hdc, arrC.data(), static_cast<int>(arrC.size()));
    DeleteObject(hPenC);
  }

  // Рисуем легенду
  SelectObject(hdc, hPenAxis);
  int legendX = left + width - 140;
  int legendY = top + 10;
  RECT rcLeg = {legendX, legendY, legendX + 130, legendY + 80};
  HBRUSH hBrWhite = CreateSolidBrush(RGB(255, 255, 255));
  FillRect(hdc, &rcLeg, hBrWhite);
  DeleteObject(hBrWhite);
  FrameRect(hdc, &rcLeg, (HBRUSH)GetStockObject(BLACK_BRUSH));
  SetBkMode(hdc, TRANSPARENT);

  // Линия для A(t)
  HPEN hPenA = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
  SelectObject(hdc, hPenA);
  MoveToEx(hdc, legendX + 10, legendY + 15, nullptr);
  LineTo(hdc, legendX + 30, legendY + 15);
  SelectObject(hdc, hPenAxis);
  DeleteObject(hPenA);
  TextOut(hdc, legendX + 35, legendY + 8, L"A(t)", 4);

  // Линия для B(t)
  HPEN hPenB = CreatePen(PS_SOLID, 1, RGB(0, 128, 0));
  SelectObject(hdc, hPenB);
  MoveToEx(hdc, legendX + 10, legendY + 30, nullptr);
  LineTo(hdc, legendX + 30, legendY + 30);
  SelectObject(hdc, hPenAxis);
  DeleteObject(hPenB);
  TextOut(hdc, legendX + 35, legendY + 23, L"B(t)", 4);

  // Линия для C(t)
  HPEN hPenC = CreatePen(PS_SOLID, 1, RGB(255, 128, 0));
  SelectObject(hdc, hPenC);
  MoveToEx(hdc, legendX + 10, legendY + 45, nullptr);
  LineTo(hdc, legendX + 30, legendY + 45);
  SelectObject(hdc, hPenAxis);
  DeleteObject(hPenC);
  TextOut(hdc, legendX + 35, legendY + 38, L"C(t)", 4);

  // Экспериментальные точки (красный крест)
  HPEN hPenExp2 = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
  SelectObject(hdc, hPenExp2);
  MoveToEx(hdc, legendX + 7, legendY + 60, nullptr);
  LineTo(hdc, legendX + 13, legendY + 60);
  MoveToEx(hdc, legendX + 10, legendY + 57, nullptr);
  LineTo(hdc, legendX + 10, legendY + 63);
  SelectObject(hdc, hPenAxis);
  DeleteObject(hPenExp2);

  // Освобождение ресурсов
  SelectObject(hdc, hPenOld);
  DeleteObject(hPenAxis);
  DeleteObject(hPenExp);
}
