#ifndef CONTROL_H_INCLUDED
#define CONTROL_H_INCLUDED
//////キーボードからの入力を格納し、返す関数群///////////

/////////////入力状態/////////////////////
//キーの入力状態
enum {//各行動のcontrol関数からの戻り値 順に、A,S.D,W
	CURE_KEY = 1,

};

//スティックの入力状態
enum {
	RIGHT_KEY = 1,
	LEFT_KEY = 180,
	UP_KEY = 3,
	DOWN_KEY = 4
};
////////////////////////////////////////////

int UpdateKey();

void mouse_limit(int& hand_x, const int hand_y);
//左クリックの状態
void mouse_click();
int left_click();
int right_click();

//スティックの入力状態
int control_stick();
int control_stick2();

//キーの入力状態
int control_cure();
int control_q();
int control_quit();

//FPS表示のオンオフ
int control_debug();

//スペースキー
int control_space();

//ESCキー：ゲーム終了
int control_esc();

#endif