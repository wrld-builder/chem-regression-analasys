#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * \file Constants.h
 * \brief ����� �������� � ��������������� ��� ������� ���������.
 */

/**
 * \brief ������������� ��� ���� ����� ���������� �����.
 */
constexpr INT_PTR IDC_POINTS_EDIT = 101;

/**
 * \brief ������������� ��� ���� ����� ������������ Cb.
 */
constexpr INT_PTR IDC_CONC_CB_EDIT = 102;

/**
 * \brief ������������� ��� ���� ����� ������������ Cc.
 */
constexpr INT_PTR IDC_CONC_CC_EDIT = 103;

/**
 * \brief ������������� ������ ��������.
 */
constexpr int IDC_BUTTON_GRAPH = 104;

/**
 * \brief ������������� ������ ������.
 */
constexpr int IDC_BUTTON_EXIT = 105;

/**
 * \brief ������������ ����� ����������������� �����.
 */
constexpr int MAX_POINTS = 20;

/**
 * \brief ���� ��������������� ��� ����� ����� Ca.
 */
constexpr int IDC_BASE_CA = 200;

/**
 * \brief ���� ��������������� ��� ����� ����� t.
 */
constexpr int IDC_BASE_T = 300;

/**
 * \brief ��� ������ ���� �����������.
 */
constexpr wchar_t CLASSNAME_RESULT[] = L"ResultWindowClass";

/**
 * \brief ������������� ������������ ������ ��� ����������� ������� ��������
 * ������� (n)�.
 */
constexpr INT_PTR IDC_STATIC_RESULT1 = 401;

/**
 * \brief ������������� ������������ ������ ��� ����������� ������� ����������
 * �������� (k)�.
 */
constexpr INT_PTR IDC_STATIC_RESULT2 = 402;

/**
 * \brief ������������� ������������ ������ ��� ����������� ������� �����������.
 */
constexpr INT_PTR IDC_STATIC_RESULT3 = 403;

/**
 * \brief ������������� ������������ ������ ��� ����������� �������
 * ������������.
 */
constexpr INT_PTR IDC_STATIC_RESULT4 = 404;

constexpr INT_PTR IDC_STATIC_RESULT_A = 405;

constexpr INT_PTR IDC_STATIC_RESULT_EA = 406;

/**
 * \brief ������������� ������ OK � ���� �����������.
 */
constexpr INT_PTR IDC_BUTTON_OK = 405;

/**
 * \brief ������������� ����� ��� ����������� ��������� �� �������.
 */
constexpr INT_PTR IDC_COORD_LABEL = 999;

//----------------------------------------------------------------------
// ���� �� ����������� ��������� �������������� ��� ����� EDIT
// (��������, ��� ������ ����������� �������), �� �������� �� ����:
//----------------------------------------------------------------------

/**
 * \brief ������������� ���� EDIT ��� ������ n (������� �������).
 */
constexpr INT_PTR IDC_EDIT_RESULT_N = 501;

/**
 * \brief ������������� ���� EDIT ��� ������ k (��������� ��������).
 */
constexpr INT_PTR IDC_EDIT_RESULT_K = 502;

/**
 * \brief ������������� ���� EDIT ��� ������ ���������.
 */
constexpr INT_PTR IDC_EDIT_RESULT_D = 503;

/**
 * \brief ������������� ���� EDIT ��� ������ ������������ ����������.
 */
constexpr INT_PTR IDC_EDIT_RESULT_R = 504;

constexpr INT_PTR IDC_EDIT_RESULT_A = 505;

constexpr INT_PTR IDC_EDIT_RESULT_EA = 506;

#endif  // CONSTANTS_H
