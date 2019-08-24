#include "element_dataStruct.h"
#include "ConOperator.h"
#include <string>
#include <iostream>
#include <windows.h>

using namespace std;

/*
 *
 * following defination is from class Grid
 *
 */
Grid::Grid() 
{
	x = -1; 
	y = -1;
}

Grid::Grid(Grid& g)
{
	x = g.x;
	y = g.y;
}

Grid::Grid(int _x, int _y)
{
	x = _x;
	y = _y;
}

Grid& Grid::operator=(Grid& grid)
{
	x = grid.x;
	y = grid.y;
	return *this;
}

bool Grid::operator==(Grid& grid)
{
	return (x == grid.x) && (y == grid.y);
}

bool Grid::operator!=(Grid& grid)
{
	return !(*this == grid);
}

/*
 *
 * following defination is from class Wall
 *
 */
Wall::Wall(GameSpace** & gs, int w, int h, string b)
{
	wide = w;
	high = h;
	_x = 0;
	_y = 0;

	string brick = b.substr(0, 2);

	upLeftBrick = brick;
	upRightBrick = brick;
	bottomLeftBrick = brick;
	bottomRightBrick = brick;
	horizonBrick = brick;
	verticalBrick = brick;
	
	for (int i = 0; i != wide; i++) {
		gs[0][i] = GameSpace(-1,-1,true);
		gs[high-1][i] = GameSpace(-1,-1,true);
	}
	
	for (int i = 1; i != high - 1; i++) {
		gs[i][0] = GameSpace(-1,-1,true);
		gs[i][wide-1] = GameSpace(-1,-1,true);
	}
}

void Wall::set_brick(std::string b)
{
	string brick = b.substr(0, 2);

	upLeftBrick = brick;
	upRightBrick = brick;
	bottomLeftBrick = brick;
	bottomRightBrick = brick;
	horizonBrick = brick;
	verticalBrick = brick;
}

void Wall::set_brick(string ulb,string urb,string blb,\
					 string brb,string hb,string vb)
{
	upLeftBrick = ulb.substr(0,2);
	upRightBrick = urb.substr(0,2);
	bottomLeftBrick = blb.substr(0,2);
	bottomRightBrick = brb.substr(0,2);
	horizonBrick = hb;
	verticalBrick = vb;
}


void Wall::draw(int x,int y)
{
	_x = x;
	_y = y;
	color(WALLCOL);
	grid(0+x,0+y,upLeftBrick);
	grid(wide - 1+x,0+y,upRightBrick);
	grid(0+x, high - 1+y,bottomLeftBrick);
    
	//
	// 通过块转移的方式移动最后一个围墙格
	//
	grid(wide - 2+x,high - 1+y,bottomRightBrick);
	//grid(wide - 2+x,high - 1+y,"ab");

	/*块范围坐标*/
	SMALL_RECT rc;
	rc.Left = 2 * (wide - 2+x);
	rc.Right = rc.Left + 1;
	rc.Top = rc.Bottom = high - 1 + y;

	/*目标块的起始坐标*/
	COORD startCoord = {rc.Right + 1, rc.Top };

	/*填充字符*/
	CHAR_INFO chFill;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(hStdOut, &bInfo); 
	chFill.Attributes = bInfo.wAttributes;
	chFill.Char.AsciiChar = ' ';
	
	/*移动块*/
	ScrollConsoleScreenBuffer(hStdOut, &rc, NULL, startCoord, &chFill);

	
	for (int i = 1; i != wide-1; i++) {
		grid(i+x, 0+y, horizonBrick);
		grid(i+x, high - 1+y, horizonBrick);
	}

	for (int i = 1; i != high - 1; i++) {
		grid(0+x, i+y, verticalBrick);
		grid(wide - 1+x, i+y, verticalBrick);
	}
}

void Wall::erase()
{
	grid(0+_x,0+_y,"  ");
	grid(wide - 1+_x,0+_y,"  ");
	grid(0+_x, high - 1+_y,"  ");
	grid(wide - 1+_x,high - 1+_y,"  ");
	
	for (int i = 1; i != wide-1; i++) {
		grid(i+_x, 0+_y, "  ");
		grid(i+_x, high - 1+_y, "  ");
	}

	for (int i = 1; i != high - 1; i++) {
		grid(0+_x, i+_y, "  ");
		grid(wide - 1+_x, i+_y, "  ");
	}
}


/*
 *
 * following defination is from class Tank
 *
 */
