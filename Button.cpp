#include"Button.h"
#include"SDvalue.h"
#include"Control.h"
#include"Weapon.h"
#include"Dash.h"
#include"DxLib.h"
#include<string>
#include<sstream>

using namespace std;

/////�{�^��/////////////
Button::Button(string tag, int x, int y, int wide, int height, int color, int color2, int font, int font_color) {
	m_graph_handle = -1;
	m_x = x; m_y = y;
	m_wide = wide; m_height = height;
	m_color = color;
	m_color2 = color2;
	m_font = font;
	m_font_color = font_color;
	set_string(tag);
}

//�{�^���̕`�� ���Ƀ��x���̕�������\���ł���
void Button::draw(int hand_x, int hand_y) {
	if (overlap(hand_x, hand_y)) {
		DrawBox(m_x - 5, m_y - 5, m_x + m_wide + 5, m_y + m_height + 5, m_color2, TRUE);
	}
	DrawBox(m_x, m_y, m_x + m_wide, m_y + m_height, m_color, TRUE);
	//�G�̕`��
	if (!m_flag) { SetDrawBright(100, 100, 100); }
	DrawRotaGraph(m_x + (m_wide / 2), m_y + (m_height / 2), m_graph_ex, 0, m_graph_handle, TRUE, FALSE);
	SetDrawBright(255,255,255);
	//�����̕`��
	DrawStringToHandle(m_dx, m_dy, m_tag.c_str(), m_font_color, m_font);
	//DrawFormatString(m_x, m_y - 20, WHITE, "font_size = %d", m_font_size);
	//DrawFormatString(m_x, m_y - 40, WHITE, "string_size = %d", m_string_size);
}

//�{�^���̃I���E�I�t
void Button::change_flag(bool f, int new_color) {
	m_flag = f;
	m_color = new_color;
}

//�t���O���擾
bool Button::get_flag() {
	return m_flag;
}

//�{�^�����}�E�X�J�[�\���Əd�Ȃ��Ă��邩
bool Button::overlap(int hand_x, int hand_y) {
	if (!m_flag) {
		return false;
	}
	if (hand_x >= m_x && hand_x <= m_x + m_wide && hand_y >= m_y && hand_y <= m_y + m_height) {
		return true;
	}
	return false;
}

//�{�^�����ɕ����ł͂Ȃ��G��\������悤�ɃZ�b�g
void Button::set_graph(int handle, int ex) {
	m_graph_handle = handle;
	m_graph_ex = ex;
	m_tag = "";
}

//�^�O�����Ȃ���
void Button::set_string(std::string tag) {
	m_tag = tag;
	//�����񂪎�镝�𕶎��̑傫���ƕ�����̒�������v�Z
	int m_font_size;//�ꕶ���̑傫��
	int m_string_size;//�����񂪂Ƃ镝
	GetFontStateToHandle(NULL, &m_font_size, NULL, m_font);
	m_string_size = m_font_size * (m_tag.size() / 2);
	m_dx = m_x + (m_wide - m_string_size) / 2 - (m_tag.size()/2);
	m_dy = m_y + (m_height - m_font_size) / 2;
}

//�摜���擾
int Button::get_handle() {
	return m_graph_handle;
}

//�����̐��k�̃{�^�����Ǘ�����
StButton::StButton(int size, int handle[], int height) {
	m_size = size;
	int distance = 10;//�l�p�̊Ԃ̕�
	int edge = 50;//���[�̋�
	int wide = (GAME_WIDE - (edge * 2)) / size;//�l�p�̉���
	int y = GAME_HEIGHT - height - edge;//y���W
	st = new Button*[size];
	for (int i = 0; i < size; i++) {
		st[i] = new Button("", edge + (wide * i) + distance, y, wide - distance, height, LIGHT_YELLOW, YELLOW, -1, BLACK);
		st[i]->set_graph(handle[i], DOT_EXTEND);
	}
	st_absent();//���t�����āA�܂����ԂɂȂ��Ă��Ȃ����k������
}

StButton::~StButton() {
	for (int i = 0; i < m_size; i++) {
		delete st[i];
	}
	delete[] st;
}

