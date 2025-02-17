/**
 * \file MainWindow.cpp
 * \brief Реализация класса MainWindow.
 *
 * Этот файл содержит реализацию методов класса MainWindow, который отвечает за создание
 * и управление главным окном приложения, обработку сообщений, создание дочерних элементов,
 * обработку событий мыши, запуск расчётов и перерисовку графика.
 *
 * \author Шунин Михаил Дмитриевич
 * \date Февраль 2025
 */

#include "MainWindow.h"

#include <cwchar>
#include <sstream>
#include <stdexcept>
#include <windowsx.h>

#include "ChartDrawer.h"
#include "ResultWindow.h"
#include "Utils.h"

/**
 * \brief Конструктор класса MainWindow.
 *
 * Инициализирует значения по умолчанию, устанавливая:
 * - указатель на главное окно в nullptr;
 * - количество точек (\c m_nPoints) равным 5;
 * - начальные концентрации (\c m_Cb, \c m_Cc) в 0.0;
 * - параметры расчёта (\c m_n, \c m_k, \c m_r, \c m_disp) в 0.0;
 * - флаг нахождения курсора в области графика (\c m_inChartArea) в false;
 * - флаг инициализации (\c m_initializing) в true.
 *
 * Также производится резервирование памяти для векторов дескрипторов полей ввода.
 */
MainWindow::MainWindow()
    : m_hWnd(nullptr),
      m_nPoints(5),
      m_Cb(0.0),
      m_Cc(0.0),
      m_n(0.0),
      m_k(0.0),
      m_r(0.0),
      m_disp(0.0),
      m_inChartArea(false),
      m_initializing(true)  // Включаем флаг инициализации
{
    m_EditsCa.resize(MAX_POINTS, nullptr);
    m_EditsTm.resize(MAX_POINTS, nullptr);
}

/**
 * \brief Деструктор класса MainWindow.
 */
MainWindow::~MainWindow() {}

/**
 * \brief Регистрирует класс окна и создаёт главное окно приложения.
 *
 * Вызывает метод \c RegisterWindowClass() для регистрации класса окна, а затем
 * создаёт окно с заданными стилями и размерами. В качестве параметра \c this передается
 * указатель на текущий объект, чтобы потом можно было получить доступ к нему через
 * GWLP_USERDATA.
 *
 * \param hInstance Дескриптор экземпляра приложения.
 * \return \c true, если окно успешно создано, иначе \c false.
 */
bool MainWindow::Create(HINSTANCE hInstance) {
  if (!RegisterWindowClass(hInstance))
    return false;

  m_hWnd = CreateWindowEx(
      0, L"ChemConstMainClass",
      L"Оценка химических констант (3A = 7B + 3C) методом рег. анализа",
      WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION | WS_VISIBLE,
      CW_USEDEFAULT, CW_USEDEFAULT, 900, 600,
      nullptr, nullptr, hInstance, this);

  return (m_hWnd != nullptr);
}

/**
 * \brief Регистрирует класс главного окна.
 *
 * Заполняет структуру \c WNDCLASS и регистрирует её с помощью \c RegisterClass.
 *
 * \param hInstance Дескриптор экземпляра приложения.
 * \return \c true, если регистрация прошла успешно, иначе \c false.
 */
bool MainWindow::RegisterWindowClass(HINSTANCE hInstance) {
  WNDCLASS wc = {};
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = MainWindow::StaticWndProc;
  wc.hInstance = hInstance;
  wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
  wc.lpszClassName = L"ChemConstMainClass";

  return (RegisterClass(&wc) != 0);
}

/**
 * \brief Запускает цикл обработки сообщений.
 *
 * Функция запускает стандартный цикл обработки сообщений Windows, который
 * продолжает работу, пока не поступит сообщение о завершении приложения.
 */
