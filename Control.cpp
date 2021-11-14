#include "DxLib.h"
#include "control.h"


//////////キーボードの入力状態を格納する////////////////

int Key[256]; // キーが押されているフレーム数を格納する

			  // キーの入力状態を更新する
int UpdateKey() {
	char tmpKey[256]; // 現在のキーの入力状態を格納する
	GetHitKeyStateAll(tmpKey); // 全てのキーの入力状態を得る
	for (int i = 0; i<256; i++) {
		if (tmpKey[i] != 0) { // i番のキーコードに対応するキーが押されていたら
			Key[i]++;     // 加算
		}
		else {              // 押されていなければ
			Key[i] = 0;   // 0にする
		}
	}
	return 0;
}

void mouse_limit(int& hand_x, const int hand_y) {
	if (hand_x < 0 && hand_y > 0 && hand_y < 480) {
		hand_x = 0;
		SetMousePoint(0, hand_y);//マウスカーソルの位置
	}
	else if (hand_x > 640 && hand_y > 0 && hand_y < 480) {
		hand_x = 640;
		SetMousePoint(640, hand_y);//マウスカーソルの位置
	}
}

///////////マウスの入力状態を返す//////////////
static int left_cnt = 0;
static int right_cnt = 0;
void mouse_click() {
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) { left_cnt++; }
	else { left_cnt = 0; }
	if (GetMouseInput() == MOUSE_INPUT_RIGHT) { right_cnt++; }
	else { right_cnt = 0; }
}
int left_click() {
	return left_cnt;
}
int right_click() {
	return right_cnt;
}


//左右の移動
int control_stick()
{
	//同時入力には長時間押している方を採用
	if (Key[KEY_INPUT_D] >= 1 && Key[KEY_INPUT_A] >= 1) {
		if (Key[KEY_INPUT_D] > Key[KEY_INPUT_A]) {
			return RIGHT_KEY;
		}
		else if (Key[KEY_INPUT_D] <= Key[KEY_INPUT_A]) {
			return LEFT_KEY;
		}
	}
	else if (Key[KEY_INPUT_D] >= 1) { // 右キーが押されていたら
		return RIGHT_KEY;
	}
	else if (Key[KEY_INPUT_A] >= 1) { // 左キーが押されていたら
		return LEFT_KEY;
	}
	return FALSE;
}

//左右の移動
int control_stick2()
{
	if (Key[KEY_INPUT_S] >= 1 && Key[KEY_INPUT_W] >= 1) {
		if (Key[KEY_INPUT_S] > Key[KEY_INPUT_W]) {
			return DOWN_KEY;
		}
		else if (Key[KEY_INPUT_S] <= Key[KEY_INPUT_W]) {
			return UP_KEY;
		}
	}
	else if (Key[KEY_INPUT_S] >= 1) { // 下キーが押されていたら
		return DOWN_KEY;
	}
	else if (Key[KEY_INPUT_W] >= 1) { // 下キーが押されていたら
		return UP_KEY;
	}
	return FALSE;
}

//防御
int control_cure()
{
	// Eキーが押されたら回復
	if (Key[KEY_INPUT_E] >= 1) {
		return CURE_KEY;
	}
	return FALSE;
}

//Qキー（探索終了）
int control_q()
{
	// Qキーが押された瞬間
	if (Key[KEY_INPUT_Q] == 1) {
		return TRUE;
	}
	return FALSE;
}

//Qキー（イベント終了)
int control_quit()
{
	// Qキーが押された瞬間
	if (Key[KEY_INPUT_Q] == 30) {
		return TRUE;
	}
	return FALSE;
}

//デバッグモード起動用
int control_debug() {
	if (Key[KEY_INPUT_F] == 1) { // Fキーが押されていたら
		return TRUE;
	}
	return FALSE;
}

//spaceキー
int control_space()
{
	// spaceキーが押された瞬間
	if (Key[KEY_INPUT_SPACE] == 1) {
		return TRUE;
	}
	return FALSE;
}

//ゲーム終了用
int control_esc() {
	if (Key[KEY_INPUT_ESCAPE] == 1) { //ESCキーが1カウント押されていたら
		return TRUE;
	}
	return FALSE;
}