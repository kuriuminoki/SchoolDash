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
	//�傫���t�H���g
	font_handle = CreateFontToHandle(NULL, 60, 8);
	haikei = LoadGraph("picture/�w�i1.png");
	string name;
	for (int i = 0; i < 10; i++) {
		name = "picture/dot/";
		name += st_name[i];
		run[i][0] = LoadGraph((name + "����1.png").c_str());
		run[i][1] = LoadGraph((name + "����2.png").c_str());
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


	//�w�i
	DrawRotaGraph2(haikei_x, 0, 0, 0, 1.0, 0, haikei, TRUE, FALSE);
	DrawRotaGraph2(GAME_WIDE + haikei_x, 0, 0, 0, 1.0, 0, haikei, TRUE, FALSE);
	//�N��������
	for (int i = 0; i < 6; i++) {
		other_x[i] -= 6;
		DrawRotaGraph2(other_x[i], 130, 0, 0, DOT_EXTEND, 0, other[i], TRUE, FALSE);
	}
	//����h�b�g�G
	for (int i = 0; i < 10; i++) {
		DrawRotaGraph2(run_x[i], run_y[i], 0, 0, DOT_EXTEND, 0, run[i][((cnt + 3) / 4) % 2], TRUE, FALSE);
	}

	//�N���W�b�g
	if (cnt < 300) {
		DrawStringToHandle(10, 700, "���F�̂�����", WHITE, font_handle);
	}
	else if (cnt < 500) {
		DrawStringToHandle(10, 700, "���āE�r�{�F�̂�����", WHITE, font_handle);
	}
	else if (cnt < 700) {
		DrawStringToHandle(10, 700, "�L�����N�^�[�f�U�C���F�̂�����", WHITE, font_handle);
	}
	else if (cnt < 900) {
		DrawStringToHandle(10, 700, "�V�X�e���v�����i�[�F�̂�����", WHITE, font_handle);
	}
	else if (cnt < 1100) {
		DrawStringToHandle(10, 700, "�V�X�e���O���t�B�b�N�F�̂�����", WHITE, font_handle);
	}
	else if (cnt < 1200) {
		DrawStringToHandle(10, 700, "�f�B���N�^�[�F�̂�����", WHITE, font_handle);
	}
	else if (cnt < 1400) {
		DrawStringToHandle(10, 700, "�v���O�����F�̂�����", WHITE, font_handle);
	}
	else if (cnt < 1600) {
		DrawStringToHandle(10, 700, "�G���f�B���O�e�[�}", WHITE, font_handle);
		DrawStringToHandle(10, 800, "�u����̏��M�v", WHITE, font_handle);
		DrawStringToHandle(10, 900, "�f�ޒ񋟁F������", WHITE, font_handle);
	}
	else {
		DrawStringToHandle(10, 700, "Thank you for playing!!", YELLOW, font_handle);
	}
	//DrawFormatString(10, 900, WHITE, "cnt = %d", cnt);
	//�I����
	if (check_music() == 0) {
		return true;
	}
	return false;
}