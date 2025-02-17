/**
 * \file MainWindow.cpp
 * \brief ���������� ������ MainWindow.
 *
 * ���� ���� �������� ���������� ������� ������ MainWindow, ������� �������� �� ��������
 * � ���������� ������� ����� ����������, ��������� ���������, �������� �������� ���������,
 * ��������� ������� ����, ������ �������� � ����������� �������.
 *
 * \author ����� ������ ����������
 * \date ������� 2025
 */

#include "MainWindow.h"

#include <cwchar>
#include <sstream>
#include <stdexcept>
#include <windowsx.h>

#include "ChartDrawer.h"
#include "ResultWindow.h"
#include "Utils.h"

/**
 * \brief ����������� ������ MainWindow.
 *
 * �������������� �������� �� ���������, ������������:
 * - ��������� �� ������� ���� � nullptr;
 * - ���������� ����� (\c m_nPoints) ������ 5;
 * - ��������� ������������ (\c m_Cb, \c m_Cc) � 0.0;
 * - ��������� ������� (\c m_n, \c m_k, \c m_r, \c m_disp) � 0.0;
 * - ���� ���������� ������� � ������� ������� (\c m_inChartArea) � false;
 * - ���� ������������� (\c m_initializing) � true.
 *
 * ����� ������������ �������������� ������ ��� �������� ������������ ����� �����.
 */
MainWindow::MainWindow()
    : m_hWnd(nullptr),
      m_nPoints(5),
      m_Cb(0.0),
      m_Cc(0.0),
      m_n(0.0),
      m_k(0.0),
      m_r(0.0),
      m_disp(0.0),
      m_inChartArea(false),
      m_initializing(true)  // �������� ���� �������������
{
    m_EditsCa.resize(MAX_POINTS, nullptr);
    m_EditsTm.resize(MAX_POINTS, nullptr);
}

/**
 * \brief ���������� ������ MainWindow.
 */
MainWindow::~MainWindow() {}

/**
 * \brief ������������ ����� ���� � ������ ������� ���� ����������.
 *
 * �������� ����� \c RegisterWindowClass() ��� ����������� ������ ����, � �����
 * ������ ���� � ��������� ������� � ���������. � �������� ��������� \c this ����������
 * ��������� �� ������� ������, ����� ����� ����� ���� �������� ������ � ���� �����
 * GWLP_USERDATA.
 *
 * \param hInstance ���������� ���������� ����������.
 * \return \c true, ���� ���� ������� �������, ����� \c false.
 */
bool MainWindow::Create(HINSTANCE hInstance) {
  if (!RegisterWindowClass(hInstance))
    return false;

  m_hWnd = CreateWindowEx(
      0, L"ChemConstMainClass",
      L"������ ���������� �������� (3A = 7B + 3C) ������� ���. �������",
      WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION | WS_VISIBLE,
      CW_USEDEFAULT, CW_USEDEFAULT, 900, 600,
      nullptr, nullptr, hInstance, this);

  return (m_hWnd != nullptr);
}

/**
 * \brief ������������ ����� �������� ����.
 *
 * ��������� ��������� \c WNDCLASS � ������������ � � ������� \c RegisterClass.
 *
 * \param hInstance ���������� ���������� ����������.
 * \return \c true, ���� ����������� ������ �������, ����� \c false.
 */
bool MainWindow::RegisterWindowClass(HINSTANCE hInstance) {
  WNDCLASS wc = {};
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = MainWindow::StaticWndProc;
  wc.hInstance = hInstance;
  wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
  wc.lpszClassName = L"ChemConstMainClass";

  return (RegisterClass(&wc) != 0);
}

/**
 * \brief ��������� ���� ��������� ���������.
 *
 * ������� ��������� ����������� ���� ��������� ��������� Windows, �������
 * ���������� ������, ���� �� �������� ��������� � ���������� ����������.
 */