Tank::Tank(int k, int n)
{
	no = n;                            // 坦克编号
	kind = k;                          // 坦克种类
	bulletExist = false; 
	direction = -1;                    // 无前进方向
	speed = TANKSPEED;
	freezeTime = 0;                   // 非法迟缓时间                       
	srand((unsigned)time(0));          // 系统随机种子
	set_picture(kind);                 // 初始化坦克图形及其它属性
	

	for (int y = 0; y != 3; y++) {     // 坐标矩阵
		for (int x = 0; x != 3; x++)
			xy[y][x] = Grid(1+x,1+y);
	}
}

void Tank::set_picture(int kind)
{
	switch (kind) {
	case 1:	
	    /*
	     *    ■■
	     *      ■■
      	 *    ■■
		 */
		picture[0][0][0] = "■";
		picture[0][0][1] = "■";
		picture[0][1][1] = "■";
		picture[0][1][2] = "■";
		picture[0][2][0] = "■";
		picture[0][2][1] = "■";
		/*
		 *      ■
		 *    ■■■
		 *    ■  ■
		*/
		picture[1][0][1] = "■";
		picture[1][1][0] = "■";
		picture[1][1][1] = "■";
		picture[1][1][2] = "■";
		picture[1][2][0] = "■";
		picture[1][2][2] = "■";
		/*
		 *      ■■
		 *    ■■
		 *      ■■
		 */
		picture[2][0][1] = "■";
		picture[2][0][2] = "■";
		picture[2][1][0] = "■";
		picture[2][1][1] = "■";
		picture[2][2][1] = "■";
		picture[2][2][2] = "■";
		/*
		 *    ■  ■
		 *    ■■■
		 *      ■
		 */
		picture[3][0][0] = "■";
		picture[3][0][2] = "■";
		picture[3][1][0] = "■";
		picture[3][1][1] = "■";
		picture[3][1][2] = "■";
		picture[3][2][1] = "■";
		blood = 1;                  // 生命值，不同坦克不一样   
		col = 0x07;                 // 颜色值，不同坦克不一样
		break;
	case 2:
	    /*
	     *      ■◣
	     *        ■■
	     *      ■◤
	     *      
		 */
		picture[0][0][0] = "■";
		picture[0][0][1] = "◣";
		picture[0][1][1] = "■";
		picture[0][1][2] = "■";
		picture[0][2][0] = "■";
		picture[0][2][1] = "◤";
		/*
		 *      ■
		 *    ◢■◣
		 *    ■  ■
		 */
		picture[1][0][1] = "■";
		picture[1][1][0] = "◢";
		picture[1][1][1] = "■";
		picture[1][1][2] = "◣";
		picture[1][2][0] = "■";
		picture[1][2][2] = "■";
		/*
		 *      ◢■
		 *    ■■
		 *      ◥■
		 */
		picture[2][0][1] = "◢";
		picture[2][0][2] = "■";
		picture[2][1][0] = "■";
		picture[2][1][1] = "■";
		picture[2][2][1] = "◥";
		picture[2][2][2] = "■";
		/*
		 *    ■  ■
		 *    ◥■◤
		 *      ■
		 */
		picture[3][0][0] = "■";
		picture[3][0][2] = "■";
		picture[3][1][0] = "◥";
		picture[3][1][1] = "■";
		picture[3][1][2] = "◤";
		picture[3][2][1] = "■";
		blood = 1;
		col = 0x0c;
		break;
	}
}


void Tank::init_move(GameSpace** &gs, int _x, int _y)
{
	for (int y = 0; y != 3; y++) {
		for (int x = 0; x != 3; x++) {
			if (_x)
				xy[y][x].x += _x;
			if (_y)
				xy[y][x].y += _y;
			gs[xy[y][x].y][xy[y][x].x] = GameSpace(no,kind,true);
		}
	}
}

void Tank::init_xy()
{
	for (int y = 0; y != 3; y++) {     // 坐标矩阵
		for (int x = 0; x != 3; x++)
			xy[y][x] = Grid(1+x,1+y);
	}
}

