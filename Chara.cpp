#include"DxLib.h"
#include"Chara.h"
#include"Weapon.h"
#include"Control.h"
#include"SDvalue.h"

#include<string>
using namespace std;

static const int SPEED_DEFENSE = 2;//�h�䒆�̑���X�s�[�h�ቺ
static const int SPEED_DEBUFF = 2;//�f�o�t��H����Ă���Ƃ��̃X�s�[�h

//�f�X�g���N�^
Character::~Character() {
	//����̍폜
	delete weapon;
	//�摜�̍폜
	DeleteGraph(run[0]);
	DeleteGraph(run[1]);
	DeleteGraph(debuff_handle);
}

//�L�����̃h�b�g�G��`�悷�� bright_flag��false�Ȃ�P�x��ύX���Ȃ�
void Character::draw_dot(bool bright_flag) {
	if (damage_flag > 0 && bright_flag) {//�_���[�W���󂯂��Ƃ���Ȃ�Ԃ��Ȃ�
		SetDrawBright(255, 50, 50);
	}
	DrawRotaGraph2(x, y, 0, 0, DOT_EXTEND, 0, run[((cnt + 3) / 4) % 2], TRUE, FALSE);
	if (damage_flag > 0 && bright_flag) { 
		SetDrawBright(255, 255, 255);
		damage_flag--;
	}
}

//AI�̃e���v���@�G���A�̍��A�����A�E�Ɉړ�����m���A�㉺�ړ��̕p�x, ���[�̍��W
void Character::ai_normal(int left, int middle, int right, int updown, int left_limit) {
	//�E�Ɉړ�
	if (input_stick != LEFT_KEY && x < next_x) {
		input_stick = RIGHT_KEY;
	}//���Ɉړ�
	else if (input_stick != RIGHT_KEY && x > next_x) {
		input_stick = LEFT_KEY;
	}
	//�ڕW�n�_�𒴂����ꍇ
	if ((input_stick == RIGHT_KEY && x > next_x) || (input_stick == LEFT_KEY && x < next_x)) {
		next_x = x;//�ڕW�n�_�ɓ��B�������Ƃɂ���
	}
	//�~�܂�
	if (next_x == x) { input_stick = FALSE; }
	//���E�̈ړ��J�n
	if (next_x == x && GetRand(1000) < 20) {
		int r = GetRand(left + middle + right - 1);
		if (r < left) {
			next_x = GetRand(180) + left_limit;
		}
		else if (r < left + middle) {
			next_x = GetRand(180) + 180 + left_limit;
		}
		else {
			next_x = GetRand(180) + 360 + left_limit;
		}
	}

	//��Ɉړ�
	if (input_stick2 != DOWN_KEY && y > next_y) {
		input_stick2 = UP_KEY;
	}//���Ɉړ�
	else if (input_stick2 != UP_KEY && y < next_y) {
		input_stick2 = DOWN_KEY;
	}
	//�ڕW�n�_�𒴂����ꍇ
	if ((input_stick2 == UP_KEY && y < next_y) || (input_stick2 == DOWN_KEY && y > next_y)) {
		next_y = y;//�ڕW�n�_�ɓ��B�������Ƃɂ���
	}
	//�~�܂�
	if (next_y == y) { input_stick2 = FALSE; }
	//�㉺�̈ړ��J�n
	if (next_y == y && GetRand(1000) < updown) {
		next_y = 265 + GetRand(380) - y_size;
	}
}

//�e�ϐ��擾�p
int Character::get_cx() const {
	return x + x_size / 2;
}

int Character::get_cy() const {
	return y + y_size / 2;
}

int Character::get_x() const {
	return x;
}

int Character::get_y() const {
	return y;
}

int Character::get_xsize() const {
	return x_size;
}

int Character::get_ysize() const {
	return y_size;
}

int Character::get_max_hp() const {
	return max_hp;
}