//���t�����āA�܂����ԂɂȂ��Ă��Ȃ����k�◣�E���̐��k������
void StButton::st_absent() {
	int index = 0;//�C���f�b�N�X�w��p
	if (m_size != 10) { index = 1; }//�A�J�c�L�͂��Ȃ�
	//�n���_�ƌ��ܒ�
	if (year == 3 && (month == 1 || (month == 2 && week <= 3))) {
		st[1 - index]->set_graph(-1, 1);
		st[1 - index]->change_flag(false, BLACK);
	}
	//�I�I�^���n���_�Ƃƌ��ܒ�
	if ((year == 2 && month == 7 && week == 4) || (year == 2 && month == 9 && week <= 3)) {
		st[2 - index]->set_graph(-1, 1);
		st[2 - index]->change_flag(false, BLACK);
	}
	//�I�I�^����g���`�܂ł̂W�l
	for (int i = 0; i < 8; i++) {
		if (week_num(year, month, week) < week_num(join_year[i], join_month[i], join_week[i])) {
			//�A�J�c�L�A�n���_�̂Q�l�𑫂��āA�A�J�c�L�����Ȃ��Ȃ����ςP����
			st[i + 2 - index]->set_graph(-1, 1);
			st[i + 2 - index]->change_flag(false, BLACK);
		}
	}
}

//n�Ԗڂ̉摜�n���h�����擾
int StButton::get_handle(int num) {
	return st[num]->get_handle();
}

//n�Ԗڂ̃t���O���擾
bool StButton::get_flag(int num) {
	return st[num]->get_flag();
}

//num�Ԗڂ̐��k�̃t���O��ύX
void StButton::change_flag(int num, bool f) {
	if (num < 0 || num >= m_size) { return; }
	int c;
	if (f) { c = LIGHT_YELLOW; }
	else { c = DARK_YELLOW; }
	st[num]->change_flag(f, c);
}

//�摜�n���h����ύX�i���Ԃ���Ȃ����k��-1�ɂ���p�j
void StButton::change_pic(int num, int handle, int ex) {
	if (num < 0 || num >= m_size) { return; }
	st[num]->set_graph(handle, ex);
}

//�}�E�X�J�[�\�����d�Ȃ��Ă���Ȃ� �O�`
int StButton::overlap(int hand_x, int hand_y) {
	for (int i = 0; i < m_size; i++) {
		if (st[i]->overlap(hand_x, hand_y)) {
			return i;
		}
	}
	return -1;
}

void StButton::draw(int hand_x, int hand_y) {
	for (int i = 0; i < m_size; i++) {
		st[i]->draw(hand_x, hand_y);
	}
}


