#include"Movie.h"
#include"SDvalue.h"
#include"DxLib.h"
#include<string>

using namespace std;

Ed::Ed() {
	haikei_x = 0;
	cnt = 0;
	other_x = new int[6];
	other_x[0] = 10000;
	other_x[1] = 3000;
	other_x[2] = 3100;
	other_x[3] = 6000;
	other_x[4] = 6200;
	other_x[5] = 6500;
	//大きいフォント
	font_handle = CreateFontToHandle(NULL, 60, 8);
	haikei = LoadGraph("picture/背景1.png");
	string name;
	for (int i = 0; i < 10; i++) {
		name = "picture/dot/";
		name += st_name[i];
		run[i][0] = LoadGraph((name + "走り1.png").c_str());
		run[i][1] = LoadGraph((name + "走り2.png").c_str());
	}
	for (int i = 0; i < 6; i++) {
		name = "picture/dot/";
		name += other_name[i];
		other[i] = LoadGraph((name + ".png").c_str());
	}
	change_music("music/kobune.mp3", true, false);
}

Ed::~Ed() {
	delete[] other_x;
	DeleteFontToHandle(font_handle);
	for (int i = 0; i < 10; i++) {
		DeleteGraph(run[i][0]);
		DeleteGraph(run[i][1]);
	}
	for (int i = 0; i < 6; i++) {
		DeleteGraph(other[i]);
	}
	DeleteGraph(haikei);
}

bool Ed::play() {
	cnt++;
	haikei_x -= 6;
	if (haikei_x <= -1 * GAME_WIDE) {
		haikei_x = 0;
	}

	if (cnt < 30) {
		return false;
	}
	else if (cnt < 30 + 255) {
		SetDrawBright(cnt - 30, cnt - 30, cnt - 30);
	}
	else {
		SetDrawBright(255, 255, 255);
	}


	//背景
	DrawRotaGraph2(haikei_x, 0, 0, 0, 1.0, 0, haikei, TRUE, FALSE);
	DrawRotaGraph2(GAME_WIDE + haikei_x, 0, 0, 0, 1.0, 0, haikei, TRUE, FALSE);
	//クルメたち
	for (int i = 0; i < 6; i++) {
		other_x[i] -= 6;
		DrawRotaGraph2(other_x[i], 130, 0, 0, DOT_EXTEND, 0, other[i], TRUE, FALSE);
	}
	//走るドット絵
	for (int i = 0; i < 10; i++) {
		DrawRotaGraph2(run_x[i], run_y[i], 0, 0, DOT_EXTEND, 0, run[i][((cnt + 3) / 4) % 2], TRUE, FALSE);
	}

	//クレジット
	if (cnt < 300) {
		DrawStringToHandle(10, 700, "企画：のけもの", WHITE, font_handle);
	}
	else if (cnt < 500) {
		DrawStringToHandle(10, 700, "原案・脚本：のけもの", WHITE, font_handle);
	}
	else if (cnt < 700) {
		DrawStringToHandle(10, 700, "キャラクターデザイン：のけもの", WHITE, font_handle);
	}
	else if (cnt < 900) {
		DrawStringToHandle(10, 700, "システムプランナー：のけもの", WHITE, font_handle);
	}
	else if (cnt < 1100) {
		DrawStringToHandle(10, 700, "システムグラフィック：のけもの", WHITE, font_handle);
	}
	else if (cnt < 1200) {
		DrawStringToHandle(10, 700, "ディレクター：のけもの", WHITE, font_handle);
	}
	else if (cnt < 1400) {
		DrawStringToHandle(10, 700, "プログラム：のけもの", WHITE, font_handle);
	}
	else if (cnt < 1600) {
		DrawStringToHandle(10, 700, "エンディングテーマ", WHITE, font_handle);
		DrawStringToHandle(10, 800, "「白銀の小舟」", WHITE, font_handle);
		DrawStringToHandle(10, 900, "素材提供：魔王魂", WHITE, font_handle);
	}
	else {
		DrawStringToHandle(10, 700, "Thank you for playing!!", YELLOW, font_handle);
	}
	//DrawFormatString(10, 900, WHITE, "cnt = %d", cnt);
	//終了時
	if (check_music() == 0) {
		return true;
	}
	return false;
}