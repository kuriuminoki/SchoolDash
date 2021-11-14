#include"Weapon.h"
#include"Chara.h"
#include"DxLib.h"
#include"SDvalue.h"
#include"Button.h"
#include"Control.h"

#include<cmath>
#include<algorithm>
#include<string>
#include<sstream>

using namespace std;

//�e�̎�ނƏ������W�ݒ�
Bullet::Bullet(int k, int sx, int sy) {
	x = sx; y = sy;
	kind = k;
	distance = 0;

	//�f�t�H���g�ݒ�
	rx = 10; ry = 10;
	dx = 10; dy = 0;
	damage = 10;
}

bool Bullet::operator> (const Bullet &other) const {
	return y > other.get_y();
}

//����(�X�s�[�h)�̐ݒ�
void Bullet::set_dir(int srx, int sry) {
	rx = srx; ry = sry;
}
//�傫���̐ݒ�
void Bullet::set_scale(int sdx, int sdy) {
	dx = sdx; dy = sdy;
}
//�_���[�W�̐ݒ�
void Bullet::set_damage(int after_damage) {
	damage = after_damage;
}

//�e�ϐ��̎擾
int Bullet::get_x() const {
	return x;
}
int Bullet::get_y() const {
	return y;
}
int Bullet::get_dx() const {
	return dx;
}
int Bullet::get_dy() const {
	return dy;
}
int Bullet::get_damage() const {
	return damage;
}

int Bullet::get_kind() const {
	return kind;
}

int Bullet::get_distance() const {
	return distance;
}

//�ړ�
void Bullet::move() {
	x += rx;
	y += ry;
	distance += (int)sqrt((double)(rx*rx + ry*ry));
}

///////////��������Weapon�N���X////////////

//�f�X�g���N�^
//�X�e�[�W��̒e�����ׂď���
Weapon::~Weapon() {
	delete wi;
	reset();
}

void Weapon::reset() {
	while (!bullets.empty()) {
		delete bullets.front();
		bullets.pop();
	}
}

//�A�˗͂��擾
int Weapon::get_interval() {
	return wi->INTERVAL;
}

//�ǉ��̖h��͂��擾
int Weapon::get_defense() {
	return wi->DEFENSE;
}

//���x�ቺ���Ԃ��擾
int Weapon::get_debuff() {
	return wi->DEBUFF;
}

//����̐��\�␳ �G�p
void Weapon::cus(int level) {
	//�U���͂Ȃǂ�ύX����
	//wi->DAMAGE += level * WEAPON_DAMAGE_UP;
	wi->DAMAGE *= level * WEAPON_DAMAGE_UP;
}

//�e�̈ړ��ƃX�e�[�W�O�ɍs�������̊m�F
void Weapon::limit(int left, int right, int up, int down) {
	unsigned int size = bullets.size();
	//�S�Ă̒e���P�񂸂m�F����
	for (unsigned int i = 0; i < size; i++) {
		Bullet* b = bullets.front();
		bullets.pop();
		b->move();//�e�̈ړ�
		if (b->get_distance() > wi->DISTANCE) {
			delete b;//�˒��O
		}
		else if (b->get_x()+ wi->DX < left || b->get_x()- wi->DX > right || b->get_y()+ wi->DY < up || b->get_y()- wi->DY > down) {
			//�X�e�[�W�O
			delete b;
		}
		else {//�X�e�[�W��
			bullets.push(b);
		}
	}
}

//�e�̔��� click�͍U���̓��͂�����Ă��邩�ǂ���
bool Weapon::attack(bool click, int x, int y) {
	if (cnt > 0) {//�C���^�[�o����
		cnt--;
	}
	if (!click) {//�U�����Ă��Ȃ�
		
	}
	else {//�U����
		if (cnt == 0) {
			//����
			Bullet* b = new Bullet(wi->KIND, x, y);
			b->set_damage(wi->DAMAGE);//�_���[�W�ݒ�
			//�����i�X�s�[�h�j�ݒ�
			if (wi->RX == 0 && wi->RY == 0) {//�����_���ȕ���
				random_r();
				b->set_dir(wi->RX, wi->RY);
				wi->RX = 0; wi->RY = 0;
			}
			else {//���܂�������
				b->set_dir(wi->RX, wi->RY);
			}		
			b->set_scale(wi->DX, wi->DY);//�傫���ݒ�
			bullets.push(b);//�e�̃Z�b�g����
			//�����Ă�܂ł̎���
			cnt = MAX_INTERVAL - wi->INTERVAL + 1;
			return true;
		}
	}
	return false;
}

