#include"DxLib.h"
#include"SDvalue.h"
#include<string>

using namespace std;

//�e�X�g�p�Ɏg��
int week_num(int m_year, int m_month, int m_week);

int loading_haikei;

//�{�Q�[���Ŏg�p����萔

////�Z�[�u����f�[�^//////////
int SoundVolume = 50;	//�Q�[���̉���(�f�t�H���g�͂T�O)
int weapon_list[100];	//�e����̏�������ۑ�
int material[10];		//�e�f�ނ̏�������ۑ�
int year = 1;			//�����N��
int month = 4;			//������
int week = 1;			//�����̉�����
int level[10];			//�e���k�̃��x��
int exp_sum[10];		//�e���k�̌o���l
int love[10];			//�e���k�̐e���x�i�A�J�c�L�͋󔒁j
int item_num = 0;		//�񕜖�̏�����
int money = 0;			//������
int have_weapon[10];	//�e���k�̑��������� ����̔ԍ� 1�`
int classmate[10];		//�e���k���ǂ��̃N���X��
int max_wave = 0;		//�ō��ŉ�wave�܂ōs������
int finish_event[10];	//���I������e���x�C�x���g�̐�0-5
int finish_story;		//���I������X�g�[���[�̐�(�����������̂��܂�)
bool class_event;		//�����A�N���X���C�g�Ƃ̃C�x���g����������ǂ����i���I�̌��ʊ֌W�Ȃ��j
bool sub_event_flag;	//�����A�T�u�C�x���g����������ǂ����i���݂��邩�͊֌W�Ȃ��j
bool story_list[120];	//�e���C���X�g�[�����ς����Ƃ����邩
////////////////////////////

//�Z�[�u
bool game_save() {
	FILE *fp;
	if (fopen_s(&fp, "�Z�[�u�f�[�^/savedata.dat", "wb") != 0) {
		return false;
	}
	fwrite(&SoundVolume, sizeof(int), 1, fp);
	fwrite(weapon_list, sizeof(int), WEAPON_SUM, fp);
	fwrite(material, sizeof(int), MATERIAL_SUM, fp);
	fwrite(&year, sizeof(int), 1, fp);
	fwrite(&month, sizeof(int), 1, fp);
	fwrite(&week, sizeof(int), 1, fp);
	fwrite(level, sizeof(int), 10, fp);
	fwrite(exp_sum, sizeof(int), 10, fp);
	fwrite(love, sizeof(int), 10, fp);
	fwrite(&item_num, sizeof(int), 1, fp);
	fwrite(&money, sizeof(int), 1, fp);
	fwrite(have_weapon, sizeof(int), 10, fp);
	fwrite(classmate, sizeof(int), 10, fp);
	fwrite(&max_wave, sizeof(int), 1, fp);
	fwrite(finish_event, sizeof(int), 10, fp);
	fwrite(&finish_story, sizeof(int), 1, fp);
	fwrite(&class_event, sizeof(bool), 1, fp);
	fwrite(&sub_event_flag, sizeof(bool), 1, fp);
	fwrite(story_list, sizeof(bool), 120, fp);
	fclose(fp);
	return true;
}

//���[�h
bool game_load() {
	//for (int i = 0; i < 120; i++) {
	//	story_list[i] = false;
	//}
	//for (int i = 0; i < 10; i++) {
	//	material[i] = 0;
	//}
	//class_event = false;
	//sub_event_flag = false;
	//year = 3;
	//month = 4;
	//week = 1;
	//finish_story = week_num(year, month, week) - 1;
	//money = 20000;
	//SoundVolume = 50;
	//max_wave = 100;
	//for (int i = 0; i < 10; i++) {
	//	finish_event[i] = 0;
	//	level[i] = 1;
	//	exp_sum[i] = 0;
	//	love[i] = 1000;
	//	have_weapon[i] = i + 1;
	//	classmate[i] = GetRand(3) + 1;
	//}
	////�Œ�ł�1�l�̓N���X���C�g�ɂȂ�
	//while (true) {
	//	int r = GetRand(8) + 1;
	//	if (r != 8) {//�C�o���J���͗�O
	//		classmate[r] = classmate[0];
	//		break;
	//	}
	//}
	//for (int i = 0; i < 100; i++) {
	//	if (i % 10 >= 0 && i % 10 <= 9) {
	//		weapon_list[i] = 0;
	//	}
	//}
	//return true;

	FILE *fp;
	if (fopen_s(&fp, "�Z�[�u�f�[�^/savedata.dat", "rb") != 0) {
		return false;
	}
	fread(&SoundVolume, sizeof(int), 1, fp);
	fread(weapon_list, sizeof(int), WEAPON_SUM, fp);
	fread(material, sizeof(int), MATERIAL_SUM, fp);
	fread(&year, sizeof(int), 1, fp);
	fread(&month, sizeof(int), 1, fp);
	fread(&week, sizeof(int), 1, fp);
	fread(level, sizeof(int), 10, fp);
	fread(exp_sum, sizeof(int), 10, fp);
	fread(love, sizeof(int), 10, fp);
	fread(&item_num, sizeof(int), 1, fp);
	fread(&money, sizeof(int), 1, fp);
	fread(have_weapon, sizeof(int), 10, fp);
	fread(classmate, sizeof(int), 10, fp);
	fread(&max_wave, sizeof(int), 1, fp);
	fread(finish_event, sizeof(int), 10, fp);
	fread(&finish_story, sizeof(int), 1, fp);
	fread(&class_event, sizeof(bool), 1, fp);
	fread(&sub_event_flag, sizeof(bool), 1, fp);
	fread(story_list, sizeof(bool), 120, fp);
	fclose(fp);
	return true;
}
//////////////////////////////

