#pragma GCC optimize("O2")
#pragma GCC optimize("unroll-loops")
#include <windows.h>
#include <math.h>
#include <string>
#include "conf.h"
#include "resource.h"

HINSTANCE hInstance;
HICON hSmallIcon;
HICON hLargeIcon;
HBITMAP bmp_haruhi;
const char function_list[6][13] = {"階乗","偏差値","標準変量","ポアソン","信頼区間","共振回路"};
int math_mode = 0; /* 計算モード */
std::string window_input = ""; /* 入力 */
double keep_input[5]; /* 引数 */
int curr_input = 0; /* 現示引数 */
std::string ans_text = "モードを選択して下さい"; /* 計算結果 */
static unsigned long long pmt(long long n){
	unsigned long long a = 1;
	for (long long i = n; i > 0; --i){
		a *= i;
	}
	return a;
}
static std::string math_tools(void){
	if (window_input != ""){
		keep_input[curr_input] = to_i(window_input);
		window_input = "";
		curr_input++;
	}
	double e = 2.71828;
	double pi = 3.14159265;
	char file_out[1024];
	int l = curr_input;
	switch (math_mode){
		case 1:
		{
			if (curr_input < 1){
				return "60以下の整数を入力";
			}
			long long n = (long long)keep_input[0];
			if (n > 60){
				curr_input = 0;
				return "計算不能です。";
			}
			unsigned long long a = pmt(n);
			curr_input = 0;
			sprintf(file_out, "階乗: permutation(%lld) = %lld\n", n, a);
			file_put_contents(out_file_name.c_str(), file_out, "ab");
			return std::to_string(n)+"の階乗は"+std::to_string(a)+"です。";
		}
		break;
		case 2:
		{
			if (l < 3){
				switch (l){
					case 0:
					return "平均値を入力";
					case 1:
					return "自分の点数を入力";
					case 2:
					return "標準偏差を入力";
				}
			} else if (l == 3){
				if (keep_input[2] == 0){
					curr_input -= 1;
					return "標準偏差は0ではいけません";
				}
				char args[128];
				char result[256];
				double ave = keep_input[0];
				double my = keep_input[1];
				double d = keep_input[2];
				sprintf(args, "(%.3f, %.3f, %.3f)", ave, my, d);
				sprintf(result, "%.5f", (10*(my - ave) / d) +50);
				sprintf(file_out, "偏差値 Ti(%.5f, %.5f, %.5f) = %.5f\n", ave, my, d, (10*(my - ave) / d) +50);
				file_put_contents(out_file_name.c_str(), file_out, "ab");
				curr_input = 0;
				return std::string(args)+" 偏差値は"+std::string(result)+"です。";
			}
		}
		break;
		case 3:
		{
			if (l < 3){
				switch (l){
					case 0:
					return "平均値を入力";
					case 1:
					return "自分の点数を入力";
					case 2:
					return "標準偏差を入力";
				}
			} else if (l == 3){
				if (keep_input[2] == 0){
					curr_input -= 1;
					return "標準偏差は0じゃダメです";
				}
				char args[128];
				char result[256];
				double ave = keep_input[0];
				double my = keep_input[1];
				double d = keep_input[2];
				sprintf(args, "(%.3f, %.3f, %.3f)", ave, my, d);
				sprintf(result, "%.5f", (my - ave) / d);
				sprintf(file_out, "標準変化量 Zi(%.5f, %.5f, %.5f) = %.5f\n", ave, my, d, (my - ave) / d);
				file_put_contents(out_file_name.c_str(), file_out, "ab");
				curr_input = 0;
				return std::string(args)+" 標準変化量は"+std::string(result)+"ですぅ。";
			}
		}
		break;
		case 4:
		{
			if (l < 2){
				if (l == 0){
					return "Xの値を入力";
				}
				if (l == 1){
					if (keep_input[0] > 60){
						curr_input = 0;
						return "xは60以下で入力して下さい。";
					}
					return "ラムダを入力";
				}
			} else {
				char args[128];
				char result[256];
				long long x = (long long)keep_input[0];
				double ram = keep_input[1];
				sprintf(args, "(%lld, %.3f)", x, ram);
				sprintf(result, "%.10f", pow(ram, x) * (pow(e, -ram) / pmt(x)));
				sprintf(file_out, "ポアソン分布 poason(%lld, %.5f) = %.10f\n", x, ram, pow(ram, x) * (pow(e, -ram) / pmt(x)));
				file_put_contents(out_file_name.c_str(), file_out, "ab");
				curr_input = 0;
				return std::string(args)+" ポアソン分布は"+std::string(result)+"です。";
			}
		}
		break;
		case 5:
		{
			if (l < 4){
				switch (l){
					case 0:
					return "母集団nを入力";
					break;
					case 1:
					if (keep_input[0] < 2){
						curr_input = 0;
						return "母集団は複数必要です。";
					} else if (keep_input[0] < 30){
						return "母集団が小さいのでt分布を使います。平均値を入力";
					} else {
						return "母集団が十分大きいのでz分布を使います。平均値を入力";
					}
					break;
					case 2:
					if (keep_input[0] < 30){
						return "t値を入力";
					} else {
						return "z値を入力";
					}
					break;
					case 3:
					return "シグマを入力";
					break;
				}
			} else {
				char args[128];
				char result[256];
				long long N = keep_input[0];
				double ave = keep_input[1];
				double torz = keep_input[2];
				double sig = keep_input[3];
				double a;
				double b;
				sprintf(args, "(%lld, %.2f, %.2f, %.2f)", N, ave, torz, sig);
				if (N < 30){
					a = ave - torz * sig / pow((N - 1), 0.5);
					b = ave + torz * sig / pow((N - 1), 0.5);
					sprintf(file_out, "信頼区間 t値使用(%lld, %.5f, %.5f, %.5f) = %.5f から %.5f\n", N, ave, torz, sig, a, b);
				} else {
					a = ave - torz * sig / pow(N, 0.5);
					b = ave + torz * sig / pow(N, 0.5);
					sprintf(file_out, "信頼区間 z値使用(%lld, %.5f, %.5f, %.5f) = %.5f から %.5f\n", N, ave, torz, sig, a, b);
				}
				sprintf(result, "%.5f から %.5f", a, b);
				file_put_contents(out_file_name.c_str(), file_out, "ab");
				curr_input = 0;
				return std::string(args)+" 信頼区間は"+std::string(result)+"です。";
			}
		}
		break;
		case 6:
		{
			if (l == 0){
				return "コンデンサの容量を入力(単位はpF)";
			} else if (l == 1){
				if (keep_input[0] <= 0){
					curr_input = 0;
					return "0以上の値を入力して下さい。";
				}
				return "コイルの容量を入力(単位はuH)";
			}
			if (keep_input[1] <= 0){
				curr_input = 1;
				return "0以上の値を入力して下さい。";
			}
			char args[128];
			char result[256];
			double cond = keep_input[0] * pow(10, -12);
			double coil = keep_input[1] * pow(10, -6);
			sprintf(args, "(%.5fpF, %.5fuH)", cond *pow(10, 12), coil *pow(10, 6));
			sprintf(file_out, "共振周波数 f(%.5fpF, %.5fuH) = %.5fkHz\n", cond *pow(10, 12), coil *pow(10, 6), (1/(2*pi*pow(cond * coil, 0.5))/1000));
			sprintf(result, "%.5fkHz", (1/(2*pi*pow(cond * coil, 0.5)))/1000);
			file_put_contents(out_file_name.c_str(), file_out, "ab");
			curr_input = 0;
			return std::string(args)+"共振周波数は"+std::string(result)+"です。";
		}
		break;
		default:
		break;
	}
	curr_input = 0;
	return "モードを選択して下さい。";
}
static void view_his(HWND hwnd){
	FILE *fp = fopen(out_file_name.c_str(), "rb");
	if (fp == NULL){
		MessageBoxW(hwnd, L"履歴がありません。", L"履歴", MB_ICONINFORMATION);
	} else {
		int l = 0;
		char c[1024];
		while (fgets(c, 1024, fp) != NULL){
			l++;
		}
		if (l > 30){
			std::string str;
			rewind(fp);
			for (int i, row = 0; fgets(c, 1024, fp) != NULL; ++row){
				if (row >= l - 30){
					str += c;
					++i;
				}
			}
			fclose(fp);
			file_put_contents(out_file_name.c_str(), str.c_str(), "wb");
		} else {
			fclose(fp);
		}
		MessageBoxW(hwnd, to_wide(file_get_contents(out_file_name.c_str(), "rb")).c_str(), L"履歴", MB_ICONINFORMATION);
	}
}
static void remv_his(HWND hwnd){
	if (MessageBoxW(hwnd, L"履歴を削除しますか?\n30回前のデータは自動的に削除されます。", L"確認", MB_OKCANCEL|MB_ICONQUESTION) == IDOK){
		remove(out_file_name.c_str());
	}
}

