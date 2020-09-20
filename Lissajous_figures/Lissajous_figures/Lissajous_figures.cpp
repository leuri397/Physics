// Lissajous_figures.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Lissajous_figures.h"
#include "Generator.h"
#define UPDATE_TIME_MS 25
#define ITERATIONS_IN_UPDATE 500

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
const UINT_PTR timer1 = 1;
// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void Oscillograph(HDC hDC, HWND hWnd);
Generator gen;
TCHAR a_text[40];
TCHAR b_text[40];
TCHAR phase_text[40];
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LISSAJOUSFIGURES, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LISSAJOUSFIGURES));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LISSAJOUSFIGURES));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    //wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(0, 0, 0)));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LISSAJOUSFIGURES);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//

HWND hEditA;
HWND hEditB;
HWND hEditPhase;
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{


    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_THICKFRAME,
        CW_USEDEFAULT, 0, 1000, 500, nullptr, nullptr, hInstance, nullptr);
    RECT whole;
    GetWindowRect(hWnd, &whole);
    int x_coord = (whole.right - whole.left) / 2, y_coord = (whole.bottom - whole.top) / 10;
    hEditA = CreateWindowEx(WS_EX_CLIENTEDGE , L"Edit", L"1", WS_VISIBLE | WS_CHILD | ES_LEFT| ES_AUTOHSCROLL | ES_NUMBER,
        x_coord, y_coord, 360, 23, hWnd, nullptr, hInst, 0);
    y_coord = 2* (whole.bottom - whole.top) / 10;
    hEditB = CreateWindowEx(WS_EX_CLIENTEDGE, L"Edit", L"1", WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL | ES_NUMBER,
        x_coord, y_coord, 360, 23, hWnd, nullptr, hInst, 0);
    y_coord = 3 * (whole.bottom - whole.top) / 10;
    hEditPhase = CreateWindowEx(WS_EX_CLIENTEDGE, L"Edit", L"90", WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL | ES_NUMBER,
        x_coord, y_coord, 360, 23, hWnd, nullptr, hInst, 0);
    a_text[0] = 40;
    b_text[0] = 40;
    phase_text[0] = 40;
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   SetTimer(hWnd, timer1, UPDATE_TIME_MS, (TIMERPROC)NULL);
   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    HDC hdc;
    HBRUSH hPurpleBrush;
    RECT whole = {0, 0, 0, 0};
    LRESULT result;
    switch (message)
    {
    case WM_TIMER:
        switch (wParam)
        {
        case timer1:
            GetWindowRect(hWnd, &whole);
            whole.right -= whole.left;
            whole.left = 0;
            whole.right /= 2.3;
            whole.bottom -= whole.top;
            whole.top = 0;
            result = RedrawWindow(hWnd, &whole , nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_ERASENOW);
            int length_a = SendMessage(hEditA, EM_GETLINE, 1, (LPARAM)a_text);
            int length_b = SendMessage(hEditB, EM_GETLINE, 1, (LPARAM)b_text);
            int length_phase = SendMessage(hEditPhase, EM_GETLINE, 1, (LPARAM)phase_text);
            double a, b, phase;
            _tcsncpy_s(a_text, a_text, length_a);
            _tcsncpy_s(b_text, b_text, length_b);
            _tcsncpy_s(phase_text, phase_text, length_phase);
            a = _tstof(a_text);
            b = _tstof(b_text);
            phase = _tstof(phase_text);
            gen.setPara(phase, a, b);
            a_text[0] = 40;
            b_text[0] = 40;
            phase_text[0] = 40;
            break;
        }
        break;
    case WM_ERASEBKGND:
        GetWindowRect(hWnd, &whole);
        whole.right -= whole.left;
        whole.left = 0;
        whole.top -= whole.bottom;
        whole.top = 0;
        return FillRect( (HDC)wParam, &whole, (HBRUSH)(CreateSolidBrush(RGB(0, 0, 0))) );
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:

            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case EN_CHANGE:

                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {

            PAINTSTRUCT ps;
            hdc = BeginPaint(hWnd, &ps);
            GetWindowRect(hWnd, &whole);
            int x_coord = (whole.right - whole.left) / 2, y_coord = (whole.bottom - whole.top) / 10 - 20;
            SetTextColor(hdc, RGB(255, 255, 0));
            SetBkColor(hdc, RGB(0, 0, 0));
            TextOut(hdc, x_coord, y_coord, L"A:", 2);
            MoveWindow(hEditA, x_coord, y_coord + 20, whole.right - whole.left - x_coord - 100, 23, false);
            y_coord = (whole.bottom - whole.top) / 10 + 40;
            TextOut(hdc, x_coord, y_coord, L"B:", 2);
            MoveWindow(hEditB, x_coord, y_coord + 20, whole.right - whole.left - x_coord - 100, 23, false);
            y_coord = (whole.bottom - whole.top) / 10 + 100;
            TextOut(hdc, x_coord, y_coord, L"Phase:", 6);
            MoveWindow(hEditPhase, x_coord, y_coord + 20, whole.right - whole.left - x_coord - 100, 23, false);
            Oscillograph(hdc, hWnd);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case EN_CHANGE:

        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void Oscillograph(HDC hDC, HWND hWnd)
{
    RECT figure_zone;
    GetWindowRect(hWnd, &figure_zone);
    double x_size = (figure_zone.right - figure_zone.left)/5;
    double y_size = (figure_zone.bottom - figure_zone.top)/2.5;

    if (x_size > y_size)
    {
        x_size = y_size;
    }
    else
    {
        y_size = x_size;
    }
    static int last_coord_x, last_coord_y;
    HPEN greenRay = CreatePen(PS_SOLID, 1, RGB(100, 255, 0));
    HGDIOBJ hPen = SelectObject(hDC, greenRay);
    MoveToEx(hDC, last_coord_x, last_coord_y, NULL);
    for (int i = 0; i < ITERATIONS_IN_UPDATE; i++)
    {
        LineTo(hDC, gen.get_x() * x_size + 1.1*x_size, gen.get_y() * y_size + 1.1*y_size);
        last_coord_x = gen.get_x() * x_size + 1.1 * x_size;
        last_coord_y = gen.get_y() * y_size + 1.1 * y_size;
        gen.incTime((UPDATE_TIME_MS / 1000.0) / ITERATIONS_IN_UPDATE);
    }
    DeleteObject(greenRay);
}