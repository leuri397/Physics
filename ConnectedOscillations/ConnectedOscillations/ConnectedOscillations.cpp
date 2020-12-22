// ConnectedOscillations.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "ConnectedOscillations.h"
#include "Oscillators.h"
#define UPDATE_TIME_MS 35
#define ITERATIONS_IN_UPDATE 5000

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
//Generator gen;
TCHAR a_text[40];
TCHAR b_text[40];
TCHAR phase_text[40];
Oscillators oscill;

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
    LoadStringW(hInstance, IDC_CONNECTEDOSCILLATIONS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CONNECTEDOSCILLATIONS));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CONNECTEDOSCILLATIONS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CONNECTEDOSCILLATIONS);
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
HWND hwndButton;
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_THICKFRAME,
        CW_USEDEFAULT, 0, 1000, 500, nullptr, nullptr, hInstance, nullptr);
    RECT whole;
    GetWindowRect(hWnd, &whole);
    int x_coord = (whole.right - whole.left) / 2, y_coord = (whole.bottom - whole.top) / 10;
    hEditA = CreateWindowEx(WS_EX_CLIENTEDGE, L"Edit", L"0", WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL,
        x_coord, y_coord, 360, 23, hWnd, nullptr, hInst, 0);
    y_coord = 2 * (whole.bottom - whole.top) / 10;
    hEditB = CreateWindowEx(WS_EX_CLIENTEDGE, L"Edit", L"0", WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL,
        x_coord, y_coord, 360, 23, hWnd, nullptr, hInst, 0);
    y_coord = 3 * (whole.bottom - whole.top) / 10;
    hEditPhase = CreateWindowEx(WS_EX_CLIENTEDGE, L"Edit", L"10", WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL,
        x_coord, y_coord, 360, 23, hWnd, nullptr, hInst, 0);
    y_coord = 4 * (whole.bottom - whole.top) / 10;
    hwndButton = CreateWindow( L"BUTTON", L"RESET", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        x_coord, y_coord,  100, 23, hWnd, NULL, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);      // Pointer not needed.
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
double a, b, phase;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    HBRUSH hPurpleBrush;
    RECT whole = { 0, 0, 0, 0 };
    LRESULT result;
    switch (message)
    {
    case WM_TIMER:
        int length_a;
        int length_b;
        int length_phase;
        switch (wParam)
        {
        case timer1:
            GetWindowRect(hWnd, &whole);
            whole.right -= whole.left;
            whole.left = 0;
            whole.right /= 1.5;
            whole.bottom -= whole.top;
            whole.top = 0;
            result = RedrawWindow(hWnd, &whole, nullptr, RDW_ERASE | RDW_INVALIDATE );
            length_a = SendMessage(hEditA, EM_GETLINE, 1, (LPARAM)a_text);
            length_b = SendMessage(hEditB, EM_GETLINE, 1, (LPARAM)b_text);
            length_phase = SendMessage(hEditPhase, EM_GETLINE, 1, (LPARAM)phase_text);
            _tcsncpy_s(a_text, a_text, length_a);
            _tcsncpy_s(b_text, b_text, length_b);
            _tcsncpy_s(phase_text, phase_text, length_phase);
            a = _tstof(a_text);
            b = _tstof(b_text);
            phase = _tstof(phase_text);
            oscill.incrementTime(2e-2);
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
        return FillRect((HDC)wParam, &whole, (HBRUSH)(CreateSolidBrush(RGB(0, 0, 0))));
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        int bn = HIWORD(wParam);
    	if (bn == BN_CLICKED)
    	{
            oscill.restart(-a, -b, phase);
    	}

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
        TextOut(hdc, x_coord, y_coord, L"Отклонение левого маятника", 26);
        MoveWindow(hEditA, x_coord, y_coord + 20, whole.right - whole.left - x_coord - 100, 23, false);
        y_coord = (whole.bottom - whole.top) / 10 + 40;
        TextOut(hdc, x_coord, y_coord, L"Отклонение правого маятника", 27);
        MoveWindow(hEditB, x_coord, y_coord + 20, whole.right - whole.left - x_coord - 100, 23, false);
        y_coord = (whole.bottom - whole.top) / 10 + 100;
        TextOut(hdc, x_coord, y_coord, L"Жесткость пружины", 17);
        MoveWindow(hEditPhase, x_coord, y_coord + 20, whole.right - whole.left - x_coord - 100, 23, false);
        MoveWindow(hwndButton, x_coord, y_coord + 60, whole.right - whole.left - x_coord - 100, 23, false);
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
    double x_size = (figure_zone.right - figure_zone.left) / 5;
    double y_size = (figure_zone.bottom - figure_zone.top) / 2.5;

    if (x_size > y_size)
    {
        x_size = y_size;
    }
    else
    {
        y_size = x_size;
    }
    static int last_coord_x = x_size, last_coord_y = y_size;
    Point upRight{x_size * 0.5, 0}, upLeft{x_size*1.5, 0};
    oscill.setParameters(upLeft, upRight, y_size * 1.5 , y_size*0.5, 2);
    HPEN greenRay = CreatePen(PS_SOLID, 1, RGB(110, 255, 0));
    HBRUSH fill = CreateSolidBrush(RGB(0, 0, 0));
    HGDIOBJ hPen = SelectObject(hDC, greenRay), hSBrush = SelectObject(hDC, fill);
    MoveToEx(hDC, upLeft.x, upLeft.y, NULL);
    LineTo(hDC, oscill.getLeft().x, oscill.getLeft().y);
    Ellipse(hDC, oscill.getLeft().x - 20, oscill.getLeft().y - 20, oscill.getLeft().x + 20, oscill.getLeft().y + 20);
    Ellipse(hDC, oscill.getSpringLeft().x - 5, oscill.getSpringLeft().y - 5, oscill.getSpringLeft().x + 5, oscill.getSpringLeft().y + 5);
    DeleteObject(greenRay);
    HPEN redRay = CreatePen(PS_SOLID, 1, RGB(255, 100, 0));
    HGDIOBJ hSPen = SelectObject(hDC, redRay);
    MoveToEx(hDC, upRight.x, upRight.y, NULL);
    LineTo(hDC, oscill.getRight().x, oscill.getRight().y);
    Ellipse(hDC, oscill.getRight().x - 20, oscill.getRight().y - 20, oscill.getRight().x + 20, oscill.getRight().y + 20);
    Ellipse(hDC, oscill.getSpringRight().x - 5, oscill.getSpringRight().y - 5, oscill.getSpringRight().x + 5, oscill.getSpringRight().y + 5);
    DeleteObject(redRay);

    HPEN whiteRay = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    HGDIOBJ hTPen = SelectObject(hDC, whiteRay);
    MoveToEx(hDC, oscill.getSpringRight().x, oscill.getSpringRight().y, NULL);
	
    int counter = 0;
    double j = oscill.getSpringRight().y;
	for (double i = oscill.getSpringRight().x; i <= oscill.getSpringLeft().x; i+=(oscill.getSpringLeft().x - oscill.getSpringRight().x)/20)
	{
       
        if (counter % 2 == 0)
        {
            LineTo(hDC, i, j);
        }
        else
        {
            LineTo(hDC, i, j + 10);
        }
        j += (oscill.getSpringLeft().y - oscill.getSpringRight().y) / 20;
        counter++;
	}
    DeleteObject(whiteRay);
}