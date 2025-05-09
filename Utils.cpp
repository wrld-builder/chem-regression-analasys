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
#include <string>
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

/**
 * @brief Преобразует строку std::string в std::wstring.
 *
 * Функция выполняет конвертацию строки из ANSI-кодировки (используемой по
 * умолчанию CP_ACP) в широкую строку (std::wstring) с помощью Windows API
 * функции MultiByteToWideChar.
 *
 * Если входная строка пустая или при вычислении требуемого размера возникает
 * ошибка, функция возвращает пустую std::wstring.
 *
 * @param str Входная строка в формате std::string.
 * @return std::wstring Преобразованная строка в формате std::wstring. При
 * ошибке возвращается пустая строка.
 */
std::wstring s2ws(const std::string& str) {
  // Если входная строка пустая, возвращаем пустой wstring.
  if (str.empty()) return std::wstring();

  // Вычисляем, сколько символов понадобится для представления строки в виде
  // wstring.
  int size_needed = MultiByteToWideChar(
      CP_ACP, 0, str.c_str(), static_cast<int>(str.size()), nullptr, 0);
  if (size_needed <= 0) return std::wstring();

  // Выделяем буфер для wstring нужного размера.
  std::wstring wstr(size_needed, 0);

  // Производим конвертацию строки из ANSI в Unicode.
  MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.size()),
                      &wstr[0], size_needed);

  return wstr;
}