int Character::get_hp() const {
	return hp;
}

int Character::get_defense() const {
	//�U�����͖h��s��
	if (input_right > 0 && input_left == 0) {
		//���k���g�̖h��́{����̖h���
		return defense + weapon->get_defense();
	}
	return 0;
}

//cnt���C���N�������g
void Character::run_cnt(){
	cnt++;
}

//����Œe�𔭎�
void Character::attack() {
	if (debuff_time > 0) {//�f�o�t���͍U���s��
		weapon->attack(false, x + (x_size / 2), y + (y_size / 2));
	}
	else {
		weapon->attack(input_left, x + (x_size / 2), y + (y_size / 2));
	}
}

//���˂����e���X�e�[�W�O�ŏ�����
void Character::attack_limit() {
	weapon->limit(0, GAME_WIDE, 0, BATTLE_DOWN);
}

//�`��p�̃L���[�֒e������
void Character::attack_push(std::priority_queue<Bullet, std::vector<Bullet>, std::greater<Bullet> >& que) {
	weapon->push_draw(que);
}

//����Ŕ��˂����e�̂����蔻��
bool Character::atari(Character& c) {
	return weapon->atari(c);
}

//hp��+value����
void Character::change_hp(int value) {
	hp += value;
	if (value < 0) { damage_flag = 5; }//�_���[�W���󂯂�
	if (hp < 0) { hp = 0; }
}

//�f�o�t�����炤
void Character::set_debuff(int value) {
	//�h�䒆�̓f�o�t�����Ȃ�
	if (input_right > 0 && input_left == 0) {
		return;
	}
	debuff_time = value;//�㏑������� ��悹�͂���Ȃ�
}

//cnt��0�ɂ���
void Character::cnt_reset() {
	cnt = 0;
}

//�ړ�����
void Character::move() {
	//�������[�V����
	if (input_stick == FALSE && input_stick2 == FALSE) { cnt = 0; }
	else { run_cnt(); }

	int run_speed = RUN_SPEED;

	//�f�o�t�ɂ�鑬�x�ቺ
	if (debuff_time > 0) {
		debuff_time--;
		run_speed = SPEED_DEBUFF;
	}
	else if (get_defense() > 0) {//�h�䒆�̑��x�ቺ
		run_speed -= SPEED_DEFENSE;
	}

	double ex = 1;
	if (input_stick != FALSE && input_stick2 != FALSE) {
		ex = 1.414;//�΂߈ړ��A���[�g�Q�ł�邽��
	}
	switch (input_stick) {
	case LEFT_KEY:
		x -= (int)(run_speed / ex);
		break;
	case RIGHT_KEY:
		x += (int)(run_speed / ex);
		break;
	}
	switch (input_stick2) {
	case UP_KEY:
		y -= (int)(run_speed / ex);
		break;
	case DOWN_KEY:
		y += (int)(run_speed / ex);
		break;
	}
}


/////////��������Student///////////////////

Student::Student(int k, int sx, int sy, int wea, int it, int lev) {
	kind = k;
	x = sx;
	next_x = x;
	y = sy;
	next_y = y;
	//����
	weapon = new Weapon(wea);
	item = it;//�񕜖�̌�
	level = lev;

	max_hp = 1000;
	defense = 50;//�h��̓f�t�H���g��50% �G�͏��0
	//�����Ń��x���ɉ����ăX�e�[�^�X��ݒ�
	level_set();
	hp = max_hp;//�f�t�H���g��1000

	load_dot();//�h�b�g�G�̃��[�h

	//�񕜖�̃��[�h
	item_handle = LoadGraph("picture/�񕜖�.png");
	int icon_x_size;
	GetGraphSize(item_handle, &icon_x_size, &icon_y_size);
	icon_y_size *= DOT_EXTEND;

	//�񓯊��ǂݍ���
	SetUseASyncLoadFlag(TRUE);
	ase_handle = LoadGraph("picture/��.png");
	heart_handle = LoadGraph("picture/�n�[�g.png");
	debuff_handle = LoadGraph("picture/�f�o�t.png");
	SetUseASyncLoadFlag(FALSE);
	//�񓯊��ǂݍ��ݏI��
}

