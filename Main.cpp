#include "DxLib.h"
#include"Control.h"
#include"Dash.h"
#include"Text.h"
#include"SDvalue.h"
#include"Menu.h"
#include<string>
#include <math.h>

using namespace std;

static int WINDOW = FALSE;

///////fps�̒���///////////////
static int mStartTime;
static int mCount;
static int debug = FALSE;
static float mFps;
static const int N = 30;
static const int FPS = 30;

bool Update() {
	if (mCount == 0) {
		mStartTime = GetNowCount();
	}
	if (mCount == N) {
		int t = GetNowCount();
		mFps = 1000.f / ((t - mStartTime) / (float)N);
		mCount = 0;
		mStartTime = t;
	}
	mCount++;
	return true;
}

void Draw() {
	DrawFormatString(0, 0, GetColor(255, 255, 255), "%.1f", mFps);
}

void Wait() {
	int tookTime = GetNowCount() - mStartTime;
	int waitTime = mCount * 1000 / FPS - tookTime;
	if (waitTime > 0) {
		Sleep(waitTime);
	}
}

//////�^�C�g�����///////////
int student_handle[10][2];
int title_font;
int title_small_font;
//�^�C�g����ʃ��[�h
void load_title() {
	//�t�H���g
	title_font = CreateFontToHandle(NULL, 80, 8);
	title_small_font = CreateFontToHandle(NULL, 30, 8);
	//�摜
	string name;
	for (int i = 0; i < 10; i++) {
		name = "picture/title/";
		name += st_name[i];
		student_handle[i][0] = LoadGraph((name + "OP1.png").c_str());
		student_handle[i][1] = LoadGraph((name + "OP2.png").c_str());
	}
	change_music("music/12345.mp3", true, true);
}

//�^�C�g�����
void play_title() {
	static int title_cnt[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90};
	static const double PI = 3.14;
	static const double ex = 0.37;
	static const int speed = 12;
	for (int i = 0; i < 10; i++) {
		title_cnt[i]++;
	}
	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, WHITE, TRUE);

	
	DrawRotaGraph(1100, 500, ex, 3 * PI / 2, student_handle[0][0], TRUE, FALSE);
	DrawRotaGraph(1100, 850, ex, 7 * PI / 4, student_handle[6][title_cnt[6] / speed % 2], TRUE, FALSE);
	DrawRotaGraph(800, 850, ex, 0, student_handle[3][title_cnt[3] / speed % 2], TRUE, FALSE);
	DrawRotaGraph(500, 850, ex, 0, student_handle[5][title_cnt[5] / speed % 2], TRUE, FALSE);
	DrawRotaGraph(200, 800, ex, 1 * PI / 4, student_handle[8][title_cnt[8] / speed % 2], TRUE, FALSE);
	DrawRotaGraph(150, 550, ex, 1 * PI / 2, student_handle[1][title_cnt[1] / speed % 2], TRUE, FALSE);
	DrawRotaGraph(200, 220, ex, 3 * PI / 4, student_handle[7][title_cnt[7] / speed % 2], TRUE, FALSE);
	DrawRotaGraph(500, 150, ex, PI, student_handle[4][title_cnt[4] / speed % 2], TRUE, FALSE);
	DrawRotaGraph(800, 150, ex, PI, student_handle[9][title_cnt[9] / speed % 2], TRUE, FALSE);
	DrawRotaGraph(1150, 200, ex, 5 * PI / 4, student_handle[2][title_cnt[2] / speed % 2], TRUE, FALSE);

	if (title_cnt[0] / 30 % 5 == 0 && title_cnt[0] / 10 % 2 == 1) {
		DrawStringToHandle(430, 450, " (School)'", BLACK, title_font);
	}
	else {
		DrawStringToHandle(430, 450, "School Dash", BLACK, title_font);
	}
	DrawStringToHandle(530, 550, "click to start", BLACK, title_small_font);
	DrawBox(0, 990, 330, 1024, LIGHT_GREEN, TRUE);
	DrawStringToHandle(0, 990, "produced by �̂�����", BLACK, title_small_font);
}
//////////////////////////////

//////////���C���֐�///////////////////////
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetWindowSizeChangeEnableFlag(TRUE);//window�T�C�Y�ύX�\
	SetUseDirectInputFlag(TRUE);
	SetGraphMode(1280, 1024, 16);
	ChangeWindowMode(WINDOW), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);
	//SetAlwaysRunFlag(TRUE);//��ʂ���ɃA�N�e�B�u
	SetMainWindowText("School Dash");
	////�}�E�X�֘A////
	SetMouseDispFlag(TRUE);//�}�E�X�\��
	//SetMousePoint(320, 240);//�}�E�X�J�[�\���̏����ʒu
	//SetDrawMode(DX_DRAWMODE_BILINEAR);
	SetDrawMode(DX_DRAWMODE_NEAREST);
	//�Q�[���{��
	SchoolDash game;
	load_title();
	bool title_flag = false;//true�Ȃ�^�C�g����ʏI��
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		UpdateKey();
		mouse_click();

		/////���C��////
		if (GetASyncLoadNum() >= 1) {//�񓯊��ǂݍ��ݒ�
			DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, WHITE, TRUE);
			DrawExtendGraph(0, 0, GAME_WIDE, GAME_HEIGHT, loading_haikei, TRUE);//�w�i�摜
		}
		else if (!title_flag) {
			play_title();
			//�N���b�N�ŃQ�[���J�n
			if (left_click() == 1 || right_click() == 1) {
				title_flag = true;
				DeleteFontToHandle(title_font);
				DeleteFontToHandle(title_small_font);
				change_music("none", false, true);
			}
		}
		else {
			game.play();
			play_music();
		}
		///////////////

		//FPS����
		if (control_debug() == TRUE) {
			if (debug == FALSE) { debug = TRUE; }
			else { debug = FALSE; }
		}
		Update();
		if (debug == TRUE) { Draw(); }
		Wait();
		if (control_esc() == TRUE) { DxLib_End(); }
		//FPS���삱���܂�
	}

	DxLib_End(); // DX���C�u�����I������
	return 0;
}