const int RANGE = 60;//�e����Ԋp�x�͈̔�(��)
//�e�̔�ԕ����������_���Ɍ��肷��
static const double PI = 3.14159;
void Weapon::random_r() {
	int r = GetRand(RANGE) - (RANGE / 2);
	wi->RX = (int)(wi->SPEED * cos(PI * r / 180));
	wi->RY = (int)(wi->SPEED * sin(PI * r / 180));
}

//�Q�̃I�u�W�F�N�g���d�Ȃ��Ă��邩���ʂ���̂Ɏg��
bool atari_judge(int x1, int x2, int dx1, int dx2) {
	if (abs(x1 - x2) < dx1 + dx2) {
		return true;
	}
	return false;
}
//�e�̂����蔻�� �h�䉹��炷�Ƃ���true��Ԃ�
bool Weapon::atari(Character& c) {
	bool flag = false;
	//����̍��W��傫�����擾
	int cx = c.get_cx();
	int x_size = c.get_xsize();
	int cy = c.get_cy();
	int y_size = c.get_ysize();
	int d = c.get_defense();//����̖h���

	unsigned int size = bullets.size();
	//�S�Ă̒e���݂�
	for (unsigned int i = 0; i < size; i++) {
		Bullet* b = bullets.front();
		bullets.pop();
		//�L���[��Y���W�ƒe�̎�ނ�����
		if (atari_judge(b->get_x(), cx, b->get_dx(), x_size/2) && atari_judge(b->get_y(), cy, b->get_dy(), y_size/2)) {
			//��������
			c.change_hp((d - 100)*wi->DAMAGE / 100);//DAMAGE��d%����
			c.set_debuff(wi->DEBUFF);
			if (d > 0) { flag = true; }	
			else{ PlaySoundMem(wi->atari_handle, DX_PLAYTYPE_BACK); }
			delete b;
		}
		else {//�������Ă��Ȃ�
			bullets.push(b);
		}
	}
	return flag;
}

//�`��p�̗D��x�L���[�ɒe�̏�������
void Weapon::push_draw(std::priority_queue<Bullet, std::vector<Bullet>, std::greater<Bullet> >& que) {
	unsigned int size = bullets.size();
	//DrawFormatString(10, 720, GetColor(255,255,255), "tama_num = %d", bullets.size());
	//�S�Ă̒e������
	for (unsigned int i = 0; i < size; i++) {
		Bullet* b = bullets.front();
		bullets.pop();
		//�L���[�ɒe������
		que.push(*b);
		bullets.push(b);
	}
}

int Weapon::get_dot() const {
	return wi->get_dot();
}

//����̍쐬
Weapon::Weapon(int kind) {
	wi = new WeaponInfo(kind);
}

//����̃_���[�W�Ȃǂ��Z�b�g����
void Weapon::set_value(int da, int sdx, int sdy, int in, int di, int s, int srx, int sry) {
	wi->DAMAGE = da;
	wi->DX = sdx; wi->DY = sdy;
	wi->INTERVAL = in;
	wi->DISTANCE = di;
	wi->SPEED = s;
	wi->RX = srx; wi->RY = sry;
}
//����̎�ނȂǂ��Z�b�g����
void Weapon::set_looks(int kind, int defense, int debuff, const char tama_name[]) {
	wi->KIND = kind;
	wi->DEFENSE = defense;
	wi->DEBUFF = debuff;
	string s;
	s = "sound/atari/";
	s += tama_name;
	wi->atari_handle = LoadSoundMem(s.c_str());//�����莞�̌��ʉ�
	ChangeVolumeSoundMem(255 * (SoundVolume - 5) / 100, wi->atari_handle);
}


////////WeaponInfo�N���X/////////////

