#include <Mikan.h>
#include <time.h>

#define HEIGHT	480
#define WIDTH	640

int const bridgeHei = 100;
const int streamHei = 120;
const int streamWid = 60;
int const riverWid = streamWid * 7;
const int riverLeft = (WIDTH - riverWid) / 2;
const int riverRight = WIDTH - riverLeft;
const int maxLength = 2000 / streamHei;
const int maxGimmick = 100;
const int streamPower = 3;
const int viewSpase = 20;

enum GameMode {
	TITLE, MAIN, CLEAR, FALL
} gameMode = TITLE;

int stage = 0;
int stageLength;
double streamX[maxLength], streamY[maxLength];
int gimmickNum;
int viewPoint;
int trial;

int getRiverDrawY(int position) {	//position:スタート地点からの距離（大きいほど上にある）
	return HEIGHT - bridgeHei - (position - viewPoint);
}

bool isHit(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
	if (x1 < x2 + w2 && x2 < x1 + w1 && y1 < y2 + h2 && y2 < y1 + h1)
		return true;
	return false;
}

class Gimmick{
private:	static const int h = 15;
private:	int x, y, w;

public:		void init(int X, int Y, int W) {
				x = X;
				y = Y;
				w = W;
			}
public:		int getX() {
				return x;
			}
public:		int getY() {
				return y;
			}
public:		int getW() {
				return w;
			}
public:		static int getH() {
				return h;
			}
public:		void draw(int view) {
				MikanDraw->DrawTexture(0, x + riverLeft, getRiverDrawY(y), streamWid, 0, w, h);
			}
} gimmick[maxGimmick];

class Boat {
private:	static const int w = 5;
private:	static const int h = 10;
private:	double x, y;
private:	double speedX, speedY;

public:		void update() {
				x += speedX;
				y += speedY;
				speedX += -(speedX / 2);
				speedY += -(speedY / 2);
				if (x < w / 2) {
					x = w / 2;
					speedX = 0;
				}
				if (x > riverWid - w / 2) {
					x = riverWid - w / 2;
					speedX = 0;
				}
			}
public:		void beCarried(int X) {
				x = X - riverLeft;
				speedX = speedY = 0;
				y = h / 2;
			}
public:		void bePushued(double powerX, double powerY) {
				speedX += powerX;
				speedY += powerY;
			}
public:		void draw(int view) {
				MikanDraw->DrawBoxC(x + riverLeft, HEIGHT - bridgeHei - (y - view), w, h, 0xFF00FF00);
			}
public:		int getX() {
				return x;
			}
public:		int getY() {
				return y;
			}
public:		static int getW() {
				return w;
			}
public:		static int getH() {
				return h;
			}
} boat;

class Player{
private:	static const int w = 60;
private:	static const int h = 50;
private:	static const int speed = 4;
private:	static const int y = HEIGHT - bridgeHei + h / 2;
private:	int x;
private:	bool haveBoat;

public:		void init() {
				x = WIDTH / 2;
				haveBoat = true;
			}
public:		void update(int keyType) {
				if (keyType == K_LEFT)	x -= speed;
				if (keyType == K_RIGHT)	x += speed;

				int W = Boat::getW();
				if (x < riverLeft  + W / 2)	x = riverLeft  + W / 2;
				if (x > riverRight - W / 2)	x = riverRight - W / 2;

				if (haveBoat) boat.beCarried(x);
			}
public:		void releaseBoat() {
				haveBoat = false;
			}
public:		void draw() {
				MikanDraw->DrawBoxC(x, y, w, h, 0xFFFFFF00);
			}
} player;

