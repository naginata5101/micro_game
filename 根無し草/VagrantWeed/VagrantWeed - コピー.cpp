#include <Mikan.h>
#include <cmath>

const int WIN_W = 640;
const int WIN_H = 480;
const double M_PI = 3.14159265358979323846;

enum {FONT_BW, FONT_BG, FONT_NW};
const int FONTS_B = 70;
const int FONTS_N = 35;

const int CHIP = 32;
const int WIDTH = 15, HEIGHT = 15;
const int SKY_X = WIDTH * CHIP;
const int SKY_W = WIN_W - SKY_X;

const int WEED_MAX = 100;
const int WALL_MAX = (WIDTH-1 + HEIGHT-1) * 2;

const int WEEK = 7;
const int DAY = 14;
const int HOUR = 60;
const int NIGHT = 2;

// 日時計
const int CLOCK_L = SKY_W / 2 - 10;
const int CLOCK_X = SKY_X + SKY_W/2;
const int CLOCK_Y = FONTS_B * 3 + CLOCK_L + 30;

enum Dir {LEFT, DOWN, RIGHT};
enum {TITLE, GAME, RESULT} game;

const int BOOST = 5;

int sunshine[HEIGHT][WIDTH];
const int SUN_MAX = 35;

int full[HEIGHT][WIDTH];
enum {PLAYER = -2, WALL = -1};

// 6-18
const struct Time {
	int hour;
	Dir dir;
	int sun;
	unsigned sky;
} time[DAY] = {
	{ 1, DOWN,  0, 0xFF191970}, // midnightblue
	{ 6, LEFT, 15, 0xFF0000CD}, // mediumblue
	{ 7, LEFT, 20, 0xFF4169E1}, // royalblue
	{ 8, LEFT, 25, 0xFF6495ED}, // cornflowerbule
	{ 9, LEFT, 30, 0xFF87CEEB}, // skyblue
	{10, DOWN, 32, 0xFF00BFFF}, // deepskyblue
	{11, DOWN, 35, 0xFF1E90FF}, // dodgerblue
	{12, DOWN, 35, 0xFF1E90FF}, // dodgerblue
	{13, DOWN, 32, 0xFF00BFFF}, // deepskyblue
	{14,RIGHT, 30, 0xFF6495ED}, // cornflowerbule
	{15,RIGHT, 25, 0xFFFF69B4}, // hotpink
	{16,RIGHT, 20, 0xFFFF7F50}, // coral
	{17,RIGHT, 15, 0xFF800080}, // purple
	{21, DOWN,  0, 0xFF191970}
};

class Weed {
protected:
	static const int MIN_SIZE = 100;
	int x, y;
	int size;
	int energy;
public:
	Weed() {
		size = 0;
		energy = 0;
	}

	bool init(int X, int Y, int index) {
		if (full[Y][X])
			return false;

		full[y][x] = NULL;
		size = MIN_SIZE;

		x = X;
		y = Y;

		full[y][x] = index;
		return true;
	}
	bool init(int index) {
		full[y][x] = NULL;
		size = MIN_SIZE;

		do {
			x = rand() % WIDTH;
			y = rand() % HEIGHT;
		} while (full[y][x]);

		full[y][x] = index;
		return true;
	}

	double getSize() {
		return size;
	}

	void cover(const Dir dir) {
		double s = double(size) / MIN_SIZE + 1;
		int cx = x, cy = y;

		while(1) {
			switch(dir) {
			case LEFT:
				--cx;
				break;
			case RIGHT:
				++cx;
				break;
			}
			++cy;

			if (!(0 <= cx && cx < WIDTH && cy < HEIGHT && s > 1))
				break;

			sunshine[cy][cx] /= int(s);
			s /= 2;
		}
	}

	int photosynthesis() {
		return energy += 1+ sunshine[y][x]*size / (SUN_MAX*MIN_SIZE) * BOOST;
	}
	int graw() {
		if (size > 0)
			size = MIN_SIZE + photosynthesis();
		return size;
	}

	void draw() {
		MikanDraw->DrawTextureScaling(0, (x)*CHIP, (y+1)*CHIP, CHIP, 0, CHIP, -CHIP,
			CHIP, -size*CHIP/MIN_SIZE, 0);
	}
};
class Player: public Weed {
public:
	int graw() {
		photosynthesis();
		if (MikanInput->GetKeyNum(K_Z) == 1)
			size += 1 + energy/HOUR /10;
		return size;
	}

	int getEnergy() {
		return energy;
	}

	void draw() {
		MikanDraw->DrawTextureScaling(0, (x)*CHIP, (y+1)*CHIP,    0, 0, CHIP, -CHIP,
			CHIP, -size*CHIP/MIN_SIZE, 0);
	}