//����̔ԍ���"1"�X�^�[�g
//csv�t�@�C�����畐��̏��𓾂�
WeaponInfo::WeaponInfo(int kind) {
	//����̉摜��ǂݍ���
	string dot_name = "";
	int num = kind;
	while (num > 0) {
		dot_name += '0' + (num % 10);
		num /= 10;
	}
	reverse(dot_name.begin(), dot_name.end());
	dot_name = "picture/weapon/weapon" + dot_name + ".png";
	//csv�t�@�C�����琫�\�����
	int fp;
	char buff[256];
	fp = FileRead_open("text/weapon.csv");
	//�t�@�C���|�C���^���ړ�
	for (int i = 1; i <= kind; i++) {
		FileRead_gets(buff, 256, fp);//�Ō�ȊO�ǂݔ�΂�
	}
	//����̏����e�L�X�g�œ���
	string s = "";
	for (int i = 0; buff[i] != '\0'; i++) {
		if (buff[i] == ',') {
			data.push_back(s);
			s = "";
		}
		else {
			s += buff[i];
		}
	}
	data.push_back(s);

	name = data[0];
	for (unsigned int i = 1; i < data.size(); i++) {
		value.push_back(atoi(data[i].c_str()));
	}
	DAMAGE = value[0];
	DX = value[1]; DY = value[2];
	INTERVAL = value[3];
	DISTANCE = value[4];
	SPEED = value[5];
	RX = value[6]; RY = value[7];
	DEFENSE = value[8];
	DEBUFF = value[9];

	//�e�̌����ڂƌ��ʉ��𐫔\�����Ĕ��f
	if (SPEED > 0) {
		if (DX == DY) {
			KIND = BLUE_BALL;
		}
		else {
			KIND = BLUE_ZAN;
		}
	}
	else {
		if (DX == DY) {
			KIND = RED_BALL;
		}
		else {
			KIND = RED_ZAN;
		}
	}

	//���ʉ�
	if (KIND == BLUE_ZAN || KIND == RED_ZAN) {
		atari_handle = LoadSoundMem("sound/atari/�a��������.wav");
		ChangeVolumeSoundMem(255 * (SoundVolume - 5) / 100, atari_handle);
	}
	else {
		atari_handle = LoadSoundMem("sound/atari/�e������.wav");
		ChangeVolumeSoundMem(255 * SoundVolume / 100, atari_handle);
	}

	//����̃h�b�g�G
	dot_handle = LoadGraph(dot_name.c_str());

	FileRead_close(fp);
}

WeaponInfo::~WeaponInfo() {
	if (dot_handle != -1) {
		DeleteGraph(dot_handle);
	}
	DeleteSoundMem(atari_handle);
}

int WeaponInfo::get_dot() const {
	return dot_handle;
}

void WeaponInfo::get_name(std::string& s) const {
	s = name;
}

void WeaponInfo::draw(int x, int y, int color, int font, int font_color) {
	static const int WIDE = 600, HEIGHT = 300;
	DrawBox(x, y, x + WIDE, y + HEIGHT, color, TRUE);
	DrawRotaGraph2(x + 350, y + 70, 0, 0, DOT_EXTEND*2, 0, dot_handle, TRUE, FALSE);
	DrawStringToHandle(x + 10, y + 10, name.c_str(), BLACK, font);
	ostringstream s;
	s << DAMAGE << endl << SPEED << endl << INTERVAL << endl;
	s << DISTANCE << endl << DY << endl << DEFENSE << endl << DEBUFF;
	DrawStringToHandle(x + 250, y + 60, s.str().c_str(), DARK_YELLOW, font);

	ostringstream n;
	n << "�U����" << endl << "�e��" << endl << "�A�˗�" << endl;
	n << "�˒�" << endl << "�傫��" << endl << "�h���" << endl << "�f�o�t����";
	DrawStringToHandle(x + 10, y + 60, n.str().c_str(), font_color, font);
}



/////////����ύX��ʃN���X///////////////

