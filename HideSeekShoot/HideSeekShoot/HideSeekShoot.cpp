//23:00-09:30
//所要時間10:30
#include <Mikan.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 640
#define HEIGHT 480
#define FRAME 30

#define TITLE 0
#define GAME 1
#define MOMENT 2
#define RESULT 3

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

#define NPC false
#define PC true

#define PLAYER1 true
#define PLAYER2 false

#define ATTACK true
#define SCROLL false


bool CheckInput(bool player, bool button) {
	int p = 0;
	if(player == PLAYER2) {
		p = 1;
	}

	if(button == ATTACK){
		if(MikanInput->GetPadNum(p, PAD_A) == 1 ||
			MikanInput->GetPadNum(p, PAD_B) == 1 ||
			MikanInput->GetKeyNum(K_Z) == 1) {
				return true;
		}
	} else if(button == SCROLL) {
		if(MikanInput->GetPadNum(p, PAD_C) == 1 ||
			MikanInput->GetPadNum(p, PAD_D) == 1 ||
			MikanInput->GetKeyNum(K_X) == 1) {
				return true;
		}
	}
	return false;
}


struct Bullet {
	const static int size = 12;
	int x;
	int y;
	int d;
};


class Chara {
protected: 	static const int size = 32;
			static const int WaitMax = 300;

			int x;
			int y;
			int d;
			int t;

			Chara(int type) {
				x = FRAME + rand() %(WIDTH - size - FRAME * 2);
				y = FRAME + rand() %(HEIGHT - size - FRAME * 2);
				d = rand() % 4;
				t = type;
			}

			void Revise() {
				if(x < FRAME) {
					x = FRAME;
				} else if(x > WIDTH - size - FRAME) {
					x = WIDTH - size - FRAME;
				} else if(y < FRAME) {
					y = FRAME;
				} else if(y > HEIGHT - size - FRAME) {
					y = HEIGHT - size - FRAME;
				}
			}

public:		void Draw() {
				if(d == UP) {
					MikanDraw->DrawTexture(0, x, y, t * size, 1 * size, size, size, 0);
				} else if(d == RIGHT) {
					MikanDraw->DrawTexture(0, x, y, t * size, 0 * size, size, size, 0);
				} else if(d == DOWN) {
					MikanDraw->DrawTexture(0, x, y, t * size, 1 * size, size, size, DRAW_UD);
				} else if(d == LEFT) {
					MikanDraw->DrawTexture(0, x, y, t * size, 0 * size, size, size, DRAW_LR);
				}
			}

			int getX() {
				return x;
			}
			int getY() {
				return y;
			}
			int getSize() {
				return size;
			}
};

class Computer : public Chara {
private:	const static int LengthMax = 100;
			int l;
			int r;

			void Move() {
				l --;

				if(l <= 0) {
					// "3/7"の確率で静止
					r = rand() % (4 + 3);
					l = rand() % LengthMax;
				}

				if(r == UP) {
					y -= 3;
					d = UP;
				} else if(r == RIGHT) {
					x += 3;
					d = RIGHT;
				} else if(r == DOWN) {
					y += 3;
					d = DOWN;
				} else if(r == LEFT) {
					x -= 3;
					d = LEFT;
				}
			}

public:		Computer(int type) : Chara(type) {
				t = type;
				l = rand() % LengthMax;
				r = rand() % 10;
			}

			void Update() {
				Move();
				Revise();
				Draw();
			}
};

class Player : public Chara {
public:		const static int BulletMax = 3;
private:	int PadNum;
			int BulletNum;
			Bullet bullet[BulletMax];

			void Move() {
				if(MikanInput->GetPadNum(PadNum, PAD_UP) > 0 ||
					MikanInput->GetKeyNum(K_UP) > 0) {
						y -= 3;
						d = UP;
				} else if(MikanInput->GetPadNum(PadNum, PAD_RIGHT) > 0 ||
					MikanInput->GetKeyNum(K_RIGHT) > 0) {
						x += 3;
						d = RIGHT;
				} else if(MikanInput->GetPadNum(PadNum, PAD_DOWN) > 0 ||
					MikanInput->GetKeyNum(K_DOWN) > 0) {
						y += 3;
						d = DOWN;
				} else if(MikanInput->GetPadNum(PadNum, PAD_LEFT) > 0 ||
					MikanInput->GetKeyNum(K_LEFT) > 0) {
						x -= 3;
						d = LEFT;
				}
			}

			void Shoot() {
				int p = PLAYER1;
				if(PadNum == 1) {
					p = PLAYER2;
				}
				if(CheckInput(p, ATTACK) == true) {
						int i = BulletNum;
						if(d == UP) {
							bullet[i].x = x + 10;
							bullet[i].y = y - bullet[i].size;
						} else if(d == RIGHT) {
							bullet[i].x = x + size;
							bullet[i].y = y + 10;
						} else if(d == DOWN) {
							bullet[i].x = x + 10;
							bullet[i].y = y + size;
						} else if(d == LEFT) {
							bullet[i].x = x - bullet[i].size;
							bullet[i].y = y + 10;
						}
						bullet[i].d = d;
						BulletNum ++;

						MikanSound->Play(0, false);
				}
			}

