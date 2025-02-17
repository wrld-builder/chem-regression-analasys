/**
 * \file Utils.cpp
 * \brief Реализация вспомогательных функций для работы с элементами управления.
 *
 * Этот файл содержит реализации функций для получения числовых значений из элементов управления типа EDIT
 * и для вывода сообщений об ошибке.
 *
 * \author Шунин Михаил Дмитриевич
 * \date Февраль 2025
 */

#include "Utils.h"

#include <cstdlib>
#include <cwchar>

/**
 * \brief Получает числовое значение из элемента управления EDIT.
 *
 * Функция считывает текст из элемента управления, используя функцию \c GetWindowText, 
 * затем преобразует полученную строку в значение типа \c double с помощью функции \c _wtof.
 *
 * \param hEdit Дескриптор элемента управления EDIT.
 * \return Значение типа \c double, полученное из текста элемента.
 */
double GetEditDouble(HWND hEdit) {
  wchar_t buf[128] = {0};
  GetWindowText(hEdit, buf, 128);
  return _wtof(buf);
}

/**
 * \brief Отображает окно с сообщением об ошибке.
 *
 * Функция вызывает \c MessageBox для вывода окна с сообщением об ошибке, используя заголовок "Ошибка".
 *
 * \param hwndParent Дескриптор родительского окна.
 * \param msg Сообщение, которое необходимо отобразить.
 */
void ShowError(HWND hwndParent, const wchar_t* msg) {
  MessageBox(hwndParent, msg, L"Ошибка", MB_ICONERROR);
}
