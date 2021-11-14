#include"Menu.h"
#include"Control.h"
#include"Button.h"
#include"Dash.h"
#include"Text.h"
#include"Weapon.h"
#include"Shop.h"
#include"Movie.h"
#include"SDvalue.h"
#include"DxLib.h"
#include<string>
#include<sstream>
#include<algorithm>

using namespace std;


//���k�̗����G�A�h�b�g�G�A����h�b�g�G�����[�h����
void load_st_graph(int stand_handle[], int dot_handle[], int run_handle[10][2]) {
	string name;
	for (int i = 0; i < 10; i++) {
		DeleteGraph(dot_handle[i]);
		DeleteGraph(run_handle[i][0]);
		DeleteGraph(run_handle[i][1]);
		DeleteGraph(stand_handle[i]);
	}
	for (int i = 0; i < 10; i++) {
		name = "picture/dot/";
		name += st_name[i];
		if (month >= 6 && month <= 10) { name += "��"; }
		run_handle[i][0] = LoadGraph((name + "����1.png").c_str());
		run_handle[i][1] = LoadGraph((name + "����2.png").c_str());
		dot_handle[i] = LoadGraph((name + ".png").c_str());
		name = "picture/stand/";
		name += st_name[i]; name += "/"; name += st_name[i];
		if (month >= 6 && month <= 10) { name += "��"; }//�ߑւ�
		stand_handle[i] = LoadGraph((name + "�ʏ�.png").c_str());
	}
}


////////�Q�[���{��//////////////
SchoolDash::SchoolDash() {
	game_load();//���[�h
	//�����Z�[�u�f�[�^�쐬�p
	game_save();//�Z�[�u
	state = GAME_MENU;
	cnt = 0;
	load_all();
	load_st_graph(stand_handle, st_handle, run);
	window_flag = FALSE;
}

void SchoolDash::delete_all() {
	InitGraph();
	InitSoundMem();
	InitFontToHandle();
	delete button_tansaku;
	delete button_event;
	delete button_cus;
	delete button_create;
	delete button_shop;
	delete button_data;
	delete button_option;
}

void SchoolDash::load_all() {
	loading_haikei = LoadGraph("picture/loading1.png");
	decide_sound = LoadSoundMem("sound/�V�X�e����/����.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, decide_sound);
	cancel_sound = LoadSoundMem("sound/�V�X�e����/�L�����Z��.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, cancel_sound);
	haikei = LoadGraph("picture/�w�i1.png");
	stand_handle[0] = -1;
	//�傫���t�H���g
	big_font = CreateFontToHandle(NULL, 80, 10);
	//�������t�H���g
	small_font = CreateFontToHandle(NULL, 30, 8);
	//�{�^���̗p��
	button_font = CreateFontToHandle(NULL, 50, 10);
	button_tansaku = new Button("�T��", 95, 300, 300, 300, LIGHT_BLUE, BLUE, button_font, BLACK);
	button_event = new Button("�C�x���g", 490, 300, 300, 300, LIGHT_BLUE, BLUE, button_font, BLACK);
	button_data = new Button("�f�[�^", 885, 300, 300, 300, LIGHT_GREEN2, GREEN, button_font, BLACK);
	button_cus = new Button("����ύX", 95, 700, 300, 300, LIGHT_YELLOW, YELLOW, button_font, BLACK);
	button_create = new Button("����쐬", 490, 700, 300, 300, LIGHT_YELLOW, YELLOW, button_font, BLACK);
	button_shop = new Button("�V���b�v", 885, 700, 300, 300, LIGHT_YELLOW, YELLOW, button_font, BLACK);
	button_option = new Button("�I�v�V����", 970, 10, 270, 80, LIGHT_YELLOW2, RED, button_font, BLACK);
}

//���ʉ��̉��ʂ��Đݒ�
void SchoolDash::change_menu_sound() {
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, decide_sound);
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, cancel_sound);
}

SchoolDash::~SchoolDash() {
	InitGraph();
	InitSoundMem();
	DeleteFontToHandle(button_font);
	DeleteFontToHandle(big_font);
	DeleteFontToHandle(small_font);
	delete button_tansaku;
	delete button_event;
	delete button_cus;
	delete button_create;
	delete button_shop;
	delete button_data;
	delete button_option;
}

//�Q�[���{�̂̃��C���֐�
void SchoolDash::play() {
	//�e�����֕���
	switch (state) {
	case GAME_MENU:
		play_menu();
		draw_menu();
		break;
	case GAME_STORY:
		play_story();
		draw_story();
		break;
	case GAME_START:
		play_start();
		draw_start();
		break;
	case GAME_PRE_DASH:
		play_pre_dash();
		draw_pre_dash();
		break;
	case GAME_DASH:
		play_dash();
		draw_dash();
		break;
	case GAME_END_DASH:
		play_end_dash();
		draw_end_dash();
		break;
	case GAME_EVENT:
		play_event();
		draw_event();
		break;
	case GAME_DATA:
		play_data();
		draw_data();
		break;
	case GAME_CUS:
		play_cus();
		draw_cus();
		break;
	case GAME_CREATE:
		play_cre();
		draw_cre();
		break;
	case GAME_SHOP:
		play_shop();
		draw_shop();
		break;
	case GAME_OPTION:
		play_option();
		draw_option();
		break;
	case GAME_MOVIE:
		if (ed->play()) {
			delete ed;
			state = GAME_MENU;
		}
		break;
	}
}

