#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * \file Constants.h
 * \brief Набор констант и идентификаторов для оконных элементов.
 */

/**
 * \brief Идентификатор для поля ввода количества точек.
 */
constexpr int IDC_POINTS_EDIT = 101;

/**
 * \brief Идентификатор для поля ввода концентрации Cb.
 */
constexpr int IDC_CONC_CB_EDIT = 102;

/**
 * \brief Идентификатор для поля ввода концентрации Cc.
 */
constexpr int IDC_CONC_CC_EDIT = 103;

/**
 * \brief Идентификатор кнопки «Графики».
 */
constexpr int IDC_BUTTON_GRAPH = 104;

/**
 * \brief Идентификатор кнопки «Выход».
 */
constexpr int IDC_BUTTON_EXIT = 105;

/**
 * \brief Максимальное число экспериментальных точек.
 */
constexpr int MAX_POINTS = 20;

/**
 * \brief База идентификаторов для полей ввода Ca.
 */
constexpr int IDC_BASE_CA = 200;

/**
 * \brief База идентификаторов для полей ввода t.
 */
constexpr int IDC_BASE_T = 300;

/**
 * \brief Имя класса окна результатов.
 */
constexpr wchar_t CLASSNAME_RESULT[] = L"ResultWindowClass";

/**
 * \brief Идентификатор статического текста для отображения надписи «Порядок
 * реакции (n)».
 */
constexpr int IDC_STATIC_RESULT1 = 401;

/**
 * \brief Идентификатор статического текста для отображения надписи «Константа
 * скорости (k)».
 */
constexpr int IDC_STATIC_RESULT2 = 402;

/**
 * \brief Идентификатор статического текста для отображения надписи «Дисперсия».
 */
constexpr int IDC_STATIC_RESULT3 = 403;

/**
 * \brief Идентификатор статического текста для отображения надписи
 * «Корреляция».
 */
constexpr int IDC_STATIC_RESULT4 = 404;

/**
 * \brief Идентификатор кнопки OK в окне результатов.
 */
constexpr int IDC_BUTTON_OK = 405;

/**
 * \brief Идентификатор метки для отображения координат на графике.
 */
constexpr int IDC_COORD_LABEL = 999;

//----------------------------------------------------------------------
// Если вы используете отдельные идентификаторы для полей EDIT
// (например, для вывода результатов расчёта), то добавьте их сюда:
//----------------------------------------------------------------------

/**
 * \brief Идентификатор поля EDIT для вывода n (порядка реакции).
 */
constexpr int IDC_EDIT_RESULT_N = 501;

/**
 * \brief Идентификатор поля EDIT для вывода k (константы скорости).
 */
constexpr int IDC_EDIT_RESULT_K = 502;

/**
 * \brief Идентификатор поля EDIT для вывода дисперсии.
 */
constexpr int IDC_EDIT_RESULT_D = 503;

/**
 * \brief Идентификатор поля EDIT для вывода коэффициента корреляции.
 */
constexpr int IDC_EDIT_RESULT_R = 504;

#endif  // CONSTANTS_H