//���ƌ�A�܂����[�v�ł���悤������
void game_init() {
	class_event = false;
	sub_event_flag = false;
	year = 1;
	month = 4;
	week = 1;
	finish_story = week_num(year, month, week) - 1;
	money = 0;
	SoundVolume = 50;
	max_wave = 0;
	//�e���k
	for (int i = 0; i < 10; i++) {
		finish_event[i] = 0;
		level[i] = 1;
		exp_sum[i] = 0;
		love[i] = 0;
		have_weapon[i] = i + 1;
	}
	//�f��
	for (int i = 0; i < MATERIAL_SUM; i++) {
		material[i] = 0;
	}
	//����
	for (int i = 0; i < 100; i++) {
		if (i % 10 >= 0 && i % 10 <= 9) {
			weapon_list[i] = 0;
		}
	}
}

int music = -1;
bool loop_flag = true;//���[�v�Đ����Ȃ�true
int music_length = 0;//�Ȃ̒���
string music_name;
void change_music(const char* name, bool high, bool loop) {
	loop_flag = loop;
	StopSoundMem(music);
	if (music != -1) {
		DeleteSoundMem(music);
		music_name = "";
	}
	music_name = name;
	music = LoadSoundMem(name);
	music_length = GetSoundTotalTime(music);
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, music);
	if (high) {//������������
		SetFrequencySoundMem(60000, music);
	}
	if (loop_flag) {
		PlaySoundMem(music, DX_PLAYTYPE_LOOP);
	}
	else {
		PlaySoundMem(music, DX_PLAYTYPE_BACK);
	}
}
//���炩���߃��[�h�������y�ɕύX
void change_load_music(int handle, bool high, bool loop) {
	if (music != -1) {
		DeleteSoundMem(music);
		music_name = "";
	}
	music = handle;
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, music);
	if (high) {//������������
		SetFrequencySoundMem(60000, music);
	}
	loop_flag = loop;
	if (loop_flag) {
		PlaySoundMem(music, DX_PLAYTYPE_LOOP);
	}
	else {
		PlaySoundMem(music, DX_PLAYTYPE_BACK);
	}
}

//���y���Đ�����
void play_music() {
	if (!CheckSoundMem(music) && loop_flag) {//���y
		//SetLoopStartTimePosSoundMem(music_length - 2200, music);
		PlaySoundMem(music, DX_PLAYTYPE_LOOP);
	}
}

//���y���Đ������ǂ���
int check_music() {
	return CheckSoundMem(music);
}

//���ʂ�ύX����
void change_volume() {
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, music);
}

//���̃��x���A�b�v�ɕK�v�Ȍo���l�i�݌v�j
int need_exp(int now_level) {
	return ((1 + now_level) * now_level / 2) * 10;
}

//���k�ԍ�num�̌o���l���K�v�Ȍo���l������΃��x���A�b�v������
void level_up(int num) {
	while (true) {
		if (level[num] == LEVEL_MAX) {//���x���͊��ɍő�
			exp_sum[num] = need_exp(LEVEL_MAX);
			break;
		}
		if (exp_sum[num] >= need_exp(level[num])) {//�P���x���A�b�v
			level[num]++;
		}
		else {//�o���l������Ȃ�
			break;
		}
	}
}

//�N���T�������Ƃ��A���w���牽�T�ڂ���Ԃ�
int week_num(int m_year, int m_month, int m_week) {
	//m_year ���w���牽�N��������
	//m_month 4�����牽������������
	//m_week �������߂��牽�T�ڂ�
	m_year--;
	m_month -= 4;//4�����w
	if (m_month < 0) {//1-3���̏ꍇ
		m_month = 12 + m_month;
	}
	int res = m_year * 12 * 4 + m_month * 4 + m_week;
	res -= m_year * 8;//1�N�ŋx�݂̏T��8
	if (m_month > 8 - 4) {
		res -= 4;//���N�̉ċx�݂͉߂���
	}
	if (m_month > 12 + 1 - 4 || (m_month > 12 - 4 && m_week > 1)) {
		res -= 2;//���N�̓~�x�݂͉߂���
	}
	return res;
}