//�C�x���g��������Ȃ�쐬���Atrue��Ԃ�
bool SchoolDash::create_event() {
	if (!sub_event_flag) {
		sub_event_flag = true;//�T�u�C�x���g�I��
		if (check_event_file("sub", finish_story + 1)) {//�T�u�C�x���g�����݂���Ȃ�
			state = GAME_STORY;//�T�u�C�x���g�ֈڍs
			change_music("none", false, true);//BGM�͂����������
			te = new TextEvent(TEXT_SUB, finish_story + 1, true, "");
			return true;//�T�u�C�x���g�����s���邩�炢�����񂱂��܂�
		}
	}
	if (!class_event) {
		//�N���X���C�g�Ƃ̃C�x���g
		//���I�͂P���P��
		class_event = true;//�N���X���C�g�̃C�x���g�I��
		//4��1���Ƒ��Ǝ��͂Ȃ�
		if (!(month == 4 && week == 1) && !(year == 3 && month == 3 && week == 2)) {
			int r = GetRand(8) + 1;//�n���_����g���`�̂Ȃ������l�𒊑I
			//����̐��k�A�����Ȃ璆�~
			if (year == 1 && r == 8) {}//�C�o���J���]�Z�O
			else if (year == 1 && month < 7 && r == 2) {}//�I�I�^�a�ݎ���
			else if (((year == 1 && month >= 1 && month <= 3) || (year == 2 && month >= 4 && month <= 6)) && r == 6) {}//�~���g�l�a�ݎ���
			else if (year == 3 && month >= 1 && month <= 2 && r == 1) {}//�n���_�ƌ��ܒ�
			//�N���X�C�x���g�̍쐬
			else if (classmate[0] == classmate[r]) {//�����N���X�Ȃ�
				state = GAME_STORY;//�N���X�C�x���g�ֈڍs
				change_music("none", false, true);//BGM�͂����������
				te = new TextEvent(TEXT_SUB, decide_sub(r), true, st_name[r]);
				return true;//�C�x���g���쐬�����Ȃ�A�������񂱂��܂�
			}
		}
		game_save();//�Z�[�u
	}
	//�܂��X�g�[���[���ςĂ��Ȃ��Ȃ�
	if (week_num(year, month, week) > finish_story) {
		finish_story++;//�X�g�[���[���P�������ƂɂȂ�
		//�X�g�[���[
		//�N���X���[�g�̏����m�F
		if (check_event_file("story", finish_story)) {
			state = GAME_STORY;
			change_music("none", false, true);//BGM�͂����������
			te = new TextEvent(TEXT_STORY, finish_story, true);
			story_list[finish_story - 1] = true;//�X�g�[�����ς����Ƃ������Ԃɂ���
			return true;//�X�g�[���[���ς邩�炢�����񂱂��܂�
		}
	}
	return false;
}

//�ŏ��̃��j���[���
void SchoolDash::play_menu() {
	//////�C�x���g�����i�ŗD��j/////////////////
	if (create_event()) {
		return;//�C�x���g���������邩�烁�j���[��ʂ͕\�����Ȃ�
	}

	//���Ǝ��̂ݗ�O�i�T���Ȃ��A�d�c�ցj
	if (year == 3 && month == 3 && week == 2) {
		game_init();
		ed = new Ed();
		state = GAME_MOVIE;
	}

	//////////���j���[��ʂ̏���////////////////
	GetMousePoint(&hand_x, &hand_y);
	//���N���b�N���ꂽ�Ƃ�
	if (left_click() == 1) {
		//�T���{�^��
		if (button_tansaku->overlap(hand_x, hand_y)) {
			state = GAME_PRE_DASH;
			sd = new StDecide(stand_handle, st_handle, run);
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
		}
		//�C�x���g�{�^��
		if (button_event->overlap(hand_x, hand_y)) {
			state = GAME_EVENT;
			int a[9];//�A�J�c�L�ȊO�̃h�b�g�G
			for (int i = 0; i < 9; i++) { a[i] = st_handle[i + 1]; }
			se = new StEvent(a, stand_handle);
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
		}
		//�f�[�^�{�^��
		if (button_data->overlap(hand_x, hand_y)) {
			state = GAME_DATA;
			data = new Data(stand_handle, st_handle);
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
		}
		//����ύX�{�^��
		if (button_cus->overlap(hand_x, hand_y)) {
			state = GAME_CUS;
			wc = new WeaponChange(st_handle);
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
		}
		//����쐬�{�^��
		if (button_create->overlap(hand_x, hand_y)) {
			state = GAME_CREATE;
			wcre = new WeaponCreate();
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
		}
		//�V���b�v�{�^��
		if (button_shop->overlap(hand_x, hand_y)) {
			state = GAME_SHOP;
			shop = new Shop;
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
		}
		//�I�v�V�����{�^��
		if (button_option->overlap(hand_x, hand_y)) {
			state = GAME_OPTION;
			option = new Option();
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
		}
	}
	//���j���[��ʂ̉��y
	if (state == GAME_MENU && music_name != "music/menu.ogg") {
		change_music("music/menu.ogg", false, true);
	}
	if (control_space() == 1) {
		if (window_flag == TRUE) {
			window_flag = FALSE;
		}
		else {
			window_flag = TRUE;
		}
		ChangeWindowMode(window_flag);
		DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);
		SetMainWindowText("School Dash");
		////�}�E�X�֘A////
		SetMouseDispFlag(TRUE);//�}�E�X�\��
		SetDrawMode(DX_DRAWMODE_NEAREST);
		//�f�[�^�폜
		delete_all();
		//���[�h���Ȃ���
		load_all();
		change_music("none", false, true);
		//���[�h���Ȃ���
		load_st_graph(stand_handle, st_handle, run);
	}
}