//���k�P�O�l�̃h�b�g�G���󂯎��
WeaponChange::WeaponChange(int handle[]) {
	GetMouseWheelRotVol();//�z�C�[���̉�]�ʃ��Z�b�g
	//�t�H���g
	font_handle = CreateFontToHandle(NULL, 50, 10);
	font_small_handle = CreateFontToHandle(NULL, 30, 8);
	//���k10�l�̃{�^��
	button_st = new StButton(10, handle, 200);
	//�߂�{�^��
	button_cancel = new Button("�߂�", 30, 20, 150, 80, LIGHT_GREEN, GREEN, font_handle, BLACK);
	//���̉摜�n���h��
	up_handle = LoadGraph("picture/����.png");
	down_handle = LoadGraph("picture/�����.png");
	//���{�^��
	button_up = new Button("", 50, 150, 500, 70, GRAY2, YELLOW, font_handle, BLACK);
	button_up->change_flag(false, GRAY);
	button_down = new Button("", 50, 650, 500, 70, GRAY2, YELLOW, font_handle, BLACK);
	button_up->set_graph(up_handle, 3);
	button_down->set_graph(down_handle, 3);

	//����̗p��
	wi = new WeaponInfo*[WEAPON_SUM];
	for (int i = 0; i < WEAPON_SUM; i++) {
		wi[i] = new WeaponInfo(i + 1);//kind = �P�`
	}
	button_weapon = new Button*[MAX_WEAPON];
	for (int i = 0; i < MAX_WEAPON; i++) {
		button_weapon[i] = new Button("", 50, 240+(i*50), 500, 40, LIGHT_BLUE, BLUE, font_small_handle, BLACK);
	}
	weapon_num = new int[MAX_WEAPON];

	//���̑�
	now = 0;
	weapon_reset();//�{�^���ɕ����ݒ�
}

WeaponChange::~WeaponChange() {
	DeleteFontToHandle(font_handle);
	DeleteFontToHandle(font_small_handle);
	DeleteGraph(up_handle);
	DeleteGraph(down_handle);
	delete button_st;
	delete button_cancel;
	delete button_up;
	delete button_down;
	for (int i = 0; i < WEAPON_SUM; i++) {
		delete wi[i];
	}
	delete[] wi;
	for (int i = 0; i < MAX_WEAPON; i++) {
		delete button_weapon[i];
	}
	delete[] button_weapon;
	delete[] weapon_num;
}

//num�Ԗڂ̐��k�𕐊�ύX�s�ɂ���
void WeaponChange::cancel_st(int num) {
	button_st->change_flag(num, false);
}

//���C��
bool WeaponChange::play() {
	GetMousePoint(&hand_x, &hand_y);
	//���k�I���ς�
	if (st_select != -1) {
		play_wi();
	}

	//���k�I������
	return play_st();
}

//�`��
void WeaponChange::draw() {
	//�w�i
	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, DARK_GREEN, TRUE);
	//���k�I���ς�
	if (st_select != -1) {
		draw_wi();
	}

	//���k�{�^���\��
	button_st->draw(hand_x, hand_y);
	//�߂�{�^���\��
	button_cancel->draw(hand_x, hand_y);
}

//���k�I�����
bool WeaponChange::play_st() {
	//���N���b�N���ꂽ�Ƃ�
	if (left_click() == 1) {
		//�߂�{�^��
		if (button_cancel->overlap(hand_x, hand_y)) {
			return true;
		}
		int n = button_st->overlap(hand_x, hand_y);
		if (n != -1) {
			if (st_select != -1) {//���Ƃ��ƑI��ł������ktrue
				button_st->change_flag(st_select, true);
			}
			st_select = n;//���k�I��
			button_st->change_flag(n, false);//�I���������kfalse
		}
	}
	return false;
}

//����ύX����
void WeaponChange::play_wi() {
	//����{�^���ɃJ�[�\�����d�Ȃ��Ă��邩�m�F
	touch = -1;
	for (int i = 0; i < MAX_WEAPON; i++) {
		if (button_weapon[i]->overlap(hand_x, hand_y) && weapon_num[i] != -1) {
			touch = i;
		}
	}
	//�}�E�X�̃z�C�[����]
	int rot = GetMouseWheelRotVol();
	if (rot > 0 && button_up->get_flag()) {//���
		now_up();
		weapon_reset();
	}//����
	else if (rot < 0 && button_down->get_flag()) {
		now_down();
		weapon_reset();
	}//���N���b�N���ꂽ�Ƃ� ����������
	else if (left_click() == 1 || (left_click() > 10 && left_click() % 2 == 1)) {
		//����
		if (button_up->overlap(hand_x, hand_y)) {
			now_up();
			weapon_reset();
		}//�����
		else if (button_down->overlap(hand_x, hand_y)) {
			now_down();
			weapon_reset();
		}
	}
	//���N���b�N���ꂽ�Ƃ�
	if (left_click() == 1) {
		//�߂�{�^��
		if (button_cancel->overlap(hand_x, hand_y)) {
			st_select = -1;
		}
		//����{�^��
		if (touch != -1) {
			//�������̕�����O��
			weapon_list[have_weapon[st_select]-1]++;
			//�V���ɕ����t���ւ���
			have_weapon[st_select] = weapon_num[touch];
			weapon_list[weapon_num[touch]-1]--;
			weapon_reset();
		}
	}
}

