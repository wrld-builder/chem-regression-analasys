/**
 * \file ResultWindow.cpp
 * \brief ���������� ���� ��� ����������� ����������� �������.
 *
 * ������ ���� �������� ���������� ������ \c ResultWindow, ������� �������� �� ��������
 * � ���������� ����� ��� ������ ����������� �������: ������� ������� (n), ��������� �������� (k),
 * ��������� � ������������ ����������.
 *
 * \author ����� ������ ����������
 * \date ������� 2025
 */

#include "ResultWindow.h"
#include <windowsx.h>
#include <cwchar>
#include "Constants.h"

/**
 * \brief ���������� ���������� ���� �����������.
 *
 * ������������ ��� �������� ������������� ���� ����������� � �������������� ���
 * �������������� ��������.
 */
static HWND g_hResWnd = nullptr;


/**
 * \brief ����������� ������� ��������� ��� ���� �����������.
 *
 * ������� ������������ ���������, ����������� � ���� �����������, � ��������� ��������� ������:
 * - ��� ��������� ��������� \c WM_CREATE ������ ��� �������� �������� ����������:
 *   - ����������� ������� (������) ��� ������� ������� ����������.
 *   - ���� \c EDIT ��� ����������� ������������ �������� (������ ��� ������).
 *   - ������ "OK" ��� �������� ����.
 * - ������������ ��������� \c WM_COMMAND ��� �������� ���� ��� ������� ������ "OK".
 * - ������������ ��������� \c WM_CLOSE � \c WM_DESTROY ��� ����������� ���������� ������ ����.
 *
 * \param hwnd ���������� ����.
 * \param msg ��� ���������.
 * \param wParam �������������� ���������� � ��������� (��������, ������������� �������� ����������).
 * \param lParam �������������� ���������� � ���������.
 * \return ��������� ��������� ���������.
 */
LRESULT CALLBACK ResultWindow::StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  // ����������� ���������� ��� �������� ������������ ����� EDIT, � ������� ������������ ����������.
  static HWND hEdN = nullptr;
  static HWND hEdK = nullptr;
  static HWND hEdD = nullptr;
  static HWND hEdR = nullptr;

  switch (msg) {
    case WM_CREATE: {
      g_hResWnd = hwnd;

      // === ������� ������� (n) ===
      CreateWindowEx(0, L"STATIC", L"������� ������� (n):",
                     WS_CHILD | WS_VISIBLE, 20, 20, 180, 20, hwnd,
                     (HMENU)IDC_STATIC_RESULT1, nullptr, nullptr);

      hEdN = CreateWindowEx(
          WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY,
          220, 20, 140, 20, hwnd, (HMENU)IDC_EDIT_RESULT_N, nullptr, nullptr);

      // === ��������� �������� (k) ===
      CreateWindowEx(0, L"STATIC", L"��������� �������� (k):",
                     WS_CHILD | WS_VISIBLE, 20, 60, 180, 20, hwnd,
                     (HMENU)IDC_STATIC_RESULT2, nullptr, nullptr);

      hEdK = CreateWindowEx(
          WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY,
          220, 60, 140, 20, hwnd, (HMENU)IDC_EDIT_RESULT_K, nullptr, nullptr);

      // === ��������� ===
      CreateWindowEx(0, L"STATIC", L"���������:", WS_CHILD | WS_VISIBLE, 20,
                     100, 180, 20, hwnd, (HMENU)IDC_STATIC_RESULT3, nullptr,
                     nullptr);

      hEdD = CreateWindowEx(
          WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY,
          220, 100, 140, 20, hwnd, (HMENU)IDC_EDIT_RESULT_D, nullptr, nullptr);

      // === ���������� ===
      CreateWindowEx(0, L"STATIC", L"����������:", WS_CHILD | WS_VISIBLE, 20,
                     140, 180, 20, hwnd, (HMENU)IDC_STATIC_RESULT4, nullptr,
                     nullptr);

      hEdR = CreateWindowEx(
          WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY,
          220, 140, 140, 20, hwnd, (HMENU)IDC_EDIT_RESULT_R, nullptr, nullptr);

      // === ������ OK ===
      CreateWindowEx(0, L"BUTTON", L"OK",
                     WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 160, 200, 60, 25,
                     hwnd, (HMENU)IDC_BUTTON_OK, nullptr, nullptr);
      return 0;
    }

    case WM_COMMAND: {
      const int wmId = LOWORD(wParam);

      // ���� ������ ������ "OK", ��������� ����
      if (wmId == IDC_BUTTON_OK) {
        DestroyWindow(hwnd);
      }
      return 0;
    }
    
    case WM_CLOSE:
      DestroyWindow(hwnd);
      return 0;

    // ���� ���� ����������� ������������, ���������� ���������� ����������
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
 * \brief ���������� ���� ����������� ������� � ��������� �����������.
 *
 * ������� \c Show ������ ���� ��� ����������� ����������� �������, ����� ���
 * ������� ������� (n), ��������� �������� (k), ��������� � ����������� ����������.
 * ���� ���� ��� ����������, ������� ���������� ���.
 *
 * \param hwndParent ���������� ������������� ����, ������������ �������� ������������ ���� �����������.
 * \param n ������������ ������� �������.
 * \param k ������������ ��������� ��������.
 * \param disp ������������ ���������.
 * \param r ������������ ����������� ����������.
 */
void ResultWindow::Show(HWND hwndParent, double n, double k, double disp,
                        double r) {
  // ���� ���� ��� �������, ������ ���������� ��� � �������
  if (g_hResWnd) {
    SetForegroundWindow(g_hResWnd);
    return;
  }

  HINSTANCE hInst = (HINSTANCE)GetModuleHandle(nullptr);
  DWORD style = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
  DWORD exStyle = WS_EX_DLGMODALFRAME;
  int w = 400;
  int h = 300;

  // ���������� ���� ����������� �� ������ �������������
  RECT rcParent;
  GetWindowRect(hwndParent, &rcParent);
  int cx = (rcParent.left + rcParent.right) / 2 - w / 2;
  int cy = (rcParent.top + rcParent.bottom) / 2 - h / 2;

  // ������ ����
  g_hResWnd =
      CreateWindowEx(exStyle, CLASSNAME_RESULT, L"���������� �������", style,
                     cx, cy, w, h, hwndParent, nullptr, hInst, nullptr);

  if (!g_hResWnd) {
    MessageBox(hwndParent,
               L"�� ������� ������� ���� �����������!\n"
               L"(� �������� ������� ����������� .rc)",
               L"������", MB_ICONERROR);
    return;
  }

  // ����� �������� ���� ���������� ���������� � ���� (EDIT)
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