void SchoolDash::draw_menu() {
	if (state == GAME_STORY || state == GAME_MOVIE) { return; }//�C�x���g��������
	GetMousePoint(&hand_x, &hand_y);
	//�w�i
	DrawRotaGraph2(0, 0, 0, 0, 1.0, 0, haikei, TRUE, FALSE);
	draw_date(0, 0);
	button_tansaku->draw(hand_x, hand_y);
	button_event->draw(hand_x, hand_y);
	button_cus->draw(hand_x, hand_y);
	button_create->draw(hand_x, hand_y);
	button_shop->draw(hand_x, hand_y);
	button_data->draw(hand_x, hand_y);
	button_option->draw(hand_x, hand_y);
	if (button_tansaku->overlap(hand_x, hand_y)) {
		DrawBox(0, 90, 760, 210, LIGHT_GREEN, TRUE);
		DrawStringToHandle(10, 100, "�w�Z��T�����܂��B", BLACK, small_font);
		DrawStringToHandle(10, 150, "�i���t���i�݂܂��j", BLACK, small_font);
	}
	else if (button_event->overlap(hand_x, hand_y)) {
		DrawBox(0, 90, 760, 210, LIGHT_GREEN, TRUE);
		DrawStringToHandle(10, 100, "�e���x���グ���L�����Ƃ̃C�x���g���������܂��B", BLACK, small_font);
		DrawStringToHandle(10, 150, "�i���t���i�݂܂��j", BLACK, small_font);
	}
	DrawBox(800, 110, 1280, 170, LIGHT_RED, TRUE);
	DrawStringToHandle(820, 120, "�X�y�[�X�L�[�ŉ�ʕ\���̐ؑ�", BLACK, small_font);
}

//���t��\��
void SchoolDash::draw_date(int draw_x, int draw_y) {
	ostringstream s;
	s << year << "�N�� " << month << "�� " << week << "�T�� " << "�������F" << money << "pt";
	DrawBox(draw_x, draw_y, draw_x + 900, draw_y + 70, GRAY2, TRUE);
	//DrawFormatString(draw_x + 10, draw_y + 10, BLACK, s.str().c_str());
	DrawStringToHandle(draw_x + 10, draw_y + 10, s.str().c_str(), BLACK, button_font);
}

//���t��i�߂� �f�[�^�����ׂă��Z�b�g����
//�f�[�^�̃Z�[�u���s���I�I
void SchoolDash::next_date() {
	//���}�Ƃ�i�߂�
	week++;
	if (week == 5) {//���̌���
		week = 1;//��T�ڂ���
		month++;//���̌�
		if (month == 13) {//12���̎��͂P��
			month = 1;
		}
	}
	//�����x�ɂ��΂�
	if (month == 8) { month = 9; }//�ċx��
	if (month == 12 && week == 4) { month = 1; week = 2; }//�~�x��
	if (month == 3 && week == 3) { //�t�x��
		month = 4; week = 1;//4��1���ɂȂ�
		year++;//�N���ς��
	}
	class_event = false;
	sub_event_flag = false;
	state = GAME_START;
	//�Z�[�u
	game_save();
	//�f�[�^�폜
	delete_all();
	load_all();
	change_music("none", false, true);
	//�ߑւ�����������������Ȃ����烍�[�h���Ȃ���
	load_st_graph(stand_handle, st_handle, run);
}

//�X�g�[���[
void SchoolDash::play_story() {
	//�V�w���̃N���X���C�g�Љ�p 1:�n���_�`9:�g���`
	static int story_loop = 1;
	//�X�g�[���[�Đ��I����
	if (te->play()) {
		state = GAME_MENU;
		delete te;
		///////�N���X�ւ��i�S���P�T�ڂ̂݁j//////////
		//&�V�w���̃N���X���C�g�Љ�
		if (class_event && month == 4 && week == 1) {
			//�܂��͑S���k�̃N���X�̌���
			if (story_loop == 1) {//���߂Ɉ�񂾂��N���X���I
				for (int i = 0; i < 10; i++) {//�e���k
					//�V�����N���X�̒��I
					classmate[i] = GetRand(3) + 1;
					//classmate[i] = classmate[0];//�e�X�g�@�S�������N���X
				}
				//�Œ�ł��P�l�͓����N���X�ɂȂ�
				while (true) {
					int r = GetRand(8) + 1;
					if (year != 1 || r != 8) {//1�N�ł̓C�o���J���i�]�Z���j����O
						classmate[r] = classmate[0];
						break;
					}
				}
			}
			//�N���X�����܂�����̓N���X���C�g�̏Љ�����ɍs��
			for (story_loop; story_loop < 10; story_loop++) {
				if (year == 1 && story_loop == 8) { continue; }//�C�o���J���]�Z�O
				if (classmate[0] == classmate[story_loop]) {//�����N���X�Ȃ�
					state = GAME_STORY;//�X�g�[���[�͑���
					te = new TextEvent(TEXT_OTHER, year, true, st_name[story_loop]);
					story_loop++;
					break;//��������~�߂āA�Љ�̌㑱������
				}
			}
		}
	}
	if (state == GAME_MENU) { 
		story_loop = 1;
		game_save();//�Z�[�u
	}//���S�I����
}
void SchoolDash::draw_story() {

}