//////�T���O�̏�����ʗp�N���X///////////
//�T���ɍs�����k�̑I���E�E�F�[�u���w��
StDecide::StDecide(int stand_handle[], int dot_handle[], int run_handle[][2]) {
	dx[0] = START_DX;
	dx[1] = 0; dx[2] = 0;
	font = CreateFontToHandle(NULL, 50, 10);
	font_small_handle = CreateFontToHandle(NULL, 30, 8);
	m_start_wave = max_wave;
	//���k�̉摜
	m_run_cnt = 0;
	m_st_num[0] = 0;//��l�ڂ̓A�J�c�L�Ŋm��
	m_st_num[1] = -1;
	m_st_num[2] = -1;
	for (int i = 0; i < 10; i++) {
		m_stand_handle[i] = stand_handle[i];
		m_dot_handle[i] = dot_handle[i];
		m_run_handle[i][0] = run_handle[i][0];
		m_run_handle[i][1] = run_handle[i][1];
	}
	//�{�^���̏���
	int distance = 50;//�l�p�̊Ԃ̕�
	int edge = 50;//���[�̋� ����150
	int wide = 205;//�l�p�̉���
	int y = 500;//y���W
	//�T���֍s��3�l�̃{�^��
	for (int i = 0; i < 3; i++) {
		m_st_button[i] = new Button("", edge + (wide * i) + distance, y, wide - distance, 250, LIGHT_BLUE2, BLUE, font_small_handle, BLACK);
	}
	button_decide = new Button("����", 1100, 20, 150, 80, LIGHT_BLUE, BLUE, font, BLACK);
	button_cancel = new Button("�߂�", 30, 20, 150, 80, LIGHT_GREEN, GREEN, font, BLACK);
	//���k���������̕���
	for (int i = 0; i < 10; i++) {
		m_wi[i] = new WeaponInfo(have_weapon[i]);
	}
	//���̏���
	m_right_handle = LoadGraph("picture/�E���.png");
	m_left_handle = LoadGraph("picture/�����.png");
	m_button_right = new Button("", 800, 20, 50, 50, LIGHT_YELLOW, YELLOW, font, BLACK);
	m_button_left = new Button("", 400, 20, 50, 50, LIGHT_YELLOW, YELLOW, font, BLACK);
	m_button_left->change_flag(false, DARK_YELLOW);
	m_button_right->set_graph(m_right_handle, 2);
	m_button_left->set_graph(m_left_handle, 2);
	//���k�X�l�̃{�^��
	int a[9];
	for (int i = 0; i < 9; i++) { a[i] = dot_handle[i + 1]; }
	m_button_st = new StButton(9, a, 200);
	//���ʉ�
	decide_sound = LoadSoundMem("sound/�V�X�e����/����.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, decide_sound);
}

StDecide::~StDecide() {
	for (int i = 0; i < 3; i++) {
		delete m_st_button[i];
	}
	for (int i = 0; i < 10; i++) {
		delete m_wi[i];
	}
	delete m_button_right;
	delete m_button_left;
	delete button_decide;
	delete button_cancel;
	delete m_button_st;
	DeleteGraph(m_right_handle);
	DeleteGraph(m_left_handle);
	DeleteFontToHandle(font);
	DeleteFontToHandle(font_small_handle);
	DeleteSoundMem(decide_sound);
}

//���k���Z�b�g����
int StDecide::set_st(int new_num) {
	int res;
	int num = 1;//������l�߂Ă���(�Q�l�ڂ���)
	if (m_st_num[1] != -1) { num = 2; }//�Q�l�ڂ����܂��Ă���
	res = m_st_num[num];
	m_st_num[num] = new_num;
	dx[num] = START_DX * num;
	return res;
}

//���k���O��
int StDecide::cancel_st() {
	if (left_click() == 1) {//���N���b�N���ꂽ�Ƃ�
		for (int i = 1; i < 3; i++) {
			//�J�[�\�����d�Ȃ��Ă���{�^��
			if (m_st_button[i]->overlap(hand_x, hand_y)) {
				PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
				int res = m_st_num[i];
				if (res == -1) { break; }//���ɋ�
				m_st_num[i] = -1;//�O��
				if (i == 1) {//���ɋl�߂�
					m_st_num[1] = m_st_num[2];
					dx[1] = -1 * START_DX;
					m_st_num[2] = -1;//�O��1;
				}
				return res;//�O�����L�����ԍ���Ԃ�
			}
		}
	}
	return -1;
}

//�T���N���X���쐬����
Dash* StDecide::get_dash() {;
	int d_weapon[3];
	int d_item[3];
	int d_level[3];
	for (int i = 0; i < 3; i++) {
		d_weapon[i] = have_weapon[m_st_num[i]];
		//���������񕜖�̌�
		d_item[i] = GetRand(3) + GetRand(level[m_st_num[i]]) / 30;
		d_level[i] = level[m_st_num[i]];
	}
	d_item[0] = item_num;
	return new Dash(m_start_wave, m_st_num, d_weapon, d_item, d_level);
}

int StDecide::play() {
	GetMousePoint(&hand_x, &hand_y);
	for (int i = 0; i < 3; i++) {
		if (dx[i] != 0) {
			dx[i] = (int)(dx[i] * 0.75);
		}
	}
	if (left_click() == 1) {
		//�߂�{�^��
		if (button_cancel->overlap(hand_x, hand_y)) {
			return -1;
		}
		//����{�^��
		if (button_decide->overlap(hand_x, hand_y)) {
			return 1;
		}
	}
	m_run_cnt++;//�h�b�g�̑���A�j���p
	//�E�F�[�u�w��̖��̏������
	if (m_start_wave == max_wave) { m_button_right->change_flag(false, DARK_YELLOW); }
	else { m_button_right->change_flag(true, LIGHT_YELLOW); }
	if (m_start_wave == 0) { m_button_left->change_flag(false, DARK_YELLOW); }
	else { m_button_left->change_flag(true, LIGHT_YELLOW); }
	//�E�F�[�u���ύX
	if (left_click() == 1 || (left_click() > 10 && left_click() % 2 == 1)) {
		if (m_button_right->overlap(hand_x, hand_y)) {
			m_start_wave++;
		}
		if (m_button_left->overlap(hand_x, hand_y)) {
			m_start_wave--;
		}
	}
	//���k������E����ւ���
	int n;
	if (left_click() == 1) {
		//�L����������
		n = m_button_st->overlap(hand_x, hand_y);//n�͑I�𒆂̃L�����ԍ�-1
		if (n != -1) {//���k��I������
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
			int x = set_st(n + 1);//���k���Z�b�g x�͊O�ꂽ�L�����ԍ�
			//����ւ��ŊO�ꂽ���k�̃{�^��
			if (x > 0) { m_button_st->change_flag(x - 1, true); }
			//�I�����ꂽ���k�̃{�^��
			m_button_st->change_flag(n, false);
		}
	}
	//���k���O��
	n = cancel_st();
	if (n > 0) {
		m_button_st->change_flag(n - 1, true);
	}
	return 0;
}

void StDecide::draw() {
	//�w�i
	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, GRAY2, TRUE);
	//�I���ς݃L�����̗����G
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	for (int i = 0; i < 3; i++) {
		if (m_st_num[i] == -1) {//�L�������I��
			break;
		}
		DrawRotaGraph2(-150 + (i * 400) - dx[i], -50, 0, 0, STAND_EXTEND, 0, m_stand_handle[m_st_num[i]], TRUE, FALSE);	
	}
	SetDrawMode(DX_DRAWMODE_NEAREST);
	//�I���ς݃L�����̃h�b�g�G�ƕ���ڍ�
	for (int i = 0; i < 3; i++) {
		if (m_st_num[i] != -1) {//�L�����I���ς�
			m_st_button[i]->set_graph(m_run_handle[m_st_num[i]][((m_run_cnt + 3) / 4) % 2], DOT_EXTEND);
			m_st_button[i]->draw(hand_x, hand_y);
			m_st_button[i]->draw(hand_x, hand_y);
			//���k�̖��O
			DrawStringToHandle(100 + 205 * i, 505, st_name[m_st_num[i]], BLACK, font_small_handle);
		}
		else {//�L�������I��
			m_st_button[i]->set_graph(-1, DOT_EXTEND);
			m_st_button[i]->set_string("���I��");
			m_st_button[i]->draw(hand_x, hand_y);
		}
		//���k���������̕���̏ڍ�
		if (m_st_num[i] != -1 && m_st_button[i]->overlap(hand_x, hand_y)) {
			m_wi[m_st_num[i]]->draw(700, 450, LIGHT_BLUE, font_small_handle, DARK_RED);
		}
	}
	int n = m_button_st->overlap(hand_x, hand_y);
	if (n >= 0) {
		m_wi[n + 1]->draw(700, 450, LIGHT_BLUE, font_small_handle, DARK_RED);
	}
	//�E�F�[�u���w��
	DrawBox(420, 20, 820, 70, LIGHT_BLUE, TRUE);
	ostringstream w;
	w << "Wave" << m_start_wave+1 << "����X�^�[�g";
	DrawStringToHandle(480, 30, w.str().c_str(), BLACK, font_small_handle);
	m_button_right->draw(hand_x, hand_y);
	m_button_left->draw(hand_x, hand_y);
	//���k�X�l
	m_button_st->draw(hand_x, hand_y);
	//���k�̃��x��
	for (int i = 0; i < 9; i++) {
		if (m_button_st->get_handle(i) == -1) {
			continue;
		}
		DrawBox(70 + (i * 131), 983, 171 + (i * 131), 1015, LIGHT_RED, TRUE);
		ostringstream w;
		w << "Lv " << level[i+1];
		DrawStringToHandle(80 + (i*131), 985, w.str().c_str(), BLACK, font_small_handle);
	}
	//����{�^���E�߂�{�^��
	button_decide->draw(hand_x, hand_y);
	button_cancel->draw(hand_x, hand_y);
}