void gameInit() {
	const int standLen = HEIGHT - bridgeHei - viewSpase;
	const int standGim = 5;
	const int maxGimmickW = 100;
	const int minGimmickW = 40;

	switch(stage) {
	case 0:
		stageLength = standLen * 1;
		gimmickNum = standGim * 1;
		break;
	case 1:
		stageLength = standLen / 2 * 3;
		gimmickNum = standGim * 2;
		break;
	case 2:
		stageLength = standLen * 3;
		gimmickNum = standGim * 4;
		break;
	}

	//流れ(stream)の設定
	streamX[0] = 0;
	int streamNum = stageLength / streamHei;
	for (int i = 1; i <= streamNum; i++) {
		double r = rand() % (10 + 10 + 1 + 1); //直前の流れより -1.0~0.0~1.0 の間で増減する
		double power = streamX[i - 1] + (r - 11) / 10;
		if (abs(power) * sqrt(2) > streamPower) --i;
		else streamX[i] = power;
	}
	for (int i = 0; i <= streamNum; i++)
		streamY[i] = sqrt(streamPower * streamPower - streamX[i] * streamX[i]);

	//障害物(gimmick)の設定
	for (int i = 0; i < gimmickNum; i++) {
		int x = rand() % (riverWid - minGimmickW + 1);
		int y = (rand() % ((stageLength - Gimmick::getH()) / streamHei) + 1) * streamHei;
		int w = rand() % (maxGimmickW - minGimmickW + 1) + minGimmickW;

		bool flag = true;
		for (int j = 0; j < i && flag; j++) {
			Gimmick g = gimmick[j];
			if (isHit(x, y, w, Gimmick::getH(), g.getX(), g.getY(), g.getW(), g.getH()) ||
				x + w > riverWid) flag = false;
		}
		if (flag)	gimmick[i].init(x, y, w);
		else		--i;
	}

	viewPoint = 0;
	stage = 0;
	trial = 1;

	player.init();
}

GameMode checkGame() {
	if (MikanInput->GetKeyNum(K_X) == 1) {
		stage = 0;
		return GameMode::TITLE;
	}

	if (boat.getY() > stageLength) return CLEAR;
	for (int i = 0; i < gimmickNum; i++) {
		Gimmick g = gimmick[i];
		if (isHit(boat.getX() - boat.getW() / 2, boat.getY() - boat.getH() / 2, boat.getW(), boat.getH(), g.getX(), g.getY(), g.getW(), g.getH()))
			return FALL;
	}
	return MAIN;
}

void moveView(){
	const int viewSpeed = 5;
	const int maxView = stageLength + viewSpase - (HEIGHT - bridgeHei);
	if (MikanInput->GetKeyNum(K_UP  ) > 0) viewPoint += viewSpeed;
	if (MikanInput->GetKeyNum(K_DOWN) > 0) viewPoint -= viewSpeed;
	if (viewPoint < 0)				viewPoint = 0;
	if (viewPoint > maxView)	viewPoint = maxView;
}

void update() {
	int boatY = boat.getY() / streamHei;

	if (MikanInput->GetKeyNum(K_Z) == 1) player.releaseBoat();
	player.update(MikanInput->GetKeyWhichButton());

	boat.bePushued(streamX[boatY], streamY[boatY]);
	boat.update();
}

void drawBridge() {
	const int bridgeTop = HEIGHT - bridgeHei;
	const int boardInter = 40;

	MikanDraw->DrawBox(0, bridgeTop, WIDTH, bridgeHei, 0xFFA0522D);
	for (int i = 0; i < WIDTH / boardInter; i++)
		MikanDraw->DrawBox(i * boardInter - 1, bridgeTop, 2, bridgeHei, 0xFF000000);

	player.draw();
}

void drawRiver() {
	MikanDraw->DrawBox(riverLeft, 0, riverWid, HEIGHT - bridgeHei, 0xFF3333FF);

	// 流れの描画
	int w = streamWid;
	int h = streamHei;
	for (int y = 0; y <= stageLength / h; y++)
		for (int x = 0; x < riverWid / w; x++)
			MikanDraw->DrawTextureRotationC(0, x * w + w / 2 + riverLeft, getRiverDrawY(y * h + h / 2),
			0, 0, w, h, streamX[y] / streamY[y]);
	// 障害物の描画
	for (int i = 0; i < gimmickNum; i++) 
		gimmick[i].draw(viewPoint);

	MikanDraw->DrawBox(riverLeft, getRiverDrawY(stageLength) - 5, riverWid, 5, 0xFFFF0000);
	boat.draw(viewPoint);

	MikanDraw->DrawBox(0, 0, riverLeft, HEIGHT, 0xFF888888);
	MikanDraw->DrawBox(riverRight, 0, WIDTH - riverRight, HEIGHT, 0xFF888888);
}