Student::~Student() {
	DeleteGraph(yarare_handle);
	DeleteGraph(item_handle);
	DeleteGraph(ase_handle);
	DeleteGraph(heart_handle);
}

//���x���ɉ����ăX�e�[�^�X����
void Student::level_set() {
	max_hp += (level - 1) * STUDENT_HP_UP;
}

int Student::get_weapon_dot() const{
	return weapon->get_dot();
}

int Student::get_kind() const {
	return kind;
}

int Student::get_item() const {
	return item;
}

//�h�b�g�G�̃��[�h
void Student::load_dot() {
	string name = "picture/dot/";
	name += st_name[kind];
	if (month >= 6 && month <= 10) { name += "��"; }
	run[0] = LoadGraph((name + "����1.png").c_str());
	run[1] = LoadGraph((name + "����2.png").c_str());
	GetGraphSize(run[0], &x_size, &y_size);
	x_size *= DOT_EXTEND;
	y_size *= DOT_EXTEND;
	yarare_handle = LoadGraph((name + "����.png").c_str());
}

//�L�[�{�[�h�ő��삷��
void Student::control() {
	input_stick = control_stick();
	input_stick2 = control_stick2();
	input_left = left_click();
	input_right = right_click();
	input_e = control_cure();
	//input_q = 
}

//��ʊO�ւ͍s���Ȃ�
void Student::limit() {
	if (x < 0) { x = 0; next_x = x;}
	if(x + x_size > GAME_WIDE / 2){
		x = GAME_WIDE / 2 - x_size;
		next_x = x;
	}
	if (y + y_size < 265) {//120
		y = 265 - y_size;
		next_y = y;
	}
	if (y + y_size > 645 ) {//500
		y = 645 - y_size;
		next_y = y;
	}
}

//�L�����̃h�b�g�G��`��
void Student::draw(bool bright_flag) {
	if (hp <= 0) {
		DrawRotaGraph2(x, y, 0, 0, DOT_EXTEND, 0, yarare_handle, TRUE, FALSE);
		return;
	}
	draw_dot(bright_flag);//�L�����̃h�b�g�G
	//�s���`���̊�
	if (hp * 100 / max_hp <= 30) {
		DrawRotaGraph2(x - 20, y - icon_y_size + 60, 0, 0, DOT_EXTEND - 2, 0, ase_handle, TRUE, FALSE);
	}
	if (icon_cnt > 0) {
		icon_cnt--;
		if (heart) {
			DrawRotaGraph2(x, y - icon_y_size - 20 + icon_cnt, 0, 0, DOT_EXTEND, 0, heart_handle, TRUE, FALSE);
		}
		else {
			DrawRotaGraph2(x, y - icon_y_size - 20 + icon_cnt, 0, 0, DOT_EXTEND, 0, item_handle, TRUE, FALSE);
		}
	}

	//�h�䒆
	if (get_defense() > 0) {
		DrawBox(x + x_size, y, x + x_size + 10, y + y_size, LIGHT_BLUE2, TRUE);
	}
	if (debuff_time > 0) {
		DrawRotaGraph2(x, y, 0, 0, DOT_EXTEND, 0, debuff_handle, TRUE, FALSE);
	}
	//DrawFormatString(10, 10, GetColor(0, 0, 0), "x = %d", x);
	//DrawFormatString(10, 40, GetColor(0, 0, 0), "y_size = %d", y_size);
}

