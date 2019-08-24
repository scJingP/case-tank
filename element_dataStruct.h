#ifndef ELEMENT_DATASTRUCT_GUARD
#define ELEMENT_DATASTRUCT_GUARD

#include <string>
#include <time.h>

#define WALLCOL            0x08
#define TANKSPEED          4
#define TANKFREEZE         7


/*
 *
 *             class    GameSpace     declaration
 *
 */
struct GameSpace {
	GameSpace();
	GameSpace(int,int,bool);

	GameSpace& operator=(GameSpace&);
	int no;
	int kind;
	bool have;
};


/*
 *
 *             class   Grid     declaration
 *
 */
struct Grid{
	// struct function
	Grid();
	Grid(Grid&);
	Grid(int,int);

	// operator function
	Grid& operator=(Grid&);
	bool operator==(Grid&);
	bool operator!=(Grid&);

	// data
	int x, y;
};


/*
 *
 *             class    Wall    declaration
 *
 */
struct Wall{
	// struct function
	Wall(GameSpace** &, int w= 40, int h= 40, std::string = "〓");
	

	// functional function
	void set_brick(std::string);
	/*"┏","┓","┗","┛","━","┃"*/
	void set_brick(std::string,std::string,std::string,\
		           std::string,std::string,std::string);
	void draw(int=0,int=0);
	void erase();

	// data
	std::string upLeftBrick;          // 左上角砖块
	std::string upRightBrick;         // 右上角砖块
	std::string bottomLeftBrick;      // 左下角砖块
	std::string bottomRightBrick;     // 右下角砖块
	std::string horizonBrick;         // 水平砖块
	std::string verticalBrick;        // 竖直砖块


	int _x;
	int _y;

	int wide;
	int high;
};


/*
 *
 *             class    Bullet     declaration
 *
 */

struct Bullet {

	Bullet(int=1);           // 默认敌人

	void init(int);
	void move(GameSpace** &gameSpace,int&);
	void clear();
	void show();
	void registration(GameSpace** &);
	void unregistration(GameSpace** &);

	bool disapper;                     // 是否消失
    int host;                          // 绑定坦克编号
	int kind;                          // 坦克种类
	int dir;                           // 子弹运行方向
	Grid xy;                           // 子弹当前坐标
};


/*
 *
 *             class    Tank     declaration
 *
 */
struct Tank{

	// struct function
	Tank(int=1,int=-1);

	
	// operator function
	

	// output function
	void draw_tank();
	void clear_old();

	// set property function
	void set_picture(int);
	void set_kind(int);
	void set_no(int);
	void init_xy();
	bool reset(GameSpace** &, int x = 1, int y = 1);

	// functional function
	void trans_direction(int);
	void init_move(GameSpace** &,int x=0, int y=0);
	void move(GameSpace** &);
	void rand_direction(GameSpace**&);
	void trans_to_direction(int);
	int tank_rand(int);
	void be_shot(GameSpace** &);
	Bullet rand_shot(GameSpace** &, int, int&);
	Bullet make_bullet(GameSpace** &gs, int no,int &);
	      
	
	// data

	/*坦克坐标及图库*/
	std::string picture[4][3][3];      // 坦克图案
	Grid xy[3][3];                     // 坦克板块坐标

	/*坦克类型、编号、颜色、生命值*/
	int kind;                          // 坦克种类
	int no;                            // 坦克编号
	int col;                           // 坦克颜色
	int blood;                         // 坦克血长
	int speed;                         // 坦克速度
	int freezeTime;                    // 运行迟缓时间

	/*坦克运行记录*/
	int direction;                     // 坦克方向 1右 2上 3左 4下
	bool bulletExist;                  // 子弹存在性
};

/*
 *
 *             class    Map     declaration
 *
 */
struct Map {
	Map();
	void load(GameSpace** &, int);
	void clear();
    void draw();

	int total;                          // 地图总数
	int no;                             // 当前使用的地图
	bool exist[4][40][40];              // 地图集
};


/*
 *
 *             class    Frame    declaration
 *
 */
class Frame {
public:
	Frame(int=4,int=4);
	void draw_frame(int= 0, int= 0, int= 0x0e);
	void clear_frame();
private:

	int x;
	int y;

	int wide;
	int high;
};


#endif