/**
 * \file ResultWindow.cpp
 * \brief Реализация окна для отображения результатов расчёта.
 *
 * Данный файл содержит реализацию класса \c ResultWindow, который теперь
 * выводит:
 * - Порядок реакции (n)
 * - Константу скорости (k)
 * - Дисперсию
 * - Коэффициент корреляции (r)
 * - Предэкспоненциальный коэффициент (A)
 * - Энергию активации (Ea, Дж/моль)
 *
 * \author Шунин Михаил Дмитриевич
 * \date Февраль 2025
 */

#include "ResultWindow.h"

#include <windowsx.h>

#include <cwchar>

#include "Constants.h"

// Глобальный дескриптор окна результатов.
static HWND g_hResWnd = nullptr;

/**
 * \brief Статическая оконная процедура для окна результатов.
 *
 * Обрабатывает создание элементов управления, кнопку OK, а также сообщения
 * закрытия окна.
 */
LRESULT CALLBACK ResultWindow::StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam,
                                             LPARAM lParam) {
  // Статические дескрипторы полей, в которых будут отображаться результаты.
  static HWND hEdN = nullptr;
  static HWND hEdK = nullptr;
  static HWND hEdD = nullptr;
  static HWND hEdR = nullptr;
  static HWND hEdA = nullptr;
  static HWND hEdEa = nullptr;

  switch (msg) {
    case WM_CREATE: {
      g_hResWnd = hwnd;

      // === Порядок реакции (n) ===
      CreateWindowEx(0, L"STATIC", L"Порядок реакции (n):",
                     WS_CHILD | WS_VISIBLE, 20, 20, 200, 20, hwnd,
                     (HMENU)IDC_STATIC_RESULT1, nullptr, nullptr);
      hEdN = CreateWindowEx(
          WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY,
          230, 20, 140, 20, hwnd, (HMENU)IDC_EDIT_RESULT_N, nullptr, nullptr);

      // === Константа скорости (k) ===
      CreateWindowEx(0, L"STATIC", L"Константа скорости (k):",
                     WS_CHILD | WS_VISIBLE, 20, 60, 200, 20, hwnd,
                     (HMENU)IDC_STATIC_RESULT2, nullptr, nullptr);
      hEdK = CreateWindowEx(
          WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY,
          230, 60, 140, 20, hwnd, (HMENU)IDC_EDIT_RESULT_K, nullptr, nullptr);

      // === Дисперсия ===
      CreateWindowEx(0, L"STATIC", L"Дисперсия:", WS_CHILD | WS_VISIBLE, 20,
                     100, 200, 20, hwnd, (HMENU)IDC_STATIC_RESULT3, nullptr,
                     nullptr);
      hEdD = CreateWindowEx(
          WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY,
          230, 100, 140, 20, hwnd, (HMENU)IDC_EDIT_RESULT_D, nullptr, nullptr);

      // === Корреляция (r) ===
      CreateWindowEx(0, L"STATIC", L"Корреляция (r):", WS_CHILD | WS_VISIBLE,
                     20, 140, 200, 20, hwnd, (HMENU)IDC_STATIC_RESULT4,
                     nullptr, nullptr);
      hEdR = CreateWindowEx(
          WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY,
          230, 140, 140, 20, hwnd, (HMENU)IDC_EDIT_RESULT_R, nullptr, nullptr);

      // === Предэкспоненциальный коэффициент (A) ===
      CreateWindowEx(0, L"STATIC", L"Предэксп. коэф. (A):",
                     WS_CHILD | WS_VISIBLE, 20, 180, 200, 20, hwnd,
                     (HMENU)IDC_STATIC_RESULT_A, nullptr, nullptr);
      hEdA = CreateWindowEx(
          WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY,
          230, 180, 140, 20, hwnd, (HMENU)IDC_EDIT_RESULT_A, nullptr, nullptr);

      // === Энергия активации (Ea) ===
      CreateWindowEx(0, L"STATIC", L"Энергия активации (Ea):",
                     WS_CHILD | WS_VISIBLE, 20, 220, 200, 20, hwnd,
                     (HMENU)IDC_STATIC_RESULT_EA, nullptr, nullptr);
      hEdEa = CreateWindowEx(
          WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY,
          230, 220, 140, 20, hwnd, (HMENU)IDC_EDIT_RESULT_EA, nullptr, nullptr);

      // === Кнопка OK ===
      CreateWindowEx(0, L"BUTTON", L"OK",
                     WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 160, 260, 60, 25,
                     hwnd, (HMENU)IDC_BUTTON_OK, nullptr, nullptr);

      return 0;
    }

    case WM_COMMAND: {
      const int wmId = LOWORD(wParam);
      if (wmId == IDC_BUTTON_OK) {
        DestroyWindow(hwnd);
      }
      return 0;
    }

    case WM_CLOSE:
      DestroyWindow(hwnd);
      return 0;

    case WM_DESTROY:
      if (g_hResWnd == hwnd) {
        g_hResWnd = nullptr;
      }
      return 0;

    default:
      return DefWindowProc(hwnd, msg, wParam, lParam);
  }
}

