#ifndef TANK_H_GUARD
#define TANK_H_GUARD

#include "element_dataStruct.h"
#include <vector>

#define TIME_UNIT          100
#define REBIRTHTIME        5



#define UP                 72
#define DOWN               80
#define LEFT               75
#define RIGHT              77
#define ESC                27
#define ENTER              13


class TankGame {
public:
	TankGame(int=40,int=40);
	~TankGame();
	void start();

	void test();
private:
	void enter_picture(char&);
	void success_gameover();
	void game_over();
	void turn_to_next_task(char&);
	void runing_a_task();
	void load_init();
	void clear_picture();


	void move_tank(int x, int y);
	void reset(Tank&);
	void update_keyBoard_char(char&);

	int wide;
	int high;
	GameSpace** gameSpace;

	int no;                       // ����

	int deadCount;                // ����̹��
	Map map;                      // ��ͼ
	Wall* wall;                   // �߿�
	Bullet bullet[4];             // �ӵ���¼

	int creatPlace;               // ���˵���λ��
	int freezeTime[3];            // ���˵���ʱ��

	int existCount;               // ���˴�����
	int leftCount;                // ʣ�µ�����

	Tank enemy[3];                // ����̹��
	Tank me;                      // �Լ�̹��
};


#endif