void MainWindow::MessageLoop() {
  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

/**
 * \brief Статическая оконная процедура для главного окна.
 *
 * Извлекает указатель на объект MainWindow из пользовательских данных окна (GWLP_USERDATA)
 * и вызывает член-функцию \c WndProc для обработки сообщения.
 *
 * \param hwnd Дескриптор окна.
 * \param msg Код сообщения.
 * \param wParam Дополнительная информация о сообщении (параметр).
 * \param lParam Дополнительная информация о сообщении (параметр).
 * \return Результат обработки сообщения.
 */
LRESULT CALLBACK MainWindow::StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  MainWindow* pThis = nullptr;
  if (msg == WM_NCCREATE) {
    CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
    pThis = reinterpret_cast<MainWindow*>(pCreate->lpCreateParams);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    pThis->m_hWnd = hwnd;
  } else {
    pThis = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
  }
  if (pThis)
    return pThis->WndProc(msg, wParam, lParam);
  else
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

/**
 * \brief Член-функция оконной процедуры для главного окна.
 *
 * Обрабатывает основные сообщения окна:
 * - \c WM_CREATE: создание дочерних элементов.
 * - \c WM_MOUSEMOVE: обновление координат курсора.
 * - \c WM_COMMAND: обработка команд от элементов управления.
 * - \c WM_PAINT: перерисовка графика.
 * - \c WM_DESTROY: завершение работы приложения.
 *
 * \param msg Код сообщения.
 * \param wParam Дополнительная информация о сообщении.
 * \param lParam Дополнительная информация о сообщении.
 * \return Результат обработки сообщения.
 */
LRESULT MainWindow::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
  switch (msg) {
    case WM_CREATE:
      CreateChildControls();
      break;

    case WM_MOUSEMOVE:
      OnMouseMove(wParam, lParam);
      break;

    case WM_COMMAND:
      OnCommand(wParam, lParam);
      break;

    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(m_hWnd, &ps);
      RECT rcClient;
      GetClientRect(m_hWnd, &rcClient);

      // Определение области графика: правая часть окна (оставляем ~250px слева)
      RECT rcChart = rcClient;
      rcChart.left += 250;
      FillRect(hdc, &rcChart, (HBRUSH)(COLOR_WINDOW + 1));

      // Считывание экспериментальных данных из полей ввода
      std::vector<double> Ca;
      std::vector<double> Tm;
      for (int i = 0; i < m_nPoints; i++) {
        Ca.push_back(GetEditDouble(m_EditsCa[i]));
        Tm.push_back(GetEditDouble(m_EditsTm[i]));
      }

      // Рисование графика с использованием функции DrawChart
      DrawChart(hdc, rcChart, Ca, Tm, m_Cb, m_Cc, m_k, m_n);
      EndPaint(m_hWnd, &ps);
    }
      break;

    case WM_DESTROY:
      PostQuitMessage(0);
      break;

    default:
      return DefWindowProc(m_hWnd, msg, wParam, lParam);
  }
  return 0;
}

/**
 * \brief Создаёт дочерние элементы управления главного окна.
 *
 * Функция создаёт и размещает на главном окне следующие элементы управления:
 * - Статические надписи с описанием реакции, количества точек, Cb, Cc.
 * - Поля ввода для количества точек, Cb, Cc.
 * - Кнопки "Графики" и "Выход".
 * - Метку для отображения координат курсора.
 * - Поля ввода для экспериментальных значений Ca и t.
 *
 * После создания всех элементов управления, флаг инициализации \c m_initializing
 * устанавливается в \c false.
 */