void MainWindow::MessageLoop() {
  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

/**
 * \brief ����������� ������� ��������� ��� �������� ����.
 *
 * ��������� ��������� �� ������ MainWindow �� ���������������� ������ ���� (GWLP_USERDATA)
 * � �������� ����-������� \c WndProc ��� ��������� ���������.
 *
 * \param hwnd ���������� ����.
 * \param msg ��� ���������.
 * \param wParam �������������� ���������� � ��������� (��������).
 * \param lParam �������������� ���������� � ��������� (��������).
 * \return ��������� ��������� ���������.
 */
LRESULT CALLBACK MainWindow::StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  MainWindow* pThis = nullptr;
  if (msg == WM_NCCREATE) {
    CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
    pThis = reinterpret_cast<MainWindow*>(pCreate->lpCreateParams);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    pThis->m_hWnd = hwnd;
  } else {
    pThis = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
  }
  if (pThis)
    return pThis->WndProc(msg, wParam, lParam);
  else
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

/**
 * \brief ����-������� ������� ��������� ��� �������� ����.
 *
 * ������������ �������� ��������� ����:
 * - \c WM_CREATE: �������� �������� ���������.
 * - \c WM_MOUSEMOVE: ���������� ��������� �������.
 * - \c WM_COMMAND: ��������� ������ �� ��������� ����������.
 * - \c WM_PAINT: ����������� �������.
 * - \c WM_DESTROY: ���������� ������ ����������.
 *
 * \param msg ��� ���������.
 * \param wParam �������������� ���������� � ���������.
 * \param lParam �������������� ���������� � ���������.
 * \return ��������� ��������� ���������.
 */
LRESULT MainWindow::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
  switch (msg) {
    case WM_CREATE:
      CreateChildControls();
      break;

    case WM_MOUSEMOVE:
      OnMouseMove(wParam, lParam);
      break;

    case WM_COMMAND:
      OnCommand(wParam, lParam);
      break;

    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(m_hWnd, &ps);
      RECT rcClient;
      GetClientRect(m_hWnd, &rcClient);

      // ����������� ������� �������: ������ ����� ���� (��������� ~250px �����)
      RECT rcChart = rcClient;
      rcChart.left += 250;
      FillRect(hdc, &rcChart, (HBRUSH)(COLOR_WINDOW + 1));

      // ���������� ����������������� ������ �� ����� �����
      std::vector<double> Ca;
      std::vector<double> Tm;
      for (int i = 0; i < m_nPoints; i++) {
        Ca.push_back(GetEditDouble(m_EditsCa[i]));
        Tm.push_back(GetEditDouble(m_EditsTm[i]));
      }

      // ��������� ������� � �������������� ������� DrawChart
      DrawChart(hdc, rcChart, Ca, Tm, m_Cb, m_Cc, m_k, m_n);
      EndPaint(m_hWnd, &ps);
    }
      break;

    case WM_DESTROY:
      PostQuitMessage(0);
      break;

    default:
      return DefWindowProc(m_hWnd, msg, wParam, lParam);
  }
  return 0;
}

/**
 * \brief ������ �������� �������� ���������� �������� ����.
 *
 * ������� ������ � ��������� �� ������� ���� ��������� �������� ����������:
 * - ����������� ������� � ��������� �������, ���������� �����, Cb, Cc.
 * - ���� ����� ��� ���������� �����, Cb, Cc.
 * - ������ "�������" � "�����".
 * - ����� ��� ����������� ��������� �������.
 * - ���� ����� ��� ����������������� �������� Ca � t.
 *
 * ����� �������� ���� ��������� ����������, ���� ������������� \c m_initializing
 * ��������������� � \c false.
 */
