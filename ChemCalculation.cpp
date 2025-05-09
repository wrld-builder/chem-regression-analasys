/**
 * \file ChemCalculation.cpp
 * \brief ���������� ������ ChemCalculation ��� ������� ���������� ���������� �������.
 *
 * ������ ���� �������� ���������� ������ \c Calculate, ������� ��������� ������ ���������� ���������� �������,
 * ��������� ����������������� ������ (�������� Ca � Tm) � ��������� ������������ Cb � Cc.
 *
 * \author ����� ������ ����������
 * \date ������� 2025
 */

#include "ChemCalculation.h"

#include <cmath>
#include <sstream>
#include <stdexcept>

#include "Utils.h"

/**
 * \brief ��������� ������ ���������� ���������� �������.
 *
 * ������� ��������� ����������������� ������, �������������� � ���� ���� ��������:
 * - \a Ca �������� �������� ������������ A,
 * - \a Tm �������� ��������������� �������� �������.
 *
 * ������������� �������� ��������� ������������:
 * - \a Cb ��� �������� B,
 * - \a Cc ��� �������� C.
 *
 * ����� ������� ������� ������� ��������� ������������ ������� ������:
 * - ��������� ������������ \a Cb � \a Cc �� ����� ���� ��������������.
 * - ��� �������� ������� \a Ca ������ ���� ����������������.
 * - �������� ������� \a Tm ������ ������ ���������� (t[i+1] > t[i]).
 * - ��� ������� ��������� �� ����� ���� �����.
 *
 * ����� ����������� ������������� ������ ������� ���������� ���������:
 * - ����������� ��������� ������������ (s2, s3, s4, s5, s6) �� ������ ���������� �������� ������� � ������������.
 * - �� ��� �������������� ������� ������� \a n � ��������� �������� \a k.
 * - ����������� ���������� \a r ����� ������������ �� ������ ���������� ����.
 *
 * ����� ������� ��������� ��������� (\a disp) ������� �������������� � �������������� ��������:
 * - �������������� ������������ �� ���������� ����� ������������������ �������.
 * - ��� ������� ��������� ����������� ������� ����� ����������������� ��������� � ���������������,
 *   ����� ����������� �������� ���� ������.
 *
 * \param Ca ������ ����������������� �������� ������������ A.
 * \param Tm ������ ����������������� �������� �������.
 * \param Cb ��������� ������������ �������� B.
 * \param Cc ��������� ������������ �������� C.
 *
 * \return ��������� \c CalculationResult, ���������� ������������ ���������:
 * - \a n: ������� �������.
 * - \a k: ��������� ��������.
 * - \a r: ����������� ����������.
 * - \a disp: ���������.
 *
 * \throw std::runtime_error ���� ������� ������ �����������:
 * - ���� \a Cb ��� \a Cc ������������.
 * - ���� �����-���� �������� � \a Ca ������������.
 * - ���� �������� � \a Tm �� ����������.
 * - ���� ���������� ����� ������ ����.
 * - ���� ��������� ������� �� ���� ��� ���������� ������������� �������������� �������.
 */
