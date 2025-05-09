/**
 * \file ResultWindow.cpp
 * \brief ���������� ���� ��� ����������� ����������� �������.
 *
 * ������ ���� �������� ���������� ������ \c ResultWindow, ������� ������
 * �������:
 * - ������� ������� (n)
 * - ��������� �������� (k)
 * - ���������
 * - ����������� ���������� (r)
 * - �������������������� ����������� (A)
 * - ������� ��������� (Ea, ��/����)
 *
 * \author ����� ������ ����������
 * \date ������� 2025
 */

#include "ResultWindow.h"

#include <windowsx.h>

#include <cwchar>

#include "Constants.h"

// ���������� ���������� ���� �����������.
static HWND g_hResWnd = nullptr;

/**
 * \brief ����������� ������� ��������� ��� ���� �����������.
 *
 * ������������ �������� ��������� ����������, ������ OK, � ����� ���������
 * �������� ����.
 */
LRESULT CALLBACK ResultWindow::StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam,
                                             LPARAM lParam) {
  // ����������� ����������� �����, � ������� ����� ������������ ����������.
  static HWND hEdN = nullptr;
  static HWND hEdK = nullptr;
  static HWND hEdD = nullptr;
  static HWND hEdR = nullptr;
  static HWND hEdA = nullptr;
  static HWND hEdEa = nullptr;

  switch (msg) {
    case WM_CREATE: {
      g_hResWnd = hwnd;

      // === ������� ������� (n) ===
      CreateWindowEx(0, L"STATIC", L"������� ������� (n):",
                     WS_CHILD | WS_VISIBLE, 20, 20, 200, 20, hwnd,
                     (HMENU)IDC_STATIC_RESULT1, nullptr, nullptr);
      hEdN = CreateWindowEx(
          WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY,
          230, 20, 140, 20, hwnd, (HMENU)IDC_EDIT_RESULT_N, nullptr, nullptr);

      // === ��������� �������� (k) ===
      CreateWindowEx(0, L"STATIC", L"��������� �������� (k):",
                     WS_CHILD | WS_VISIBLE, 20, 60, 200, 20, hwnd,
                     (HMENU)IDC_STATIC_RESULT2, nullptr, nullptr);
      hEdK = CreateWindowEx(
          WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY,
          230, 60, 140, 20, hwnd, (HMENU)IDC_EDIT_RESULT_K, nullptr, nullptr);

      // === ��������� ===
      CreateWindowEx(0, L"STATIC", L"���������:", WS_CHILD | WS_VISIBLE, 20,
                     100, 200, 20, hwnd, (HMENU)IDC_STATIC_RESULT3, nullptr,
                     nullptr);
      hEdD = CreateWindowEx(
          WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY,
          230, 100, 140, 20, hwnd, (HMENU)IDC_EDIT_RESULT_D, nullptr, nullptr);

      // === ���������� (r) ===
      CreateWindowEx(0, L"STATIC", L"���������� (r):", WS_CHILD | WS_VISIBLE,
                     20, 140, 200, 20, hwnd, (HMENU)IDC_STATIC_RESULT4,
                     nullptr, nullptr);
      hEdR = CreateWindowEx(
          WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY,
          230, 140, 140, 20, hwnd, (HMENU)IDC_EDIT_RESULT_R, nullptr, nullptr);

      // === �������������������� ����������� (A) ===
      CreateWindowEx(0, L"STATIC", L"��������. ����. (A):",
                     WS_CHILD | WS_VISIBLE, 20, 180, 200, 20, hwnd,
                     (HMENU)IDC_STATIC_RESULT_A, nullptr, nullptr);
      hEdA = CreateWindowEx(
          WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY,
          230, 180, 140, 20, hwnd, (HMENU)IDC_EDIT_RESULT_A, nullptr, nullptr);

      // === ������� ��������� (Ea) ===
      CreateWindowEx(0, L"STATIC", L"������� ��������� (Ea):",
                     WS_CHILD | WS_VISIBLE, 20, 220, 200, 20, hwnd,
                     (HMENU)IDC_STATIC_RESULT_EA, nullptr, nullptr);
      hEdEa = CreateWindowEx(
          WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY,
          230, 220, 140, 20, hwnd, (HMENU)IDC_EDIT_RESULT_EA, nullptr, nullptr);

      // === ������ OK ===
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
 * \brief ���������� ���� ����������� ������� � ��������� �����������.
 *
 * \param hParent ���������� ������������� ����.
 * \param n ������� �������.
 * \param k ��������� ��������.
 * \param disp ���������.
 * \param r ����������� ����������.
 * \param A �������������������� �����������.
 * \param Ea ������� ��������� (��/����).
 */
void ResultWindow::Show(HWND hParent, double n, double k, double disp, double r,
                        double A, double Ea) {
  // �������������� ����� ����, ���� ����� (��������):
  // (����� ������� ��� � ������ �����, �������, ����� ����� ��� ���������������
  // ���� ���)
  WNDCLASS wc = {};
  wc.lpfnWndProc = ResultWindow::StaticWndProc;
  wc.hInstance = GetModuleHandle(nullptr);
  wc.lpszClassName = L"ResultWindowClass";
  wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
  RegisterClass(&wc);

  // ������ ���� ����������� �� �������, ������������ ��������� �������
  HWND hResultWnd = CreateWindowEx(
      0, L"ResultWindowClass", L"���������� �������",
      (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX) &
          ~(WS_MAXIMIZEBOX | WS_THICKFRAME),
      CW_USEDEFAULT, CW_USEDEFAULT, 420, 340, hParent, nullptr, wc.hInstance,
      nullptr);

  if (!hResultWnd) return;

  // ���������� ����
  ShowWindow(hResultWnd, SW_SHOW);
  UpdateWindow(hResultWnd);

  // ��������� ���� ���������� � �������� ��������
  wchar_t buf[64];

  // n (������ 1-2, 3 ����� ����� �������)
  swprintf_s(buf, L"%.3f", n);
  SetWindowText(GetDlgItem(hResultWnd, IDC_EDIT_RESULT_N), buf);

  // k, disp, A (����� ������ ��������) � ���������� %.4g
  swprintf_s(buf, L"%.4g", k);
  SetWindowText(GetDlgItem(hResultWnd, IDC_EDIT_RESULT_K), buf);

  swprintf_s(buf, L"%.4g", disp);
  SetWindowText(GetDlgItem(hResultWnd, IDC_EDIT_RESULT_D), buf);

  swprintf_s(buf, L"%.4g", A);
  SetWindowText(GetDlgItem(hResultWnd, IDC_EDIT_RESULT_A), buf);

  // r (������ ����� -1 � 1, ��� ����� ����� �������)
  swprintf_s(buf, L"%.3f", r);
  SetWindowText(GetDlgItem(hResultWnd, IDC_EDIT_RESULT_R), buf);

  // Ea (����� ���� � ������� ��/����, ��� ����� ����� �������)
  swprintf_s(buf, L"%.2f", Ea);
  SetWindowText(GetDlgItem(hResultWnd, IDC_EDIT_RESULT_EA), buf);
}
