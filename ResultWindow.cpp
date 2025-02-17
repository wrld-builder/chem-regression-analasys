/**
 * \file ResultWindow.cpp
 * \brief Реализация окна для отображения результатов расчёта.
 *
 * Данный файл содержит реализацию класса \c ResultWindow, который отвечает за создание
 * и управление окном для вывода результатов расчёта: порядка реакции (n), константы скорости (k),
 * дисперсии и коэффициента корреляции.
 *
 * \author Шунин Михаил Дмитриевич
 * \date Февраль 2025
 */

#include "ResultWindow.h"
#include <windowsx.h>
#include <cwchar>
#include "Constants.h"

/**
 * \brief Глобальный дескриптор окна результатов.
 *
 * Используется для контроля существования окна результатов и предотвращения его
 * множественного создания.
 */
static HWND g_hResWnd = nullptr;


/**
 * \brief Статическая оконная процедура для окна результатов.
 *
 * Функция обрабатывает сообщения, поступающие в окно результатов, и выполняет следующие задачи:
 * - При получении сообщения \c WM_CREATE создаёт все дочерние элементы управления:
 *   - Статические надписи (лейблы) для подписи каждого результата.
 *   - Поля \c EDIT для отображения рассчитанных значений (только для чтения).
 *   - Кнопку "OK" для закрытия окна.
 * - Обрабатывает сообщение \c WM_COMMAND для закрытия окна при нажатии кнопки "OK".
 * - Обрабатывает сообщения \c WM_CLOSE и \c WM_DESTROY для корректного завершения работы окна.
 *
 * \param hwnd Дескриптор окна.
 * \param msg Код сообщения.
 * \param wParam Дополнительная информация о сообщении (например, идентификатор элемента управления).
 * \param lParam Дополнительная информация о сообщении.
 * \return Результат обработки сообщения.
 */
LRESULT CALLBACK ResultWindow::StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  // Статические переменные для хранения дескрипторов полей EDIT, в которых отображаются результаты.
  static HWND hEdN = nullptr;
  static HWND hEdK = nullptr;
  static HWND hEdD = nullptr;
  static HWND hEdR = nullptr;

  switch (msg) {
    case WM_CREATE: {
      g_hResWnd = hwnd;

      // === Порядок реакции (n) ===
      CreateWindowEx(0, L"STATIC", L"Порядок реакции (n):",
                     WS_CHILD | WS_VISIBLE, 20, 20, 180, 20, hwnd,
                     (HMENU)IDC_STATIC_RESULT1, nullptr, nullptr);

      hEdN = CreateWindowEx(
          WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY,
          220, 20, 140, 20, hwnd, (HMENU)IDC_EDIT_RESULT_N, nullptr, nullptr);

      // === Константа скорости (k) ===
      CreateWindowEx(0, L"STATIC", L"Константа скорости (k):",
                     WS_CHILD | WS_VISIBLE, 20, 60, 180, 20, hwnd,
                     (HMENU)IDC_STATIC_RESULT2, nullptr, nullptr);

      hEdK = CreateWindowEx(
          WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY,
          220, 60, 140, 20, hwnd, (HMENU)IDC_EDIT_RESULT_K, nullptr, nullptr);

      // === Дисперсия ===
      CreateWindowEx(0, L"STATIC", L"Дисперсия:", WS_CHILD | WS_VISIBLE, 20,
                     100, 180, 20, hwnd, (HMENU)IDC_STATIC_RESULT3, nullptr,
                     nullptr);

      hEdD = CreateWindowEx(
          WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY,
          220, 100, 140, 20, hwnd, (HMENU)IDC_EDIT_RESULT_D, nullptr, nullptr);

      // === Корреляция ===
      CreateWindowEx(0, L"STATIC", L"Корреляция:", WS_CHILD | WS_VISIBLE, 20,
                     140, 180, 20, hwnd, (HMENU)IDC_STATIC_RESULT4, nullptr,
                     nullptr);

      hEdR = CreateWindowEx(
          WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY,
          220, 140, 140, 20, hwnd, (HMENU)IDC_EDIT_RESULT_R, nullptr, nullptr);

      // === Кнопка OK ===
      CreateWindowEx(0, L"BUTTON", L"OK",
                     WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 160, 200, 60, 25,
                     hwnd, (HMENU)IDC_BUTTON_OK, nullptr, nullptr);
      return 0;
    }

    case WM_COMMAND: {
      const int wmId = LOWORD(wParam);

      // Если нажата кнопка "OK", закрываем окно
      if (wmId == IDC_BUTTON_OK) {
        DestroyWindow(hwnd);
      }
      return 0;
    }
    
    case WM_CLOSE:
      DestroyWindow(hwnd);
      return 0;

    // Если окно результатов уничтожается, сбрасываем глобальный дескриптор
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
 * Функция \c Show создаёт окно для отображения результатов расчёта, таких как
 * порядок реакции (n), константа скорости (k), дисперсия и коэффициент корреляции.
 * Если окно уже существует, функция активирует его.
 *
 * \param hwndParent Дескриптор родительского окна, относительно которого центрируется окно результатов.
 * \param n Рассчитанный порядок реакции.
 * \param k Рассчитанная константа скорости.
 * \param disp Рассчитанная дисперсия.
 * \param r Рассчитанный коэффициент корреляции.
 */
void ResultWindow::Show(HWND hwndParent, double n, double k, double disp,
                        double r) {
  // Если окно уже создано, просто активируем его и выходим
  if (g_hResWnd) {
    SetForegroundWindow(g_hResWnd);
    return;
  }

  HINSTANCE hInst = (HINSTANCE)GetModuleHandle(nullptr);
  DWORD style = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
  DWORD exStyle = WS_EX_DLGMODALFRAME;
  int w = 400;
  int h = 300;

  // Расположим окно результатов по центру родительского
  RECT rcParent;
  GetWindowRect(hwndParent, &rcParent);
  int cx = (rcParent.left + rcParent.right) / 2 - w / 2;
  int cy = (rcParent.top + rcParent.bottom) / 2 - h / 2;

  // Создаём окно
  g_hResWnd =
      CreateWindowEx(exStyle, CLASSNAME_RESULT, L"Результаты расчёта", style,
                     cx, cy, w, h, hwndParent, nullptr, hInst, nullptr);

  if (!g_hResWnd) {
    MessageBox(hwndParent,
               L"Не удалось создать окно результатов!\n"
               L"(В реальном проекте используйте .rc)",
               L"Ошибка", MB_ICONERROR);
    return;
  }

  // После создания окна записываем результаты в поля (EDIT)
  wchar_t buf[64];

  swprintf_s(buf, L"%.4f", n);
  SetDlgItemText(g_hResWnd, IDC_EDIT_RESULT_N, buf);

  swprintf_s(buf, L"%.4f", k);
  SetDlgItemText(g_hResWnd, IDC_EDIT_RESULT_K, buf);

  swprintf_s(buf, L"%.4f", disp);
  SetDlgItemText(g_hResWnd, IDC_EDIT_RESULT_D, buf);

  swprintf_s(buf, L"%.4f", r);
  SetDlgItemText(g_hResWnd, IDC_EDIT_RESULT_R, buf);
}
