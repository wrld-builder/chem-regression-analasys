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
#include <string>
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

/**
 * @brief ����������� ������ std::string � std::wstring.
 *
 * ������� ��������� ����������� ������ �� ANSI-��������� (������������ ��
 * ��������� CP_ACP) � ������� ������ (std::wstring) � ������� Windows API
 * ������� MultiByteToWideChar.
 *
 * ���� ������� ������ ������ ��� ��� ���������� ���������� ������� ���������
 * ������, ������� ���������� ������ std::wstring.
 *
 * @param str ������� ������ � ������� std::string.
 * @return std::wstring ��������������� ������ � ������� std::wstring. ���
 * ������ ������������ ������ ������.
 */
std::wstring s2ws(const std::string& str) {
  // ���� ������� ������ ������, ���������� ������ wstring.
  if (str.empty()) return std::wstring();

  // ���������, ������� �������� ����������� ��� ������������� ������ � ����
  // wstring.
  int size_needed = MultiByteToWideChar(
      CP_ACP, 0, str.c_str(), static_cast<int>(str.size()), nullptr, 0);
  if (size_needed <= 0) return std::wstring();

  // �������� ����� ��� wstring ������� �������.
  std::wstring wstr(size_needed, 0);

  // ���������� ����������� ������ �� ANSI � Unicode.
  MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.size()),
                      &wstr[0], size_needed);

  return wstr;
}
