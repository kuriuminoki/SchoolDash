#include "DxLib.h"
#include "control.h"


//////////�L�[�{�[�h�̓��͏�Ԃ��i�[����////////////////

int Key[256]; // �L�[��������Ă���t���[�������i�[����

			  // �L�[�̓��͏�Ԃ��X�V����
int UpdateKey() {
	char tmpKey[256]; // ���݂̃L�[�̓��͏�Ԃ��i�[����
	GetHitKeyStateAll(tmpKey); // �S�ẴL�[�̓��͏�Ԃ𓾂�
	for (int i = 0; i<256; i++) {
		if (tmpKey[i] != 0) { // i�Ԃ̃L�[�R�[�h�ɑΉ�����L�[��������Ă�����
			Key[i]++;     // ���Z
		}
		else {              // ������Ă��Ȃ����
			Key[i] = 0;   // 0�ɂ���
		}
	}
	return 0;
}

void mouse_limit(int& hand_x, const int hand_y) {
	if (hand_x < 0 && hand_y > 0 && hand_y < 480) {
		hand_x = 0;
		SetMousePoint(0, hand_y);//�}�E�X�J�[�\���̈ʒu
	}
	else if (hand_x > 640 && hand_y > 0 && hand_y < 480) {
		hand_x = 640;
		SetMousePoint(640, hand_y);//�}�E�X�J�[�\���̈ʒu
	}
}

///////////�}�E�X�̓��͏�Ԃ�Ԃ�//////////////
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


//���E�̈ړ�
int control_stick()
{
	//�������͂ɂ͒����ԉ����Ă�������̗p
	if (Key[KEY_INPUT_D] >= 1 && Key[KEY_INPUT_A] >= 1) {
		if (Key[KEY_INPUT_D] > Key[KEY_INPUT_A]) {
			return RIGHT_KEY;
		}
		else if (Key[KEY_INPUT_D] <= Key[KEY_INPUT_A]) {
			return LEFT_KEY;
		}
	}
	else if (Key[KEY_INPUT_D] >= 1) { // �E�L�[��������Ă�����
		return RIGHT_KEY;
	}
	else if (Key[KEY_INPUT_A] >= 1) { // ���L�[��������Ă�����
		return LEFT_KEY;
	}
	return FALSE;
}

//���E�̈ړ�
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
	else if (Key[KEY_INPUT_S] >= 1) { // ���L�[��������Ă�����
		return DOWN_KEY;
	}
	else if (Key[KEY_INPUT_W] >= 1) { // ���L�[��������Ă�����
		return UP_KEY;
	}
	return FALSE;
}

//�h��
int control_cure()
{
	// E�L�[�������ꂽ���
	if (Key[KEY_INPUT_E] >= 1) {
		return CURE_KEY;
	}
	return FALSE;
}

//Q�L�[�i�T���I���j
int control_q()
{
	// Q�L�[�������ꂽ�u��
	if (Key[KEY_INPUT_Q] == 1) {
		return TRUE;
	}
	return FALSE;
}

//Q�L�[�i�C�x���g�I��)
int control_quit()
{
	// Q�L�[�������ꂽ�u��
	if (Key[KEY_INPUT_Q] == 30) {
		return TRUE;
	}
	return FALSE;
}

//�f�o�b�O���[�h�N���p
int control_debug() {
	if (Key[KEY_INPUT_F] == 1) { // F�L�[��������Ă�����
		return TRUE;
	}
	return FALSE;
}

//space�L�[
int control_space()
{
	// space�L�[�������ꂽ�u��
	if (Key[KEY_INPUT_SPACE] == 1) {
		return TRUE;
	}
	return FALSE;
}

//�Q�[���I���p
int control_esc() {
	if (Key[KEY_INPUT_ESCAPE] == 1) { //ESC�L�[��1�J�E���g������Ă�����
		return TRUE;
	}
	return FALSE;
}