//���w���牽���ڂ�����͂Ƃ��A���t�𓾂�
void get_date(int sum_week, int& m_year, int& m_month, int& m_week) {
	if (sum_week % 40 == 0) {//�w�N�ŏI���̓R�[�i�[�P�[�X
		m_year = sum_week / 40;
		m_month = 3;
		m_week = 2;
		return;
	}
	m_year = sum_week / 40 + 1;//�܂��͊w�N���m��
	sum_week %= 40;
	//���Ɍ������m��
	for (int m = 1; m <= 12; m++) {
		int now = m + 3;//4���X�^�[�g�ɒ���
		int weeks;//�����͉��T�Ԃ���H
		if (now == 8) { continue; }//�ċx��
		if (now == 12) { weeks = 3;}//�~�x��
		else if (now == 13) { weeks = 3; }//�~�x��
		else if (now == 15) { weeks = 2; }//�t�x��
		else { weeks = 4; }//�ʏ�
		if (sum_week - weeks <= 0) {
			m_month = now;
			m_week = sum_week;
			break;
		}
		sum_week -= weeks;
	}
	if (m_month == 13) {//1��������2�T�ڂ���X�^�[�g
		m_week++;
	}
	if (m_month >= 13) {//13����1��
		m_month -= 12;
	}
}

const char* st_name[] = { "�A�J�c�L", "�n���_",
							"�I�I�^", "�C�V", "�^�C��",
							"�I���_", "�~���g�l", "�E�`���g",
							"�C�o���J��", "�g���`" };

const char* st_kanji_name[] = { "�Ԍ��@����", "���c�@�T�l",
								"���c�@����", "�Ɏu�@����", "�䗅�@���I",
								"���a�c�@�ǎq", "�{�����@����", "���{�@�t��",
								"���@�V�q", "�u�p�@�{�b" };

const char* st_full_name[] = { "�A�J�c�L�@�����E��", "�n���_�@���E�g",
								"�I�I�^�@�J�c��", "�C�V�@�P���X�P", "�^�C���@�R�E�L",
								"�I���_�@�A���R", "�~���g�l�@�����J", "�E�`���g�@�n���J",
								"�C�o���J���@�e���R", "�g���`�@�X�G" };

const char* other_name[] = { "�N����", "�}�N�^", "�V���V�L", "�T�C�J", "�T�L�K�P", "�~�Y�E��"};

const char* other_kanji_name[] = { "����@�P�q", "�ԋ㑽�@�ߒ�",
									"�Ύ��@�[���Y", "�ʉ��@�T��",
									"��R�@�O", "���Y�@�؍�"};

const char* other_full_name[] = { "�N�����@�L�R", "�}�N�^�@�C�d��",
									"�V���V�L�@�P�C�^���E", "�T�C�J�@�R�m�~",
									"�T�L�K�P�@�g�I��", "�~�Y�E���@�J�i"};

const char* en_name[] = { "fing", "hang", "ace", "tehira", "tako",
 "kumo", "mizu", "kuti", "um01", "um02", "um03", "tessie", "gu", "um01b", "um02b", "um03b" };


const char* material_name[] = { "�����؍�", "�d����", "��������", 
								"���ꂢ�Ȑ�", "���͂Ȏ�", "��̉t��",
								"���ȕ���", "�Ԃ�����", "�_��̕�", 
								"�n���̗��Ƃ���"};

const char* start_weapon_name[] = { "���q�p�c�C���\�[�h", "���q�p�}�V���K��",
								"���q�p�r�b�O�\�[�h", "���q�p�T���_�[",
								"���q�p�X�i�C�p�[", "���q�p�\�[�h",
								"���q�p�V�[���h�E�K��", "���q�p�N���\",
								"���q�p�����O�\�[�h", "���q�p�T�C�Y" };

const char* rare_weapon_name[] = { "�_�C�������h�c�C���\�[�h", "�_�C�������h�}�V���K��",
									"�_�C�������h�r�b�O�\�[�h", "�_�C�������h�T���_�[",
									"�_�C�������h�X�i�C�p�[", "�_�C�������h�\�[�h",
									"�_�C�������h�V�[���h�E�K��", "�_�C�������h�N���\",
									"�_�C�������h�����O�\�[�h", "�_�C�������h�T�C�Y" };