/* event */
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	static PrintText input_console(hwnd, 80, 80);
	static PrintText out_console(hwnd, 80, 110);
	static PrintText msg_out(hwnd, WIDTH -500, 380);
	static int haruhip = 0;
	switch (msg){
		case WM_ERASEBKGND:{
			return 0;
		}
		break;
		case WM_RBUTTONDOWN:{
			if (haruhip == 0){
				haruhip = -100;
			} else {
				haruhip = 0;
			}
			SendMessage(hwnd, WM_PAINT, wParam, lParam);
			InvalidateRect(hwnd , NULL , TRUE);
			return 0;
		}
		break;
		/* キー入力 */
		case WM_KEYDOWN:{
			if ((int)wParam >= (int)'0' && (int)wParam <= (int)'9'){
				window_input += wParam;//数字
			} else if ((int)wParam >= 0x60 && (int)wParam <= 0x69){
				window_input += (int)wParam - 0x60 + '0';//テンキー
			} else if ((int)wParam >= 0x70 && (int)wParam <= 0x75){
				math_mode = (int)wParam - 0x70 + 1;//モード
				window_input = "";
				curr_input = 0;
				ans_text = math_tools();
			} else if (wParam == VK_BACK && window_input != ""){
				window_input = window_input.substr(0, window_input.length() -1);//消す
			} else if (wParam == VK_RETURN || wParam == VK_SEPARATOR){
				ans_text = math_tools();//enter
			} else if ((wParam == VK_DECIMAL || wParam == VK_OEM_PERIOD) && window_input.find(".") == std::string::npos){
				window_input += ".";//小数点
			} else if (wParam == VK_OEM_MINUS || wParam == VK_SUBTRACT){
				/* マイナス */
				if (window_input != "" && window_input.substr(0, 1) == "-"){
					window_input = window_input.substr(1, window_input.length());
				} else {
					window_input = "-" + window_input;
				}
			} else if (wParam == 'H'){
				view_his(hwnd);
				return 0;
			} else if (wParam == 'D'){
				remv_his(hwnd);
				return 0;
			} else {
				return 0;
			}
			SendMessage(hwnd, WM_PAINT, wParam, lParam);
			return 0;
		}
		break;
		/* マウス操作 */
		case WM_COMMAND:{
			SetFocus(hwnd);
			int wmId = LOWORD(wParam);
			char c;
			if (wmId >= 100 && wmId < 106){
				math_mode = wmId - 99;
				window_input = "";
				curr_input = 0;
				ans_text = math_tools();
			} else if (wmId > 0 && wmId < 10){
				c = '0' + wmId;
				window_input += c;
			} else {
				switch (wmId){
					case 10:
					window_input += "0";
					break;
					case 11:
					if (window_input != "" && window_input.substr(0, 1) == "-"){
						window_input = window_input.substr(1, window_input.length());
					} else {
						window_input = "-" + window_input;
					}
					break;
					case 12:
					/* 確定キー(入力完了または再入力または計算開始) */
					ans_text = math_tools();
					break;
					case 13:
					if (window_input != ""){
						window_input = window_input.substr(0, window_input.length() -1);
					}
					break;
					case 20:
					if (window_input.find(".") == std::string::npos){
						window_input += ".";
					}
					break;
					case 1000:
					{
						view_his(hwnd);
						return 0;
					}
					break;
					case 1001:
					{
						remv_his(hwnd);
						return 0;
					}
					break;
					default:
					return 0;
					break;
				}
			}
			SendMessage(hwnd, WM_PAINT, wParam, lParam);
			return 0;
		}
		break;
		case WM_CREATE:{
		HFONT hFont = PrintText::set_font(40);
		HFONT hFont2 = PrintText::set_font(18);

		/* mode */
		long long id = 100;
		for (int i = 0; i < 6; ++i, ++id){
			HWND btn = CreateWindowExW(
				WS_EX_APPWINDOW, L"BUTTON", to_wide(function_list[i]).c_str(),
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
				i * 100, 0, 100, 30,
				hwnd, (HMENU)id, hInstance, NULL
			);
			SendMessage(btn, WM_SETFONT, (WPARAM)hFont2, MAKELPARAM(FALSE, 0));
		}

		/* テンキー */
		int i;
		for (id = 1, i = 1; i <= 3; ++i){
			for (int j = 3; j > 0; --j, ++id){
				HWND btn = CreateWindowExW(
					WS_EX_APPWINDOW, L"BUTTON", to_wide(std::to_string(id)).c_str(),
					WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
					WIDTH -j * 80, HEIGHT -(i * 80 + 100), 50, 50,
					hwnd, (HMENU)id, hInstance, NULL
				);
				SendMessage(btn, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0));
			}
		}
		HWND btn = CreateWindowExW(
			WS_EX_APPWINDOW, L"BUTTON", L"0",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			WIDTH -240, HEIGHT -100, 50, 50,
			hwnd, (HMENU)10, hInstance, NULL
		);
		SendMessage(btn, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0));
		btn = CreateWindowExW(
			WS_EX_APPWINDOW, L"BUTTON", L"-",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			WIDTH -160, HEIGHT -100, 50, 50,
			hwnd, (HMENU)11, hInstance, NULL
		);
		SendMessage(btn, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0));
		btn = CreateWindowExW(
			WS_EX_APPWINDOW, L"BUTTON", L"R",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			WIDTH -80, HEIGHT -100, 50, 50,
			hwnd, (HMENU)12, hInstance, NULL
		);
		SendMessage(btn, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0));
		btn = CreateWindowExW(
			WS_EX_APPWINDOW, L"BUTTON", L".",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			WIDTH -320, HEIGHT -100, 50, 50,
			hwnd, (HMENU)20, hInstance, NULL
		);
		SendMessage(btn, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0));
		btn = CreateWindowExW(
			WS_EX_APPWINDOW, L"BUTTON", L"Back",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			200, HEIGHT -80, 80, 30,
			hwnd, (HMENU)13, hInstance, NULL
		);
		SendMessage(btn, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0));
		btn = CreateWindowExW(
			WS_EX_APPWINDOW, L"BUTTON", L"履歴",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			100, HEIGHT -90, 80, 40,
			hwnd, (HMENU)1000, hInstance, NULL
		);
		SendMessage(btn, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0));
		btn = CreateWindowExW(
			WS_EX_APPWINDOW, L"BUTTON", L"消去",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			10, HEIGHT -90, 80, 40,
			hwnd, (HMENU)1001, hInstance, NULL
		);
		SendMessage(btn, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0));
		}
		break;
		case WM_PAINT:{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd , &ps);
			HDC buffer_haruhi = CreateCompatibleDC(hdc);
			HBITMAP oldbit = (HBITMAP)SelectObject(buffer_haruhi, bmp_haruhi);
			BitBlt(hdc , WIDTH -300 -haruhip, HEIGHT -330, WIDTH, HEIGHT, buffer_haruhi, 0, 0 , SRCCOPY);
			DeleteDC(buffer_haruhi);
			DrawIcon(hdc, WIDTH -40, 5, hLargeIcon);
			SelectObject(hdc, oldbit);
			DeleteObject(buffer_haruhi);
			EndPaint(hwnd, &ps);
			ReleaseDC(hwnd, hdc);
			print_text(hwnd, "入力欄:", 10, 80, 20, RGB(0,0,0), 0);
			print_text(hwnd, "出力欄:", 10, 110, 20, RGB(0,0,0), 0);
			print_text(hwnd, title_text, 0, 30, 25, RGB(255,255,255), RGB(200,50,20));
			print_text(hwnd, "使い方", 10, 200, 20, RGB(0,100,0), 0);
			print_text(hwnd, "F1からF6キーがモード切替に対応。", 10, 220, 20, RGB(0,100,0), 0);
			print_text(hwnd, "EnterキーかRボタン: 決定・表示", 10, 240, 20, RGB(0,100,0), 0);
			print_text(hwnd, "履歴: Hキー、履歴削除: Dキー", 10, 260, 20, RGB(0,100,0), 0);
			print_text(hwnd, "計算結果はファイルに保存されます。", 10, 280, 20, RGB(0,100,0), 0);
			print_text(hwnd, "終了: Alt+F4", 10, 300, 20, RGB(0,100,0), 0);
			print_text(hwnd, copy_right_text, 10, HEIGHT -50, 12, RGB(0, 0, 0), 0);
			if (math_mode != 0){
				msg_out.print(std::string(function_list[math_mode-1])+"を計算するわよ！",20,RGB(255,255,255),RGB(0,0,150));
			} else {
				msg_out.print("モードを選択して下さい", 20, RGB(255,255,255), RGB(220,0,0));
			}
			if (window_input.length() > 20){
				window_input = window_input.substr(0, window_input.length() -1);
				MessageBoxW(hwnd, L"桁数が大き過ぎです。20桁以下にして下さい。", L"警告", MB_ICONINFORMATION);
			}
			input_console.print(window_input, 20, RGB(0,0,0), 0);
			out_console.print(ans_text, 20, RGB(0,0,0), 0);
		}
		break;

		case WM_DESTROY:{
		PostQuitMessage(0);
		return 0;
		}
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