//�������̕���ƑI�ׂ镐���\��
void WeaponChange::draw_wi() {
	button_up->draw(hand_x, hand_y);//����
	button_down->draw(hand_x, hand_y);//�����
	DrawStringToHandle(600, 20, "�������̕���", GREEN, font_handle);
	wi[have_weapon[st_select]-1]->draw(600, 70, LIGHT_BLUE, font_small_handle, DARK_RED);
	if (touch != -1 && weapon_num[touch] > 0) {//�J�[�\��������{�^���ɏd�Ȃ��Ă���Ƃ�
		DrawStringToHandle(600, 400, "�I�𒆂̕���", YELLOW, font_handle);
		wi[weapon_num[touch]-1]->draw(600, 450, LIGHT_BLUE, font_small_handle, DARK_RED);
	}

	//����{�^��
	for (int i = 0; i < MAX_WEAPON; i++) {
		button_weapon[i]->draw(hand_x, hand_y);
	}
	//����̐���\��
	ostringstream s;
	s << "�������Ă��镐��F" << sum << "/100";
	DrawStringToHandle(50, 110, s.str().c_str(), GREEN, font_small_handle);
	DrawBox(20, 240, 40, 630, BLACK, TRUE);
	if (sum != 0) {
		int rate1 = (int)(390 * sum_rate / sum);
		int rate2 = min(390, (int)(390 * (sum_rate + MAX_WEAPON) / sum));
		DrawBox(20, 240 + rate1, 40, 240 + rate2, WHITE, TRUE);
	}
	//�e�X�g
	//DrawFormatString(0, 0, BLACK, "sum = %d", sum);
	//DrawFormatString(100, 0, BLACK, "rate2 = %d", rate2);
}

//����{�^���̕\�������Z�b�g�@��󂪉����ꂽ�Ƃ�
void WeaponChange::weapon_reset() {
	sum = 0; sum_rate = 0;
	bool up_flag = false;//���󂪉����邩�ǂ���
	bool down_flag = false;//����󂪉����邩�ǂ���
	int d = 0;//�{�^���̃^�O�������ߏI�������
	for (int i = 0; i < WEAPON_SUM; i++) {
		if (weapon_list[i] == 0) {
			continue;
		}
		sum++;
		if (i < now) { sum_rate++; }
		if (!up_flag && i < now) {
			up_flag = true;//�܂���ɕ��킪����
		}
		if (!down_flag && d >= MAX_WEAPON) {
			down_flag = true;//�܂����ɕ��킪����
		}
		//�{�^���ɕ\�����镐��
		if (i >= now && d < MAX_WEAPON) {
			weapon_num[d] = i+1;//����ԍ����Z�b�g
			string n;
			ostringstream s;
			wi[i]->get_name(n);//����̖��O���擾
			s << n.c_str() << "�~" << weapon_list[i];
			button_weapon[d]->set_string(s.str().c_str());//����̖��O���{�^���ɃZ�b�g
			d++;
		}
	}
	//���܂�Ȃ������������󔒂ɂ���
	for (d; d < MAX_WEAPON; d++) {
		weapon_num[d] = -1;
		button_weapon[d]->set_string("");
	}
	//���{�^���̐ݒ�
	if (up_flag) {
		button_up->change_flag(true, GRAY2);
	}
	else {
		button_up->change_flag(false, GRAY);
	}
	if (down_flag) {
		button_down->change_flag(true, GRAY2);
	}
	else {
		button_down->change_flag(false, GRAY);
	}
}

void WeaponChange::now_up() {
	for (int i = now - 1; i >= 0; i--) {
		if (weapon_list[i] > 0) {
			now = i;
			break;
		}
	}
}

void WeaponChange::now_down() {
	for (int i = now + 1; i < WEAPON_SUM; i++) {
		if (weapon_list[i] > 0) {
			now = i;
			break;
		}
	}
}



///////////����쐬���///////////////////

