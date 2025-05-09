#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <windows.h>

/**
 * \brief ����� ���� ��� ����������� ����������� �������.
 *
 * ���� ������� ������������ ���������: ������� �������, ��������� ��������,
 * ��������� � ����������� ����������.
 */
class ResultWindow {
 public:
  /**
   * \brief ������ � ���������� ���� � ������������ �������.
   * \param hwndParent ���������� ������������� ����.
   * \param n ������� �������.
   * \param k ��������� ��������.
   * \param disp ���������.
   * \param r ����������� ����������.
   */
  static void Show(HWND hParent, double n, double k, double disp, double r,
                   double A, double Ea);

  /// ����������� ������� ��������� ��� ���� �����������.
  static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam,
                                        LPARAM lParam);
};

#endif  // RESULTWINDOW_H