CalculationResult ChemCalculation::Calculate(const std::vector<double>& Ca,
                                             const std::vector<double>& Tm,
                                             double Cb, double Cc) {
  CalculationResult result = {0.0, 0.0, 0.0, 0.0};
  const int nPoints = static_cast<int>(Ca.size());

  // 1. �������� ������� ������
  if (Cb < 0.0 || Cc < 0.0) {
    throw std::runtime_error(
        "��������� ������������ Cb � Cc �� ����� ���� ��������������!");
  }
  for (size_t i = 0; i < Ca.size(); i++) {
    if (Ca[i] < 0.0) {
      throw std::runtime_error("�������� Ca �� ����� ���� �������������!");
    }
  }
  if (nPoints < 2) {
    throw std::runtime_error("������������ ����� ��� �������!");
  }
  for (int i = 0; i < nPoints - 1; i++) {
    if (Tm[i + 1] <= Tm[i]) {
      throw std::runtime_error(
          "����� ������ ������ ���������� (t[i+1] > t[i])!");
    }
  }

  // 2. ����� ���� ��� �������������� �������
  double s1 = static_cast<double>(nPoints - 1);
  double s2 = 0.0, s3 = 0.0, s4 = 0.0, s5 = 0.0, s6 = 0.0;

  for (int i = 0; i < nPoints - 1; i++) {
    double dC = Ca[i + 1] - Ca[i];
    double dt = Tm[i + 1] - Tm[i];
    // ������ �� ������� ���������� dt
    if (dt <= 1e-15) dt = 1e-15;

    // ��������� ��������
    double w = std::fabs(dC / dt);
    // ������ �� w=0 => log(0) = -inf
    if (w < 1e-15) {
      w = 1e-15;
    }

    // ������������� ������������ (������ �� ����)
    double cVal = (Ca[i] < 1e-15) ? 1e-15 : Ca[i];
    double x = std::log(cVal);
    double y = std::log(w);

    s2 += x;
    s3 += y;
    s4 += x * x;
    s5 += x * y;
    s6 += y * y;
  }

  // 3. ���������� ������� ������� (n) � ln(k) �� ������� �������� ���������
  double denom = (s1 * s4 - s2 * s2);
  if (std::fabs(denom) < 1e-15) {
    throw std::runtime_error(
        "���������� ��������� ��������� (������� �� ����). ��������� ������!");
  }

  result.n = (s1 * s5 - s2 * s3) / denom;    // slope
  double t_k = (s3 * s4 - s2 * s5) / denom;  // intercept
  result.k = std::exp(t_k);  // ��������� ��������

  // 4. ������ ������������ ���������� r
  double denom_r_val = (s1 * s4 - s2 * s2) * (s1 * s6 - s3 * s3);
  if (denom_r_val < 1e-15) {
    // ������ ��������� (������ ����� ��� y ���������) -> r �� ��������
    result.r = 0.0;
  } else {
    double denom_r = std::sqrt(denom_r_val);
    result.r = (s1 * s5 - s2 * s3) / denom_r;
    // ������ � �������� [-1..1] �� ������ �����������
    if (result.r > 1.0) result.r = 1.0;
    if (result.r < -1.0) result.r = -1.0;
  }

  // 5. ���������� ��������� ������� ���������� ��������������
  double sumSq = 0.0;
  double Acur = Ca[0];
  double tcur = Tm[0];

  for (int i = 1; i < nPoints; i++) {
    double dtFull = Tm[i] - tcur;
    if (dtFull < 1e-15) {
      dtFull = 1e-15;  // ������
    }
    const int subSteps = 70;  // ���������� ��������
    double dtSub = dtFull / subSteps;
    double Atemp = Acur;

    for (int s = 0; s < subSteps; s++) {
      // rate = k * A^n
      // ���� Atemp < 0, �� pow(Atemp, n) ���� NaN ��� n < 1
      if (Atemp < 1e-15) {
        Atemp = 0.0;  // ��������� �������������/������� ����� ��������
      }
      double rate = result.k * std::pow(Atemp, result.n);
      Atemp -= rate * dtSub;
      if (Atemp < 0.0) {
        Atemp = 0.0;  // �� ��� ���� � �����
      }
    }

    double diff = Ca[i] - Atemp;
    // ���������, ����� ��������� ���������� ������ double
    sumSq += std::round(diff * diff * 1e4) / 1e4;

    Acur = Atemp;
    tcur = Tm[i];
  }
  // ����� �� (nPoints - 1) ���� nPoints > 1 (�� ��������� ����)
  result.disp = sumSq / (nPoints - 1);

  // 6. �������������� �������� �� NaN/inf
  // � C++17 ����� ������������ std::isfinite,
  // � MSVC �� C++17 ����� _finite(...) � �.�.
  if (!std::isfinite(result.n) || !std::isfinite(result.k) ||
      !std::isfinite(result.r) || !std::isfinite(result.disp)) {
    throw std::runtime_error(
        "����������� ��������� �������� NaN/inf! ��������� ������.");
  }

  return result;
}