//���t�ύX���
void SchoolDash::play_start() {
	cnt++;
	if (cnt == 150) {//�I��
		state = GAME_MENU;
		cnt = 0;
	}
}
void SchoolDash::draw_start() {
	int c = min((int)(cnt*2.0), 255);
	ostringstream s;
	s << year << "�N�� " << month << "�� " << week << "�T�� ";
	DrawStringToHandle(300, 420, s.str().c_str(), GetColor(c, c, c), big_font);
}

//�T�����O
void SchoolDash::play_pre_dash() {
	int sd_result = sd->play();//�߂�Ƃ���-1 �T���֍s���Ƃ���1
	if (sd_result == -1) {//�߂�{�^��
		state = GAME_MENU;
		PlaySoundMem(cancel_sound, DX_PLAYTYPE_BACK);//���ʉ�
	}
	else if (sd_result == 1) {//����{�^��
		//state = GAME_MENU;//��V�m�F�I�����������[�N�e�X�g
		//next_date();//���t��i�߂郁�������[�N�e�X�g�e�X�g
		state = GAME_DASH;//�T�����ֈڍs
		dash = sd->get_dash();//�T���N���X�̍쐬
		PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
	}	
}
void SchoolDash::draw_pre_dash() {
	sd->draw();
	//�I�����Ă���Ȃ�
	if (state != GAME_PRE_DASH) {
		delete sd;
	}
}

//�T����
void SchoolDash::play_dash() {
	if (dash->play()) {
		state = GAME_END_DASH;//�T���I��
	}
}
void SchoolDash::draw_dash() {
	dash->draw();
}

//�T���I��
void SchoolDash::play_end_dash() {
	//���N���b�N���ꂽ��
	if (dash->play_reward()) {
		state = GAME_MENU;//��V�m�F�I��
		PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
		next_date();//���t��i�߂�
	}
}
void SchoolDash::draw_end_dash() {
	if (state == GAME_END_DASH) {
		dash->draw_reward();
	}
	//�T���̊��S�I��
	if (state != GAME_END_DASH) {
		delete dash;
	}
}

//�C�x���g
void SchoolDash::play_event() {
	if (se->play()) {
		state = GAME_MENU;
		if (se->get_flag()) {//�e�L�X�g�C�x���g�����������ł̏I���Ȃ�
			next_date();//���t��i�߂�
		}
		else {
			PlaySoundMem(cancel_sound, DX_PLAYTYPE_BACK);//�߂���ʉ�
		}
	}
}
void SchoolDash::draw_event() {
	se->draw();
	if (state != GAME_EVENT) {
		delete se;
	}
}


//����ύX���
void SchoolDash::play_cus() {
	if (wc->play()) {
		state = GAME_MENU;
		PlaySoundMem(cancel_sound, DX_PLAYTYPE_BACK);//���ʉ�
	}
}
void SchoolDash::draw_cus() {
	wc->draw();
	if (state != GAME_CUS) {
		delete wc;
	}
}

//����쐬���
void SchoolDash::play_cre() {
	if (wcre->play()) {
		state = GAME_MENU;
		PlaySoundMem(cancel_sound, DX_PLAYTYPE_BACK);//���ʉ�
	}
}
void SchoolDash::draw_cre() {
	wcre->draw();
	if (state != GAME_CREATE) {
		delete wcre;
	}
}

//�V���b�v���
void SchoolDash::play_shop() {
	if (shop->play()) {
		state = GAME_MENU;
		PlaySoundMem(cancel_sound, DX_PLAYTYPE_BACK);//���ʉ�
	}
}
void SchoolDash::draw_shop() {
	shop->draw();
	if (state != GAME_SHOP) {
		delete shop;
	}
}

//�I�v�V�������
void SchoolDash::play_option() {
	if (option->play()) {
		state = GAME_MENU;
		change_menu_sound();
		PlaySoundMem(cancel_sound, DX_PLAYTYPE_BACK);//���ʉ�
	}
}
void SchoolDash::draw_option() {
	option->draw();
	if (state != GAME_OPTION) {
		delete option;
	}
}

//�f�[�^���
void SchoolDash::play_data() {
	if (data->play()) {
		state = GAME_MENU;
		PlaySoundMem(cancel_sound, DX_PLAYTYPE_BACK);//���ʉ�
	}
}
void SchoolDash::draw_data() {
	data->draw();
	if (state != GAME_DATA) {
		delete data;
	}
}