			void BulletMove() {
				for(int i = 0; i < BulletMax; i ++) {
					if(bullet[i].x >= 0 - bullet[i].size + FRAME &&
						bullet[i].x < WIDTH - FRAME &&
						bullet[i].y >= 0 - bullet[i].size + FRAME &&
						bullet[i].y < HEIGHT - FRAME ) {
							if(bullet[i].d == UP) {
								bullet[i].y -= 10;
							} else if(bullet[i].d == RIGHT) {
								bullet[i].x += 10;
							} else if(bullet[i].d == DOWN) {
								bullet[i].y += 10;
							} else if(bullet[i].d == LEFT) {
								bullet[i].x -= 10;
							}
					}
				}
			}

public:		Player(int type, int PlayerNum) : Chara(type) {
				PadNum = PlayerNum;
				BulletNum = 0;
				for(int i = 0; i < 3; i ++) {
					bullet[i].x = -100;
					bullet[i].y = -100;
				}
			}

			void Update() {
				Move();
				Revise();
				Shoot();
				BulletMove();
				Draw();
				BulletDraw();
			}

			void BulletDraw() {
				for(int i = 0; i < BulletMax; i ++) {
					MikanDraw->DrawTexture(0, bullet[i].x, bullet[i].y, 0, size * 2, bullet[i].size, bullet[i].size);
				}
			}

			int getT() {
				return t;
			}
			int getBulletX(int n) {
				return bullet[n].x;
			}
			int getBulletY(int n) {
				return bullet[n].y;
			}
			int getBulletSize() {
				return bullet[0].size;
			}
			int getBulletNum() {
				return BulletNum;
			}
};


Player player1(0, 0), player2(0, 0);
Computer computer[8] = {(0), (0), (0), (0), (0), (0), (0), (0)};

int mode = TITLE;
int sheen = 0;
bool win;


//プレイヤーキャラ決定
//ギャラクシー
//気にしたら負け
void Assign() {
	bool check[10];
	for(int i = 0; i < 10; i ++) {
		check[i] = false;
	}

	int r = rand() % 10;
	r = rand() % 10;
	player1 = Player(r, 0);
	check[r] = true;

	bool done = false;
	while(done == false) {
		int ran = rand() % 10;
		if(check[ran] == false) {
			player2 = Player(ran, 1);
			check[ran] = true;
			done = true;
		}
	}

	int i = 0;
	while(i < 8) {
		int ran = rand() % 10;
		if(check[ran] == false) {
			computer[i] = Computer(ran);
			check[ran] = true;
			i ++;
		}
	}
};

void BulletDidHit(bool PlayerNumber, bool CharaType, int cx, int cy, int cs, int bx, int by, int bs) {
	if(cx <= bx + bs && bx < cx + cs && cy <= by + bs && by < cy + cs) {
		if((PlayerNumber == PLAYER1 && CharaType == PC) || (PlayerNumber == PLAYER2 && CharaType == NPC)) {
			win = PLAYER1;
		} else {
			win = PLAYER2;
		}
		mode = MOMENT;

		MikanSound->Play(1, false);
	}
};

void DrawFrame(int p1Bullet, int p2Bullet) {
	MikanDraw->DrawBox(0, 0, WIDTH, FRAME, 0xff000000);
	MikanDraw->DrawBox(0, HEIGHT - FRAME, WIDTH, FRAME, 0xff000000);
	MikanDraw->DrawBox(0, FRAME, FRAME, HEIGHT - FRAME * 2, 0xff000000);
	MikanDraw->DrawBox(WIDTH - FRAME, FRAME, FRAME, HEIGHT - FRAME * 2, 0xff000000);

	for(int i = 0; i < (3 - p1Bullet); i ++) {
		MikanDraw->DrawTexture(0, (12 + 18) * i + 10, 5, 0, 32 * 2, 12, 12);
	}
	for(int i = 0; i < (3 - p2Bullet); i ++) {
		MikanDraw->DrawTexture(0, (WIDTH - (12 + 18) * 3) + (12 + 18) * i - 10 + 18, 5, 0, 32 * 2, 12, 12);
	}
};


//プログラム起動時に1度だけ実行
//システム設定などを行う
void SystemInit(void) {
	MikanWindow->SetWindowName("Hide_Seek_and_Shoot");
	MikanWindow->SetWindowIcon("GAME_ICON");
}