/* main */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
	HANDLE hMutex = CreateMutexW(NULL, TRUE, L"KIT-SOS_dan_math_tools_gui3.2.0");
	if (GetLastError() == ERROR_ALREADY_EXISTS){
		return 0;
	}
	//UTF8(標準入出力でしか使えない)
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	//やむを得ずshif-jisになる文字列もある
	switch_acp();

	FILE *fp = fopen(function_file_name.c_str(), "r");
	if (fp == NULL){
		file_put_contents(function_file_name.c_str(), "階乗: a=1,nからn=1までaに乗算\n偏差値: (10*(my-ave)/d)+50\n標準変化量: (my-ave)/d\nポアソン: ram^x*e^-ram/!x\n信頼区間(t): ave+-t*sig/(N - 1)^0.5\n信頼区間(z): ave+-z*sig/N^0.5\n共振周波数: 1/(2*pi*√C*L)\n\ne=2.71828\npi=3.14159265\n", "wb");
	} else {
		fclose(fp);
	}

	const char CLASS_NAME[] = "main";
	WNDCLASSA wc = {0};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255,255,255));
	RegisterClassA(&wc);
	HWND hwnd = CreateWindowExA(
		WS_EX_APPWINDOW, CLASS_NAME, TITLE.c_str(),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		//WS_OVERLAPPEDWINDOW|WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,// 初期位置
		WIDTH, HEIGHT,	// 初期サイズ
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (hwnd == NULL){
		return 0;
	}
	hSmallIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_APP_ICON), IMAGE_ICON, 16, 16, 0);
	hLargeIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_APP_ICON), IMAGE_ICON, 32, 32, 0);
	bmp_haruhi = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(HARUHI_IMG), IMAGE_BITMAP, 0, 0, 0);
	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hSmallIcon);
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hLargeIcon);
	ShowWindow(hwnd, nShowCmd);
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
	return 0;
}

