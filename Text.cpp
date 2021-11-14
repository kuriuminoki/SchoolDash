#include"DxLib.h"
#include"SDvalue.h"
#include"Control.h"
#include"Text.h"
#include"Button.h"
#include<fstream>
#include<iostream>
#include<string>
#include<algorithm>
#include<vector>

using namespace std;

//�C�x���g�̖��O��Ԃ�
string get_event_name(string kind, int num) {
	//csv�t�@�C������f�[�^�����
	int fp;
	char buff[256];
	string pass;
	pass = "text/" + kind + ".csv";
	fp = FileRead_open(pass.c_str());
	//�t�@�C���|�C���^���ړ�
	for (int i = 1; i <= num; i++) {
		FileRead_gets(buff, 256, fp);//�Ō�ȊO�ǂݔ�΂�
	}
	FileRead_close(fp);
	//�����e�L�X�g�œ���
	string s = "";
	for (int i = 0; buff[i] != '\0'; i++) {
		if (buff[i] == ',') {
			return s.c_str();
		}
		else {
			s += buff[i];
		}
	}
	return s.c_str();
}

//csv�t�@�C���F[�C�x���g��, �N, ��, �T, �����N���X�ɕK�v�Ȑ��k]
//csv�t�@�C������C�x���g�̔��������m�F�A�����Ȃ�true
bool check_event_file(string kind, int num) {
	//csv�t�@�C������f�[�^�����
	int fp;
	char buff[256];
	string pass;
	pass = "text/" + kind + ".csv";
	fp = FileRead_open(pass.c_str());
	//�t�@�C���|�C���^���ړ�
	for (int i = 1; i <= num; i++) {
		FileRead_gets(buff, 256, fp);//�Ō�ȊO�ǂݔ�΂�
	}
	//�����e�L�X�g�œ���
	string s = "";
	vector<string> data;
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
	//�t�@�C���N���[�Y
	FileRead_close(fp);
	int y, m, w;
	y = atoi(data[1].c_str());
	m = atoi(data[2].c_str());
	w = atoi(data[3].c_str());
	//�T�u�C�x���g�̑��ݗL���𒲂ׂ�ꍇ
	if (kind == "sub") {
		if (data[4] == "�T�u") {
			return true;
		}
		else {
			return false;
		}
	}
	//�L�������ɑΉ�����L�����ԍ��𒲂ׂ�
	int n = 0;//���Ȃ��ꍇ��0�A��΂ɏ����𖞂���
	for (int i = 1; i < 10; i++) {
		if (data[4] == st_name[i]) {//��v����L�����̖��O����
			n = i;
			break;
		}
	}
	//���t�ƃN���X���C�g�̏����𖞂����Ă��邩
	if (week_num(year, month, week) >= week_num(y, m, w) && classmate[0] == classmate[n]) {
		return true;
	}
	return false;
}

//�N���X���C�g�Ƃ̃C�x���g�I��
const int sub_sum = 3;
int decide_sub(int st_num) {
	switch (st_num) {
	case HANDA:
		return GetRand(sub_sum - 1) +1;
		break;
	case OOTA:
		return GetRand(sub_sum - 1) +1;
		break;
	case ISHI:
		return GetRand(sub_sum - 1) +1;
		break;
	case TAIRA:
		return GetRand(sub_sum - 1) +1;
		break;
	case OWADA:
		return GetRand(sub_sum - 1) +1;
		break;
	case MIYATONE:
		return GetRand(sub_sum - 1) +1;
		break;
	case UCHIMOTO:
		return GetRand(sub_sum - 1) +1;
		break;
	case IBARAKAWA:
		return GetRand(sub_sum - 1) +1;
		break;
	case TOMOCHI:
		if(year == 1){ return 1; }
		else{ return GetRand(sub_sum - 1) + 2; }
		break;
	}
	return 1;
}

//�`��ʒu�̐ݒ�
void Stand::set_position(bool left) {
	if (left) {
		reverse = FALSE;
		position = -100;
	}
	else {
		reverse = TRUE;
		position = 1280 - (int)(1240*STAND_EXTEND) + 100;
	}
}

//�����G�̃��[�h
void Stand::load(const char *name) {
	if (handle != -1) {//�O�̉摜������Ȃ�������Ă���
		DeleteGraph(handle);
	}
	handle = LoadGraph(name);
	GetGraphSize(handle, &x_size, &y_size);
}