//MainLoop開始前に1度だけ実行
//テクスチャの読み込みなどのゲーム開始前の準備を行う
void UserInit(void) {
	srand((unsigned) time(NULL));
	MikanDraw->CreateFont(0, 32, 0xFF000000);
	MikanDraw->CreateTexture(0, "data/texture.png", 0xff00ffff);
	MikanSound->Load(0, "data/shoot02.wav", true);
	MikanSound->Load(1, "data/shoot10.wav", true);

	Assign();
}

//1秒間に60回実行
//0を返している間は何度でも実行される
int MainLoop(void) {
	//画面の初期化
	MikanDraw->ClearScreen(0xFFFFFFFF);

	switch(mode) {
	case TITLE:
		switch(sheen) {
		case 0:
			MikanDraw->Printf(0, (WIDTH - 21 * 32 / 2) / 2, 224, "Hide, Seek and Shoot!");
			if(CheckInput(PLAYER1, SCROLL) == true ||
				CheckInput(PLAYER2, SCROLL) == true) {
					sheen ++;
			}
			break;

		case 1:
			MikanDraw->Printf(0, 75, 50,
				"両プレイヤーは以下のいずれかを\n　それぞれ操作キャラとします。\n"
				"NPCを巻き込まないように、\n　相手の操作キャラを撃って下さい。");
			MikanDraw->Printf(0, 125, 350, "それではゲーム開始です！");
			for(int i = 0; i < 10; i++) {
				int size = player1.getSize();
				MikanDraw->DrawTexture(0, (WIDTH - (size * 10 + 20 * 9)) / 2 + i * (size + 20), 250,
					i * size, 1 * size, size, size);
			}
			if(CheckInput(PLAYER1, SCROLL) == true ||
				CheckInput(PLAYER2, SCROLL) == true) {
					sheen ++;
			}
			break;

		default:
			mode = GAME;
		}
		break;

	case GAME:
		player1.Update();
		player1.Draw();
		player1.BulletDraw();
		player2.Update();
		player2.Draw();
		player2.BulletDraw();
		for(int i = 0; i < 8; i ++) {
			computer[i].Update();
			computer[i].Draw();
		}
		DrawFrame(player1.getBulletNum(), player2.getBulletNum());

		for(int i = 0; i < player1.BulletMax; i ++) {
			int cx = player2.getX();
			int cy = player2.getY();
			int cs = player2.getSize();
			int bx = player1.getBulletX(i);
			int by = player1.getBulletY(i);
			int bs = player1.getBulletSize();
			BulletDidHit(PLAYER1, PC, cx, cy, cs, bx, by, bs);

			for(int j = 0; j < 8; j ++) {
				cx = computer[j].getX();
				cy = computer[j].getY();
				cs = computer[j].getSize();
				BulletDidHit(PLAYER1, NPC, cx, cy, cs, bx, by, bs);
			}
		}

		for(int i = 0; i < player2.BulletMax; i ++) {
			int cx = player1.getX();
			int cy = player1.getY();
			int cs = player1.getSize();
			int bx = player2.getBulletX(i);
			int by = player2.getBulletY(i);
			int bs = player2.getBulletSize();
			BulletDidHit(PLAYER2, PC, cx, cy, cs, bx, by, bs);

			for(int j = 0; j < 8; j ++) {
				cx = computer[j].getX();
				cy = computer[j].getY();
				cs = computer[j].getSize();
				BulletDidHit(PLAYER2, NPC, cx, cy, cs, bx, by, bs);
			}
		}

		break;

	case MOMENT:
		player1.Draw();
		player1.BulletDraw();
		player2.Draw();
		player2.BulletDraw();
		for(int i = 0; i < 8; i ++) {
			computer[i].Draw();
		}
		DrawFrame(player1.getBulletNum(), player2.getBulletNum());

		if(CheckInput(PLAYER1, SCROLL) == true ||
			CheckInput(PLAYER2, SCROLL) == true) {
				mode = RESULT;
		}

		break;

	case RESULT:
		if(win == PLAYER1) {
			int size = player1.getSize();
			MikanDraw->Printf(0, (WIDTH - 17 * 32 / 2) / 2, 150, "Winner is Player1!");
			MikanDraw->DrawTexture(0, (WIDTH - size) / 2, 250, player1.getT() * 32, 0, size, size); 
		} else {
			int size = player2.getSize();
			MikanDraw->Printf(0, (WIDTH - 17 * 32 / 2) / 2, 150, "Winner is Player2!");
			MikanDraw->DrawTexture(0, (WIDTH - size) / 2, 250, player2.getT() * 32, 0, size, size); 
		}

		if(CheckInput(PLAYER1, SCROLL) == true ||
			CheckInput(PLAYER2, SCROLL) == true) {
				mode = 100;
		}

		break;

	default:
		return 1;
	}

	if(MikanInput->GetKeyNum(K_ESC) > 0) {
		return 1;
	} else {
		return 0;
	}
}

//MainLoop終了後に1度だけ実行
//後片付けなどを行う
void CleanUp(void) {
}
