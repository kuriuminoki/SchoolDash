#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include<string>

class Button;
class StButton;

static const int STAND_LEFT = 250;
static const int STAND_RIGHT = 930;

//csv�t�@�C������C�x���g�̔��������m�F�A�����Ȃ�true
bool check_event_file(std::string kind, int num);

//�N���X���C�g�Ƃ̃C�x���g�̔ԍ�����
int decide_sub(int st_num);

//���N���X
class Stand {
private:
	bool reverse = false;//�����G�����E���]���邩�ǂ���
	int position = 0;//�`��ʒu
	int handle = -1;//�摜�n���h��
	int x_size, y_size;
public:
	void set_position(bool left);
	void load(const char *name);
	void draw(int dx, int dy);
	~Stand();
};

//�X�g�[���[�A�����A�G�k
enum EVENT_KIND {
	TEXT_STORY,
	TEXT_SUB,
	TEXT_OTHER,
	TEXT_EVENT,
	TEXT_BATTLE
};

//�L��������b����C�x���g
class TextEvent {
private:
	std::string event_name;
	std::string speaker;//�����҂̖��O
	int file;//�t�@�C���J�p
	bool bright_flag;//true�Ȃ�J�n���ƏI������bright_value��ς���
	int bright_value;//��ʂ̈Â�
	bool finish_text;//true�Ȃ�I�� ��ʂ��Â�����Ȃ炱�̂Ƃ��Â�����
	Stand* stand;//�L�����̗����G�z�� 2�l��
	bool first_flag;//��ڂ̃Z���t���I��������ǂ���
private:
	int text_now = 0;//�Z���t���������ڂ܂ŕ\�����邩
	static const int TEXT_NEXT_CNT = 30;
	int next_cnt = 0;//��N���b�N�h�~�p
	std::string saying;//�Z���t
	std::string sub_saying[3];//�Z���t��3������������
	bool say_left;//���̐l������ׂ��Ă���Ȃ�true
	bool say_none;//�N������ׂ�Ȃ��Ȃ�true
private:
	int haikei;//�w�i
	int huki;//�t�L�_�V
	int textfont;//�e�L�X�g�̃t�H���g
	int kettei;//���N���b�N�̌��ʉ�
	int effect_sound;//���ʉ��i�t�@�C����ǂݍ��񂾂Ƃ��Ɏg���w�����o��j
public:
	static const int START_DX = 800;//400
	static const int START_DY = 400;
	int left_dx, right_dx, text_dy;//�\���̂���
public:
	//�t�@�C�����J�� //�C�x���g�̎�ށA�ԍ��A�J�n�ƏI�����ɈÓ]���邩
	TextEvent(EVENT_KIND kind, int num, bool bright, std::string folder = "");
	//�t�@�C�������
	~TextEvent();
public:
	bool play();//���C�� �I���Ȃ�true
private:
	void deal_text();//�Z���t����ʂɂP�������\������悤��
	bool next();//���̃u���b�N������ �t�@�C���̏I���Ȃ�true
	void file_read();//�t�@�C���̃X�N���v�g���ꏈ��
	void saying_divide();//�Z���t���R��������
	void draw();//�`��
};

//�D���x���グ�Č���C�x���g���
class StEvent {
private:
	int hand_x, hand_y;
	int dx1, dx2;//�����G�̕\�����炵�p
	const int START_DX = 600;
	int m_st_select;//���ݑI�𒆂̐��k
	int m_stand_handle[10];
	Button* button_cancel;
	Button* button_decide;
	StButton* button_st;
	int font_handle;
	int font_small_handle;
	bool able_event[10];//�e���k�̃C�x���g������邩
	int decide_sound;
private:
	TextEvent* te;
	bool te_flag;//�e�L�X�g�C�x���g����(true)�A�I����ʂ�(false)
public:
	StEvent(int st_dot[], int stand_handle[]);
	~StEvent();
public:
	bool play();
	void draw();
	bool get_flag();//�e�L�X�g�C�x���g�����Ă��邩�ǂ���
private:
	void check_event();//�e���k�̃C�x���g������邩���ׂ�i�e���x�Ɛi�s�󋵂ɂ��j
};

#endif