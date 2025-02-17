/**
 * \file Utils.cpp
 * \brief ���������� ��������������� ������� ��� ������ � ���������� ����������.
 *
 * ���� ���� �������� ���������� ������� ��� ��������� �������� �������� �� ��������� ���������� ���� EDIT
 * � ��� ������ ��������� �� ������.
 *
 * \author ����� ������ ����������
 * \date ������� 2025
 */

#include "Utils.h"

#include <cstdlib>
#include <cwchar>

/**
 * \brief �������� �������� �������� �� �������� ���������� EDIT.
 *
 * ������� ��������� ����� �� �������� ����������, ��������� ������� \c GetWindowText, 
 * ����� ����������� ���������� ������ � �������� ���� \c double � ������� ������� \c _wtof.
 *
 * \param hEdit ���������� �������� ���������� EDIT.
 * \return �������� ���� \c double, ���������� �� ������ ��������.
 */
double GetEditDouble(HWND hEdit) {
  wchar_t buf[128] = {0};
  GetWindowText(hEdit, buf, 128);
  return _wtof(buf);
}

/**
 * \brief ���������� ���� � ���������� �� ������.
 *
 * ������� �������� \c MessageBox ��� ������ ���� � ���������� �� ������, ��������� ��������� "������".
 *
 * \param hwndParent ���������� ������������� ����.
 * \param msg ���������, ������� ���������� ����������.
 */
void ShowError(HWND hwndParent, const wchar_t* msg) {
  MessageBox(hwndParent, msg, L"������", MB_ICONERROR);
}
