#ifndef CHARA_H_INCLUDED
#define CHARA_H_INCLUDED

//���k�ƓG�̃I�u�W�F�N�g

#include<queue>
#include<utility>
#include<functional>

class Weapon;
class Bullet;

//1Level�オ�邲�Ƃɑ̗͂��ǂꂾ�������邩
#define STUDENT_HP_UP 120
#define ENEMY_HP_UP 4

//���N���X
class Character {
protected://���͊֘A
	int input_stick;//ASDW�L�[�̍��E(A,D)
	int input_stick2;//�㉺�iS,W�j
	int input_left;
	int input_right;
	int input_e;
	int input_q;
protected:
	int RUN_SPEED = 6;//����X�s�[�h���k�̓f�t�H���g6
	int level;
	int max_hp;
	int hp;
	int defense;//�h���
	int debuff_time = 0;//���x�ቺ�c�莞��
	int x, y;
	int cnt;
	int damage_flag = 0;//�_���[�W���󂯂����A�h�b�g�G��Ԃ�����p
	Weapon* weapon;//����
protected:
	int run[2];//�h�b�g�G
	int x_size, y_size;
	int debuff_handle;
public:
	~Character();
	int get_cx() const;//���W�i���S�j���擾
	int get_cy() const;//���W�i���S�j���擾
	int get_x() const;
	int get_y() const;
	int get_xsize() const;
	int get_ysize() const;
	int get_max_hp() const;
	int get_hp() const;
	int get_defense() const;//E�L�[�̓��͏�Ԃɂ����0�܂��͖h��͂�Ԃ�
public:
	void change_hp(int value);//hp��+value����
	void set_debuff(int value);//���x�ቺ�����炤
	void cnt_reset();//cnt��0�ɂ���
public:
	void move();//�ړ�
	void run_cnt();//���点��cnt�C���N�������g
	void attack();//����̎g�p
	void attack_limit();//�e�̃X�e�[�W�O����
	bool atari(Character& c);//�G�ɍU��������������
	//�`��p�̃L���[�ɒe������
	void attack_push(std::priority_queue<Bullet, std::vector<Bullet>, std::greater<Bullet> >& que);
public:
	void ai_normal(int left, int middle, int right, int updown, int left_limit);
	virtual void ai() = 0;
	int next_x;//���Ɍ��������W
	int next_y;//���Ɍ��������W
protected:
	void draw_dot(bool bright_flag);
};

//���k
class Student :
	public Character {
private:
	int kind;//���k�̎�� �A�J�c�L�A�n���_ �` "0"�X�^�[�g
	int yarare_handle;//���ꂽ�Ƃ��̃h�b�g�G
	int item;//�񕜖�̌�
	int item_handle;//�񕜖�̉摜
	int ase_handle;//���̉摜
	int heart_handle;//�n�[�g�̉摜
	int icon_y_size;
private:
	int target = -1;//�񕜂��悤�Ƃ��Ă��鐶�k(�O�`�Q or -1)
	int icon_cnt = 0;//�h�b�g�G�㕔�ɃA�C�R����\������c�莞��
	bool heart = true;//�񕜂��ꂽ���Ȃ�true,�������Ȃ�false
public:
	void set_target(int t);
	int get_target();
	void cure_hp();//�񕜖�ŉ񕜂���
	bool cure_friend(Student& s);//���Ԃɉ񕜖���g�p����
	void cure_ai(int n, Student& s);//���Ԃ��񕜂��邩���f����AI
	void cure_reset();//target = -1
public:
	//��ށA���W�A����̎�ށA�񕜖�̌��A���x��
	Student(int k, int sx, int sy, int wea, int it, int lev);
	~Student();
private:
	void load_dot();//�h�b�g�G�̃��[�h
	void level_set();//���x���ɉ�����HP����
public:
	int get_weapon_dot() const;
	int get_kind() const;
	int get_item() const;
public:
	void draw(bool bright_flag);//�`��
public:
	void control();//�L�[�{�[�h�ő���
	void input_reset();//���͏�Ԃ����Z�b�g
	void limit();//�ړ����E
	void ai();
	int attack_cnt = 0;//�U����������c�莞��
	void ai_attack(int left, int middle, int right);//�U���E�h��̐؂�ւ� �U���̊m����������
};

//�G
static const int ENEMY_KIND = 16;//�G�̎�ސ�
const int drop_money[] = { 20, 25, 25, 25,
							30, 30, 30, 30,
							35, 35, 35, 50, 75, 
							100, 125, 150 };
class Enemy :
	public Character {
private:
	int kind;//�G�̎��
	int hp_dx = 0;//�G�̎�ނɂ���Ă�HP�o�[�̈ʒu�𒲐�����
public:
	//��ށA���W�A���x��
	Enemy(int k, int sx, int sy, int lev);
	~Enemy();
	void create_weapon();//kind�ɉ����ĕ���p��
private:
	void load_dot();//�h�b�g�G�̃��[�h
	void level_set();//���x���ɉ�����HP����
public:
	void dash(int dx);//�w�i�ړ��̍�
	void draw(bool bright_flag);//�`��
public:
	void limit();
	void ai();
public:
	int get_money();//�|�����Ƃ��ɗ��Ƃ�����
	int get_drop();//�h���b�v�A�C�e���̔ԍ���Ԃ�
};

#endif
