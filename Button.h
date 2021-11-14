#ifndef INCLUDED_BUTTON_H
#define INCLUDED_BUTTON_H

#include<string>

class WeaponInfo;
class Dash;

class Button {
private:
	bool m_flag = true;//�@�\���Ȃ��{�^����false
	std::string m_tag = "";
	int m_x, m_y;//�{�^���̈ʒu
	int m_wide, m_height;
	int m_color;//�l�p�̒��̐F
	int m_color2;//�}�E�X���d�Ȃ��Ă���Ƃ��Ɏg���F
	int m_font_color;//�����̐F
	int m_font;
	int m_dx, m_dy;//������\��������W
	int m_graph_handle = -1;//�G
	int m_graph_ex;//�G�̊g�嗦
public:
	Button(std::string, int x, int y, int wide, int height, int color, int color2, int font, int font_color);
public:
	void change_flag(bool f, int new_color);
	bool get_flag();
	int get_handle();//�摜���擾
	bool overlap(int hand_x, int hand_y);
	void draw(int hand_x, int hand_y);
	void set_graph(int handle, int ex);
	void set_string(std::string new_string);//�^�O�����Ȃ���
};

//�����̐��k�̃{�^�����Ǘ�����
class StButton {
private:
	int m_size;
	Button** st;
public:
	StButton(int size, int handle[], int height);
	~StButton();
public:
	//n�Ԗڂ̉摜�n���h�����擾
	int get_handle(int num);
	//n�Ԗڂ̃t���O���擾
	bool get_flag(int num);
	//num�Ԗڂ̐��k�̃t���O�ύX
	void change_flag(int num, bool f);
	//�摜�n���h����ύX�i���Ԃ���Ȃ����k��-1�ɂ���p�j
	void change_pic(int num, int handle, int ex);
	//�}�E�X�J�[�\�����d�Ȃ��Ă���Ȃ� �O�` �Ⴄ�Ȃ�-1
	int overlap(int hand_x, int hand_y);
	//�{�^���̕`��
	void draw(int hand_x, int hand_y);
private:
	void st_absent();//���t�����āA�܂����ԂɂȂ��Ă��Ȃ����k������
};

//�T���ɏo�����鐶�k��I�ԉ��
class StDecide {
private:
	int hand_x, hand_y;
	const int START_DX = 400;
	int dx[3];
	int m_start_wave = 0;//���E�F�[�u�ڂ���X�^�[�g���邩
	int m_st_num[3];//���ݑI�΂�Ă���R�g�̐��k�ԍ�
	int m_stand_handle[10];
	int m_dot_handle[10];
	int m_run_handle[10][2];
	Button* m_st_button[3];//�I�𒆂̐��k������
	Button* m_button_right;//���{�^��
	Button* m_button_left;
	Button* button_decide;//�E��̌���{�^��
	Button* button_cancel;//����̖߂�{�^��
	StButton* m_button_st;//���k�X�l�̃{�^��
	int m_right_handle, m_left_handle;//���̉摜�n���h��
	int m_run_cnt;
	WeaponInfo* m_wi[10];//�������̕���
	int font;
	int font_small_handle;
	int decide_sound;
public:
	StDecide(int stand_handle[], int dot_handle[], int run_handle[][2]);
	~StDecide();
public:
	int play();
	void draw();
	Dash* get_dash();//�T���N���X�̍쐬
private:
	int set_st(int new_num);//���k���Z�b�g���� �O�ꂽ�L�����ԍ���Ԃ�
	int cancel_st();//���k���O��
};

#endif