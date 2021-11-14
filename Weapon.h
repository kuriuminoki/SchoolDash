#ifndef WEAPON_H_INCLUDED
#define WEAPON_H_INCLUDED

#include<queue>
#include<functional>
#include<string>
#include<vector>
class Character;
class Button;
class StButton;

//1LEVEL�オ�邲�Ƃɂǂꂾ���_���[�W�������邩
#define WEAPON_DAMAGE_UP 1

//�e�̎�ވꗗ
enum {
	BLUE_BALL,//���k�̒e
	RED_BALL,//�G�̒e
	BLUE_ZAN,//���k�̎a��
	RED_ZAN//�G�̎a��
};

//�U���̒e�A�a���̃N���X
class Bullet {
	int kind;//�e�̎��
	int x, y;//���W
	int dx, dy;//�傫��(��ӂ�1/2 or �~�̔��a) �~���l�p�Ƃ݂Ȃ���
	int rx, ry;//���ł�������(�X�s�[�h)
	int damage;
	int distance;//��񂾋���
public:
	Bullet(int k, int sx, int sy);//��ނƏ������W
	bool operator> (const Bullet &other) const;
public:
	void set_dir(int srx, int sry);//���ł�������(�X�s�[�h)��ݒ�
	void set_scale(int sdx, int sdy);//�傫����ݒ�
	void set_damage(int after_damage);//�_���[�W��ݒ�
public:
	int get_x() const;
	int get_y() const;
	int get_dx() const;
	int get_dy() const;
	int get_damage() const;
	int get_kind() const;
	int get_distance() const;
public:
	void move();//�e�̈ړ�
};

static const int MAX_INTERVAL = 90;//�A�˗͂̍ő�l


class WeaponInfo {
public:
	int dot_handle;//����̃h�b�g�G
	std::string name;//����̖��O
	int KIND;//�e�̎��
	int DAMAGE;//�U����
	int SPEED;//�e�� �e�̕����������_���̂Ƃ��Ɏg��
	int INTERVAL;//�A�˗�
	int DISTANCE;//�˒�
	int DX, DY;//�e�̑傫��
	int RX, RY;//�e�̈ړ����� ����0�Ȃ烉���_�����Ӗ�����
	int DEFENSE;//�h��� �������邾���Ŗh��͂���悹�����d�g��
	int DEBUFF;//�����������̑��x�ቺ����
	int atari_handle;//�����莞�̌��ʉ�
	std::vector<std::string> data;//csv�t�@�C������ǂݍ��񂾃e�L�X�g�f�[�^
	std::vector<int> value;//�e�L�X�g�𐮐��ɕϊ��������́iname�ȊO�j
public:
	WeaponInfo(int kind);
	~WeaponInfo();
	int get_dot() const;//����̃h�b�g�G��Ԃ�
	void get_name(std::string& tag) const;//����̖��O���擾
	void draw(int x, int y, int color, int font, int font_color);//����̏���`��
};

//����̊��N���X (�e�̊Ǘ����s��)
class Weapon {
protected:
	WeaponInfo* wi;
private:
	std::queue<Bullet*> bullets;//���˒��̒e
	int cnt = 0;//�A�˗p�̃J�E���g
protected:
	void set_value(int da, int sdx, int sdy, int in, int di, int s, int srx, int sry);
	void set_looks(int kind, int defense, int debuff, const char tama_name[]);
public:
	Weapon(int kind);
	~Weapon();
	int get_dot() const;
	int get_interval();
	int get_defense();
	int get_debuff();
public:
	void random_r();//�e�̕�����SPEED�����ƂɃ����_���Ɍ���
	void cus(int level);//���\�����x���ɂ���Ē����ł���
	void limit(int left, int right, int up, int down);//�X�e�[�W�O�̒e������
	bool attack(bool click, int x, int y);//���ˎ���true��Ԃ�
	bool atari(Character& c);////�e�̂����蔻�� �h�䉹��炷�Ƃ���true��Ԃ�
	void reset();//�������e�����ׂď���
public:
	void push_draw(std::priority_queue<Bullet, std::vector<Bullet>, std::greater<Bullet> >& que);//�`��L���[�ɒe������
};

//����ύX���
class WeaponChange {
private:
	const int MAX_WEAPON = 8;//��x�ɕ\�����镐��̐�
	int sum = 0;//�������P�ȏ�̕���̐�
	int sum_rate = 0;//�ォ�牽�Ԗڂ̕��킪now��
	int hand_x, hand_y;
	int now = 0;//��ԏ�ɕ\�����镐��̔ԍ�
	int touch = -1;//�Q�ƒ��̕���(�܂�ύX���镐����)
	int st_select = -1;//���k�I�щ�ʂȂ�-1����ύX���Ȃ琶�k�ԍ�
	int* weapon_num;//����{�^���̕���ԍ�

	StButton* button_st;//�ǂ̐��k�̕����ύX���邩�I�ԃ{�^��
	Button* button_cancel;//����̖߂�{�^��
	Button* button_up;//���{�^��
	Button* button_down;
	Button** button_weapon;//����̃{�^��
	WeaponInfo** wi;//�������Ă��镐�킷�ׂ�

	int font_handle;//�t�H���g
	int font_small_handle;//�t�H���g
	int up_handle, down_handle;//���̉摜�n���h��
public:
	WeaponChange(int handle[]);//�L�����̃h�b�g�G���󂯎��
	~WeaponChange();
	void cancel_st(int num);//���ԂɂȂ��Ă��Ȃ����k���L�����Z��
public:
	bool play();//�I����true�ԋp
	void draw();//�`��t�F�[�Y
private:
	bool play_st();//���k�I�����
	void play_wi();//����ύX���
	void draw_wi();
	void weapon_reset();//��󂪉����ꂽ�Ƃ�
	void now_up();//now����ԋ߂���̕���ɍ��킹��
	void now_down();//now����ԋ߂����̕���ɍ��킹��
};

class WeaponCreate {
private:
	const int MAX_WEAPON = 8;//��x�ɕ\�����镐��̐�
	int hand_x, hand_y;
	int now = 0;//��ԏ�ɕ\�����镐��̃C���f�b�N�X
	int touch = -1;
	Button* button_cancel;//����̖߂�{�^��
	Button* button_up;//���{�^��
	Button* button_down;
	Button** button_weapon;//����̃{�^��
	WeaponInfo** wi;//���镐��
	int font_handle;//�t�H���g
	int font_small_handle;//�t�H���g
	int up_handle, down_handle;//���̉摜�n���h��
	int** need;//�K�v�f��
	bool* able;//����������Ȃ�true
public:
	WeaponCreate();
	~WeaponCreate();
public:
	bool play();
	void draw();
private:
	void weapon_reset();//����{�^���̕\�����X�V
	void check_able();//��������邩���ׂ�
	void draw_material(int x, int y);//�K�v�f�ނ�\��
	void create();//�f�ނ�����ĕ�������
};

#endif