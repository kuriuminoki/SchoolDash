#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include<string>
#include<vector>

class Button;
class StButton;
class StDecide;
class Dash;
class TextEvent;
class StEvent;
class WeaponChange;
class WeaponCreate;
class Shop;
class Option;
class Data;
class Ed;

//�Q�[����ʂ̎��
enum GAME_MODE {
	GAME_MENU,//�ŏ��̃��j���[���

	GAME_STORY,//�X�g�[���[���

	GAME_PRE_DASH,//�T�����O
	GAME_DASH,//�T����
	GAME_END_DASH,//�T���I��

	GAME_EVENT,//�C�x���g

	GAME_DATA,//�f�[�^���

	GAME_CUS,//����ύX���

	GAME_CREATE,//����̍쐬

	GAME_SHOP,//�V���b�v���

	GAME_START,//���t�ύX���

	GAME_OPTION,//�I�v�V�������

	GAME_MOVIE,//���[�r�[���
};

//�Q�[���{�̂ƂȂ�
class SchoolDash {
private:
	//����摜
	int haikei;
	int st_handle[10];//���k�̃h�b�g�����G
	int run[10][2];//����h�b�g�G
	int stand_handle[10];//�����G
	int decide_sound;//���艹
	int cancel_sound;//�߂鉹
private:
	//�Q�[���Ǘ��p
	int hand_x, hand_y;//�}�E�X�̍��W
	int window_flag;
	int cnt;
	int big_font;
	int small_font;
	GAME_MODE state;//���Q�[�����ǂ̉�ʂ�
	TextEvent* te;//�e�L�X�g�C�x���g
private:
	//�{�^������
	int button_font;
	Button* button_tansaku;//�T���{�^��
	Button* button_event;//�C�x���g�{�^��
	Button* button_cus;//�����{�^��
	Button* button_shop;//�V���b�v�{�^��
	Button* button_create;//����쐬�{�^��
	Button* button_data;//�f�[�^�{�^��
	Button* button_option;//�I�v�V�����{�^��
public:
	SchoolDash();
	~SchoolDash();
	void load_all();
	void delete_all();
	void change_menu_sound();//���j���[��ʂ̌��ʉ��̉��ʂ��Đݒ�
private:
	//���𗧂��֐��Q
	void draw_date(int draw__x, int draw__y);//����ɓ��t�\��
	void next_date();//���t��i�߂�
public:
	//���C��
	void play();
	bool create_event();
private:
	//�ŏ��̃��j���[���
	void play_menu();
	void draw_menu();
private:
	//�X�g�[���[�{�����
	void play_story();
	void draw_story();
private:
	//���t�ύX���
	void play_start();
	void draw_start();
private:
	//�T�����O���
	StDecide* sd;
	void play_pre_dash();
	void draw_pre_dash();
private:
	//�T����
	Dash* dash;//�T��
	void play_dash();
	void draw_dash();
private:
	//�T���I��
	void play_end_dash();
	void draw_end_dash();
private:
	//�C�x���g���
	StEvent* se;
	void play_event();
	void draw_event();
private:
	//�f�[�^
	Data* data;
	void play_data();
	void draw_data();
private:
	//����ύX���
	WeaponChange* wc;//����ύX���
	void play_cus();
	void draw_cus();
private:
	//����쐬���
	WeaponCreate* wcre;
	void play_cre();
	void draw_cre();
private:
	//�V���b�v
	Shop* shop;
	void play_shop();
	void draw_shop();
private:
	//�I�v�V����
	Option* option;
	void play_option();
	void draw_option();
private:
	//�G���f�B���O
	Ed* ed;
};

class Option {
private:
	int hand_x, hand_y;
	int font_handle;
	Button* m_button_right;//���{�^��
	Button* m_button_left;
	Button* m_button_cancel;//�߂�{�^��
	int m_right_handle, m_left_handle;//���̉摜�n���h��
public:
	Option();
	~Option();
public:
	bool play();
	void draw();
};


//�ǂ̃f�[�^���{������
enum {
	DATA_MENU,
	DATA_ST,
	DATA_OTHER,
	DATA_EVENT
};

class StData;
class OtherData;
class EventData;

//�f�[�^�{����ʃN���X
class Data {
private:
	int data_state;//�ǂ̃f�[�^���{������
	int hand_x, hand_y;
	int decide_sound;//���艹
	int cancel_sound;//�߂鉹
	int font_handle;//�t�H���g
	int font_small_handle;//�t�H���g
	Button* button_cancel;//�߂�{�^��
	Button* button_st;//���k�{����ʂ�
	Button* button_other;//�N�����B�̉{����ʂ�
	Button* button_event;//�C�x���g�{����ʂ�
private:
	StData* m_st_data;//���k�{����ʗp
	OtherData* m_other_data;
	EventData* m_event_data;
public:
	Data(int stand_handle[], int dot_handle[]);
	~Data();
public:
	bool play();
	void draw();
};

class StData {
private:
	int hand_x, hand_y;
	static const int START_DX = 600;//��ʕ\���̍ŏ��̂���
	int dx = 0;//��ʕ\���̂���
	int handle[10];//���k�̗����G
	std::vector<std::string> prof[10];//�L�����̏Љ
	StButton* sb;//���k�P�O�l�̃{�^��
	int st_select;//�ǂ̐��k���{�����邩
	int decide_sound;//���艹
	int font_handle;//�t�H���g
	int font_small_handle;//�t�H���g
	Button* button_change;//�Љ�̕\���ɐ؂�ւ���{�^��
	bool draw_prof;//�Љ�̕\����
public:
	StData(int stand_handle[], int dot_handle[]);
	~StData();
public:
	void play();
	void draw();
	void reset();
};

class OtherData {
private:
	int hand_x, hand_y;
	static const int START_DX = 600;//��ʕ\���̍ŏ��̂���
	int dx;//��ʕ\���̂���
	int handle[6];//�N�����B�̗����G
	std::vector<std::string> prof[6];//�L�����̏Љ
	int st_select;//���Ԗڂ̐��k���{������
	int decide_sound;//���艹
	int font_handle;//�t�H���g
	int font_small_handle;//�t�H���g
	Button* button_next;//���փ{�^��
	Button* button_prev;//�O�փ{�^��
public:
	OtherData();
	~OtherData();
public:
	void play();
	void draw();
	void reset();
};


class EventData {
private:
	int hand_x, hand_y;
	static const int DRAW_TEXT_BUTTON = 6;//��ʏ�ɕ\������X�g�[���[�{�^���̐�
	Button** button_story;//�����ƃX�g�[���[���ς��
	Button* button_up;//����
	Button* button_down;//�����
	int up_handle, down_handle;//���̉摜
	int m_now;//��ԏ�ɕ\�����̃X�g�[���[�͉��ԁH
	bool te_now;//���C�x���g�������H
	TextEvent* te;
	int decide_sound;//���艹
	int font_handle;//�t�H���g
	int font_small_handle;//�t�H���g
	std::string story_name[120];//�C�x���g�̖��O
public:
	EventData();
	~EventData();
public:
	void reflesh();//�{�^�����X�V
	bool play();//�C�x���g��������true
	bool draw();//�C�x���g��������true
};


#endif