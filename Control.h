#ifndef CONTROL_H_INCLUDED
#define CONTROL_H_INCLUDED
//////�L�[�{�[�h����̓��͂��i�[���A�Ԃ��֐��Q///////////

/////////////���͏��/////////////////////
//�L�[�̓��͏��
enum {//�e�s����control�֐�����̖߂�l ���ɁAA,S.D,W
	CURE_KEY = 1,

};

//�X�e�B�b�N�̓��͏��
enum {
	RIGHT_KEY = 1,
	LEFT_KEY = 180,
	UP_KEY = 3,
	DOWN_KEY = 4
};
////////////////////////////////////////////

int UpdateKey();

void mouse_limit(int& hand_x, const int hand_y);
//���N���b�N�̏��
void mouse_click();
int left_click();
int right_click();

//�X�e�B�b�N�̓��͏��
int control_stick();
int control_stick2();

//�L�[�̓��͏��
int control_cure();
int control_q();
int control_quit();

//FPS�\���̃I���I�t
int control_debug();

//�X�y�[�X�L�[
int control_space();

//ESC�L�[�F�Q�[���I��
int control_esc();

#endif