void MainWindow::CreateChildControls() {
  // Текст с описанием реакции
  CreateWindowEx(0, L"STATIC", L"Реакция: 3A = 7B + 3C\nСкорость: W = k * Ca^n",
                 WS_CHILD | WS_VISIBLE | SS_LEFT,
                 20, 10, 160, 40, m_hWnd,
                 nullptr, nullptr, nullptr);

  // Надпись "Количество точек"
  CreateWindowEx(0, L"STATIC", L"Количество точек",
                 WS_CHILD | WS_VISIBLE | SS_LEFT,
                 20, 60, 120, 20, m_hWnd,
                 nullptr, nullptr, nullptr);

  // Поле для ввода количества точек
  m_hPointsEdit = CreateWindowEx(
      WS_EX_CLIENTEDGE, L"EDIT", L"5",
      WS_CHILD | WS_VISIBLE | ES_NUMBER,
      140, 60, 50, 20, m_hWnd,
      (HMENU)IDC_POINTS_EDIT, nullptr, nullptr);

  // Надпись "Cb:"
  CreateWindowEx(0, L"STATIC", L"Cb:",
                 WS_CHILD | WS_VISIBLE | SS_LEFT,
                 20, 90, 30, 20, m_hWnd,
                 nullptr, nullptr, nullptr);

  // Поле для ввода Cb
  m_hCbEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"0.1",
                             WS_CHILD | WS_VISIBLE | ES_LEFT,
                             60, 90, 60, 20, m_hWnd,
                             (HMENU)IDC_CONC_CB_EDIT, nullptr, nullptr);

  // Надпись "Cc:"
  CreateWindowEx(0, L"STATIC", L"Cc:",
                 WS_CHILD | WS_VISIBLE | SS_LEFT,
                 140, 90, 30, 20, m_hWnd,
                 nullptr, nullptr, nullptr);

  // Поле для ввода Cc
  m_hCcEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"0.7",
                             WS_CHILD | WS_VISIBLE | ES_LEFT,
                             180, 90, 60, 20, m_hWnd,
                             (HMENU)IDC_CONC_CC_EDIT, nullptr, nullptr);

  // Кнопка "Графики"
  CreateWindowEx(0, L"BUTTON", L"Графики",
                 WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                 20, 130, 100, 30, m_hWnd,
                 (HMENU)IDC_BUTTON_GRAPH, nullptr, nullptr);

  // Кнопка "Выход"
  CreateWindowEx(0, L"BUTTON", L"Выход",
                 WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                 140, 130, 100, 30, m_hWnd,
                 (HMENU)IDC_BUTTON_EXIT, nullptr, nullptr);

  // Метка для отображения координат (новая)
  m_hCoordLabel = CreateWindowEx(
      0, L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_LEFT,
      460, 10, 200, 20, m_hWnd,
      nullptr, nullptr, nullptr);

  // Создаём поля для ввода экспериментальных значений Ca и t
  int startY = 180;
  double defaultCa[5] = {2.0, 1.8, 1.6, 1.4, 1.2};
  double defaultT[5] = {0.0, 1.0, 2.0, 3.0, 4.0};

  for (int i = 0; i < MAX_POINTS; i++) {
    // Создание поля для Ca
    m_EditsCa[i] = CreateWindowEx(
        WS_EX_CLIENTEDGE, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | ES_LEFT,
        20, startY + i * 25, 60, 20,
        m_hWnd, (HMENU)(IDC_BASE_CA + i), nullptr, nullptr);

    // Создание поля для t
    m_EditsTm[i] = CreateWindowEx(
        WS_EX_CLIENTEDGE, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | ES_LEFT,
        90, startY + i * 25, 60, 20,
        m_hWnd, (HMENU)(IDC_BASE_T + i), nullptr, nullptr);

    // Показываем или скрываем поля в зависимости от m_nPoints
    if (i < m_nPoints) {
      ShowWindow(m_EditsCa[i], SW_SHOW);
      ShowWindow(m_EditsTm[i], SW_SHOW);

      // Если для первых 5 точек заданы дефолтные значения, устанавливаем их
      if (i < 5) {
        wchar_t buf[32];
        swprintf_s(buf, L"%.1f", defaultCa[i]);
        SetWindowText(m_EditsCa[i], buf);

        swprintf_s(buf, L"%.1f", defaultT[i]);
        SetWindowText(m_EditsTm[i], buf);
      }
    } else {
      // Если i >= m_nPoints, прячем поля ввода
      ShowWindow(m_EditsCa[i], SW_HIDE);
      ShowWindow(m_EditsTm[i], SW_HIDE);
    }
  }

  // Завершаем инициализацию дочерних элементов
  this->m_initializing = false;
}

