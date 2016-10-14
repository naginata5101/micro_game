#include <Mikan.h>
#include <cmath>
#include <windows.h>

const int WIN_W = 640;
const int WIN_H = 480;
const double M_PI = 3.14159265358979323846;

enum {FONT_BW, FONT_BG, FONT_NW, FONT_NR};
const int FONTS_B = 70;
const int FONTS_N = 35;
enum {BGM, CLOCK, GROW, SELECT};

const int CHIP = 32;
const int WIDTH = 15, HEIGHT = 15;
const int SKY_X = WIDTH * CHIP;
const int SKY_W = WIN_W - SKY_X;

const int WEED_MAX = 100;
const int WALL_MAX = (WIDTH-1 + HEIGHT-1) * 2;
//219,235
const int SPAN = 3;
const int DAY = 14;
const int HOUR = 40;
const int NIGHT = 2;

// 日時計
const int CLOCK_L = SKY_W / 2 - 10;
const int CLOCK_X = SKY_X + SKY_W/2;
const int CLOCK_Y = FONTS_B * 3 + CLOCK_L + 30;

enum Dir {LEFT, DOWN, RIGHT};
enum {TITLE, GAME, RESULT} game;

const int SUN_MAX = 35;
int sunshine[HEIGHT][WIDTH];

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

class Wall {
protected:
	void cover(int size, int x, int y, const Dir dir) {
		while(1) {
			switch(dir) {
			case LEFT:
				--x;
				break;
			case RIGHT:
				++x;
				break;
			}
			++y;

			if (!(0 <= x && x < WIDTH && y < HEIGHT && size > 1))
				break;

			sunshine[y][x] /= size;
			size -= size/2;
		}
	}
public:
	virtual void cover(int x, int y, const Dir dir) {
		cover(5, x, y, dir);
	}
	virtual void draw(int x, int y) {
		MikanDraw->DrawBox(x*CHIP, y*CHIP, CHIP, CHIP, 0xFF4F2702);
	}
};
class Weed: public Wall {
protected:
	static const int MIN_SIZE = 1000;
	int size;
	int energy;
public:
	Weed() {
		size = MIN_SIZE;
		energy = 0;
	}
	double getSize() {
		return size;
	}

	virtual void cover(int x, int y, const Dir dir) {
		Wall::cover(2*size/MIN_SIZE +1, x, y, dir);
	}

	int photosynthesis(int x, int y) {
		return energy += 5+ sunshine[y][x]*size / (SUN_MAX*MIN_SIZE) *10;//*5 *3/2 *7/5;
	}
	virtual int grow(int x, int y) {
		if (size > 0)
			size = MIN_SIZE + 10*photosynthesis(x, y);
		return size;
	}

	virtual void draw(int x, int y) {
		MikanDraw->DrawTextureScaling(0, x*CHIP, (y+1)*CHIP, CHIP, 0, CHIP, -CHIP,
			CHIP, -size*CHIP/MIN_SIZE, 0);
	}
};

Wall *map[HEIGHT][WIDTH];

class Player: public Weed {
private:
	int x, y;
	int effect;
public:
	Player(int X, int Y) {
		size = MIN_SIZE;
		energy = 0;
		x = X;
		y = Y;
		effect = 100;
	}

	virtual int grow(int X, int Y) {
		photosynthesis(X, Y);
		// 連打数：219,235
		if (MikanInput->GetKeyNum(K_Z) == 1) {
			size += 5 + energy/HOUR /4;
			effect = 0;
			MikanSound->Play(GROW, false);
		}
		++effect;

		return size;
	}
	int grow() {
		return grow(x,y);
	}

	void move() {
		map[y][x] = nullptr;

		const int interval = 20;
		if (MikanInput->GetKeyNum(K_UP   ) % interval == 1 && y >        0 && !map[y-1][x])
			--y;
		if (MikanInput->GetKeyNum(K_RIGHT) % interval == 1 && x <  WIDTH-1 && !map[y][x+1])
			++x;
		if (MikanInput->GetKeyNum(K_DOWN ) % interval == 1 && y < HEIGHT-1 && !map[y+1][x])
			++y;
		if (MikanInput->GetKeyNum(K_LEFT ) % interval == 1 && x >        0 && !map[y][x-1])
			--x;

		map[y][x] = this;
	}

	// 理論上最大値：568325
	int getEnergy() {
		return energy;
	}