	void move() {
		full[y][x] = 0;

		const int interval = 20;
		if (MikanInput->GetKeyNum(K_UP   ) % interval == 1 && y >        0 && !full[y-1][x])
			--y;
		if (MikanInput->GetKeyNum(K_RIGHT) % interval == 1 && x <  WIDTH-1 && !full[y][x+1])
			++x;
		if (MikanInput->GetKeyNum(K_DOWN ) % interval == 1 && y < HEIGHT-1 && !full[y+1][x])
			++y;
		if (MikanInput->GetKeyNum(K_LEFT ) % interval == 1 && x >        0 && !full[y][x-1])
			--x;

		full[y][x] = PLAYER;
	}
};
class Wall {
private:
	int x,y;
public:
	void init(int X, int Y) {
		x = X;
		y = Y;
		full[y][x] = WALL;
	}

	void cover(const Dir dir) {
		double s = 5;
		int cx = x, cy = y;

		while(1) {
			switch(dir) {
			case LEFT:
				--cx;
				break;
			case RIGHT:
				++cx;
				break;
			}
			++cy;

			if (!(0 <= cx && cx < WIDTH && cy < HEIGHT && s > 1))
				break;

			sunshine[cy][cx] /= int(s);
			s /= 2;
		}
	}
	void draw() {
		MikanDraw->DrawBox((x)*CHIP, (y)*CHIP, CHIP, CHIP, 0xFF4F2702);
	}
};

Weed weed[WEED_MAX];
Wall wall[WALL_MAX];
Player player;
int weedNum;
int clockX[DAY], clockY[DAY];

// TIMER
int timer, hour, day;
Time current;

// sunshine setting
void sun() {
	// 初期値
	for (int x = 0; x < WIDTH; x++)
		for (int y = 0; y < HEIGHT; y++)
			sunshine[y][x] = current.sun;
	// 変化
	for (int i = 0; i < WEED_MAX; i++)
		weed[i].cover(current.dir);
	for (int i = 0; i < WALL_MAX; i++)
		wall[i].cover(current.dir);
	player.cover(current.dir);
	// 修正
	for (int x = 0; x < WIDTH; x++)
		for (int y = 0; y < HEIGHT; y++)
			if (sunshine[y][x] < 0)
				sunshine[y][x] = 0;
}

void draw(bool right) {
	//画面の初期化
	MikanDraw->ClearScreen(current.sky);

	if (right) {
		// 右側
		MikanDraw->Printf(FONT_BW, SKY_X+5, 1*FONTS_B, "%d day", day+1);
		if (current.sun > 0)
			MikanDraw->Printf(FONT_BW, SKY_X+5, 2*FONTS_B, "%02d:%02d", current.hour, timer%HOUR);
		else
			MikanDraw->Printf(FONT_BW, SKY_X+5, 2*FONTS_B, "--:%02d", timer%HOUR);

		// 日時計
		MikanDraw->DrawCircleC(CLOCK_X, CLOCK_Y, CLOCK_L+1, 0xFFFFFFFF);
		MikanDraw->DrawCircleC(CLOCK_X, CLOCK_Y, CLOCK_L-1, current.sky);
		MikanDraw->DrawBox(SKY_X, CLOCK_Y, SKY_W, WIN_H-CLOCK_Y, current.sky);
		for (int i = 0; i < DAY; i++)
			MikanDraw->DrawLine(CLOCK_X, CLOCK_Y, clockX[i], clockY[i]);
		MikanDraw->DrawCircleC(clockX[hour], clockY[hour], 7, 0xFFFF4500);

		// スコア
		MikanDraw->Printf(FONT_NW, SKY_X+5, 5*FONTS_B, "Score:%d", player.getEnergy());
	}

	// 地面・壁
	MikanDraw->DrawBox(0, 0, WIDTH*CHIP, HEIGHT*CHIP, 0xFF000000);
	for (int x = 0; x < WIDTH; x++)
		for (int y = 0; y < HEIGHT; y++)
			MikanDraw->DrawBox((x)*CHIP, (y)*CHIP, CHIP, CHIP,
				0xC0A06A + (sunshine[y][x] *0xFF /SUN_MAX *0x01000000));
	for (int i = 0; i < WALL_MAX; i++)
		wall[i].draw();
	// 壁・草・プレイヤー
	/*
	for (int i = 0; i < WEED_MAX; i++)
		weed[i].draw();
	player.draw();
	*/
	int x = 1;
	for (int y = 0; y < HEIGHT; y++)
	//	for (int x = 0; x < WIDTH; x++)
			switch (full[y][x]) {
			case NULL:
				break;
			case PLAYER:
				player.draw();
				break;
			default:
				weed[full[y][x]-1].draw();
			}
	//MikanDraw->Printf(0,0,0,"day:%d,hour:%d ; timer:%d,current:%d", day, hour, timer, current.hour);

	if (!right)
		MikanDraw->DrawBox(0, 0, WIN_W, WIN_H, 0xAA000000);
}