/**
 * \brief Обрабатывает сообщения WM_COMMAND.
 *
 * Функция анализирует идентификатор команды и выполняет соответствующие действия:
 * - Для изменения количества точек (IDC_POINTS_EDIT): обновляет значение \c m_nPoints и
 *   показывает или скрывает соответствующие поля ввода.
 * - Для кнопки "Графики" (IDC_BUTTON_GRAPH): вызывает расчет параметров и перерисовку окна.
 * - Для кнопки "Выход" (IDC_BUTTON_EXIT): инициирует закрытие главного окна.
 *
 * \param wParam Содержит идентификатор команды и информацию о событии.
 * \param lParam Дополнительная информация о сообщении (не используется).
 */
void MainWindow::OnCommand(WPARAM wParam, LPARAM lParam) {
  int wmId = LOWORD(wParam);
  int wmEvent = HIWORD(wParam);

  switch (wmId) {
    case IDC_POINTS_EDIT:
      if (wmEvent == EN_CHANGE && !this->m_initializing) {
        wchar_t buf[32];
        GetWindowText(m_hPointsEdit, buf, 32);
        int val = _wtoi(buf);
        if (val < 1)
          val = 1;
        if (val > MAX_POINTS)
          val = MAX_POINTS;
        m_nPoints = val;
        // Показываем или скрываем поля ввода в зависимости от нового значения m_nPoints
        for (int i = 0; i < MAX_POINTS; i++) {
          if (i < m_nPoints) {
            ShowWindow(m_EditsCa[i], SW_SHOW);
            ShowWindow(m_EditsTm[i], SW_SHOW);
          } else {
            ShowWindow(m_EditsCa[i], SW_HIDE);
            ShowWindow(m_EditsTm[i], SW_HIDE);
          }
        }
      }
      break;

    case IDC_BUTTON_GRAPH:
      OnCalculateAndRedraw();
      break;

    case IDC_BUTTON_EXIT:
      PostMessage(m_hWnd, WM_CLOSE, 0, 0);
      break;
  }
}

/**
 * \brief Обрабатывает сообщение WM_MOUSEMOVE для обновления координат курсора.
 *
 * Функция получает координаты курсора в клиентской области, изменяет курсор в зависимости
 * от его положения (в области графика или вне её) и, если курсор находится в области графика,
 * вычисляет модельные координаты, которые затем отображаются в метке.
 *
 * \param wParam Содержит информацию о кнопках мыши (не используется).
 * \param lParam Содержит координаты курсора в клиентских координатах.
 */