WeaponCreate::WeaponCreate() {
	GetMouseWheelRotVol();//�z�C�[���̉�]�ʃ��Z�b�g
	font_handle = CreateFontToHandle(NULL, 50, 10);
	font_small_handle = CreateFontToHandle(NULL, 30, 8);
	//�߂�{�^��
	button_cancel = new Button("�߂�", 30, 20, 150, 80, LIGHT_GREEN, GREEN, font_handle, BLACK);
	//���̉摜�n���h��
	up_handle = LoadGraph("picture/����.png");
	down_handle = LoadGraph("picture/�����.png");
	//���{�^��
	button_up = new Button("", 50, 150, 500, 70, GRAY2, YELLOW, font_handle, BLACK);
	button_up->change_flag(false, GRAY);
	button_down = new Button("", 50, 650, 500, 70, GRAY2, YELLOW, font_handle, BLACK);
	button_up->set_graph(up_handle, 3);
	button_down->set_graph(down_handle, 3);

	//����̗p��
	wi = new WeaponInfo*[CREATE_SUM];
	for (int i = 0; i < CREATE_SUM; i++) {
		wi[i] = new WeaponInfo(i + 1 + (WEAPON_SUM - CREATE_SUM));
	}
	//����{�^��
	button_weapon = new Button*[MAX_WEAPON];
	for (int i = 0; i < MAX_WEAPON; i++) {
		button_weapon[i] = new Button("", 50, 240 + (i * 50), 500, 40, LIGHT_BLUE, BLUE, font_small_handle, BLACK);
	}
	//�K�v�f�ނ̏��
	need = new int*[CREATE_SUM];
	for (int i = 0; i < CREATE_SUM; i++) {
		need[i] = new int[MATERIAL_SUM];
	}
	//csv�t�@�C�����琫�\�����
	int fp;
	char buff[256];
	fp = FileRead_open("text/material.csv");
	//�P�s���ǂ�ł���
	for (int i = 0; i < CREATE_SUM; i++) {
		string s = "";
		FileRead_gets(buff, 256, fp);
		int n = 0;
		//����̏����e�L�X�g�œ���
		for (int j = 0; buff[j] != '\0'; j++) {
			if (buff[j] == ',' && n < MATERIAL_SUM) {
				need[i][n] = atoi(s.c_str());
				n++;
				s = "";
			}
			else {
				s += buff[j];
			}
		}
		need[i][n] = atoi(s.c_str());
	}
	FileRead_close(fp);//�t�@�C���N���[�Y
	able = new bool[CREATE_SUM];
	check_able();//���킪���邩�m�F
	weapon_reset();//����{�^����ݒ�
}

WeaponCreate::~WeaponCreate() {
	DeleteFontToHandle(font_handle);
	DeleteFontToHandle(font_small_handle);
	DeleteGraph(up_handle);
	DeleteGraph(down_handle);
	delete button_cancel;
	delete button_up;
	delete button_down;
	for (int i = 0; i < CREATE_SUM; i++) {
		delete wi[i];
		delete[] need[i];
	}
	delete[] wi;
	delete[] need;
	for (int i = 0; i < MAX_WEAPON; i++) {
		delete button_weapon[i];
	}
	delete[] button_weapon;
	delete[] able;
}

bool WeaponCreate::play() {
	GetMousePoint(&hand_x, &hand_y);
	//����{�^���ɃJ�[�\�����d�Ȃ��Ă��邩�m�F
	touch = -1;
	for (int i = 0; i < MAX_WEAPON; i++) {
		if (button_weapon[i]->overlap(hand_x, hand_y)) {
			touch = i;
		}
	}

	//�}�E�X�̃z�C�[����]
	int rot = GetMouseWheelRotVol();
	if (rot > 0 && button_up->get_flag()) {//���
		now--;
		weapon_reset();
	}
	else if (rot < 0 && button_down->get_flag()) {//����
		now++;
		weapon_reset();
	}
	else if (left_click() == 1) {//���N���b�N��
		if (button_up->overlap(hand_x, hand_y) && button_up->get_flag()) {
			now--;
			weapon_reset();
		}
		if (button_down->overlap(hand_x, hand_y) && button_down->get_flag()) {
			now++;
			weapon_reset();
		}
		if (button_cancel->overlap(hand_x, hand_y)) {
			return true;
		}
		if (touch != -1 && able[now + touch]) {//���
			create();
			check_able();//�f�ނ�����̂ōēx�`�F�b�N
			weapon_reset();//����ɍ��킹�ĕ���{�^�����X�V
		}
	}
	return false;
}