void Tank::move(GameSpace** &gs)
{
	if (direction == -1) //无方向
		return;

	int _x = 0, _y = 0;
	switch (direction) {
	case 0:
		_x++;
		break;
	case 1:
		_y--;
		break;
	case 2:
		_x--;
		break;
	case 3:
		_y++;
		break;
	}

	// 计算下一步坐标
	Grid txy[3][3];
	for (int x = 0; x != 3; x++) {
		for (int y = 0; y != 3; y++) {
			txy[y][x].x = xy[y][x].x + _x;
			txy[y][x].y = xy[y][x].y + _y;
		}
	}

	

	bool ok = true;
	for (int x = 0; x != 3; x++) {
		for (int y = 0; y != 3; y++) {
			_x = txy[y][x].x;
			_y = txy[y][x].y;
			if (gs[_y][_x].have && gs[_y][_x].no != no) {// 有重叠部分
				if (gs[_y][_x].kind == 3)   // 是子弹
					blood--;
				ok = false;
				break;
			}
		}
	}

    
	// 可以前行
	if (ok) {
		// 擦除旧图
		clear_old();
		// 擦出游戏空间中原有图形
		for (int y = 0; y != 3; y++) {
			for (int x = 0; x != 3; x++) {	
					_x = xy[y][x].x;
					_y = xy[y][x].y;
					gs[_y][_x] = GameSpace(0,0,false);			
				    xy[y][x] = txy[y][x];
			}
		}
		
		// 在游戏空间中登记加入图形
		for (int y = 0; y != 3; y++) {
			for (int x = 0; x != 3; x++) {
					_x = xy[y][x].x;
					_y = xy[y][x].y;
					gs[_y][_x] = GameSpace(no,kind,true);
			}
		}
		// 刷新新图
		draw_tank();
		// 更新移动变量
		speed = TANKSPEED;
	}	
}


void Tank::rand_direction(GameSpace** &gs)
{
	bool can[4];
	for (int i = 0; i != 4; i++)
		can[i] = true;

	int count = 4;
	for (int i = 0; i != 3; i++) {
		// 往右搜索
		if (can[0] && gs[xy[i][2].y][xy[i][2].x+1].have) {
			can[0] = false;
			count--;
		}
		// 往上搜索
		if (can[1] && gs[xy[0][i].y-1][xy[0][i].x].have) {
			can[1] = false;
			count--;
		}
		// 往左搜索
		if (can[2] && gs[xy[i][0].y][xy[i][0].x-1].have) {
			can[2] = false;
			count--;
		}
		// 往下搜索
		if (can[3] && gs[xy[2][i].y+1][xy[2][i].x].have) {
			can[3] = false;
			count--;
		}
	}

	if (count == 0)                 // 没有可行出口
		return;

	// 获取一个随机可行的方向
	int newDir;     
	int r = tank_rand(count) + 1;
	for (int i = 0; i != 4; i++) {
		if (can[i]) {
			if (r > 1)
				r--;
			else {
				newDir = i;
				break;
			}
		}
	}

	trans_to_direction(newDir);
	freezeTime = TANKFREEZE;	
}


bool Tank::reset(GameSpace** &gs, int _x, int _y) {
	// 判断此处是否可以诞生
	for (int y = 0; y != 3; y++) {
		for (int x = 0; x != 3; x++) {
			if (gs[y+_y][x+_x].have)
				return false;				
		}
	}
	
	for (int y = 0; y != 3; y++) {
		for (int x = 0; x != 3; x++) {
			xy[y][x] = Grid(x+_x, y+_y);
			gs[y+_y][x+_x] = GameSpace(no, kind, true);			
		}
	}

	blood = 1;
	bulletExist = false;
	return true;
}

void Tank::set_kind(int k)
{
	kind = k;
	set_picture(k);
}

void Tank::set_no(int n)
{
	no = n;
}

void Tank::trans_to_direction(int dir) {
	clear_old();
	trans_direction(dir);
	draw_tank();
}

void Tank::trans_direction(int dir) 
{	
	direction = dir;
}

void Tank::be_shot(GameSpace** &gs)
{
	blood--;
	// 没死
	if (blood > 0)
		return;
	
	// 死了
	int _x, _y;
	for (int y = 0; y != 3; y++) {
		for (int x = 0; x != 3; x++) {
			_x = xy[y][x].x;
			_y = xy[y][x].y;
			gs[_y][_x] = GameSpace(0, 0, false);
		}
	}

	clear_old();
}

Bullet Tank::rand_shot(GameSpace** &gs, int no, int& shot)
{
	// 死了
	if (blood <= 0)
		return Bullet();

	int k = tank_rand(200);

	if ((k>50 && k<100) || (k>150 && k<200))
		return make_bullet(gs, no, shot);

	return Bullet();
}

