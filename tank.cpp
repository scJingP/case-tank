#include "tank.h"
#include "ConOperator.h"
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <iostream>


using namespace std;


TankGame::TankGame(int w, int h)
{
	// 设定当前关数
	no = 1;

	// 设定游戏整体高宽
	wide = w;
	high = h;

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD sizePos = {2 * wide, high};
	SetConsoleScreenBufferSize(hStdOut, sizePos);

	// 分配游戏空间状态表
	gameSpace = new GameSpace*[high];
	for (int i = 0; i != high; i++)
		gameSpace[i] = new GameSpace[wide];

	// 添加围墙
	wall = new Wall(gameSpace, wide, high);
	wall->draw();

}

void TankGame::load_init()
{
	// 初始化游戏空间
	for (int y = 1; y != high-1; y++) {
		for (int x = 1; x != wide-1; x++) {
			gameSpace[y][x].have = false;
			gameSpace[y][x].kind = 0;
			gameSpace[y][x].no = 0;
		}
	}

	// 添加地图
	map.load(gameSpace, no);
	
	// 初始化自己坦克
	me.init_xy();
	me.trans_direction(1);
	me.set_no(0);
	me.init_move(gameSpace, wide / 2 - 2, high - 5);

	// 按顺序诞生敌人坦克
	creatPlace = 0;
	for (int i = 0; i != 3; i++) {
		freezeTime[i] = REBIRTHTIME;
		enemy[i].init_xy();
		enemy[i].set_kind(2);
		enemy[i].set_no(i+1);
		reset(enemy[i]);
	}
	// 数目初始化
	leftCount = 17;
	existCount = 3;

	// 显示图片	
	me.draw_tank();
	for (int i = 0; i != 3; i++)
		enemy[i].draw_tank();
	map.draw();
}

TankGame::~TankGame()
{
	if (wall)
		delete wall;

	if (gameSpace) {
		for (int i = 0; i != high; i++)
			delete[] gameSpace[i];
		delete[] gameSpace;
	}
}

void TankGame::reset(Tank& tank)
{
	
	bool success = false;
	int t = 3;
	
	while (t && !success) {		
		creatPlace++;
		if (creatPlace == 4)
			creatPlace = 1;

		// 诞生地点选择
		switch (creatPlace) {
		case 1:
			tank.trans_direction(3);     //朝下
			if (tank.reset(gameSpace,1,1)) 
				success = true;
			break;
		case 2:
			tank.trans_direction(3);
			if (tank.reset(gameSpace,wide / 2 - 2, 1)) 
				success = true;
			break;
		case 3:
			tank.trans_direction(3);
			if (tank.reset(gameSpace,wide - 4, 1))
				success = true;
			break;
		}

		if (!success)
			t--;
	}
}


void TankGame::start()
{
	char command;
	enter_picture(command);

	if (command != ENTER)
		return;

	while (me.blood > 0) {
		// init data for runing
		load_init();
		// run a task
		runing_a_task();

		// determine why return
		if (me.blood > 0) {
			no++; 
			if (no > map.total) {
				success_gameover();
				break;
			}

			char command;
			turn_to_next_task(command);
			if (command != ENTER)
				break;
		} 
		else game_over();
	}
}

//void

void TankGame::runing_a_task()
{
	while ((leftCount || existCount) && me.blood) {
		
		clock_t now = clock();
		
		char command = -1;                              // 主人命令
		while (clock() - now < TIME_UNIT)              // 小于一个时间片，暂停
			update_keyBoard_char(command);
		
		/*更新敌人坦克*/
		for (int i = 0; i != 3; i++) {
			if (enemy[i].blood == 0)
				continue;
			if (enemy[i].freezeTime == 0) {					
				enemy[i].rand_direction(gameSpace);     // 产生随机方向
			} 
			else {
				if (enemy[i].speed == 0) {
					enemy[i].move(gameSpace);
					if (bullet[i+1].disapper) {
						int shot = -1;
						bullet[i+1] = enemy[i].rand_shot(gameSpace,i+1, shot);
						if (shot == 0)
							me.be_shot(gameSpace);
						bullet[i+1].registration(gameSpace);
					}
					enemy[i].freezeTime--;
				} else {
					enemy[i].speed--;
				}
			}
		}
		
		/*更新自己坦克*/
		if (command >=0 && command < 4) {
			me.trans_to_direction(command);
			me.move(gameSpace);
		}
		
		/*更新子弹*/
		for (int i = 0; i !=4; i++) {		
			if (!bullet[i].disapper) {              // 存在的子弹
				int shot;
				bullet[i].move(gameSpace, shot);    // 运行，是否射中
				if (shot == 0)
					me.be_shot(gameSpace);
				else if (shot >= 1 && shot <= 3) {
					enemy[shot-1].be_shot(gameSpace);
					existCount--;
				} else if (shot >= 7 && shot <= 10) {
					bullet[shot-7].unregistration(gameSpace);
					bullet[shot-7].clear();
					bullet[shot-7].init(bullet[shot-7].kind);
				}
			}
		}	
		/*自己产生子弹*/
		if (bullet[0].disapper) {
			if (command == 32) {
				int shot;
				bullet[0] = me.make_bullet(gameSpace,0, shot);
				if (shot >= 1 && shot <= 3) {
					enemy[shot-1].be_shot(gameSpace);
					existCount--;
				}
				bullet[0].registration(gameSpace);
			}
		}
		
		/*诞生缺损敌人*/
		if (leftCount > 0) {
			for (int i = 0; i != 3; i++) {
				if (enemy[i].blood == 0) {
					if (freezeTime[i] == 0) {
						freezeTime[i] = REBIRTHTIME;
						reset(enemy[i]);
						existCount++;
						leftCount--;
					} 
					else freezeTime[i]--;
				}
			}
		}
	}	
}


