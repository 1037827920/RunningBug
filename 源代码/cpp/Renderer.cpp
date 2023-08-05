#include"MyConsoleEnegine.h"

Renderer::Renderer()
{
	p_console_window_title = new std::string;
}
Renderer::~Renderer()
{
	delete p_console_window_title;
}



//初始化
void Renderer::Init(SHORT screen_height, SHORT screen_width)
{
	//创建两个屏幕缓冲
	h_screen_buffer1 = CreateConsoleScreenBuffer(
		GENERIC_WRITE | GENERIC_READ,//定义进程可以往缓冲区读写数据
		FILE_SHARE_WRITE | FILE_SHARE_READ,//定义缓冲区可共享读写权限
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);
	h_screen_buffer2 = CreateConsoleScreenBuffer(
		GENERIC_WRITE | GENERIC_READ,//定义进程可以往缓冲区读写数据
		FILE_SHARE_WRITE | FILE_SHARE_READ,//定义缓冲区可共享读写权限
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);
	h_p_output = &h_screen_buffer1;
	choose_buffer = true;

	//隐藏两个缓冲区的光标
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = false;
	cci.dwSize = 1;
	SetConsoleCursorInfo(h_screen_buffer1, &cci);
	SetConsoleCursorInfo(h_screen_buffer2, &cci);

	//设置窗口和缓冲区为固定大小  宽 177    高  53
	COORD coord = { screen_width,screen_height };
	SetConsoleScreenBufferSize(h_screen_buffer1, coord);
	SetConsoleScreenBufferSize(h_screen_buffer2, coord);

	//设置控制台编码为UTF-8;
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	//设置字体大小为 16   设置字体类型为Consolas
	CONSOLE_FONT_INFOEX cfi = { 0 };
	cfi.cbSize = sizeof(cfi);//没有啥用但是必不可少
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 16;//设置字体大小
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;//字体粗细
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(h_screen_buffer1, FALSE, &cfi);
	SetCurrentConsoleFontEx(h_screen_buffer2, FALSE, &cfi);
}
//设置创建标题
void Renderer::SetWindowTitle(const char* title)
{
	SetConsoleTitleA(title);
	*p_console_window_title = title;
}
//切换缓冲区
void Renderer::SwitchBuffer()
{
	choose_buffer = !choose_buffer;
	if (choose_buffer)
		h_p_output = &h_screen_buffer1;
	else
		h_p_output = &h_screen_buffer2;
	SetConsoleActiveScreenBuffer(*h_p_output);
}