void MainWindow::CreateChildControls() {
  // ����� � ��������� �������
  CreateWindowEx(0, L"STATIC", L"�������: 3A = 7B + 3C\n��������: W = k * Ca^n",
                 WS_CHILD | WS_VISIBLE | SS_LEFT,
                 20, 10, 160, 40, m_hWnd,
                 nullptr, nullptr, nullptr);

  // ������� "���������� �����"
  CreateWindowEx(0, L"STATIC", L"���������� �����",
                 WS_CHILD | WS_VISIBLE | SS_LEFT,
                 20, 60, 120, 20, m_hWnd,
                 nullptr, nullptr, nullptr);

  // ���� ��� ����� ���������� �����
  m_hPointsEdit = CreateWindowEx(
      WS_EX_CLIENTEDGE, L"EDIT", L"5",
      WS_CHILD | WS_VISIBLE | ES_NUMBER,
      140, 60, 50, 20, m_hWnd,
      (HMENU)IDC_POINTS_EDIT, nullptr, nullptr);

  // ������� "Cb:"
  CreateWindowEx(0, L"STATIC", L"Cb:",
                 WS_CHILD | WS_VISIBLE | SS_LEFT,
                 20, 90, 30, 20, m_hWnd,
                 nullptr, nullptr, nullptr);

  // ���� ��� ����� Cb
  m_hCbEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"0.1",
                             WS_CHILD | WS_VISIBLE | ES_LEFT,
                             60, 90, 60, 20, m_hWnd,
                             (HMENU)IDC_CONC_CB_EDIT, nullptr, nullptr);

  // ������� "Cc:"
  CreateWindowEx(0, L"STATIC", L"Cc:",
                 WS_CHILD | WS_VISIBLE | SS_LEFT,
                 140, 90, 30, 20, m_hWnd,
                 nullptr, nullptr, nullptr);

  // ���� ��� ����� Cc
  m_hCcEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"0.7",
                             WS_CHILD | WS_VISIBLE | ES_LEFT,
                             180, 90, 60, 20, m_hWnd,
                             (HMENU)IDC_CONC_CC_EDIT, nullptr, nullptr);

  // ������ "�������"
  CreateWindowEx(0, L"BUTTON", L"�������",
                 WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                 20, 130, 100, 30, m_hWnd,
                 (HMENU)IDC_BUTTON_GRAPH, nullptr, nullptr);

  // ������ "�����"
  CreateWindowEx(0, L"BUTTON", L"�����",
                 WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                 140, 130, 100, 30, m_hWnd,
                 (HMENU)IDC_BUTTON_EXIT, nullptr, nullptr);

  // ����� ��� ����������� ��������� (�����)
  m_hCoordLabel = CreateWindowEx(
      0, L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_LEFT,
      460, 10, 200, 20, m_hWnd,
      nullptr, nullptr, nullptr);

  // ������ ���� ��� ����� ����������������� �������� Ca � t
  int startY = 180;
  double defaultCa[5] = {2.0, 1.8, 1.6, 1.4, 1.2};
  double defaultT[5] = {0.0, 1.0, 2.0, 3.0, 4.0};

  for (int i = 0; i < MAX_POINTS; i++) {
    // �������� ���� ��� Ca
    m_EditsCa[i] = CreateWindowEx(
        WS_EX_CLIENTEDGE, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | ES_LEFT,
        20, startY + i * 25, 60, 20,
        m_hWnd, (HMENU)(IDC_BASE_CA + i), nullptr, nullptr);

    // �������� ���� ��� t
    m_EditsTm[i] = CreateWindowEx(
        WS_EX_CLIENTEDGE, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | ES_LEFT,
        90, startY + i * 25, 60, 20,
        m_hWnd, (HMENU)(IDC_BASE_T + i), nullptr, nullptr);

    // ���������� ��� �������� ���� � ����������� �� m_nPoints
    if (i < m_nPoints) {
      ShowWindow(m_EditsCa[i], SW_SHOW);
      ShowWindow(m_EditsTm[i], SW_SHOW);

      // ���� ��� ������ 5 ����� ������ ��������� ��������, ������������� ��
      if (i < 5) {
        wchar_t buf[32];
        swprintf_s(buf, L"%.1f", defaultCa[i]);
        SetWindowText(m_EditsCa[i], buf);

        swprintf_s(buf, L"%.1f", defaultT[i]);
        SetWindowText(m_EditsTm[i], buf);
      }
    } else {
      // ���� i >= m_nPoints, ������ ���� �����
      ShowWindow(m_EditsCa[i], SW_HIDE);
      ShowWindow(m_EditsTm[i], SW_HIDE);
    }
  }

  // ��������� ������������� �������� ���������
  this->m_initializing = false;
}