//�����G�̕`��
void Stand::draw(int dx, int dy) {
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	DrawRotaGraph2(position + dx, 0 + dy, 0, 0, STAND_EXTEND, 0, handle, TRUE, reverse);
	SetDrawMode(DX_DRAWMODE_NEAREST);
}

Stand::~Stand() {
	if (handle != -1) {//�O�̉摜������Ȃ�������Ă���
		DeleteGraph(handle);
	}
}



//////////�e�L�X�g�C�x���g�N���X/////////////////
TextEvent::TextEvent(EVENT_KIND kind, int num, bool bright, string folder) {
	first_flag = false;
	event_name = "";
	finish_text = false;
	bright_value = 255;//���Ƃ��Ƃ͖��邢�܂܂̐ݒ�
	bright_flag = bright;
	if (bright_flag) { bright_value = 0; }//�^���ÃX�^�[�g
	string file_name;
	switch (kind) {
	case TEXT_STORY:
		file_name = "story";
		event_name = get_event_name(file_name, num);
		file_name = "text/story/story_";
		break;
	case TEXT_SUB:
		file_name = "text/sub/";
		file_name += folder;
		file_name += "/sub_";
		break;
	case TEXT_OTHER:
		file_name = "text/other/";
		file_name += folder;
		file_name += "/other_";
		break;
	case TEXT_EVENT:
		file_name = "text/event/";
		file_name += folder;
		file_name += "/event_";
		event_name = get_event_name(folder, num);
		break;
	case TEXT_BATTLE:
		file_name = "text/battle/";
		file_name += folder;
		file_name += "/battle_";
		break;
	}
	string num_s;//�ԍ��̕�����
	if (num == 0) { 
		num_s = '0';
		event_name = "";
	}
	while (num > 0) {
		num_s += '0' + (num % 10);
		num /= 10;
	}
	reverse(num_s.begin(), num_s.end());
	file_name += num_s;
	file_name += ".txt";
	//�����G
	stand = new Stand[2];
	stand[0].set_position(true);
	stand[1].set_position(false);
	//�t�L�_�V
	huki = LoadGraph("picture/�t�L�_�V.png");
	//�w�i
	char sentence[256];
	string str;
	//�t�@�C�����J��
	file = FileRead_open(file_name.c_str());
	FileRead_gets(sentence, 256, file);//1�s�ǂݍ���
	str = sentence;
	haikei = LoadGraph(("picture/haikei/" + str).c_str());
	//���ʉ�
	kettei = LoadSoundMem("sound/�V�X�e����/����.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, kettei);
	say_left = false;
	say_none = false;
	textfont = CreateFontToHandle(NULL, 30, 8);
	//��ʕ\���̂���
	left_dx = -1 * START_DX; right_dx = START_DX; text_dy = START_DY;
	effect_sound = -1;
}

//�f�X�g���N�^
TextEvent::~TextEvent() {
	FileRead_close(file);//�t�@�C�������
	delete[] stand;//�E�ƍ��̂Q�l������
	DeleteGraph(haikei);
	DeleteGraph(huki);
	DeleteSoundMem(kettei);
	DeleteFontToHandle(textfont);
	if (effect_sound != -1) {
		DeleteSoundMem(effect_sound);
	}
}

//���C�� �`�������
bool TextEvent::play() {
	//�����t�F�[�Y
	if (next_cnt > 0) {//��N���b�N�h�~
		next_cnt--;
	}
	if (bright_value == 255) {
		//��ʕ\���̂���
		if (left_dx != 0) { left_dx = (int)(left_dx * 0.75); }
		if (right_dx != 0) { right_dx = (int)(right_dx * 0.75); }
		if (text_dy != 0) { text_dy = (int)(text_dy * 0.75); }
		//���N���b�N���ꂽ�Ƃ� or �Z���t����̎��i�ŏ��j
		if (left_click() == 1 || control_space() == TRUE || saying.size() == 0) {
			if (next()) {
				finish_text = true;//�t�@�C�����I�[������I��
				//�Â����Ă���
				if (bright_flag) { bright_value = 254; }
			}
		}
		deal_text();//�Z���t���P�������\�����钲��
	}
	if (bright_value != 255) {
		if (finish_text) {//�I�����A�Â�����
			bright_value = (int)(bright_value * 0.75);
		}
		else {//�J�n�܂��͔w�i�ύX���A���邭����
			if (bright_value < 4) { bright_value = 4; }
			bright_value = (int)(bright_value * 1.25);
			if (bright_value > 255) { bright_value = 255; }
		}
	}

	//�`��t�F�[�Y
	draw();
	if (control_quit() == TRUE) {
		finish_text = true;//�I��
		//�Â����Ă���
		if (bright_flag) { bright_value = 254; }
	}
	if (!bright_flag && finish_text) {
		return true;
	}
	if (bright_flag && bright_value == 0 && finish_text) {
		return true;
	}

	return false;
}

//�Z���t���P�������\������悤�ɒ���
void TextEvent::deal_text() {
	if (text_now < (int)saying.size()) {
		text_now += 2;
		if (text_now >(int)saying.size()) {
			text_now = (int)saying.size();
		}
	}
}

//���N���b�N���ꂽ��Ă΂��
bool TextEvent::next() {
	//�Z���t�̓r���������ꍇ�F�Z���t���Ō�܂ŃX�L�b�v����
	if (text_now > 0 && text_now != (int)saying.size()) {
		text_now = (int)saying.size();
		next_cnt = 0;
	}
	else if (next_cnt > 0) {
		next_cnt = 0;
		return false;//��N���b�N�Ƃ݂Ȃ��Ď��̃e�L�X�g�Ɉڍs���Ȃ�
	}//�Z���t���I���ς݂������F���̃u���b�N�ֈړ�
	else if(FileRead_eof(file) == 0){//�t�@�C�����Ō�łȂ��Ȃ�
		if (saying.size() != 0) {
			first_flag = true;
		}
		if (left_click() == 1 || control_space() == TRUE) {
			PlaySoundMem(kettei, DX_PLAYTYPE_BACK);//���ʉ�
		}
		left_dx = 0; right_dx = 0;
		file_read();//���̃Z���t
	}
	else {//�I��
		return true;
	}
	return false;
}

//�e�L�X�g�t�@�C���̃X�N���v�g����̉���
void TextEvent::file_read() {
	char sentence[256];
	string str;
	do {//�t�@�C���̏I�[�܂��͋�s�܂Ń��[�v
		FileRead_gets(sentence, 256, file);//1�s�ǂݍ���
		str = sentence;
		if (str == "@name") {//�L�����̗����G�ύX
			say_none = false;//�N������ׂ�Ȃ���ԉ���
			FileRead_gets(sentence, 256, file);//1�s�ǂݍ���
			str = sentence;//-left or -right or �L�����̖��O
			//�����G�ύX����̂ɁA���E���w�肵���ꍇ
			if (str == "-left") {
				say_left = false;//���͉E������ׂ��Ă��邱�Ƃɂ���
				FileRead_gets(sentence, 256, file);//1�s�ǂݍ���
			}
			else if (str == "-right") {
				say_left = true;
				FileRead_gets(sentence, 256, file);//1�s�ǂݍ���
			}
			//�����G���X���C�h�����ēo�ꂳ����ꍇ
			str = sentence;
			if (str == "-move") {
				if (say_left) { right_dx = START_DX; }
				else{ left_dx = -1 * START_DX; }
				FileRead_gets(sentence, 256, file);//1�s�ǂݍ���
			}
			//�������痧���G�̃��[�h
			string stand_name;//(�L�����̖��O)
			stand_name = "picture/stand/";
			stand_name += sentence;// [/�L������] �̃p�X�ǉ�
			stand_name += "/";
			stand_name += sentence;// [�L������/�L�������\��]�̌`��
			speaker = sentence;//�����҂̖��O�X�V
			FileRead_gets(sentence, 256, file);//1�s�ǂݍ���(�L�����̕\��)
			if (month >= 6 && month <= 10) { stand_name += "��"; }//�ߑւ�
			stand_name += sentence;// [�\��] �̃t�@�C������ǉ�
			stand_name += ".png";
			stand[say_left].load(stand_name.c_str());
			say_left = !say_left;
		}
		else if (str == "@get") {//����Q�b�g
			FileRead_gets(sentence, 256, file);//1�s�ǂݍ���(����̔ԍ�)
			int num = atoi(sentence);
			weapon_list[num - 1]++;//����Q�b�g
		}
		else if (str == "@music") {//BGM�ύX
			FileRead_gets(sentence, 256, file);//1�s�ǂݍ���
			string pitch = sentence;
			bool high = false;
			if (pitch == "-high") { //"-high"�ȊO�Ȃ特�t�@�C���̖��O
				high = true;
				FileRead_gets(sentence, 256, file);//1�s�ǂݍ���(���t�@�C���̖��O)
			}
			change_music(sentence, high, true);
		}
		else if (str == "@love") {//�D���x�A�b�v
			FileRead_gets(sentence, 256, file);//1�s�ǂݍ���(�D���x�A�b�v�̐��k�c��)
			str = sentence;
			for (int i = 0; i < 10; i++) {//�w�肳�ꂽ�c���̐��k�̔ԍ��𒲂ׂ�
				if (str == st_name[i]) {
					FileRead_gets(sentence, 256, file);//1�s�ǂݍ���(�オ�鐔�l)
					love[i] += atoi(sentence); 
					if (love[i] > LOVE_MAX) { love[i] = LOVE_MAX; }
					break;
				}
			}
		}
		else if (str == "@sound") {
			//���ʉ��̃f�[�^������
			if (effect_sound != -1) { DeleteSoundMem(effect_sound); }
			FileRead_gets(sentence, 256, file);//1�s�ǂݍ���
			effect_sound = LoadSoundMem(sentence);
			ChangeVolumeSoundMem(255 * SoundVolume / 100, effect_sound);
			PlaySoundMem(effect_sound, DX_PLAYTYPE_BACK);
		}
		else if (str == "@none") {//�N������ׂ�Ȃ�
			say_none = true;
		}
		else if (str == "@haikei") {
			if (haikei != -1) { DeleteGraph(haikei); }
			FileRead_gets(sentence, 256, file);//1�s�ǂݍ���
			str = sentence;
			haikei = LoadGraph(("picture/haikei/" + str).c_str());
			bright_value = 0;//��ʂ��Â�����
			text_dy = START_DY;//�t�L�_�V�����炷
		}
		else if(sentence[0] != 0){//�P�ɃZ���t���Z�b�g���邾��
			text_now = 0;
			saying = str;
			if (!say_none) {
				saying = "�u" + saying + "�v";
			}
		}
	} while (FileRead_eof(file) == 0 && sentence[0] != 0);
}

//�Z���t���R��������
static const int MAX_SAYING = 33;//��s�ɑS�p�ŉ������܂ł�
void TextEvent::saying_divide() {
	sub_saying[0] = saying.substr(0, min(MAX_SAYING * 2, text_now));
	if (text_now > MAX_SAYING * 2) {
		sub_saying[1] = saying.substr(MAX_SAYING * 2, min(MAX_SAYING * 2, text_now - MAX_SAYING * 2));
	}
	else {
		sub_saying[1] = "";
	}
	if (text_now > MAX_SAYING * 4) {
		sub_saying[2] = saying.substr(MAX_SAYING * 4, text_now - MAX_SAYING * 4);
	}
	else {
		sub_saying[2] = "";
	}
}

//�w�i�A�����G�A�t�L�_�V�̕`��
void TextEvent::draw() {
	//��ʂ��Â�����
	SetDrawBright(bright_value, bright_value, bright_value);
	//�w�i
	//DrawBox(0, 0, 1280, 1024, GRAY, TRUE);
	DrawRotaGraph2(0, 0, 0, 0, 1.0, 0, haikei, TRUE, FALSE);//�w�i�摜

	stand[0].draw(left_dx, 0);//���̐l
	stand[1].draw(right_dx, 0);//�E�̐l

	//�C�x���g��(����ɕ\��)
	if (event_name.size() > 0) {
		DrawBox(0, 0, 38 + event_name.size() * 16, 60, GRAY2, TRUE);
		DrawStringToHandle(20, 18, event_name.c_str(), BLACK, textfont);
	}

	//��ʂ̖��邳�����ɖ߂�
	SetDrawBright(255, 255, 255);
	//��ʂ��Â��Ȃ��Ă���Ƃ��̓Z���t�\�����Ȃ�
	if (bright_value != 255) { return; }

	//�t�L�_�V�ƃZ���t�\��
	static const int tLEFT = 250;
	static const int tRIGHT = 1280 - tLEFT - 100;
	//�t�L�_�V
	DrawRotaGraph2(83, 710 + text_dy, 0, 0, 1, 0, huki, TRUE, FALSE);
	//�t�L�_�V�̂���Ȃ��@���@�N���̔������Ȃ���
	if (text_dy == 0 && !say_none) {
		if (say_left) {//���̐l��������
			DrawTriangle(tLEFT - 5, 721, tLEFT + 105, 721, tLEFT + 50, 655, BLACK, TRUE);
			DrawTriangle(tLEFT, 721, tLEFT + 100, 721, tLEFT + 50, 660, WHITE, TRUE);
		}
		else {//�E�̐l��������
			DrawTriangle(tRIGHT - 5, 721, tRIGHT + 105, 721, tRIGHT + 50, 655, BLACK, TRUE);
			DrawTriangle(tRIGHT, 721, tRIGHT + 100, 721, tRIGHT + 50, 660, WHITE, TRUE);
		}
	}
	//�Z���t
	saying_divide();
	for (int i = 0; i < 3; i++) {//770 + i * 70
		DrawStringToHandle(127, 810 + i * 60 + text_dy, sub_saying[i].c_str(), BLACK, textfont);
	}
	//�����҂̖��O
	if (!say_none) {
		if (say_left) {
			DrawStringToHandle(150, 750 + text_dy, speaker.c_str(), DARK_BLUE, textfont);
		}
		else {
			DrawStringToHandle(GAME_WIDE - 450, 750 + text_dy, speaker.c_str(), DARK_BLUE, textfont);
		}
	}
	if (text_now > 0 && text_now == (int)saying.size()) {
		if (first_flag) {
			DrawTriangle(1104, 957, 1146, 957, 1125, 998, DARK_YELLOW, TRUE);
			DrawTriangle(1110, 960, 1140, 960, 1125, 992, YELLOW, TRUE);
		}
		else {
			DrawStringToHandle(800, 950, "�p�L�[�������ŃX�L�b�v", BLACK, textfont);
		}
	}
}



/////////////////////////////////////////////////////
//////////�e���x���グ�Č���C�x���g�̉��///////////
StEvent::StEvent(int st_dot[], int stand_handle[]) {
	te_flag = false;
	dx2 = -1 * START_DX;
	dx1 = 0;
	m_st_select = -1;
	for (int i = 0; i < 10; i++) {
		m_stand_handle[i] = stand_handle[i];
	}
	//�t�H���g
	font_handle = CreateFontToHandle(NULL, 50, 10);
	font_small_handle = CreateFontToHandle(NULL, 30, 8);
	//�߂�{�^��
	button_cancel = new Button("�߂�", 30, 20, 150, 80, LIGHT_GREEN, GREEN, font_handle, BLACK);
	button_st = new StButton(9, st_dot, 200);
	button_decide = new Button("�J�n", 1100, 20, 150, 80, LIGHT_BLUE, BLUE, font_handle, BLACK);
	//�C�x���g������邩�̃`�F�b�N
	check_event();
	//���ʉ�
	decide_sound = LoadSoundMem("sound/�V�X�e����/����.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, decide_sound);
}

StEvent::~StEvent() {
	DeleteFontToHandle(font_handle);
	DeleteFontToHandle(font_small_handle);
	delete button_cancel;
	delete button_st;
	DeleteSoundMem(decide_sound);
}

//�e���k�̃C�x���g���ς�邩���ׂ�
void StEvent::check_event() {
	for (int i = 1; i < 10; i++) {
		//�܂���wave�Ɛe���x�Ɛi�s��
		if (max_wave / (100 / EVENT_SUM) >= finish_event[i] && love[i] / (LOVE_MAX / EVENT_SUM) > finish_event[i]) {
			//csv�t�@�C�����m�F
			able_event[i] = check_event_file(st_name[i], finish_event[i] + 1);
		}
		else {
			able_event[i] = false;
		}
	}
}

bool StEvent::play() {
	if (te_flag) {//�e�L�X�g�C�x���g��
		bool f = te->play();
		if (f) { //�I��
			finish_event[m_st_select]++;
			delete te;
		}
		return f;
	}
	GetMousePoint(&hand_x, &hand_y);
	//�����G�̂��炵
	if (dx1 != 0) {
		dx1 = (int)(dx1 * 0.75);
	}
	if (dx2 != 0) {
		dx2 = (int)(dx2 * 0.75);
	}
	//���N���b�N��
	if (left_click() == 1) {
		if (button_cancel->overlap(hand_x, hand_y)) {
			return true;
		}
		//�L������I��ł��āA�C�x���g�������Ȃ�
		if (button_decide->overlap(hand_x, hand_y) && m_st_select != -1 && able_event[m_st_select]) {
			te_flag = true;
			//�e�L�X�g�C�x���g�쐬
			te = new TextEvent(TEXT_EVENT, finish_event[m_st_select] + 1, true, st_name[m_st_select]);
		}
		//���k�̑I��
		int n = button_st->overlap(hand_x, hand_y);
		if (n != -1) {
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
			if (m_st_select != -1) {//�O���L����
				button_st->change_flag(m_st_select - 1, true);
			}
			m_st_select = n + 1;//�L�����ԍ����Z�b�g
			button_st->change_flag(n, false);//�Z�b�g����L����
			dx1 = START_DX;
		}
	}
	return false;
}

void StEvent::draw() {
	if (te_flag) {//�e�L�X�g�C�x���g���͕`�悵�Ȃ�
		return;
	}
	//�w�i
	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, GRAY2, TRUE);
	//���k�̗����G
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	//�A�J�c�L
	DrawRotaGraph2(600 - dx2, -50, 0, 0, STAND_EXTEND, 0, m_stand_handle[0], TRUE, FALSE);
	if (m_st_select != -1) {
		DrawRotaGraph2(-50 - dx1, -50, 0, 0, STAND_EXTEND, 0, m_stand_handle[m_st_select], TRUE, FALSE);
	}
	SetDrawMode(DX_DRAWMODE_NEAREST);
	//�C�x���g�ڍ�
	if (m_st_select != -1) {
		if (able_event[m_st_select]) {
			DrawBox(300, 620, 980, 690, LIGHT_BLUE, TRUE);
			DrawStringToHandle(430, 630, "�C�x���g�J�n�\", DARK_GREEN, font_handle);
		}
		else {
			DrawBox(300, 620, 980, 720, LIGHT_BLUE, TRUE);
			DrawStringToHandle(430, 630, "�C�x���g�J�n�s��", RED, font_handle);
			if (finish_event[m_st_select] == EVENT_SUM) {
				DrawStringToHandle(432, 680, "(�S�ẴC�x���g���I���ς�)", RED, font_small_handle);
			}
			else if (max_wave / (100 / EVENT_SUM) < finish_event[m_st_select]) {
				DrawStringToHandle(440, 680, "(��荂Wave�̒T�����K�v)", RED, font_small_handle);
			}
			else if (love[m_st_select] / (LOVE_MAX / EVENT_SUM) <= finish_event[m_st_select]) {
				DrawStringToHandle(490, 680, "(�e���x������Ȃ�)", RED, font_small_handle);
			}
			else {//�e���x�C�x���g�ɃN���X���C�g�ɂ�鐧���͂Ȃ�
				DrawStringToHandle(417, 680, "(�܂��J�n�ł��鎞���ł͂Ȃ�)", RED, font_small_handle);
			}
		}
	}
	//�{�^��
	button_cancel->draw(hand_x, hand_y);
	//�L������I��ł��āA�C�x���g�������Ȃ�
	if (m_st_select != -1 && able_event[m_st_select]) {
		button_decide->draw(hand_x, hand_y);
	}
	button_st->draw(hand_x, hand_y);
	//�C�x���g�{���\�Ȃ番����₷�����邵������
	for (int i = 0; i < 9; i++) {
		if (!able_event[i + 1]) { continue; }
		if (i + 1 != m_st_select&& !button_st->get_flag(i)) { continue; }
		DrawBox(90 + (i * 131), 983, 151 + (i * 131), 1015, GRAY, TRUE);
		DrawStringToHandle(112 + (i * 131), 985, "!", GREEN, font_small_handle);
	}
}

bool StEvent::get_flag() {
	return te_flag;
}