///////////�I�v�V�������////////////
Option::Option() {
	//�t�H���g
	font_handle = CreateFontToHandle(NULL, 50, 10);
	//�߂�{�^��
	m_button_cancel = new Button("�߂�", 30, 20, 150, 80, LIGHT_GREEN, GREEN, font_handle, BLACK);
	//���̏���
	m_right_handle = LoadGraph("picture/�E���.png");
	m_left_handle = LoadGraph("picture/�����.png");
	m_button_right = new Button("", 950, 300, 50, 50, LIGHT_YELLOW, YELLOW, font_handle, BLACK);
	m_button_left = new Button("", 350, 300, 50, 50, LIGHT_YELLOW, YELLOW, font_handle, BLACK);
	m_button_right->set_graph(m_right_handle, 2);
	m_button_left->set_graph(m_left_handle, 2);
}
Option::~Option() {
	DeleteFontToHandle(font_handle);
	DeleteGraph(m_right_handle);
	DeleteGraph(m_left_handle);
	delete m_button_cancel;
	delete m_button_right;
	delete m_button_left;
}
bool Option::play() {
	GetMousePoint(&hand_x, &hand_y);
	//���N���b�N���ꂽ�Ƃ�
	if (left_click() == 1 || (left_click() > 10 && left_click() % 2 == 1)) {
		//���{�^��
		if (m_button_right->overlap(hand_x, hand_y)) {
			SoundVolume++;
			change_volume();
		}
		if (m_button_left->overlap(hand_x, hand_y)) {
			SoundVolume--;
			change_volume();
		}
	}
	if (SoundVolume == 100) {
		m_button_right->change_flag(false, DARK_YELLOW);
	}
	else {
		m_button_right->change_flag(true, LIGHT_YELLOW);
	}
	if (SoundVolume == 0) {
		m_button_left->change_flag(false, DARK_YELLOW);
	}
	else {
		m_button_left->change_flag(true, LIGHT_YELLOW);
	}
	if (left_click() == 1) {
		//�߂�{�^��
		if (m_button_cancel->overlap(hand_x, hand_y)) {
			return true;//�I�v�V������ʏI��
		}
	}
	return false;
}
void Option::draw() {
	//�w�i
	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, GRAY2, TRUE);
	//�߂�{�^��
	m_button_cancel->draw(hand_x, hand_y);
	//���{�^��
	DrawBox(300, 270, 1050, 370, LIGHT_BLUE, TRUE);
	m_button_right->draw(hand_x, hand_y);
	m_button_left->draw(hand_x, hand_y);
	//���݂̉��ʕ\��
	ostringstream s;
	s << "���݂̉��ʁF" << SoundVolume << "��";
	DrawStringToHandle(470, 300, s.str().c_str(), BLACK, font_handle);
}