//AI
void Student::ai() {
	switch (kind) {
	case HANDA:
		ai_attack(100, 80, 30);
		ai_normal(30, 40, 30, 100, 0);
		break;
	case OOTA:
		ai_attack(100, 100, 90);
		ai_normal(0, 20, 80, 50, 0);
		break;
	case ISHI:
		ai_attack(100, 90, 70);
		ai_normal(20, 60, 20, 130, 0);
		break;
	case TAIRA:
		ai_attack(100, 100, 100);
		ai_normal(100, 0, 0, 100, 0);
		break;
	case OWADA:
		ai_attack(100, 70, 60);
		ai_normal(10, 60, 30, 50, 0);
		break;
	case MIYATONE:
		ai_attack(100, 50, 10);
		ai_normal(10, 30, 60, 30, 0);
		break;
	case UCHIMOTO:
		ai_attack(100, 100, 100);
		ai_normal(0, 40, 60, 130, 0);
		break;
	case IBARAKAWA:
		ai_attack(100, 90, 90);
		ai_normal(30, 40, 30, 180, 0);
		break;
	case TOMOCHI:
		ai_attack(100, 100, 50);
		ai_normal(20, 40, 40, 50, 0);
		break;
	}
}

void Student::ai_attack(int left, int middle, int right) {
	if (attack_cnt > 0) {//�U����
		attack_cnt--;
		input_right = 0;
		input_left = 1;
	}
	else if (attack_cnt < 0) {//�h�䒆
		attack_cnt++;
		input_right = 1;
		input_left = 0;
	}
	else {
		input_right = 0;
		input_left = 0;
		int r = GetRand(99);
		int i = MAX_INTERVAL - weapon->get_interval() + 1;
		if (x < 180 && r < left) {
			attack_cnt = GetRand(5) * i;
		}
		else if (x >= 180 && x < 360 && r < middle) {
			attack_cnt = GetRand(5) * i;
		}
		else if(x >= 360 && r < right){
			attack_cnt = GetRand(5) * i;
		}
		else {//�h��
			attack_cnt = -1 * (GetRand(60) + 30);
		}
	}
}

void Student::input_reset() {
	input_stick = FALSE;
	input_stick2 = FALSE;
	input_left = FALSE;
	input_right = FALSE;
	input_e = FALSE;
	debuff_time = 0;//�f�o�t���I��
	weapon->reset();//�e�����ׂď���
}

//�񕜂���^�[�Q�b�g��ݒ�
void Student::set_target(int n) {
	target = n;
}

//�񕜂���^�[�Q�b�g���擾
int Student::get_target() {
	return target;
}

//�����ɉ񕜂��Ă��炤
void Student::cure_hp() {
	hp = max_hp;
	heart = true;
	icon_cnt = 30;
}

//�����ɉ񕜖���g�p
bool Student::cure_friend(Student& s) {
	if (hp <= 0) {//���g������Ă���
		return false;
	}
	//�񕜂���Ă���r��
	if (heart == false && icon_cnt > 0) {
		return false;
	}
	//�񕜃{�^���������Ă��Ȃ��A�܂��͑���̗̑͂��}�b�N�X�A�܂��͉񕜖򂪂Ȃ�
	if (input_e != CURE_KEY || s.get_hp() <= 0 || s.get_hp() == s.get_max_hp() || item == 0) {
		return false;
	}
	else {
		item--;
		s.cure_hp();
		heart = false;
		icon_cnt = 30;
	}
	return true;
}

const int CURE_RATE = 25;//HP�������ȉ��Ȃ�񕜂��邩
//���Ԃ��񕜂��邩���f����AI
void Student::cure_ai(int n, Student& s) {
	if (hp <= 0) { return; }//���g�����ꂽ�Ȃ�񕜂ł��Ȃ�
	if (s.get_hp() * 100 / s.get_max_hp() <= CURE_RATE && item > 0 && GetRand(99) < 3) {
		target = n;
		input_e = CURE_KEY;
	}
}

//�񕜂̒��~
void Student::cure_reset() {
	target = -1;
	input_e = FALSE;
}