/**
 * \brief ������������ ��������� WM_COMMAND.
 *
 * ������� ����������� ������������� ������� � ��������� ��������������� ��������:
 * - ��� ��������� ���������� ����� (IDC_POINTS_EDIT): ��������� �������� \c m_nPoints �
 *   ���������� ��� �������� ��������������� ���� �����.
 * - ��� ������ "�������" (IDC_BUTTON_GRAPH): �������� ������ ���������� � ����������� ����.
 * - ��� ������ "�����" (IDC_BUTTON_EXIT): ���������� �������� �������� ����.
 *
 * \param wParam �������� ������������� ������� � ���������� � �������.
 * \param lParam �������������� ���������� � ��������� (�� ������������).
 */
void MainWindow::OnCommand(WPARAM wParam, LPARAM lParam) {
  int wmId = LOWORD(wParam);
  int wmEvent = HIWORD(wParam);

  switch (wmId) {
    case IDC_POINTS_EDIT:
      if (wmEvent == EN_CHANGE && !this->m_initializing) {
        wchar_t buf[32];
        GetWindowText(m_hPointsEdit, buf, 32);
        int val = _wtoi(buf);
        if (val < 1)
          val = 1;
        if (val > MAX_POINTS)
          val = MAX_POINTS;
        m_nPoints = val;
        // ���������� ��� �������� ���� ����� � ����������� �� ������ �������� m_nPoints
        for (int i = 0; i < MAX_POINTS; i++) {
          if (i < m_nPoints) {
            ShowWindow(m_EditsCa[i], SW_SHOW);
            ShowWindow(m_EditsTm[i], SW_SHOW);
          } else {
            ShowWindow(m_EditsCa[i], SW_HIDE);
            ShowWindow(m_EditsTm[i], SW_HIDE);
          }
        }
      }
      break;

    case IDC_BUTTON_GRAPH:
      OnCalculateAndRedraw();
      break;

    case IDC_BUTTON_EXIT:
      PostMessage(m_hWnd, WM_CLOSE, 0, 0);
      break;
  }
}

/**
 * \brief ������������ ��������� WM_MOUSEMOVE ��� ���������� ��������� �������.
 *
 * ������� �������� ���������� ������� � ���������� �������, �������� ������ � �����������
 * �� ��� ��������� (� ������� ������� ��� ��� �) �, ���� ������ ��������� � ������� �������,
 * ��������� ��������� ����������, ������� ����� ������������ � �����.
 *
 * \param wParam �������� ���������� � ������� ���� (�� ������������).
 * \param lParam �������� ���������� ������� � ���������� �����������.
 */
