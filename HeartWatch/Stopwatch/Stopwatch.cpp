#include <Mikan.h>
#include <time.h>

#define WIDTH 640
#define HEIGHT 480
#define BOTTON K_SPACE

const int MaxInterval = 1 * 60;
const int FontSize = 30;
const int TexSize = 20;
const int MaxHeart = 100;


class Heart {
private :bool exit;
private :int x;
private :int y;

public :Heart::Heart() {
			exit = false;
		}

public :void Draw() {
			if(exit == true) {
				MikanDraw->DrawTexture(0, x, y, 0, 0, TexSize, TexSize );
			}
		}

public :void Make() {
			exit = true;
			x = rand() % (WIDTH - TexSize);
			y = rand() % (HEIGHT - TexSize);
		}

public :bool GetExit() {
			return exit;
		}
};

class Watch {
private :int RequestPush;
private :int RequestTime;

private :bool work;

private :int timer;
private :int interval;
private :int push;
private :int score;

private :Heart heart[MaxHeart];


public :Watch::Watch() {}

public :Watch::Watch(int ReqTime, int ReqPush) {
			RequestTime = ReqTime;
			RequestPush = ReqPush;

			work = true;

			timer = 0;
			interval = 0;
			push = 0;

			for(int i = 0; i < MaxHeart; i ++) {
				heart[i] = Heart();
			}
		}

public :void Work() {
			Timer();
			Push();
			CheckEnd();
		}

private :void Timer() {
			 if(work == true) {
				 timer ++;
				 interval ++;
			 }
		 }


private :void Push() {
			 if(work == true && MikanInput->GetKeyNum(BOTTON) == 1) {
				 interval = 0;
				 push ++;
				 MakeHeart();
			 }
		 }

private :void CheckEnd() {
			 if(work == true && (interval > MaxInterval || push >= RequestPush)) {
				 work = false;
				 score = Score();
			 }
		 }

private :int Score() {
			 int sc;

			 if(push >= RequestPush) {
				 sc = 3000 - 100 * abs((RequestTime - timer) / 60);

				 if(sc < 1) {
					 sc = 1;
				 }
			 } else {
				 sc = 0;
			 }

			 return sc;
		 }

private :void MakeHeart() {
			 for(int i = 0; i < MaxHeart; i ++) {
				 if(heart[i].GetExit() == false) {
					 heart[i].Make();
					 break;
				 }
			 }
		 }

public :void Draw() {
			for(int i = 0; i < MaxHeart; i ++) {
				heart[i].Draw();
			}

			if(work == false) {
				MikanDraw->Printf(1, 3 * FontSize, 9 * FontSize, "The stopwatch stops.");
				MikanDraw->Printf(0, FontSize, 11 * FontSize,
					"���ێ��ԁF%d�@�@���ۃn�[�g���F%d\n"
					"�X�R�A�F%d",
					timer / 60, push, score);
			} else {
				MikanDraw->Printf(2, 3 * FontSize, 9 * FontSize, "The stopwatch is working.");
			}
		}
};


void DrawInformation(int ReqTime, int ReqPush, bool game) {
	MikanDraw->Printf( 0, FontSize, 1 * FontSize,
		"�w�莞�Ԃ҂�����Ŏw�萔�̃n�[�g����낤�I\n"
		"�{�^�����������ƂŃn�[�g�����邼�I\n\n"
		"�A���A1�b�ȏ�{�^���������Ȃ��ł����\n"
		"�Q�[���I�[�o�[�ɂȂ邼�I�I\n\n"
		"�w�莞�ԁF%d�@�@�w��n�[�g���F%d",
		ReqTime / 60, ReqPush);

	if(game == false) {
		MikanDraw->Printf(1, 3 * FontSize, 9 * FontSize, "The stopwatch stops.");
	}
}


bool game = false;
int RequestPush;
int RequestTime;
Watch watch = Watch();


//�v���O�����N������1�x�������s
//�V�X�e���ݒ�Ȃǂ��s��
void SystemInit( void ) {
	MikanWindow->SetWindowName( "�n�[�g�ʎY�Q�[��" );
	MikanWindow->SetWindowIcon( "GAME_ICON" );
}

//MainLoop�J�n�O��1�x�������s
//�e�N�X�`���̓ǂݍ��݂Ȃǂ̃Q�[���J�n�O�̏������s��
void UserInit( void ) {
	MikanDraw->CreateFont(0, FontSize, 0xFFFFFF00);
	MikanDraw->CreateFont(1, FontSize, 0xFFFF0000);
	MikanDraw->CreateFont(2, FontSize, 0xFF00FFFF);
	MikanDraw->CreateTexture(0, "heart.png", TRC_ZERO);

	srand((unsigned)time(NULL));

	RequestPush = 80 + rand() % 21;
	RequestTime = (20 + rand() % 21) * 60;
}

//1�b�Ԃ�60����s
//0��Ԃ��Ă���Ԃ͉��x�ł����s�����
int MainLoop( void ) {
	//��ʂ̏�����
	MikanDraw->ClearScreen();

	if(game == false) {

		if(MikanInput->GetKeyNum(BOTTON) > 0) {
			game = true;
			watch = Watch(RequestTime, RequestPush);
		}
	} else {
		watch.Work();
		watch.Draw();
	}

	DrawInformation(RequestTime, RequestPush, game);


	if(MikanInput->GetKeyNum(K_ESC) > 0) {
		return 1;
	}
	return 0;
}

//MainLoop�I�����1�x�������s
//��Еt���Ȃǂ��s��
void CleanUp( void ) {
}
