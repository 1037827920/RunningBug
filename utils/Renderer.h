#pragma once

class Renderer
{
public:
	//创建双缓冲句柄
	HANDLE h_screen_buffer1;
	HANDLE h_screen_buffer2;
	//显示指针
	HANDLE* h_p_output;
	bool choose_buffer;
	//窗口标题
	std::string* p_console_window_title;

public:
	Renderer();
	~Renderer();
	//初始化
	void Init(SHORT screen_width, SHORT screen_height);
	//设置创建标题
	void SetWindowTitle(const char* title);
	//切换缓冲区
	void SwitchBuffer();
};