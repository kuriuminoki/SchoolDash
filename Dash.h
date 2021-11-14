#ifndef DASH_H_INCLUDED
#define DASH_H_INCLUDED

//�w�Z�̒T��������p�[�g

#include<string>
#include<queue>
#include<vector>
#include<functional>

class Student;
class Enemy;
class Bullet;
class Button;
class TextEvent;

struct CompareBulletGreater;

enum {
	RUN,		//����
	BATTLE,		//�퓬
	EVENT,		//�C�x���g
	GET,		//�󕨃Q�b�g
};

class Dash {
private:
	static const int LOVE_UP = 5;//�e���x����x�̃C�x���g�łǂꂾ���オ�邩
	static const int EVENT_RAND = 40;//�e���x�C�x���g����������m��
	static const int EXP_RATE = 7;//���炦��o���l�@WAVE�~
	static const int GET_RAND = 50;//�󔠂̏o���m��50
	static const int MATERIAL_CNT = 60;//�Q�b�g�����f�ނ̒ʒm�̕\������
	int hand_x, hand_y;//�}�E�X�̍��W
	Button* button_decide;//�I���{�^��
	Button* button_cancel;//�ĊJ�{�^��
	TextEvent* te;//�C�x���g�N���X
private:
	bool pause_flag = false;//true�Ȃ�ꎞ��~
	int state;//�퓬���H�_�b�V�����H
	int wave = 0;//���E�F�[�u�ڂ�
	int now_wave = 0;//����̒T���ŉ��E�F�[�u�N���A������
	int cnt = 0;//�J�E���g�p
	std::queue<int> que_material;//�Q�b�g�����f�ޖ���\������p �f�ޔԍ�������
	int material_cnt = 0;
private:
	int sn;//���k�̐�
	int en;//�G�̐�
	Student** student;
	std::queue<Enemy*> enemy;
private:
	//�`�悷��e��a���̃L���[
	std::priority_queue<Bullet, std::vector<Bullet>, std::greater<Bullet> > draw_que;
	int szangeki_handle;//���k�a���̉摜
	int ezangeki_handle;//�G�a���̉摜
	int defense_handle;//�h��̌��ʉ�
	int item_handle;//�񕜖�̉摜
	int item_sound;//�񕜂̌��ʉ�
private:
	int* haikei_handle;//�w�i�摜
	int now, next;//���`�悷��w�i�A���ɕ`�悷��w�i
	int x;//�w�i�̕`��ʒu
	int treasure_x;
	int treasure_handle;
	std::string treasure_name;
private:
	bool final_flag = false;//�N���C�}�b�N�X�̉��y�����ꂽ��true
	int final_cnt = 0;//���Ԑ؂�̒ʒm����
	int final_music;//�N���C�}�b�N�X�̉��y�i���炩���߃��[�h�j
	int FontHandle1;//�t�H���g�f�[�^
	int FontHandle2;//�t�H���g�f�[�^(���ł���)
private://���U���g�Ƃ��Ē񋟂���f�[�^
	int stand_handle[3];//�L�����̗����G�i�Ί�j
	int dash_exp;//���k���P��̒T���Ŋl�������o���l
	int dash_love[3];//�A�J�c�L�����̒T���Ŋl�������e���x
	std::vector<int>* dash_weapon;//�P��̒T���Ŋl����������
	std::vector<int>* dash_material;//�P��̒T���Ŋl�������f��
	int dash_money;//�P��̒T���Ŋl����������
public:
	//�e���k�̎�ނƕ���̎�ނƉ񕜖�̌��ƃ��x��
	Dash(int start_wave, int st[], int weapon[], int item[], int dash_level[]);
	~Dash();
public:
	bool play();//���C��
	void draw();//�`��
	bool play_reward();//��V���󂯎��
	void draw_reward();//��V���󂯎��
private:
	void load_music_final();//�N���C�}�b�N�X�̉��y�����[�h���Ă���
	void change_state();//���͐퓬�H�󕨁H
	bool get();//��Q�b�g
	void decide_treasure();//�󔠂̒��g������
	bool battle();//�o�g��
	bool dead();//���k�̒N�������ꂽ��true
	void enemy_dead();//���񂾓G�̏���
	void enemy_create();//�G�̏o��(wave�̍ŏ�)
	bool move();//�w�i�̈ړ�
	void create_event();//�C�x���g�N���X�쐬
	void draw_chara();//���k�A�G�A�e�̕`��
	void draw_state();//���k�̏������ɕ\��
	void draw_bullet(int kind, int x, int y, int dx, int dy);//�e�̎�ނɂ���ĕ`�ʂ�ς���
};


#endif