/////////�G///////

Enemy::Enemy(int k, int sx, int sy, int lev) {
	//RUN_SPEED = 6;
	hp_dx = 0;
	kind = k;//�G�̎��
	create_weapon();
	x = sx;
	next_x = x;
	y = sy;
	next_y = y;
	level = lev;
	defense = 0;//�h��̓f�t�H���g��50 �G��0
	//�����Ń��x���ɉ����ăX�e�[�^�X��ݒ�
	level_set();
	hp = max_hp;//�f�t�H���g��100
	weapon->cus(level);//����̋�������
	load_dot();//�h�b�g�G�̃��[�h

	debuff_handle = LoadGraph("picture/�f�o�t.png");
}

Enemy::~Enemy() {

}

//�G�̎�ނɉ����ĕ����p�� & HP�ݒ�
void Enemy::create_weapon() {
	weapon = new Weapon(WEAPON_SUM + kind + 1);
	max_hp = en_hp[kind];
	//�L�����ʂ̂����蔻�蒲�� (�����ڂ�肠���蔻����ɂ�����)
	switch (kind) {
	case TAKO:
		RUN_SPEED = 9;
		break;
	case TESSIE:
		x_size = (37 * x_size) / 50;
		hp_dx = 50;
		break;
	case UM01B:
		x_size = (37 * x_size) / 50;
		break;
	}
}

//���x���ɉ����ăX�e�[�^�X����
void Enemy::level_set() {
	max_hp += level * ENEMY_HP_UP;
}

//�h�b�g�G�̃��[�h
void Enemy::load_dot() {
	string name = "picture/enemy/";
	name += en_name[kind];
	run[0] = LoadGraph((name + "1.png").c_str());
	run[1] = LoadGraph((name + "2.png").c_str());
	GetGraphSize(run[0], &x_size, &y_size);
	x_size *= DOT_EXTEND;
	y_size *= DOT_EXTEND;
}


//HP�̃o�[�̍���
const int ENEMY_HP_HEIGHT = 20;
//�G�̃h�b�g�G��`��
void Enemy::draw(bool bright_flag) {
	draw_dot(bright_flag);//�L�����̃h�b�g�G
	//HP�\��
	int border = x_size * hp / max_hp;
	DrawBox(x + hp_dx , y - ENEMY_HP_HEIGHT, x + border + hp_dx, y - 10, GREEN, TRUE);
	DrawBox(x + border + hp_dx, y - ENEMY_HP_HEIGHT, x + x_size + hp_dx, y - 10, RED, TRUE);
	if (debuff_time > 0) {//�f�o�t�G�t�F�N�g
		DrawRotaGraph2(x, y, 0, 0, DOT_EXTEND, 0, debuff_handle, TRUE, FALSE);
	}
	//DrawFormatString(10, 10, GetColor(0, 0, 0), "x = %d, y = %d", x, y);
	//DrawFormatString(10, 40, GetColor(0, 0, 0), "x_size = %d", x_size);
}

//�w�i�ړ��̍ۂɍ��킹�ē������p
void Enemy::dash(int dx) {
	x += dx;
	next_x += dx;
}

//�ړ����E���W
void Enemy::limit() {
	if (x < GAME_WIDE / 2) { 
		x = GAME_WIDE / 2;
		next_x = x;
	}
	if (x + x_size > GAME_WIDE) {
		x = GAME_WIDE - x_size;
		next_x = x;
	}
	if (y + y_size < 265) {//120
		y = 265 - y_size;
		next_y = y;
	}
	if (y + y_size > 645) {//500
		y = 645 - y_size;
		next_y = y;
	}
}

//AI
void Enemy::ai() {
	input_left = 1;
	ai_normal(30, 30, 30, 70, GAME_WIDE / 2);
}

int Enemy::get_money() {
	return drop_money[kind];
}

int Enemy::get_drop() {
	return drop_item[kind];
}