#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <windows.h>

/**
 * \brief Класс окна для отображения результатов расчёта.
 *
 * Окно выводит рассчитанные параметры: порядок реакции, константу скорости,
 * дисперсию и коэффициент корреляции.
 */
class ResultWindow {
 public:
  /**
   * \brief Создаёт и отображает окно с результатами расчёта.
   * \param hwndParent Дескриптор родительского окна.
   * \param n Порядок реакции.
   * \param k Константа скорости.
   * \param disp Дисперсия.
   * \param r Коэффициент корреляции.
   */
  static void Show(HWND hwndParent, double n, double k, double disp, double r);

  /// Статическая оконная процедура для окна результатов.
  static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam,
                                        LPARAM lParam);
};

#endif  // RESULTWINDOW_H