//プログラム起動時に1度だけ実行
//システム設定などを行う
void SystemInit(void) {
	MikanWindow->SetWindowName("根無し草");
	MikanWindow->SetWindowIcon("GAME_ICON");
}

//MainLoop開始前に1度だけ実行
//テクスチャの読み込みなどのゲーム開始前の準備を行う
void UserInit(void) {
	// srand((unsigned)time(NULL));
	MikanDraw->CreateTexture(0, NULL, "TEXTURE", TRC_ZERO);
	MikanDraw->LoadFontFile(NULL, "FONT_R");
	MikanDraw->LoadFontFile(NULL, "FONT_B");
	MikanDraw->CreateFont(FONT_BW, "MigMix 1M bold"   , FONTS_B, 0xFFFFFFFF);
	MikanDraw->CreateFont(FONT_BG, "MigMix 1M bold"   , FONTS_B, 0xFF70EE70);
	MikanDraw->CreateFont(FONT_NW, "MigMix 1M regular", FONTS_N, 0xFFFFFFFF);


	// 各時刻の日時計
	for (int i = 0; i < DAY; i++) {
		double clockA = M_PI *i /(DAY-1);
		clockX[i] = CLOCK_X -CLOCK_L*cos(clockA);
		clockY[i] = CLOCK_Y -CLOCK_L*sin(clockA);
	}

	// 地面
	for (int x = 0; x < WIDTH; x++)
		for (int y = 0; y < HEIGHT; y++)
			full[y][x] = 0;

	// 壁
	int numWall = 0;
	for (int x = 0; x < WIDTH; x++) {
		wall[numWall++].init(x, 0);
		wall[numWall++].init(x, HEIGHT-1);
	}
	for (int y = 1; y < HEIGHT-1; y++) {
		wall[numWall++].init(      0, y);
		wall[numWall++].init(WIDTH-1, y);
	}

	player.init(1, 1, PLAYER);
	game = TITLE;
	timer = 0;
}

//1秒間に60回実行
//0を返している間は何度でも実行される
int MainLoop(void) {
	MikanDraw->ClearScreen();

	// TIMER-----------------
	hour = timer / HOUR;
	day = hour / DAY;
	hour -= day * DAY;
	current = time[hour];


	// BODY---------------------
	switch(game) {
	case TITLE:
		if (MikanInput->GetKeyNum(K_Z)) {
			// 草
			while(weedNum < 10)
				weed[weedNum].init(weedNum++);
			game = GAME;
		}

		// 背景
		current = time[DAY/2];
		sun();
		draw(false);

		MikanDraw->Printf(FONT_BG, (WIN_W-4*FONTS_B)/2, 50, "根無し草");
		MikanDraw->Printf(FONT_NW, 50, 150,
			"あなたは歩ける草（緑の草）です。\n"
			"他の草（赤い草）の陰を避け、\n"
			"より多くの日光を浴びましょう。\n"
			"7日間で浴びた日光量の合計が最終得点です。\n"
			"【操作方法】\n"
			"　矢印ボタン：移動\n"
			"　　◯ボタン：成長（合計日光量に応じる）\n"
			"　　△ボタン：加速");

		break;


	case GAME:
		// INCREMENT
		if (current.sun && !(timer%(2*HOUR)) && weedNum<WEED_MAX)
			weed[weedNum].init(weedNum++);

		// SUNSHINE--------------
		sun();

		// MOVING
		if (!(timer%HOUR))
			for (int i = 0; i < WEED_MAX; i++)
				weed[i].graw();
		player.graw();
		player.move();


		// DRAW------------------
		draw(true);

		if (MikanInput->GetKeyNum(K_A)) timer += BOOST;
		else ++timer;

		if (timer /DAY /HOUR >= 7)
			game = RESULT;
		break;


	case RESULT:
		if (MikanInput->GetKeyNum(K_Z) == 30)
			return 1;

		// 背景
		sun();
		draw(false);

		MikanDraw->Printf(FONT_BG, (WIN_W-4*FONTS_B)/2, 50, "リザルト");
		MikanDraw->Printf(FONT_BW, 50, 150, "合計日光量：%d", player.getEnergy());
		if (1)
			MikanDraw->Printf(FONT_BW, 100, 250, "コケ級");
		else if (1)
			MikanDraw->Printf(FONT_BW, 100, 250, "シダ級");
		else
			MikanDraw->Printf(FONT_BW, 100, 250, "アシ級");

		MikanDraw->Printf(FONT_NW, 200, 400, "◯ボタン長押しで終了");


		break;


	default:
		return 1;
	}


	if (MikanInput->GetKeyNum(K_ESC))
		return 1;
	return 0;
}

//MainLoop終了後に1度だけ実行
//後片付けなどを行う
void CleanUp(void) {
}