//////////�f�[�^�{�����//////////////////
Data::Data(int stand_handle[], int dot_handle[]) {
	data_state = DATA_MENU;
	//�t�H���g
	font_handle = CreateFontToHandle(NULL, 50, 10);
	font_small_handle = CreateFontToHandle(NULL, 30, 8);
	//���ʉ�
	decide_sound = LoadSoundMem("sound/�V�X�e����/����.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, decide_sound);
	cancel_sound = LoadSoundMem("sound/�V�X�e����/�L�����Z��.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, cancel_sound);
	//�߂�{�^��
	button_cancel = new Button("�߂�", 30, 20, 150, 80, LIGHT_GREEN, GREEN, font_handle, BLACK);
	//���k�{���{�^��
	button_st = new Button("�A�J�c�L�ƒ���", 200, 200, 800, 100, LIGHT_YELLOW, YELLOW, font_handle, BLACK);
	//�N�����B�{���{�^��
	button_other = new Button("�o��������̑��̐��k", 200, 350, 800, 100, LIGHT_YELLOW, YELLOW, font_handle, BLACK);
	if (week_num(year, month, week) < week_num(1, 5, 1)) {
		//�N�������疢�o��
		button_other->change_flag(false, DARK_YELLOW);
	}
	//�C�x���g�{���{�^��
	button_event = new Button("�ߋ��̃C�x���g�{��", 200, 500, 800, 100, LIGHT_YELLOW, YELLOW, font_handle, BLACK);
	//�e�f�[�^�̃I�u�W�F�N�g
	m_st_data = new StData(stand_handle, dot_handle);
	m_other_data = new OtherData();
	m_event_data = new EventData();

}

Data::~Data() {
	DeleteFontToHandle(font_handle);
	DeleteFontToHandle(font_small_handle);
	DeleteSoundMem(decide_sound);
	DeleteSoundMem(cancel_sound);
	delete button_cancel;
	delete button_st;
	delete button_other;
	delete button_event;
	delete m_st_data;
	delete m_other_data;
	delete m_event_data;
}

bool Data::play() {
	GetMousePoint(&hand_x, &hand_y);
	//���N���b�N���ꂽ�Ƃ�
	if (left_click() == 1) {
		//�߂�{�^��
		if (button_cancel->overlap(hand_x, hand_y)) {
			if (data_state == DATA_MENU) {
				return true;//�f�[�^�{����ʏI��
			}
			else {//�f�[�^�{���̃��j���[��ʂ֖߂�
				data_state = DATA_MENU;
				m_st_data->reset();
				m_other_data->reset();
				PlaySoundMem(cancel_sound, DX_PLAYTYPE_BACK);//���ʉ�
			}
		}
	}
	//�e�f�[�^�{�����̏���
	switch (data_state) {
	case DATA_MENU://�f�[�^�{���̃��j���[��ʒ�
		if (left_click() == 1) {
			//���k�{����ʂ�
			if (button_st->overlap(hand_x, hand_y)) {
				data_state = DATA_ST;
				PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
			}
			//�N�����B�{����ʂ�
			if (button_other->overlap(hand_x, hand_y)) {
				data_state = DATA_OTHER;
				PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
			}
			//�C�x���g�{����ʂ�
			if (button_event->overlap(hand_x, hand_y)) {
				GetMouseWheelRotVol();//�z�C�[���̉�]�ʃ��Z�b�g
				data_state = DATA_EVENT;
				PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
			}
		}
		break;
	case DATA_ST:
		m_st_data->play();
		break;
	case DATA_OTHER:
		m_other_data->play();
		break;
	case DATA_EVENT:
		if (m_event_data->play()) {
			return false;
		}
		break;
	}
	return false;
}

void Data::draw() {
	//�w�i
	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, LIGHT_BLUE, TRUE);
	switch (data_state) {
	case DATA_MENU:
		//�e�{�^��
		button_st->draw(hand_x, hand_y);
		button_other->draw(hand_x, hand_y);
		button_event->draw(hand_x, hand_y);
		break;
	case DATA_ST:
		m_st_data->draw();
		break;
	case DATA_OTHER:
		m_other_data->draw();
		break;
	case DATA_EVENT:
		if (m_event_data->draw()) {
			return;
		}
		break;
	}
	//�߂�{�^��
	button_cancel->draw(hand_x, hand_y);
}

//�e�L�X�g�t�@�C������L�����̐����������[�h����֐�//
void load_prof(string file_name, int num, vector<string> prof[]) {
	//�t�@�C�����J��
	int file = FileRead_open(file_name.c_str());
	for (int i = 0; i < num; i++) {
		char sentence[256];
		do {
			FileRead_gets(sentence, 256, file);//1�s�ǂݍ���
			prof[i].push_back(sentence);
		} while (FileRead_eof(file) == 0 && sentence[0] != 0);
	}
	FileRead_close(file);//�t�@�C�������
}


//////���k�{�����////////////
StData::StData(int stand_handle[], int dot_handle[]) {
	//�ǂ̐��k��I��ł��邩�@�����l��-1
	st_select = -1;
	//���k�̗����G
	for (int i = 0; i < 10; i++) {
		handle[i] = stand_handle[i];
	}
	//���k�{�^��
	sb = new StButton(10, dot_handle, 200);
	//�t�H���g
	font_handle = CreateFontToHandle(NULL, 50, 10);
	font_small_handle = CreateFontToHandle(NULL, 30, 8);
	//���ʉ�
	decide_sound = LoadSoundMem("sound/�V�X�e����/����.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, decide_sound);
	//�L��������
	load_prof("text/���k�Љ�.txt", 10, prof);
	//�{�^��
	button_change = new Button("�\���ؑ�", 1000, 20, 250, 80, LIGHT_YELLOW, YELLOW, font_handle, BLACK);
	draw_prof = false;
}

StData::~StData() {
	//�t�H���g
	DeleteFontToHandle(font_handle);
	DeleteFontToHandle(font_small_handle);
	//���ʉ�
	DeleteSoundMem(decide_sound);
	//���k�{�^��
	delete sb;
	delete button_change;
}

void StData::play() {
	GetMousePoint(&hand_x, &hand_y);
	//�\�������߂��Ă���
	if (dx != 0) {
		dx = (int)(dx * 0.75);
	}
	//���N���b�N���ꂽ�Ƃ�
	if (left_click() == 1) {
		//�\���ؑ�
		if (st_select != -1 && button_change->overlap(hand_x, hand_y)) {
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
			draw_prof = !draw_prof;
		}
		//���k�̃{�^��
		int n = sb->overlap(hand_x, hand_y);
		if (n != -1) {
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
			dx = START_DX;
			if (st_select != -1) {//���Ƃ��ƑI��ł������ktrue
				sb->change_flag(st_select, true);
			}
			st_select = n;//���k�I��
			sb->change_flag(n, false);//�I���������kfalse
		}
	}
}

void StData::draw() {
	if (st_select != -1) {//���k���{����
		//�����G�\��
		SetDrawMode(DX_DRAWMODE_BILINEAR);
		DrawRotaGraph2(0 - dx, -50, 0, 0, STAND_EXTEND, 0, handle[st_select], TRUE, FALSE);
		SetDrawMode(DX_DRAWMODE_NEAREST);
		//���k���
		DrawBox(800 + dx, 150, GAME_WIDE, GAME_HEIGHT, SAFE_COLOR, TRUE);
		//���O
		DrawBox(810 + dx, 160, GAME_WIDE, 260, WHITE, TRUE);
		DrawStringToHandle(820 + dx, 170, st_full_name[st_select], GRAY, font_small_handle);
		DrawStringToHandle(820 + dx, 200, st_kanji_name[st_select], BLACK, font_handle);
		if (draw_prof) {
			//�L�����Љ
			for (unsigned int i = 0; i < prof[st_select].size(); i++) {
				DrawStringToHandle(850 + dx, 350 + i * 50, prof[st_select][i].c_str(), BLACK, font_small_handle);
			}
		}
		else {
			//���x��
			ostringstream s;
			s << "Level�F" << level[st_select] << " / " << LEVEL_MAX;
			DrawStringToHandle(850 + dx, 300, s.str().c_str(), RED, font_handle);
			ostringstream e;
			e << "�o���l�F" << exp_sum[st_select] << " / " << need_exp(level[st_select]);
			DrawStringToHandle(870 + dx, 380, e.str().c_str(), LIGHT_RED, font_small_handle);
			//�N���X
			ostringstream c;
			c << "�N���X�F" << year << "�N" << classmate[st_select] << "�g";
			if (classmate[0] == classmate[st_select]) {
				DrawStringToHandle(850 + dx, 500, c.str().c_str(), YELLOW, font_handle);
			}
			else {
				DrawStringToHandle(850 + dx, 500, c.str().c_str(), DARK_GREEN, font_handle);
			}
			//�e���x
			if (st_select != 0) {//�A�J�c�L�͏��O
				DrawStringToHandle(850 + dx, 600, "�e���x", PINK, font_handle);
				DrawBox(845 + dx, 675, 1205 + dx, 735, GRAY, TRUE);
				DrawBox(850 + dx, 680, 850 + (350 * love[st_select] / LOVE_MAX) + dx, 730, PINK, TRUE);
				DrawBox(850 + dx + (350 * love[st_select] / LOVE_MAX), 680, 350 + 850 + dx, 730, BLACK, TRUE);
			}
		}	
	}
	//���̐��k�{�^��
	sb->draw(hand_x, hand_y);
	//�\���ؑփ{�^��
	if (st_select != -1) {
		button_change->draw(hand_x, hand_y);
	}
}

void StData::reset() {
	if (st_select != -1) {//���Ƃ��ƑI��ł������ktrue
		sb->change_flag(st_select, true);
	}
	st_select = -1;
	draw_prof = false;
}

//���̑��̐��k�{�����
OtherData::OtherData() {
	string name;
	for (int i = 0; i < 6; i++) {
		name = "picture/stand/";
		name += other_name[i]; name += "/"; name += other_name[i];
		if (month >= 6 && month <= 10) { name += "��"; }//�ߑւ�
		handle[i] = LoadGraph((name + "�ʏ�.png").c_str());
		//handle[i] = LoadGraph("picture/stand/�N����/�N�����ʏ�.png");
	}
	st_select = 0;
	dx = START_DX;
	//�t�H���g
	font_handle = CreateFontToHandle(NULL, 50, 10);
	font_small_handle = CreateFontToHandle(NULL, 30, 8);
	//���ʉ�
	decide_sound = LoadSoundMem("sound/�V�X�e����/����.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, decide_sound);
	//�{�^��
	button_next = new Button("����", 1100, 20, 150, 80, LIGHT_YELLOW, YELLOW, font_handle, BLACK);
	button_prev = new Button("�O��", 900, 20, 150, 80, LIGHT_YELLOW, YELLOW, font_handle, BLACK);
	button_prev->change_flag(false, DARK_YELLOW);
	if (st_select == 0 && week_num(year, month, week) < week_num(1, 7, 1)) {
		//�}�N�^���o��
		button_next->change_flag(false, DARK_YELLOW);
	}
	
	//�L��������
	load_prof("text/���̑��Љ�.txt", 6, prof);
}

OtherData::~OtherData() {
	DeleteFontToHandle(font_handle);
	DeleteFontToHandle(font_small_handle);
	DeleteSoundMem(decide_sound);
	for (int i = 0; i < 6; i++) {
		DeleteGraph(handle[i]);
	}
	delete button_next;
	delete button_prev;
}

void OtherData::play() {
	GetMousePoint(&hand_x, &hand_y);
	//���N���b�N���ꂽ�Ƃ�
	if (left_click() == 1) {
		//���փ{�^��
		if (button_next->overlap(hand_x, hand_y)) {
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
			st_select++;
			dx = START_DX;
			button_prev->change_flag(true, LIGHT_YELLOW);
		}
		//�O�փ{�^��
		if (button_prev->overlap(hand_x, hand_y)) {
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
			st_select--;
			dx = START_DX;
			button_next->change_flag(true, LIGHT_YELLOW);
		}
		//���փ{�^���A�O�փ{�^���̖������̃`�F�b�N
		if (st_select == 0) {
			//���[�܂ŗ���
			button_prev->change_flag(false, DARK_YELLOW);
		}
		if (st_select == 0 && week_num(year, month, week) < week_num(1, 7, 1)) {
			//�}�N�^���o��
			button_next->change_flag(false, DARK_YELLOW);
		}
		else if (st_select == 1 && week_num(year, month, week) < week_num(1, 9, 3)) {
			//�V���V�L���o��
			button_next->change_flag(false, DARK_YELLOW);
		}
		else if (st_select == 2 && week_num(year, month, week) < week_num(1, 11, 4)) {
			//�T�C�J���o��
			button_next->change_flag(false, DARK_YELLOW);
		}
		else if (st_select == 3 && week_num(year, month, week) < week_num(2, 4, 3)) {
			//�T�L�K�P���o��
			button_next->change_flag(false, DARK_YELLOW);
		}
		else if (st_select == 4 && week_num(year, month, week) < week_num(2, 5, 2)) {
			//�~�Y�E�����o��
			button_next->change_flag(false, DARK_YELLOW);
		}
		else if (st_select == 5) {
			//�E�[�܂ŗ���
			button_next->change_flag(false, DARK_YELLOW);
		}
	}
}

void OtherData::draw() {
	//�{�^��
	button_next->draw(hand_x, hand_y);
	button_prev->draw(hand_x, hand_y);
	//�\�������߂��Ă���
	if (dx != 0) {
		dx = (int)(dx * 0.75);
	}
	//�����G�\��
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	DrawRotaGraph2(0 - dx, -50, 0, 0, STAND_EXTEND, 0, handle[st_select], TRUE, FALSE);
	SetDrawMode(DX_DRAWMODE_NEAREST);
	//���k���
	DrawBox(800 + dx, 150, GAME_WIDE, GAME_HEIGHT, SAFE_COLOR, TRUE);
	//���O
	DrawBox(810 + dx, 160, GAME_WIDE, 260, WHITE, TRUE);
	DrawStringToHandle(820 + dx, 170, other_full_name[st_select], GRAY, font_small_handle);
	DrawStringToHandle(820 + dx, 200, other_kanji_name[st_select], BLACK, font_handle);
	//�L�����Љ�
	for (unsigned int i = 0; i < prof[st_select].size(); i++) {
		DrawStringToHandle(850 + dx, 400 + i * 50, prof[st_select][i].c_str(), BLACK, font_small_handle);
	}
}

void OtherData::reset() {
	st_select = 0;
	button_next->change_flag(true, LIGHT_YELLOW);
	if (st_select == 0 && week_num(year, month, week) < week_num(1, 7, 1)) {
		//�}�N�^���o��
		button_next->change_flag(false, DARK_YELLOW);
	}
	button_prev->change_flag(false, DARK_YELLOW);
	dx = START_DX;
}

EventData::EventData() {
	m_now = 1;
	te_now = false;
	//�t�H���g
	font_handle = CreateFontToHandle(NULL, 50, 10);
	font_small_handle = CreateFontToHandle(NULL, 30, 8);
	//���ʉ�
	decide_sound = LoadSoundMem("sound/�V�X�e����/����.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, decide_sound);
	//���̉摜�n���h��
	up_handle = LoadGraph("picture/����.png");
	down_handle = LoadGraph("picture/�����.png");
	//���{�^��
	button_up = new Button("", 200, 150, 800, 70, GRAY2, YELLOW, font_handle, BLACK);
	button_up->change_flag(false, GRAY);
	button_down = new Button("", 200, 850, 800, 70, GRAY2, YELLOW, font_handle, BLACK);
	button_up->set_graph(up_handle, 3);
	button_down->set_graph(down_handle, 3);
	//�C�x���g�̖��O���擾
	int file;
	char buff[256];
	file = FileRead_open("text/story.csv");
	for (int i = 0; i < 120; i++) {
		FileRead_gets(buff, 256, file);//�Ō�܂Ō��Ă���
		for (int j = 0; buff[j] != '\0'; j++) {
			if (buff[j] == ',') {
				break;
			}
			else {
				story_name[i] += buff[j];
			}
		}
	}
	FileRead_close(file);//�t�@�C���N���[�Y
	//�X�g�[���[�{�^��
	button_story = new Button*[DRAW_TEXT_BUTTON];
	for (int i = 0; i < DRAW_TEXT_BUTTON; i++) {
		button_story[i] = new Button("", 200, 250 + i * 100, 800, 70, LIGHT_YELLOW, YELLOW, font_small_handle, BLACK);
	}
	reflesh();
}

EventData::~EventData() {
	DeleteFontToHandle(font_handle);
	DeleteFontToHandle(font_small_handle);
	DeleteSoundMem(decide_sound);
	DeleteGraph(up_handle);
	DeleteGraph(down_handle);
	delete button_up;
	delete button_down;
	for (int i = 0; i < DRAW_TEXT_BUTTON; i++) {
		delete button_story[i];
	}
	delete[] button_story;
}

//�X�g�[���[�{�^�����X�V
void EventData::reflesh() {
	for (int i = 0; i < DRAW_TEXT_BUTTON; i++) {
		ostringstream s;
		int y, m, w;
		get_date(i + m_now, y, m, w);
		s << y << "�N��" << m << "��" << w << "�T�ځF";
		if (!story_list[i + m_now - 1]) {//�������ƂȂ��z�͌���Ȃ�
			button_story[i]->change_flag(false, DARK_YELLOW);
			s << "�H�H�H";
		}
		else {
			button_story[i]->change_flag(true, LIGHT_YELLOW);
			s << story_name[i + m_now - 1];
		}
		button_story[i]->set_string(s.str().c_str());
	}
}

bool EventData::play() {
	if (te_now) {//�C�x���g������
		return true;
	}
	GetMousePoint(&hand_x, &hand_y);

	//�}�E�X�̃z�C�[����]
	int rot = GetMouseWheelRotVol();
	if (rot > 0 && button_up->get_flag()) {//���
		m_now--;
		reflesh();
		button_down->change_flag(true, GRAY2);
	}//����
	else if (rot < 0 && button_down->get_flag()) {
		m_now++;
		reflesh();
		button_up->change_flag(true, GRAY2);
	}//�N���b�N�ł��㉺�ł���
	else if (left_click() == 1 || (left_click() > 10 && left_click() % 2 == 1)) {
		if (button_up->overlap(hand_x, hand_y)) {
			m_now--;
			reflesh();
			button_down->change_flag(true, GRAY2);
		}
		else if (button_down->overlap(hand_x, hand_y)) {
			m_now++;
			reflesh();
			button_up->change_flag(true, GRAY2);
		}
	}
	//���N���b�N�����ꂽ�Ƃ�
	if (left_click() == 1) {
		//�X�g�[���[���ς�
		for (int i = 0; i < DRAW_TEXT_BUTTON; i++) {
			if (button_story[i]->overlap(hand_x, hand_y)) {
				PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
				change_music("none", false, true);//BGM�͂����������
				te = new TextEvent(TEXT_STORY, m_now + i, true);
				te_now = true;
			}
		}
	}
	//���{�^���̏�ԍX�V
	if (m_now == 1) {
		button_up->change_flag(false, GRAY);
	}
	if (m_now == 120 - DRAW_TEXT_BUTTON + 1) {
		button_down->change_flag(false, GRAY);
	}
	return false;
}

bool EventData::draw() {
	if (te_now) {//�C�x���g������
		if (te->play()) {
			te_now = false;
			delete te;
			//���j���[��ʂ̉��y�ɖ߂��Ă���
			change_music("music/menu.ogg", false, true);
		}
		return true;
	}
	for (int i = 0; i < DRAW_TEXT_BUTTON; i++) {
		button_story[i]->draw(hand_x, hand_y);
	}
	button_up->draw(hand_x, hand_y);
	button_down->draw(hand_x, hand_y);
	return false;
}