Bullet Tank::make_bullet(GameSpace** &gs, int no, int& shot)
{
	shot  = -1;
	Grid txy = xy[1][1];
	switch (direction) {
	case 0:
		txy.x += 2;
		break;
	case 1:
		txy.y -= 2;
		break;
	case 2:
		txy.x -= 2;
		break;
	case 3:
		txy.y += 2;
		break;
	}
	
	
	if (gs[txy.y][txy.x].have) {
		if (kind != gs[txy.y][txy.x].kind && 
			gs[txy.y][txy.x].kind > 0) {
			shot = gs[txy.y][txy.x].no;
		}
		return Bullet();
	}
	Bullet ret;
	ret.kind = kind;
	ret.disapper = false;
	ret.dir = direction;
	ret.host = no;	
	
	ret.xy = txy;
	return ret;
}

void Tank::draw_tank()
{
	color(col);
	for (int y = 0; y != 3; y++) 
		for (int x = 0; x != 3; x++) 			 
			grid(xy[y][x].x, xy[y][x].y, picture[direction][y][x]);
}

void Tank::clear_old()
{
	for (int y = 0; y != 3; y++) 
		for (int x = 0; x != 3; x++) 
			grid(xy[y][x].x, xy[y][x].y, "  ");
}


int Tank::tank_rand(int max)
{
	return rand() % max;
}



/*
 *
 * following defination is from class Bullet
 *
*/
Bullet::Bullet(int k)
{
	init(k);
}

void Bullet::init(int k)
{
	disapper = true;
	host = -1;
	kind = k;
	dir = -1;
}

void Bullet::registration(GameSpace** &gs)
{
	if (!disapper) 
		gs[xy.y][xy.x] = GameSpace(host, -3, true);
}

void Bullet::unregistration(GameSpace** &gs)
{
	if (!disapper)
		gs[xy.y][xy.x] = GameSpace(0, 0, false);
}

void Bullet::move(GameSpace** &gameSpace, int& shot)
{
	shot = -100;                // -100表示运行中

	if (this->dir == -1)
		return;

	unregistration(gameSpace);  // 注销原有记录

	Grid nextxy = xy;
	switch (dir) {
	case 0:
		// right
		nextxy.x++;
		break;
	case 1:
		// up
		nextxy.y--;
		break;
	case 2:
		// right
		nextxy.x--;
		break;
	case 3:
		nextxy.y++;
		break;
	}

	if (gameSpace[nextxy.y][nextxy.x].have == false) {
		// clear the old bullet
		clear();
		unregistration(gameSpace);		
		xy = nextxy;
		// show the new bullet
		show();
		registration(gameSpace);
	}
	else if (gameSpace[nextxy.y][nextxy.x].kind > 0 &&      // 是坦克
		     gameSpace[nextxy.y][nextxy.x].kind != kind) {  // 不同类型坦克    
			 shot = gameSpace[nextxy.y][nextxy.x].no;       // 返回受伤坦克编号
			 gameSpace[xy.y][xy.x] = GameSpace(0,0,false);
			 clear();
			 init(kind);
	} else if (gameSpace[nextxy.y][nextxy.x].kind == -3) { // 与子弹相撞
		clear();
		unregistration(gameSpace);
		init(kind);
		shot = gameSpace[nextxy.y][nextxy.x].no + 7;
	} else{
		unregistration(gameSpace);//gameSpace[xy.y][xy.x] = GameSpace(0,0,false);
		clear();
		init(kind);
	}   
}

void Bullet::clear()
{
	grid(xy.x, xy.y, "  ");
}

void Bullet::show()
{
	color(0x0f);
	grid(xy.x, xy.y, "●");
}

/*
 *
 * following defination is from class Map
 *
 */

