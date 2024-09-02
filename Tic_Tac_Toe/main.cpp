#include<graphics.h>
#include<Windows.h>
#include<chrono>
#include<iostream>
#include<conio.h>

const int remain_time = 1000 / 144;

void drawWall();
void drawInformation();
void drawX(int i, int j);
void drawO(int i, int j);
void drawChess();
void checkMouseLocation(ExMessage& msg, bool& flag, int& i, int& j);
void checkEvent();
bool checkFlagWin(int flag);
void checkWin();
TCHAR* transform(const char* s);
TCHAR* transform(std::string s);
void restart();

int chess[3][3] = { 0 };
char player_now = 'O';
bool playing = true;
bool dogfall = false;

int main()
{
	initgraph(900, 600);
	BeginBatchDraw();

	while (1)
	{
		auto start_time = std::chrono::system_clock::now();

		checkEvent();
		checkWin();

		cleardevice();
		drawWall();
		drawInformation();
		drawChess();
		FlushBatchDraw();

		auto end_time = std::chrono::system_clock::now();

		//控制帧率，使用起来会卡。
		//帧率太低，改高会好很多。
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
		if (duration.count() < remain_time)
		{
			Sleep(remain_time - duration.count());
		}
		//std::cout << remain_time - duration.count() << std::endl;

		//控制帧率，使用效果跟不使用差不多。
		/*auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
		if (duration.count() < remain_time)
		{
			auto begin = std::chrono::system_clock::now();
			std::chrono::system_clock::time_point now_time = std::chrono::system_clock::now();
			while (std::chrono::duration_cast<std::chrono::microseconds>(now_time - begin).count() < remain_time - duration.count())
			{
				now_time = std::chrono::system_clock::now();
			}
			std::cout << "in" << std::chrono::duration_cast<std::chrono::microseconds>(now_time - begin).count() << std::endl;
		}*/
		//std::cout << "out" << duration.count() << std::endl;
	}
	EndBatchDraw();
	_getch();
	closegraph();
}
void checkWin()
{
	if (checkFlagWin(1))
	{
		playing = false;
		//OWin
	}
	else if (checkFlagWin(-1))
	{
		playing = false;
		//XWin
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (chess[i][j] == 0)
				{
					return;
				}
			}
		}
		playing = false;
		dogfall = true;
	}
}
bool checkFlagWin(int flag)
{
	if (chess[0][0] == flag && chess[0][1] == flag && chess[0][2] == flag)
	{
		return true;
	}
	if (chess[1][0] == flag && chess[1][1] == flag && chess[1][2] == flag)
	{
		return true;
	}
	if (chess[2][0] == flag && chess[2][1] == flag && chess[2][2] == flag)
	{
		return true;
	}
	if (chess[0][0] == flag && chess[1][0] == flag && chess[2][0] == flag)
	{
		return true;
	}
	if (chess[0][1] == flag && chess[1][1] == flag && chess[2][1] == flag)
	{
		return true;
	}
	if (chess[0][2] == flag && chess[1][2] == flag && chess[2][2] == flag)
	{
		return true;
	}
	if (chess[0][0] == flag && chess[1][1] == flag && chess[2][2] == flag)
	{
		return true;
	}
	if (chess[0][2] == flag && chess[1][1] == flag && chess[2][0] == flag)
	{
		return true;
	}
	return false;
}
void drawInformation()
{
	if (playing)
	{
		TCHAR str[12];
		swprintf_s(str, _T("player:'%c'"), player_now);
		outtextxy(700, 100, str);
	}
	else
	{
		if (dogfall)
		{
			TCHAR* s1 = transform("dogfall");
			TCHAR* s2 = transform("'r' to restart");
			outtextxy(700, 100, s1);
			outtextxy(700, 120, s2);
			delete[] s1;
			delete[] s2;
		}
		else {
			char player;
			if (player_now == 'O')
			{
				player = 'X';
			}
			else
			{
				player = 'O';
			}
			TCHAR* s1 = transform("Game Over");
			TCHAR s2[12];
			swprintf_s(s2, _T("Winner:'%c'"), player);
			TCHAR* s3 = transform("'r' to restart");
			outtextxy(700, 100, s1);
			outtextxy(700, 120, s2);
			outtextxy(700, 140, s3);
			delete[] s1;
			delete[] s3;
		}
	}
}
void drawWall()
{
	line(200, 0, 200, 600);
	line(400, 0, 400, 600);
	line(0, 200, 600, 200);
	line(0, 400, 600, 400);
	line(600, 0, 600, 600);
}
void checkEvent()
{
	ExMessage msg;
	if (peekmessage(&msg))
	{
		if (playing == true && msg.message == WM_LBUTTONDOWN)
		{
			bool flag = true;
			int i, j;
			checkMouseLocation(msg, flag, i, j);
			if (flag)
			{
				if (chess[i][j] == 0 && player_now == 'O')
				{
					chess[i][j] = 1;
					player_now = 'X';
				}
				else if (chess[i][j] == 0 && player_now == 'X')
				{
					chess[i][j] = -1;
					player_now = 'O';
				}
			}
		}
		if (playing == false && msg.message == WM_KEYDOWN)
		{
			if (msg.vkcode == 0x52)//r
			{
				restart();
			}
		}
		if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE)//esc
		{
			exit(0);
		}
	}

}
void restart()
{
	player_now = 'O';
	playing = true;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			chess[i][j] = 0;
		}
	}
}
void checkMouseLocation(ExMessage& msg, bool& flag, int& i, int& j)
{
	if (msg.x > 0 && msg.x < 200 && msg.y>0 && msg.y < 200)
	{
		i = 0; j = 0;
	}
	else if (msg.x > 200 && msg.x < 400 && msg.y>0 && msg.y < 200)
	{
		i = 0; j = 1;
	}
	else if (msg.x > 400 && msg.x < 600 && msg.y>0 && msg.y < 200)
	{
		i = 0; j = 2;
	}
	else if (msg.x > 0 && msg.x < 200 && msg.y>200 && msg.y < 400)
	{
		i = 1; j = 0;
	}
	else if (msg.x > 200 && msg.x < 400 && msg.y>200 && msg.y < 400)
	{
		i = 1; j = 1;
	}
	else if (msg.x > 400 && msg.x < 600 && msg.y>200 && msg.y < 400)
	{
		i = 1; j = 2;
	}
	else if (msg.x > 0 && msg.x < 200 && msg.y>400 && msg.y < 600)
	{
		i = 2; j = 0;
	}
	else if (msg.x > 200 && msg.x < 400 && msg.y>400 && msg.y < 600)
	{
		i = 2; j = 1;
	}
	else if (msg.x > 400 && msg.x < 600 && msg.y>400 && msg.y < 600)
	{
		i = 2; j = 2;
	}
	else
	{
		flag = false;
	}
}
void drawChess()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (chess[i][j] == 1)
			{
				drawO(i, j);
			}
			else if (chess[i][j] == -1)
			{
				drawX(i, j);
			}
		}
	}
}
void drawO(int i, int j)
{
	int y = i * 200 + 100;
	int x = j * 200 + 100;
	circle(x, y, 100);
}
void drawX(int i, int j)
{
	int y = i * 200 + 100;
	int x = j * 200 + 100;
	line(x - 100, y - 100, x + 100, y + 100);
	line(x + 100, y - 100, x - 100, y + 100);
}
TCHAR* transform(std::string s)
{
	int size = s.size() + 1;
	TCHAR* ans = new TCHAR[size];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, ans, size);
	return ans;
}
TCHAR* transform(const char* s)
{
	int size = MultiByteToWideChar(CP_ACP, 0, s, -1, NULL, 0);
	TCHAR* ans = new TCHAR[size];
	MultiByteToWideChar(CP_ACP, 0, s, -1, ans, size);
	return ans;
}