#ifndef CHARTDRAWER_H
#define CHARTDRAWER_H

#include <windows.h>

#include <vector>

/// \brief ������� ��� ��������� ������� ����������������� ������ �
/// �������������.
///
/// \param hdc �������� ���������� ��� ���������.
/// \param rcClient ������������� ���������� �������, ��� ������ ������.
/// \param Ca ������ ����������������� �������� ������������ A.
/// \param Tm ������ �������� �������.
/// \param Cb ��������� ������������ B.
/// \param Cc ��������� ������������ C.
/// \param k ����������� ��������� ��������.
/// \param n ����������� ������� �������.
void DrawChart(HDC hdc, const RECT& rcClient, const std::vector<double>& Ca,
               const std::vector<double>& Tm, double Cb, double Cc, double k,
               double n);

#endif  // CHARTDRAWER_H
