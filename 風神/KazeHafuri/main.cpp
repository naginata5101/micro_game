#include "head.h"


Item::Item() {
	x = rand() % LENGTH;
	y = -150;

	wing = rand() % maxWing + 1 + 1; // �ŏ��������傫��
	weight = rand() % maxWeight + 1;

	status = YET;
}

void			Item::move() {
	if (status == FALL) {
		const int Y = y + wing * standWing + weight * standWeight;
		const int w = wing * standWing / 2;

		x += wind * wing / 3;
		y += speed * weight / wing;

		if (x - w < 0)		x = w;
		if (x + w > LENGTH)	x = LENGTH - w;

		if (GROUND - boatH <= Y && Y < GROUND && boatX - boatW / 2 <= x && x < boatX + boatW / 2) {
			status = SUCCESS;
			MikanSound->Play(SE_SAVE, false);
		} else if (GROUND < Y) {
			status = FAIL;
			MikanSound->Play(SE_FALL, false);
		}
	}
}

Item::Status	Item::getStatus() {
	return status;
}

bool			Item::startFall() {
	if (status == YET) {
		status = FALL;
		return true;
	}
	return false;
}

void			Item::draw() {
	if (status == FALL) {
		const int wingS = wing * standWing;
		const int weightS = weight * standWeight;

		MikanDraw->DrawTextureScalingC(T_WING  , x + FRAME, y + wingS           / 2, 0, 0, standWing  , standWing  , wing  );
		MikanDraw->DrawTextureScalingC(T_WEIGHT, x + FRAME, y + wingS + weightS / 2, 0, 0, standWeight, standWeight, weight);
		//MikanDraw->DrawBoxC(x + FRAME, y + wingS / 2, wingS, wingS, 0xFF00FF00);
		//MikanDraw->DrawBoxC(x + FRAME, y + wingS + weightS / 2, weightS, weightS, 0xFFFF0000);
	}
}

//-------------------------------
bool titleUpdate() {
	mainDraw();
	MikanDraw->DrawBox(0, 0, WIDTH, HEIGHT, 0xCC000000);

	MikanDraw->Printf(F_BW, (WIDTH - FS_B *  4) / 2, 150, "�w���_�x");
	MikanDraw->Printf(F_MW, (WIDTH - FS_M * 10) / 2, 250, "���{�^���ŃX�^�[�g�I");
	MikanDraw->Printf(F_MW, (WIDTH - FS_M * 11) / 2, 300, "���E�{�^���ŕ��͑���!!");

	if (MikanInput->GetKeyNum(K_Z) == 1) {
		mainInit();
		return true;
	}
	return false;
}

void mainInit() {
	wind = 0;
	timer = 0;
}

bool mainUpdate() {
	int falledNum = 0;
	int fallInter;

	// ���͐���
	if (MikanInput->GetKeyNum(K_RIGHT) == 1)	++wind;
	if (MikanInput->GetKeyNum(K_LEFT ) == 1)	--wind;
	if (wind > windMax)		wind = windMax;
	if (wind < -windMax)	wind = -windMax;

	// boat����
	boatX += boatS * (boatRight ? 1 : -1) + wind / 5;
	if (boatX - boatW / 2 < 0) {
		boatX = boatW / 2;
		boatRight = true;
	}
	if (boatX + boatW / 2 > LENGTH) {
		boatX = LENGTH - boatW / 2;
		boatRight = false;
	}

	// item����
	for (int i = 0; i < maxItem; i++)
		if (item[i].getStatus() == Item::Status::SUCCESS || item[i].getStatus() == Item::Status::FAIL)
			++falledNum;

	if		(falledNum < 5)		fallInter = 5;
	else if (falledNum < 15)	fallInter = 4;
	else if (falledNum < 25)	fallInter = 3;
	else if (falledNum < 40)	fallInter = 2;
	else						fallInter = 1;

	for (int i = 0; i < maxItem; i++) {
		if (timer % 2 == 0)	item[i].move();
		if (timer % (60 * fallInter) == 0 && item[i].startFall())	break;
	}

	++timer;
	//timer += 60 * 5;

	if (falledNum == maxItem)	return true;
	return false;
}