void MainWindow::OnMouseMove(WPARAM wParam, LPARAM lParam) {
  int xPos = GET_X_LPARAM(lParam);
  int yPos = GET_Y_LPARAM(lParam);

  // Меняем курсор на "крестик"
  SetCursor(LoadCursor(nullptr, IDC_CROSS));

  RECT rcClient;
  GetClientRect(m_hWnd, &rcClient);
  RECT rcChart = rcClient;
  rcChart.left += 250;  // Область графика начинается с 250px от левого края

  bool nowInChart = (xPos >= rcChart.left && xPos <= rcChart.right &&
                     yPos >= rcChart.top && yPos <= rcChart.bottom);

  if (nowInChart && !m_inChartArea) {
    m_inChartArea = true;
    SetCursor(LoadCursor(nullptr, IDC_CROSS));
  } else if (!nowInChart && m_inChartArea) {
    m_inChartArea = false;
    SetCursor(LoadCursor(nullptr, IDC_ARROW));
  }

  // Если курсор находится в области графика, вычисляем модельные координаты
  if (nowInChart) {
    std::vector<double> Ca;
    std::vector<double> Tm;
    for (int i = 0; i < m_nPoints; i++) {
      Ca.push_back(GetEditDouble(m_EditsCa[i]));
      Tm.push_back(GetEditDouble(m_EditsTm[i]));
    }
    double Tmin = 1e9, Tmax = -1e9, Cmin = 1e9, Cmax = -1e9;
    for (int i = 0; i < m_nPoints; i++) {
      if (Tm[i] < Tmin)
        Tmin = Tm[i];
      if (Tm[i] > Tmax)
        Tmax = Tm[i];
      if (Ca[i] < Cmin)
        Cmin = Ca[i];
      if (Ca[i] > Cmax)
        Cmax = Ca[i];
    }
    if (m_Cb < Cmin)
      Cmin = m_Cb;
    if (m_Cb > Cmax)
      Cmax = m_Cb;
    if (m_Cc < Cmin)
      Cmin = m_Cc;
    if (m_Cc > Cmax)
      Cmax = m_Cc;
    if (Tmin > 1e8) {
      Tmin = 0;
      Tmax = 1;
    }
    if (fabs(Tmax - Tmin) < 1e-6) {
      Tmax = Tmin + 1;
    }
    if (fabs(Cmax - Cmin) < 1e-6) {
      Cmax = Cmin + 1;
    }
    if (Cmin < 0)
      Cmin = 0;
    if (Tmin < 0)
      Tmin = 0;

    double offsetX = 40.0, offsetY = 40.0;
    double plotW = (rcChart.right - rcChart.left) - 50.0;
    double plotH = (rcChart.bottom - rcChart.top) - 50.0;

    double px = static_cast<double>(xPos);
    double py = static_cast<double>(yPos);

    double tData = 0.0, cData = 0.0;
    {
      double pxChart = px - (rcChart.left + offsetX);
      if (pxChart < 0)
        pxChart = 0;
      if (pxChart > (plotW - 20))
        pxChart = (plotW - 20);
      tData = Tmin + (pxChart / (plotW - 20)) * (Tmax - Tmin);
    }
    {
      double pyChart = py - rcChart.top;
      double v = (plotH - (pyChart)) / (plotH - offsetY);
      cData = Cmin + v * (Cmax - Cmin);
      if (cData < 0)
        cData = 0;
    }

    wchar_t buf[128];
    swprintf_s(buf, L"x=%.3f, y=%.3f", tData, cData);
    SetWindowText(m_hCoordLabel, buf);
  }
}

/**
 * \brief Выполняет расчёт параметров и перерисовывает главное окно.
 *
 * Функция считывает введённые значения начальных концентраций, экспериментальных данных,
 * затем вызывает метод \c ChemCalculation::Calculate для вычисления параметров реакции.
 * В случае успешного расчёта происходит перерисовка главного окна и отображение окна результатов.
 * Если во время расчёта возникает ошибка, выводится сообщение об ошибке.
 */
void MainWindow::OnCalculateAndRedraw() {
  // Считываем значения начальных концентраций Cb и Cc
  m_Cb = GetEditDouble(m_hCbEdit);
  m_Cc = GetEditDouble(m_hCcEdit);
  if (m_Cb < 0.0 || m_Cc < 0.0) {
    ShowError(m_hWnd,
              L"Начальные концентрации Cb и Cc не могут быть отрицательными!");
    return;
  }
  std::vector<double> Ca;
  std::vector<double> Tm;
  for (int i = 0; i < m_nPoints; i++) {
    Ca.push_back(GetEditDouble(m_EditsCa[i]));
    Tm.push_back(GetEditDouble(m_EditsTm[i]));
  }

  // Выполняем расчёт через класс ChemCalculation
  try {
    CalculationResult res = ChemCalculation::Calculate(Ca, Tm, m_Cb, m_Cc);
    m_n = res.n;
    m_k = res.k;
    m_r = res.r;
    m_disp = res.disp;
  } catch (const std::runtime_error& e) {
    std::wstringstream ws;
    ws << L"Ошибка расчёта:\n" << e.what();
    ShowError(m_hWnd, ws.str().c_str());
    return;
  }

  // Обновляем окно (перерисовка графика)
  InvalidateRect(m_hWnd, nullptr, TRUE);

  // Отображаем окно результатов с вычисленными параметрами
  ResultWindow::Show(m_hWnd, m_n, m_k, m_disp, m_r);
}