void WeaponCreate::weapon_reset() {
	for (int i = 0; i < MAX_WEAPON; i++) {
		string n;
		wi[i + now]->get_name(n);//����̖��O���擾
		button_weapon[i]->set_string(n.c_str());//����̖��O���{�^���ɃZ�b�g
		if (able[i + now]) {
			button_weapon[i]->change_flag(true, LIGHT_BLUE);//���镐��
		}
		else {
			button_weapon[i]->change_flag(true, LIGHT_BLUE2);//���Ȃ�����
		}
	}
	if (now == 0) {
		button_up->change_flag(false, GRAY);
	}
	else {
		button_up->change_flag(true, GRAY2);
	}
	if (now == CREATE_SUM - MAX_WEAPON) {
		button_down->change_flag(false, GRAY);
	}
	else {
		button_down->change_flag(true, GRAY2);
	}
}

void WeaponCreate::check_able() {
	for (int i = 0; i < CREATE_SUM; i++) {
		able[i] = true;
		if (weapon_list[i % 10] <= 0) {//���q�p�̕���f��
			able[i] = false;
			continue;
		}
		for (int j = 0; j < MATERIAL_SUM; j++) {//�A�C�e���f��
			if (need[i][j] > material[j]) {
				able[i] = false;
				break;
			}
		}
	}
}

void WeaponCreate::draw() {
	//�w�i
	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, DARK_GREEN, TRUE);
	//�߂�{�^��
	button_cancel->draw(hand_x, hand_y);
	button_up->draw(hand_x, hand_y);
	button_down->draw(hand_x, hand_y);
	//�J�[�\��������{�^���ɏd�Ȃ��Ă���Ƃ�
	if (touch != -1) {
		ostringstream s;
		s << "�I�𒆂̕���F" << weapon_list[now + touch + (WEAPON_SUM - CREATE_SUM)];
		s << "����";
		DrawStringToHandle(600, 100, s.str().c_str(), YELLOW, font_handle);
		wi[now + touch]->draw(600, 150, LIGHT_BLUE, font_small_handle, DARK_RED);
		//�K�v�f�ޕ\��
		draw_material(600, 500);
	}
	//����{�^��
	for (int i = 0; i < MAX_WEAPON; i++) {
		button_weapon[i]->draw(hand_x, hand_y);
	}
	DrawBox(20, 240, 40, 630, BLACK, TRUE);
	int rate1 = (int)(390 * now / CREATE_SUM);
	int rate2 = min(390, (int)(390 * (now + MAX_WEAPON) / CREATE_SUM));
	DrawBox(20, 240 + rate1, 40, 240 + rate2, WHITE, TRUE);
}

void WeaponCreate::draw_material(int x, int y) {
	const int height = 40;
	DrawStringToHandle(x, y, "�K�v�f��", YELLOW, font_handle);
	y += 60;
	//���q�p����f��
	int k = (now + touch) % 10;
	if (weapon_list[k] > 0) {
		DrawBox(x, y, x + 600, y + height, LIGHT_GREEN, TRUE);
	}
	else {
		DrawBox(x, y, x + 600, y + height, LIGHT_RED, TRUE);
	}
	ostringstream z;
	z << start_weapon_name[k];
	z << "�F" << weapon_list[k] << " / 1";
	DrawStringToHandle(x + 10, y + 5, z.str().c_str(), BLACK, font_small_handle);
	y += height + 5;
	//�f��
	for (int i = 0; i < MATERIAL_SUM; i++) {
		if (need[now + touch][i] == 0) { continue; }
		if (need[now + touch][i] <= material[i]) {
			DrawBox(x, y, x + 600, y + height, LIGHT_GREEN, TRUE);
		}
		else {
			DrawBox(x, y, x + 600, y + height, LIGHT_RED, TRUE);
		}
		ostringstream s;
		s << material_name[i] << "�F" << material[i] << " / " << need[now + touch][i];
		DrawStringToHandle(x+10, y+5, s.str().c_str(), BLACK, font_small_handle);
		y += height + 5;
	}
}

void WeaponCreate::create() {
	//�f�ނ�����
	weapon_list[(now + touch) % 10]--;
	for (int i = 0; i < MATERIAL_SUM; i++) {
		material[i] -= need[now + touch][i];
	}
	//���������
	weapon_list[(WEAPON_SUM - CREATE_SUM) + now + touch]++;
}