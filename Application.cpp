/**
 * \file Application.cpp
 * \brief ����� ����� (wWinMain) ��� Win32-���������� � �������������� Unicode.
 *
 * ������ ���� �������� �������� ������� \c wWinMain, �������:
 * - ������ � �������������� ������� ���� (����� \c MainWindow).
 * - ������������ ����� ���� ��� ������ ����������� (����� \c ResultWindow).
 * - ��������� ���� ��������� ��������� (MessageLoop), ���� �� ��������� ����� �� ����������.
 *
 * \author ����� ������ ����������
 * \date ������� 2025
 */

#include <windows.h>
#include <cstdlib>

#include "MainWindow.h"
#include "ResultWindow.h"

/**
 * \brief ����� ����� � Win32-���������� (UNICODE).
 *
 * \param hInst ���������� ���������� ���������� (instance handle).
 * \param hPrevInst � ����������� ����������� Win32 �� ������������ (������������ ���������).
 * \param lpCmdLine ��������� �� ������ ��������� ������ � ������� Unicode.
 * \param nCmdShow ����� ������ ����������� ���� (�������, ���������, ������ � �.�.).
 * \return ������������� ���, ������������ ����������� ��� ����������.
 *
 * � ���� ������� ���������� ���������:
 * 1. �������� ������ ������ \c MainWindow, ������� �������� �� ������� ���� ����������.
 * 2. �������� ������� ������� ���� ������� \c mainWin.Create(hInst). ���� �������� � ���������� ��������� �� ������ � ��������� ���������.
 * 3. ������������ ����� ���� ����������� (\c ResultWindow), ��������� ������� \c RegisterClass. 
 *    ���� ����������� �� ������� � ���������� ��������� �� ������ � ��������� ���������.
 * 4. ��������� ���� ��������� ��������� \c mainWin.MessageLoop(). 
 *    ���� � ������� ���� ���������, ��� �������������� � ���������������� � ������� ���������.
 * 5. ���������� ��� ������, ������� ������������ ��� ���������� ����� ��������� ���������.
 */
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int) {
  // ������ ������� ����
  MainWindow mainWin;
  if (!mainWin.Create(hInst)) {
    MessageBox(nullptr, L"�� ������� ������� ������� ����!", L"������",
               MB_ICONERROR);
    return EXIT_FAILURE;
  }

  // ������������ ����� ���� �����������
  WNDCLASS wcRes = {};
  wcRes.style = CS_HREDRAW | CS_VREDRAW;
  wcRes.lpfnWndProc = ResultWindow::StaticWndProc;
  wcRes.hInstance = hInst;
  wcRes.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcRes.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
  wcRes.lpszClassName = CLASSNAME_RESULT;
  if (!RegisterClass(&wcRes)) {
    MessageBox(nullptr, L"�� ������� ���������������� ����� ���� �����������!",
               L"������", MB_ICONERROR);
    return EXIT_FAILURE;
  }

  // ��������� ���� ���������
  mainWin.MessageLoop();
  return EXIT_SUCCESS;
}
