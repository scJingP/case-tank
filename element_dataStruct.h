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
	Wall(GameSpace** &, int w= 40, int h= 40, std::string = "��");
	

	// functional function
	void set_brick(std::string);
	/*"��","��","��","��","��","��"*/
	void set_brick(std::string,std::string,std::string,\
		           std::string,std::string,std::string);
	void draw(int=0,int=0);
	void erase();

	// data
	std::string upLeftBrick;          // ���Ͻ�ש��
	std::string upRightBrick;         // ���Ͻ�ש��
	std::string bottomLeftBrick;      // ���½�ש��
	std::string bottomRightBrick;     // ���½�ש��
	std::string horizonBrick;         // ˮƽש��
	std::string verticalBrick;        // ��ֱש��


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

	Bullet(int=1);           // Ĭ�ϵ���

	void init(int);
	void move(GameSpace** &gameSpace,int&);
	void clear();
	void show();
	void registration(GameSpace** &);
	void unregistration(GameSpace** &);

	bool disapper;                     // �Ƿ���ʧ
    int host;                          // ��̹�˱��
	int kind;                          // ̹������
	int dir;                           // �ӵ����з���
	Grid xy;                           // �ӵ���ǰ����
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

	/*̹�����꼰ͼ��*/
	std::string picture[4][3][3];      // ̹��ͼ��
	Grid xy[3][3];                     // ̹�˰������

	/*̹�����͡���š���ɫ������ֵ*/
	int kind;                          // ̹������
	int no;                            // ̹�˱��
	int col;                           // ̹����ɫ
	int blood;                         // ̹��Ѫ��
	int speed;                         // ̹���ٶ�
	int freezeTime;                    // ���гٻ�ʱ��

	/*̹�����м�¼*/
	int direction;                     // ̹�˷��� 1�� 2�� 3�� 4��
	bool bulletExist;                  // �ӵ�������
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

	int total;                          // ��ͼ����
	int no;                             // ��ǰʹ�õĵ�ͼ
	bool exist[4][40][40];              // ��ͼ��
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