void MainWindow::OnMouseMove(WPARAM wParam, LPARAM lParam) {
  int xPos = GET_X_LPARAM(lParam);
  int yPos = GET_Y_LPARAM(lParam);

  // ������ ������ �� "�������"
  SetCursor(LoadCursor(nullptr, IDC_CROSS));

  RECT rcClient;
  GetClientRect(m_hWnd, &rcClient);
  RECT rcChart = rcClient;
  rcChart.left += 250;  // ������� ������� ���������� � 250px �� ������ ����

  bool nowInChart = (xPos >= rcChart.left && xPos <= rcChart.right &&
                     yPos >= rcChart.top && yPos <= rcChart.bottom);

  if (nowInChart && !m_inChartArea) {
    m_inChartArea = true;
    SetCursor(LoadCursor(nullptr, IDC_CROSS));
  } else if (!nowInChart && m_inChartArea) {
    m_inChartArea = false;
    SetCursor(LoadCursor(nullptr, IDC_ARROW));
  }

  // ���� ������ ��������� � ������� �������, ��������� ��������� ����������
  if (nowInChart) {
    std::vector<double> Ca;
    std::vector<double> Tm;
    for (int i = 0; i < m_nPoints; i++) {
      Ca.push_back(GetEditDouble(m_EditsCa[i]));
      Tm.push_back(GetEditDouble(m_EditsTm[i]));
    }
    double Tmin = 1e9, Tmax = -1e9, Cmin = 1e9, Cmax = -1e9;
    for (int i = 0; i < m_nPoints; i++) {
      if (Tm[i] < Tmin)
        Tmin = Tm[i];
      if (Tm[i] > Tmax)
        Tmax = Tm[i];
      if (Ca[i] < Cmin)
        Cmin = Ca[i];
      if (Ca[i] > Cmax)
        Cmax = Ca[i];
    }
    if (m_Cb < Cmin)
      Cmin = m_Cb;
    if (m_Cb > Cmax)
      Cmax = m_Cb;
    if (m_Cc < Cmin)
      Cmin = m_Cc;
    if (m_Cc > Cmax)
      Cmax = m_Cc;
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
    if (Cmin < 0)
      Cmin = 0;
    if (Tmin < 0)
      Tmin = 0;

    double offsetX = 40.0, offsetY = 40.0;
    double plotW = (rcChart.right - rcChart.left) - 50.0;
    double plotH = (rcChart.bottom - rcChart.top) - 50.0;

    double px = static_cast<double>(xPos);
    double py = static_cast<double>(yPos);

    double tData = 0.0, cData = 0.0;
    {
      double pxChart = px - (rcChart.left + offsetX);
      if (pxChart < 0)
        pxChart = 0;
      if (pxChart > (plotW - 20))
        pxChart = (plotW - 20);
      tData = Tmin + (pxChart / (plotW - 20)) * (Tmax - Tmin);
    }
    {
      double pyChart = py - rcChart.top;
      double v = (plotH - (pyChart)) / (plotH - offsetY);
      cData = Cmin + v * (Cmax - Cmin);
      if (cData < 0)
        cData = 0;
    }

    wchar_t buf[128];
    swprintf_s(buf, L"x=%.3f, y=%.3f", tData, cData);
    SetWindowText(m_hCoordLabel, buf);
  }
}

/**
 * \brief ��������� ������ ���������� � �������������� ������� ����.
 *
 * ������� ��������� �������� �������� ��������� ������������, ����������������� ������,
 * ����� �������� ����� \c ChemCalculation::Calculate ��� ���������� ���������� �������.
 * � ������ ��������� ������� ���������� ����������� �������� ���� � ����������� ���� �����������.
 * ���� �� ����� ������� ��������� ������, ��������� ��������� �� ������.
 */
void MainWindow::OnCalculateAndRedraw() {
  // ��������� �������� ��������� ������������ Cb � Cc
  m_Cb = GetEditDouble(m_hCbEdit);
  m_Cc = GetEditDouble(m_hCcEdit);
  if (m_Cb < 0.0 || m_Cc < 0.0) {
    ShowError(m_hWnd,
              L"��������� ������������ Cb � Cc �� ����� ���� ��������������!");
    return;
  }
  std::vector<double> Ca;
  std::vector<double> Tm;
  for (int i = 0; i < m_nPoints; i++) {
    Ca.push_back(GetEditDouble(m_EditsCa[i]));
    Tm.push_back(GetEditDouble(m_EditsTm[i]));
  }

  // ��������� ������ ����� ����� ChemCalculation
  try {
    CalculationResult res = ChemCalculation::Calculate(Ca, Tm, m_Cb, m_Cc);
    m_n = res.n;
    m_k = res.k;
    m_r = res.r;
    m_disp = res.disp;
  } catch (const std::runtime_error& e) {
    std::wstringstream ws;
    ws << L"������ �������:\n" << e.what();
    ShowError(m_hWnd, ws.str().c_str());
    return;
  }

  // ��������� ���� (����������� �������)
  InvalidateRect(m_hWnd, nullptr, TRUE);

  // ���������� ���� ����������� � ������������ �����������
  ResultWindow::Show(m_hWnd, m_n, m_k, m_disp, m_r);
}
