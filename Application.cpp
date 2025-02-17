/**
 * \file Application.cpp
 * \brief Точка входа (wWinMain) для Win32-приложения с использованием Unicode.
 *
 * Данный файл содержит основную функцию \c wWinMain, которая:
 * - Создаёт и инициализирует главное окно (класс \c MainWindow).
 * - Регистрирует класс окна для вывода результатов (класс \c ResultWindow).
 * - Запускает цикл обработки сообщений (MessageLoop), пока не произойдёт выход из приложения.
 *
 * \author Шунин Михаил Дмитриевич
 * \date Февраль 2025
 */

#include <windows.h>
#include <cstdlib>

#include "MainWindow.h"
#include "ResultWindow.h"

/**
 * \brief Точка входа в Win32-приложение (UNICODE).
 *
 * \param hInst Дескриптор экземпляра приложения (instance handle).
 * \param hPrevInst В современных приложениях Win32 не используется (исторический пережиток).
 * \param lpCmdLine Указатель на строку командной строки в формате Unicode.
 * \param nCmdShow Задаёт способ отображения окна (свёрнуто, развёрнуто, скрыто и т.д.).
 * \return Целочисленный код, возвращаемый приложением при завершении.
 *
 * В этой функции происходит следующее:
 * 1. Создаётся объект класса \c MainWindow, который отвечает за главное окно приложения.
 * 2. Пытаемся создать главное окно методом \c mainWin.Create(hInst). Если неудачно — показываем сообщение об ошибке и завершаем программу.
 * 3. Регистрируем класс окна результатов (\c ResultWindow), используя функцию \c RegisterClass. 
 *    Если регистрация не удалась — показываем сообщение об ошибке и завершаем программу.
 * 4. Запускаем цикл обработки сообщений \c mainWin.MessageLoop(). 
 *    Пока в очереди есть сообщения, они обрабатываются и перенаправляются в оконные процедуры.
 * 5. Возвращаем код выхода, который генерируется при завершении цикла обработки сообщений.
 */
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int) {
  // Создаём главное окно
  MainWindow mainWin;
  if (!mainWin.Create(hInst)) {
    MessageBox(nullptr, L"Не удалось создать главное окно!", L"Ошибка",
               MB_ICONERROR);
    return EXIT_FAILURE;
  }

  // Регистрируем класс окна результатов
  WNDCLASS wcRes = {};
  wcRes.style = CS_HREDRAW | CS_VREDRAW;
  wcRes.lpfnWndProc = ResultWindow::StaticWndProc;
  wcRes.hInstance = hInst;
  wcRes.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcRes.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
  wcRes.lpszClassName = CLASSNAME_RESULT;
  if (!RegisterClass(&wcRes)) {
    MessageBox(nullptr, L"Не удалось зарегистрировать класс окна результатов!",
               L"Ошибка", MB_ICONERROR);
    return EXIT_FAILURE;
  }

  // Запускаем цикл сообщений
  mainWin.MessageLoop();
  return EXIT_SUCCESS;
}
