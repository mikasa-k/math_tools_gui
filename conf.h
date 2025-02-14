/* 諸設定と関数定義 */


//画面
int WIDTH = 645;
int HEIGHT = 510;

//文字列
std::string TITLE = "計算ツール SOS団北見工大支部";
const std::string title_text = "計算ツール SOS団北見工業大学支部の学習支援ソフト";
const std::string copy_right_text = "2025 KIT-SOS soft. Written by Misaki.";
std::string out_file_name = "計算結果_KIT_SOS.txt";
std::string function_file_name = "公式集.txt";

/* ファイル操作 */
std::string file_get_contents(const char* file_name, const char* mode){
	FILE* file = fopen(file_name, mode);
	if (file == NULL){
		return "";
	}
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	rewind(file);
	std::string content;
	content.resize(file_size);
	size_t read_size = fread(&content[0], 1, file_size, file);
	if (read_size != (size_t)file_size){
		perror("read?");
		fclose(file);
		return "";
	}
	fclose(file);
	return content;
}
int file_put_contents(const char * file_name, const char * contents, const char * mode){
	FILE* file = fopen(file_name, mode);
	if (file == NULL){
		return 1;
	}
	fprintf(file, contents);
	rewind(file);
	fclose(file);
	return 0;
}
/* 文字列 */
double to_i(std::string str){
	double result = 0;
	/* 符号の検出 */
	int hugou = 1;
	if (str.at(0) == '-'){
		hugou = -1;
		str = str.substr(1);
	}
	/* 小数点有無 */
	int start = 0; //通常は10^0から開始
	int n = str.length();
	for (int i = 0; i < n; ++i){
		char c = str.at(i);
		if (c == '.'){
			start = -1 * str.substr(i+1).length();
			str = str.substr(0, i) + str.substr(i+1);
			n -= 1;
			break;
		}
	}
	for (int i = 1; i <= n; ++i){
		char c = str.at(n - i);
		int s = c - '0';
		result += s * pow(10, start);
		++start;
	}
	return hugou * result;
}
std::string to_sjis(const std::string& utf8){
	int utf16_length = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
	if (utf16_length == 0){
		perror("failed change1");
	}
	std::wstring utf16(utf16_length, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &utf16[0], utf16_length);
	utf16.resize(utf16_length - 1);
	int sjis_length = WideCharToMultiByte(CP_ACP, 0, utf16.c_str(), -1, NULL, 0, NULL, NULL);
	if (sjis_length == 0){
		perror("failed change2");
	}
	std::string sjis(sjis_length, '\0');
	WideCharToMultiByte(CP_ACP, 0, utf16.c_str(), -1, &sjis[0], sjis_length, NULL, NULL);
	sjis.resize(sjis_length - 1);
	return sjis;
}
std::wstring to_wide(const std::string& str){
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], size_needed);
	return wstr;
}

//描画ツール
class PrintText{
	private:
	HWND hwnd;
	std::string str;
	RECT rect;
	int x, y, fs, fg, bg;
	public:
	PrintText(HWND hwnd, int w, int h){
		this->hwnd = hwnd;
		this->x = w;
		this->y = h;
		this->str = "";
		this->rect = {0, 0, 0, 0};
	}
	static HFONT set_font(int font_size){
		return CreateFontW(
		font_size,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE,L"Arial");
	}
	void print(std::string string, int font_size, int font_color, int background_color){
		HDC hdc = GetDC(this->hwnd);
		if (string != this->str || font_size != this->fs || font_color != this->fg || background_color != this->bg){
			FillRect(hdc, &this->rect, (HBRUSH)(COLOR_WINDOW + 1));
		}
		SetTextColor(hdc, font_color);
		if (background_color == 0){
			SetBkMode(hdc, TRANSPARENT);
		} else {
			SetBkColor(hdc, background_color);
		}
		std::wstring s = to_wide(string);
		int n = s.length();
		HFONT hFont = this->set_font(font_size);
		SelectObject(hdc, hFont);
		TextOutW(hdc, this->x, this->y, s.c_str(), n);
		this->str = string;
		this->rect = {this->x, this->y, this->x + n * font_size, this->y + font_size};
		this->fs = font_size;
		this->fg = font_color;
		this->bg = background_color;
		ReleaseDC(this->hwnd, hdc);
		DeleteObject(hFont);
	}
};
void print_text(HWND hwnd, std::string str, int x, int y, int fsize, int fg, int bg){
	HDC hdc = GetDC(hwnd);
	SetTextColor(hdc, fg);
	if (bg == 0){
		SetBkMode(hdc, TRANSPARENT);
	} else {
		SetBkColor(hdc, bg);
	}
	int n = to_wide(str).length();
	HFONT hFont = PrintText::set_font(fsize);
	SelectObject(hdc, hFont);
	TextOutW(hdc, x, y, to_wide(str).c_str(), n);
	ReleaseDC(hwnd, hdc);
}


//文字化け防止
void switch_acp(){
	if (GetACP() != 65001){
		if (GetACP() == 932){
			TITLE = to_sjis(TITLE);
			out_file_name = to_sjis(out_file_name);
			function_file_name = to_sjis(function_file_name);
		} else {
			TITLE = "Math tools in KIT-SOS dan!";
			out_file_name = "result_KIT_SOS.txt";
			function_file_name = "function.txt";
		}
	}
}