//-------------------------------------
GameMode GameTitle() {
	MikanDraw->Printf(0, 100, 100, "TITLE");
	MikanDraw->Printf(0, 100, 200, "stage:%d", stage);

	if (MikanInput->GetKeyNum(K_RIGHT) == 1) ++stage;
	if (MikanInput->GetKeyNum(K_LEFT ) == 1) --stage;
	if (stage < 0) stage = 2;
	if (stage > 2) stage = 0;

	if (MikanInput->GetKeyNum(K_Z) == 1) {
		gameInit();
		return GameMode::MAIN;
	}
	return GameMode::TITLE;
}

GameMode GameMain() {
	update();
	moveView();
	drawRiver();
	drawBridge();

	MikanDraw->Printf(1, 0,  0, "挑戦回数：\n %d", trial);
	MikanDraw->Printf(1, 0, 50, "視点位置：\n %d～%d/%d", viewPoint, viewPoint + HEIGHT - bridgeHei, stageLength);
	MikanDraw->Printf(1, 0,100, "笹船座標：\n (%d, %d)", boat.getX(), boat.getY());
	MikanDraw->Printf(0, riverRight, 100, "Xキーで\nタイトル\nに戻る");

	return checkGame();
}

GameMode GameClear() {
	MikanDraw->DrawBox(0, 0, WIDTH, HEIGHT, 0x66000000);
	moveView();
	drawRiver();
	drawBridge();

	MikanDraw->Printf(0, 150, 100, "clear!");
	MikanDraw->Printf(0, 150, 200, "挑戦%d回目", trial);
	if (MikanInput->GetKeyNum(K_Z) == 1) {
		stage = 0;
		return GameMode::TITLE;
	}
	return GameMode::CLEAR;
}

GameMode GameFall() {
	MikanDraw->DrawBox(0, 0, WIDTH, HEIGHT, 0x66000000);
	moveView();
	drawRiver();
	drawBridge();

	MikanDraw->Printf(0, 150, 100, "fall...");
	MikanDraw->Printf(0, 150, 200, "挑戦%d回目", trial);
	if (MikanInput->GetKeyNum(K_Z) == 1) {
		player.init();
		viewPoint = 0;
		++trial;
		return GameMode::MAIN;
	}
	return GameMode::FALL;
}


//プログラム起動時に1度だけ実行
//システム設定などを行う
void SystemInit() {
	MikanWindow->SetWindowName("笹船流し");
	MikanWindow->SetWindowIcon("GAME_ICON");
}

//MainLoop開始前に1度だけ実行
//テクスチャの読み込みなどのゲーム開始前の準備を行う
void UserInit() {
	srand((unsigned)time(NULL));

	MikanDraw->CreateTexture(0, NULL, "IMAGES", TRC_ZERO);
	MikanDraw->CreateFont(0, "IPAex ゴシック", 32, 0xFFFFFF00);
	MikanDraw->CreateFont(1, "IPAex ゴシック", 16, 0xFFFFFFFF);
	player.init();
}

//1秒間に60回実行
//0を返している間は何度でも実行される
int MainLoop() {
	//画面の初期化
	MikanDraw->ClearScreen();

	switch(gameMode) {
	case TITLE:
		gameMode = GameTitle();
		break;
	case MAIN:
		gameMode = GameMain();
		break;
	case CLEAR:
		gameMode = GameClear();
		break;
	case FALL:
		gameMode = GameFall();
		break;
	default:
		gameMode = TITLE;
	}


	if (MikanInput->GetKeyNum(K_ESC) > 0) return 1;
	return 0;
}

//MainLoop終了後に1度だけ実行
//後片付けなどを行う
void CleanUp() {
}
