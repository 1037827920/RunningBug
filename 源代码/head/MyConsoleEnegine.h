#pragma once 

#include<Windows.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include<string>
#include<chrono>
#include<vector>
#include<utility>
#include<algorithm>
#include<ctime>
#include<queue>
#include<functional>
#include<fstream>
#include<iostream>
#include<thread>
#include<conio.h>

#include"Renderer.h"
#include"Hunter.h"
#include"Player.h"
#include"Math.h"

#define IS_KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)
#define IS_KEY_UP(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?0:1)