Map::Map()
{
	total = 4;            // 一共两关
	clear();              // 清空各张地图


	// assist variable
	int x, y, i;

	// the first map
	for ( y = 0; y !=2; y++) {
		for ( x = 0; x != 6; x++) {
			exist[0][8+y][6+x] = true;
			exist[0][6+x][8+y] = true;
			exist[0][18+y][16+x] = true;
			exist[0][16+x][18+y] = true;
			exist[0][28+y][26+x] = true;
			exist[0][26+x][28+y] = true;
			exist[0][28+y][6+x] = true;
			exist[0][22+x][6+y] = true;
			exist[0][8+y][24+x] = true;
			exist[0][8+x][30+y] = true;
		}
	}

	// the second map
	for (i = 0; i != 5; i++) {
		for (x = 6; x !=9; x++) {
			for (y = 6; y != 9; y++)
				exist[1][y][x+ 6*i] = true;
		}
		
		for (x = 4; x !=7; x++) {
			for ( y = 16; y != 23; y++)
				exist[1][y][x+ 7*i] = true;
		}
	}



	// the third map
	for ( y = 0; y != 4; y++) {
		for ( x = 0; x != 4; x++) {
			exist[2][8+y][18+x] = true;
			exist[2][15+y][11+x] = true;
			exist[2][15+y][18+x] = true;
			exist[2][15+y][25+x] = true;
			exist[2][22+y][11+x] = true;
			exist[2][22+y][25+x] = true;
		}
	}
	
	// the forth hurdle
		for (y = 0; y != 3; y++) {
		for (x = 0; x != 2; x++) {
			for (i = 0; i != 5; i++) {
				exist[3][i*6+5+y][4+x] = true;
				exist[3][i*6+5+y][34+x] = true;
			}
		}
	}

	int cirX1 = 19, cirX2 = 20, cirY = 19, tempValue;
	for (x = 12; x <= 27; x++) {
		for (y = 12; y <= 27; y++) {
			tempValue = (y-cirY)*(y-cirY) + (x-cirX1)*(x-cirX1) - 49;
			if (tempValue>=-4 && tempValue<=4 ) {
				if ((y < cirY - 2 || y > cirY + 2) &&
					(x < cirX1 - 1 || x > cirX2 + 1))
					exist[3][y][x] = true;
			}
			tempValue = (y-cirY)*(y-cirY) + (x-cirX2)*(x-cirX2) - 49;
			if (tempValue>=-4 && tempValue<=4 ) {
				if ((y < cirY - 2 || y > cirY + 2) &&
					(x < cirX1 - 1 || x > cirX2 + 1))
					exist[3][y][x] = true;
			}
		}
	}
	/*
	for ( x = 5; x !=10; x++)
		for ( y = 5; y != 10; y++)
		    exist[3][y][x] = true;

	for (x = 20; x != 25; x++)
		for ( y = 15; y != 18; y++)
		    exist[3][y][x] = true;*/
}

void Map::clear()
{
	for (int i = 0; i != total; i++)
		for (int x = 0; x != 40; x++)
			for(int y = 0; y != 40; y++)
				exist[i][y][x] = false;
}


void Map::load(GameSpace** &gs, int i)
{
	no = i;
	for (int x = 0; x != 40; x++) {
		for (int y = 0; y != 40; y++) {
			if (exist[i-1][y][x])
				gs[y][x] = GameSpace(-2,-2,true);
		}
	}
}
    
void Map::draw()
{
	color(0x08);
	for (int x = 0; x != 40; x++) {
		for (int y = 0; y != 40; y++) {
			if (exist[no-1][y][x])
				grid(x,y,"▓");
		}
	}
}


/*
 *
 * following defination is from class GameSpace
 *
 */
GameSpace::GameSpace() {
	no = 0;
	kind = 0;
	have = false;
}

GameSpace::GameSpace(int n, int k, bool h)
{
	no = n;
	kind = k;
	have = h;
}

GameSpace& GameSpace::operator=(GameSpace& gs)
{
	no = gs.no;
	kind = gs.kind;
	have = gs.have;
	return *this;
}


/*
 *
 * following defination is from class Frame
 *
 */
Frame::Frame(int w, int h)
{
	wide = w;
	high = h;
}

void Frame::clear_frame()
{
	grid(0+x,0+y,"  ");
	grid(wide - 1+x,0+y,"  ");
	grid(0+x, high - 1+y,"  ");
	grid(wide - 1,high - 1,"  ");
	
	for (int i = 1; i != wide-1; i++) {
		grid(i+x, 0+y, "  ");
		grid(i+x, high - 1+y, "  ");
	}

	for (int i = 1; i != high - 1; i++) {
		grid(0+x, i+y, "  ");
		grid(wide - 1+x, i+y, "  ");
	}
}

void Frame::draw_frame(int _x, int _y, int col)
{
	x = _x;
	y = _y;

	color(col);
	grid(0+x,0+y,"┏");
	grid(wide - 1+x,0+y,"┓");
	grid(0+x, high - 1+y,"┗");
	grid(wide - 1+x,high - 1+y,"┛");
	
	for (int i = 1; i != wide-1; i++) {
		grid(i+x, 0+y, "━");
		grid(i+x, high - 1+y, "━");
	}

	for (int i = 1; i != high - 1; i++) {
		grid(0+x, i+y, "┃");
		grid(wide - 1+x, i+y, "┃");
	}
}