	virtual void draw(int x, int y) {
		if (effect <= 9) {
			MikanDraw->SetAlpha(0, 127);
			MikanDraw->DrawTextureScaling(0, x*CHIP, (y+1)*CHIP, 0, 0, CHIP, -CHIP,
				CHIP, -size*CHIP/MIN_SIZE * effect/3, 0);
			MikanDraw->SetAlpha(0, 255);
		}
		MikanDraw->DrawTextureScaling(0, x*CHIP, (y+1)*CHIP, 0, 0, CHIP, -CHIP,
			CHIP, -size*CHIP/MIN_SIZE, 0);
	}
};


Player *player;

int weedNum;
int clockX[DAY], clockY[DAY];

// TIMER
int timer, hour, day;
Time current;


// sunshine setting
void makeWeed() {
	int x,y;
	do {
		x = rand() % WIDTH;
		y = rand() % HEIGHT;
	} while (map[y][x]);
	map[y][x] = new Weed();
}
void sun() {
	// 初期値
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x < WIDTH; x++)
			sunshine[y][x] = current.sun;
	// 変化
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x < WIDTH; x++)
			if (map[y][x] != nullptr)
				map[y][x]->cover(x, y, current.dir);
	// 修正
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x < WIDTH; x++)
			if (sunshine[y][x] < 0)
				sunshine[y][x] = 0;
}
void draw(bool isGame) {
	//画面の初期化
	MikanDraw->ClearScreen(current.sky);

	if (isGame) {
		// 右側
		MikanDraw->Printf(FONT_BW, SKY_X+10, 1*FONTS_B, "%d day", day+1);
		if (current.sun > 0)
			MikanDraw->Printf(FONT_BW, SKY_X+10, 2*FONTS_B, "%02d:%02d", current.hour, timer%HOUR);
		else
			MikanDraw->Printf(FONT_BW, SKY_X+10, 2*FONTS_B, "--:%02d", timer%HOUR);

		// 日時計
		MikanDraw->DrawCircleC(CLOCK_X, CLOCK_Y, CLOCK_L+1, 0xFFFFFFFF);
		MikanDraw->DrawCircleC(CLOCK_X, CLOCK_Y, CLOCK_L-1, current.sky);
		MikanDraw->DrawBox(SKY_X, CLOCK_Y, SKY_W, WIN_H-CLOCK_Y, current.sky);
		for (int i = 0; i < DAY; i++)
			MikanDraw->DrawLine(CLOCK_X, CLOCK_Y, clockX[i], clockY[i]);
		MikanDraw->DrawCircleC(clockX[hour], clockY[hour], 7, 0xFFFF4500);

		// スコア
		MikanDraw->Printf(FONT_NW, SKY_X+10, 5*FONTS_B, "Score:\n %6d", player->getEnergy());
	}

	// 地面
	MikanDraw->DrawBox(0, 0, WIDTH*CHIP, HEIGHT*CHIP, 0xFF000000);
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x < WIDTH; x++)
			MikanDraw->DrawBox((x)*CHIP, (y)*CHIP, CHIP, CHIP,
			0xC0A06A + (sunshine[y][x] *0xFF /SUN_MAX *0x01000000));
	// 壁・草・プレイヤー
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x < WIDTH; x++)
			if (map[y][x] != nullptr)
				map[y][x]->draw(x,y);

	//MikanDraw->Printf(0,0,0,"day:%d,hour:%d ; timer:%d,current:%d", day, hour, timer, current.hour);

	if (!isGame)
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
	srand((unsigned)GetTickCount());
	MikanDraw->CreateTexture(0, NULL, "TEXTURE", TRC_ZERO);
	MikanDraw->LoadFontFile(NULL, "FONT_R");
	MikanDraw->LoadFontFile(NULL, "FONT_B");
	MikanDraw->CreateFont(FONT_BW, "MigMix 1M bold"   , FONTS_B, 0xFFFFFFFF);
	MikanDraw->CreateFont(FONT_BG, "MigMix 1M bold"   , FONTS_B, 0xFF70EE70);
	MikanDraw->CreateFont(FONT_NW, "MigMix 1M regular", FONTS_N, 0xFFFFFFFF);
	MikanDraw->CreateFont(FONT_NR, "MigMix 1M bold", FONTS_N, 0xFFFF4444);
	MikanSound->Load(   BGM, NULL, "WOOD_FORK");
	MikanSound->Load( CLOCK, NULL, "SE_CLOCK");
	MikanSound->Load(  GROW, NULL, "SE_GROW");
	MikanSound->Load(SELECT, NULL, "SE_SELECT");

	// 各時刻の日時計
	for (int i = 0; i < DAY; i++) {
		double clockA = M_PI *i /(DAY-1);
		clockX[i] = CLOCK_X +CLOCK_L*cos(clockA);
		clockY[i] = CLOCK_Y -CLOCK_L*sin(clockA);
	}

	// 壁
	int numWall = 0;
	for (int x = 0; x < WIDTH; x++) {
		map[       0][x] = new Wall();
		map[HEIGHT-1][x] = new Wall();
	}
	for (int y = 1; y < HEIGHT-1; y++) {
		map[y][      0] = new Wall();
		map[y][WIDTH-1] = new Wall();
	}

	map[1][1] = player = new Player(1,1);
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

	// for GAME
	bool isMoved = false;
	// for RESULT
	int score;

	// BODY---------------------
	switch(game) {
	case TITLE:
		if (MikanInput->GetKeyNum(K_Z)) {
			// 草
			while(weedNum++ < 10)
				makeWeed();

			MikanSound->Play(SELECT, false);
			MikanSound->Play(BGM, true);
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
			"成長している程多くの日光を浴びます。\n"
			"%d日間で浴びた日光量の合計が最終得点です。\n"
			"【操作方法】\n"
			"　　矢印ボタン：移動\n"
			"　◯ボタン連打：成長（合計日光量に応じる）",SPAN);
		MikanDraw->Printf(FONT_NR, 50+5*FONTS_N*3/4-1, 150+2*FONTS_N, "日光");
		MikanDraw->Printf(FONT_NR, 50+0*FONTS_N*3/4-0, 150+3*FONTS_N, "成長");
		MikanDraw->Printf(FONT_NR, 50+0*FONTS_N*3/4-0, 150+4*FONTS_N, "%d日間", SPAN);
		MikanDraw->Printf(FONT_NR, 50+5*FONTS_N*3/4-1, 150+7*FONTS_N, "連打");

		break;


	case GAME:
		// INCREMENT
		if (current.sun && !(timer%(HOUR*3/2)))
			makeWeed();

		// SUNSHINE--------------
		sun();

		// MOVING
		for (int y = 1; y < HEIGHT-1; y++)
			for (int x = 1; x < WIDTH-1; x++)
				if (map[y][x] != nullptr && typeid(*map[y][x])==typeid(Weed) && !(timer%HOUR))
						((Weed*)map[y][x])->grow(x,y);

		player->grow();
		player->move();

		// DRAW------------------
		draw(true);

		if (!(timer%HOUR))
			MikanSound->Play(CLOCK, false);


		++timer;

		if (timer /DAY /HOUR >= SPAN) {
			MikanSound->Play(SELECT, false);
			MikanSound->Stop(BGM);
			game = RESULT;
		}
		break;


	case RESULT:
		if (MikanInput->GetKeyNum(K_Z) == 30)
			return 1;

		// 背景
		sun();
		draw(false);

		score = player->getEnergy();
		MikanDraw->Printf(FONT_BG, (WIN_W-4*FONTS_B)/2, 50, "リザルト");
		MikanDraw->Printf(FONT_BW, 50, 150, "合計日光量：%3d,%03d", score/1000, score%1000);
		// 2940 - 568,325, 絶好調：91,845
		if (score < 30000)
			MikanDraw->Printf(FONT_BW, 100, 250, "コケ植物　級");
		else if (score < 41000)
			MikanDraw->Printf(FONT_BW, 100, 250, "シダ植物　級");
		else if (score < 52000)
			MikanDraw->Printf(FONT_BW, 100, 250, "　　低木　級");
		else if (score < 63000)
			MikanDraw->Printf(FONT_BW, 100, 250, "　街路樹　級");
		else if (score < 74000)
			MikanDraw->Printf(FONT_BW, 100, 250, "　　大木　級");
		else if (score < 85000)
			MikanDraw->Printf(FONT_BW, 100, 250, "　御神木　級");
		else if (score < 96000)
			MikanDraw->Printf(FONT_BW, 100, 250, "　屋久杉　級");
		else
			MikanDraw->Printf(FONT_BW, 100, 250, "　世界樹　級");
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
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x < WIDTH; x++)
			if (map[y][x] != nullptr)
				delete map[y][x];
}