void TankGame::update_keyBoard_char(char& c)
{
	if (_kbhit()) {
		c = getch();
		if (c == -32) {
			c = getch();
			if (c != UP && c != DOWN && c != LEFT && c != RIGHT)
				c = -1;
			else if (c == RIGHT)
				c = 0;
			else if (c == UP)
				c = 1;
			else if (c == LEFT)
				c = 2;
			else
				c = 3;
		} 
		else
			c = 32;
	}
}


void TankGame::enter_picture(char &command)
{
	string captions[6];
	captions[0] = "◥◣        ◢◤ ■■■■■  ◥◣        ◢◤";      
    captions[1] = "  ◥◣    ◢◤        ◢◤     ◥◣    ◢◤  ";   
    captions[2] = "    ◥◣◢◤        ◢◤         ◥◣◢◤    ";       
    captions[3] = "      ◢◤        ◢◤           ◢◤◥◣    ";
    captions[4] = "    ◢◤        ◢◤           ◢◤    ◥◣  ";
    captions[5] = "  ◢◤          ■■■■■   ◢◤        ◥◣";
	string s =    "                                             ";

	color(0x0e);
	for (int i = 0; i != 6; i++)
		grid(7, 6 + i, captions[i]);

    Frame frame(30,15);
	frame.draw_frame(4,19,0x09);

	string sentence[3];
	sentence[0] = "This small game is made by yzx in 2010.";
	sentence[1] = "Wlecome to play it.";
	sentence[2] = "All Rights Reserved.";

	color(0x0e);
	for (int i = 0; i != 3; i++) {
		for (int k = 0; k != sentence[i].size(); k++) {
			grid(14+k,21+2*i,sentence[i][k]);
			sleep(50);			
		}		
		sleep(1000);
	}
	
	grid(7, 28,"Enter : 开始              ESC : 退出");
	
	
	while (true) {
		if (_kbhit()) {
			command = getch();
			if (command == ENTER || command == ESC)
				break;
		}		
	}

    // clear screen picture in wall
	clear_picture();	
}

void TankGame::turn_to_next_task(char& command)
{
	clear_picture();
	color(0x0e);
	grid(18, 20, "第");
	grid(20, 20, no);
	grid(22, 20, "关");

	while (true) {
		if (_kbhit()) {
			command = getch();
			if (command == ENTER || command == ESC)
				break;
		}		
	}

	grid(18, 20,"                  ");
}

void TankGame::success_gameover()
{
	clear_picture();

	Frame frame(30,15);
	frame.draw_frame(4,10,0x09);

	string sentence[3];
	sentence[0] = "Congratulations for Successfully finishing all tasks";
	sentence[1] = "Thank you for using it deeply.";
	sentence[2] = "And welcome to use it next time.";	

	color(0x0e);
	for (int i = 0; i != 3; i++) {
		for (int k = 0; k != sentence[i].size(); k++) {
			grid(12+k,14+2*i,sentence[i][k]);
			sleep(50);			
		}		
		sleep(1000);
	}
	
	while (true) {
		if (_kbhit()) {
			char command = getch();
			if (command == ENTER || command == ESC)
				break;
		}		
	}
}

void TankGame::game_over()
{
	clear_picture();

	Frame frame(30,15);
	frame.draw_frame(4,15,0x09);

	string sentence[4];
	sentence[0] = "Game over, but you can try it again.";
	sentence[1] = "Thank you for using it deeply.";
	sentence[2] = "And welcome to use it next time.";
	sentence[3] = "All rights reserved.(Violators will be prosecuted.) .";

	color(0x0e);
	for (int i = 0; i != 4; i++) {
		for (int k = 0; k != sentence[i].size(); k++) {
			grid(13+k,18+2*i,sentence[i][k]);
			sleep(50);			
		}		
		sleep(1000);
	}

	while (true) {
		if (_kbhit()) {
			char command = getch();
			if (command == ENTER || command == ESC)
				break;
		}		
	}
}

void TankGame::clear_picture()
{
	int cirX = wide / 2;
	int cirY = high / 2;
	int depth = cirY > cirX ? cirY + 2 : cirX + 2;
	int lx, rx, uy, by;
	int l, r, u, b;
	for (int i =  0; i != depth; i++) {
		lx = cirX - i;
		rx = cirX + i;
		uy = cirY - i;
		by = cirY + i;
		
		uy <= 0 ? u = 1 : u = uy;
		by >= high - 1 ? b = high - 2 :	b = by;
		lx <= 0 ? l = 1 : l = lx;
		rx >= wide -1 ? r = wide - 2 : r = rx;
		
		if (rx < wide - 1) {
			for (int k = u; k <= b; k++)
				grid(rx, k, "  ");
		}

		if (lx > 0) {
			for (int k = u; k <= b; k++)
				grid(lx, k, "  ");
		}

		if (by < high - 1) {
			for (int k = l; k <= r; k++)
				grid(k, by, "  ");
		}

		if (uy > 0) {
			for (int k = l; k <= r; k++)
				grid(k, uy, "  ");
		}
	}
}



void TankGame::test()
{	
	for (int y = 0; y != 40; y++) {
		for (int x = 0; x != 40; x++) {
			if (gameSpace[y][x].have)// && gameSpace[y][x].kind == 3)

				grid(x, 42+y, (char)gameSpace[y][x].no);
			else
				grid(x, 42+y, "  ");
		}
	}
}