void mainDraw() {
	// background
	MikanDraw->DrawBox(FRAME, 0, LENGTH, GROUND, 0xFF87CEFA);
	MikanDraw->DrawBox(FRAME, GROUND, LENGTH, HEIGHT - GROUND, 0xFF0000E0);

	// boat
	MikanDraw->DrawTextureC(T_BOAT, boatX + FRAME, GROUND - boatH / 2, 0, 0, boatW, boatH, (boatRight ? 0 : DRAW_LR));

	// wave
	const int ws = WAVE_SIZE;
	for (int i = 0; i <= LENGTH / ws; i++)
		if (wind < -2 || 2 < wind)
			MikanDraw->DrawTexture(T_WAVE, i * ws + FRAME - ((timer * wind / 120) % 2 ? ws / 2 : 0),
			GROUND - ws, ws, ws, ws, ws, (wind > 0 ? 0 : DRAW_LR));

	// item
	for (int i = 0; i < maxItem; i++)	item[i].draw();

	// frame
	MikanDraw->DrawBox(            0, 0, FRAME, HEIGHT, 0xFF000000);
	MikanDraw->DrawBox(WIDTH - FRAME, 0, FRAME, HEIGHT, 0xFF000000);

	// info
	int failNum = 0;
	int successNum = 0;
	for (int i = 0; i < maxItem; i++) {
		Item::Status s = item[i].getStatus();
		if (s == Item::Status::FAIL)			++failNum;
		else if (s == Item::Status::SUCCESS)	++successNum;
	}

	if (mode != Mode::TITLE) {
		MikanDraw->Printf(F_MW, 0, 0, "%02d:%02d \n(%s)%02d",
			timer / 60 / 60, (timer / 60) % 60, (wind >= 0 ? (wind == 0 ? "�E" : "��") : "��"), abs(wind));
		MikanDraw->Printf(F_MW, FRAME + LENGTH, 0, "�S:%2d \n�~:%2d \n��:%2d", maxItem, successNum, failNum);
	}
}

bool resultUpdate() {
	int score = 0;
	for (int i = 0; i < maxItem; i++)
		if (item[i].getStatus() == Item::Status::SUCCESS)	++score;

	mainDraw();
	MikanDraw->DrawBox(0, 0, WIDTH, HEIGHT, 0xCC000000);
	MikanDraw->Printf(F_BW, (WIDTH - FS_B * 7) / 2, 150, "�Q�[���I�[�o�[");
	MikanDraw->Printf(F_MW, (WIDTH - FS_M * 4) / 2, 250, "���_�F%2d", score);

	if (MikanInput->GetKeyNum(K_Z) == 1)	return true;
	return false;
}


//�v���O�����N������1�x�������s
//�V�X�e���ݒ�Ȃǂ��s��
void SystemInit() {
	MikanWindow->SetWindowName("���_");
	MikanWindow->SetWindowIcon("GAME_ICON");

	srand((unsigned)time(NULL));
}

//MainLoop�J�n�O��1�x�������s
//�e�N�X�`���̓ǂݍ��݂Ȃǂ̃Q�[���J�n�O�̏������s��
void UserInit() {
	MikanDraw->CreateFont(F_BW, "�l�r �S�V�b�N", FS_B, 0xFFFFFFFF);
	MikanDraw->CreateFont(F_MW, "�l�r �S�V�b�N", FS_M, 0xFFFFFFFF);
	MikanDraw->CreateFont(F_SW, "�l�r �S�V�b�N", FS_S, 0xFFFFFFFF);

	MikanDraw->CreateTexture(T_BOAT  , NULL, "BOAT"  , TRC_ZERO);
	MikanDraw->CreateTexture(T_WAVE  , NULL, "WAVE"  , TRC_ZERO);
	MikanDraw->CreateTexture(T_WEIGHT, NULL, "HUMAN" , 0xFFFFFFFF);
	MikanDraw->CreateTexture(T_WING  , NULL, "WING"  , TRC_ZERO);

	MikanSound->Load(S_BGM  , NULL, "BGM");
	MikanSound->Load(SE_SAVE, NULL, "SAVE");
	MikanSound->Load(SE_FALL, NULL, "FALL");
	MikanSound->Load(SE_END , NULL, "END");

	mode = TITLE;
}

//1�b�Ԃ�60����s
//0��Ԃ��Ă���Ԃ͉��x�ł����s�����
int MainLoop() {
	//��ʂ̏�����
	MikanDraw->ClearScreen();

	switch (mode) {
	case TITLE:
		if (titleUpdate()) {
			mode = MAIN;
			MikanSound->Play(S_BGM, true);
		}
		break;
	case MAIN:
		if (mainUpdate()) {
			mode = RESULT;
			MikanSound->StopAll();
			MikanSound->Play(SE_END, false);
		}
		mainDraw();
		break;
	case RESULT:
		if (resultUpdate())	mode = EXIT;
		break;
	default:
		return 1;
	}

	if (MikanInput->GetKeyNum(K_ESC) || MikanInput->GetKeyNum(K_X) > 60) return 1;
	return 0;
}

//MainLoop�I�����1�x�������s
//��Еt���Ȃǂ��s��
void CleanUp() {
}