/**
 * \brief Отображает окно результатов расчёта с заданными параметрами.
 *
 * \param hParent Дескриптор родительского окна.
 * \param n Порядок реакции.
 * \param k Константа скорости.
 * \param disp Дисперсия.
 * \param r Коэффициент корреляции.
 * \param A Предэкспоненциальный коэффициент.
 * \param Ea Энергия активации (Дж/моль).
 */
void ResultWindow::Show(HWND hParent, double n, double k, double disp, double r,
                        double A, double Ea) {
  // Зарегистрируем класс окна, если нужно (примерно):
  // (Можно сделать это в другом месте, главное, чтобы класс был зарегистрирован
  // один раз)
  WNDCLASS wc = {};
  wc.lpfnWndProc = ResultWindow::StaticWndProc;
  wc.hInstance = GetModuleHandle(nullptr);
  wc.lpszClassName = L"ResultWindowClass";
  wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
  RegisterClass(&wc);

  // Создаём окно результатов со стилями, запрещающими изменение размера
  HWND hResultWnd = CreateWindowEx(
      0, L"ResultWindowClass", L"Результаты расчёта",
      (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX) &
          ~(WS_MAXIMIZEBOX | WS_THICKFRAME),
      CW_USEDEFAULT, CW_USEDEFAULT, 420, 340, hParent, nullptr, wc.hInstance,
      nullptr);

  if (!hResultWnd) return;

  // Отображаем окно
  ShowWindow(hResultWnd, SW_SHOW);
  UpdateWindow(hResultWnd);

  // Заполняем поля значениями с разумным форматом
  wchar_t buf[64];

  // n (обычно 1-2, 3 знака после запятой)
  swprintf_s(buf, L"%.3f", n);
  SetWindowText(GetDlgItem(hResultWnd, IDC_EDIT_RESULT_N), buf);

  // k, disp, A (очень разные масштабы) — используем %.4g
  swprintf_s(buf, L"%.4g", k);
  SetWindowText(GetDlgItem(hResultWnd, IDC_EDIT_RESULT_K), buf);

  swprintf_s(buf, L"%.4g", disp);
  SetWindowText(GetDlgItem(hResultWnd, IDC_EDIT_RESULT_D), buf);

  swprintf_s(buf, L"%.4g", A);
  SetWindowText(GetDlgItem(hResultWnd, IDC_EDIT_RESULT_A), buf);

  // r (всегда между -1 и 1, три знака после запятой)
  swprintf_s(buf, L"%.3f", r);
  SetWindowText(GetDlgItem(hResultWnd, IDC_EDIT_RESULT_R), buf);

  // Ea (может быть в тысячах Дж/моль, две цифры после запятой)
  swprintf_s(buf, L"%.2f", Ea);
  SetWindowText(GetDlgItem(hResultWnd, IDC_EDIT_RESULT_EA), buf);
}
