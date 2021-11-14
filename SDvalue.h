#ifndef SDVALUE_H_INCLUDED
#define SDVALUE_H_INCLUDED

//�{�Q�[���Ŏg�p����萔

#include"DxLib.h"
#include<string>

extern int loading_haikei;

////�Z�[�u����f�[�^//////////
extern int SoundVolume;//�Q�[���̉���
extern int weapon_list[100];//�e����̏�������ۑ�
extern int material[10];//�e�f�ނ̏�������ۑ�
extern int year;//�����N��
extern int month;//������
extern int week;//�����̉�����
extern int level[10];//�e���k�̃��x��
extern int exp_sum[10];//�e���k�̌o���l
extern int love[10];//�e���k�̐e���x�i�A�J�c�L�͋󔒁j
extern int item_num;//�񕜖�̏�����
extern int money;//������
extern int have_weapon[10];//�e���k�̏�������
extern int classmate[10];//�e���k���ǂ��̃N���X��
extern int max_wave;//�ō��ŉ�wave�܂ōs������
extern int finish_event[10];//���I������e���x�C�x���g�̐�
extern int finish_story;//���I������X�g�[���[�̐�(�����������̂��܂�)
extern bool class_event;//�����A�N���X���C�g�Ƃ̃C�x���g����������ǂ����i���I�̌��ʊ֌W�Ȃ��j
extern bool sub_event_flag;//�����A�T�u�C�x���g����������ǂ����i���݂��邩�͊֌W�Ȃ��j
extern bool story_list[120];	//�e���C���X�g�[�����ς����Ƃ����邩
//////////////////////////////

//�Z�[�u�ƃ��[�h
bool game_save();
bool game_load();

//���[�v���̏�����
void game_init();

//���y
extern int music;
extern std::string music_name;
void change_music(const char* name, bool high, bool loop);
void change_load_music(int handle, bool high, bool loop);
void play_music();
int check_music();
void change_volume();

//���̃��x���A�b�v�ɕK�v�ȗ݌v�o���l
int need_exp(int now_level);
//���k�ԍ�num�̌o���l���K�v�Ȍo���l������΃��x���A�b�v������
void level_up(int num);

//�N���T�������Ƃ��A���w���牽�T�ڂ���Ԃ�
int week_num(int m_year, int m_month, int m_week);

//���w���牽���ڂ�����͂Ƃ��A���t�𓾂�
void get_date(int week, int& m_year, int& m_month, int& m_week);

//��ʂ̑傫��
#define GAME_WIDE 1280
#define GAME_HEIGHT 1024

#define BATTLE_UP 265
#define BATTLE_DOWN 645

const double STAND_EXTEND = 0.65;

//���E
#define LEFT 0
#define RIGHT 1

//�e���k�̔ԍ�
enum {
	AKATSUKI = 0,
	HANDA,
	OOTA,
	ISHI,
	TAIRA,
	OWADA,
	MIYATONE,
	UCHIMOTO,
	IBARAKAWA,
	TOMOCHI
};

//�I�I�^����g���`�܂ł́A���ԂɂȂ鎞��
const int join_year[] = { 1, 1, 1, 1, 2, 1, 2, 2};
const int join_month[] = { 6, 10, 10, 4, 6, 4, 5, 4};
const int join_week[] = { 4, 1, 1, 2, 4, 4, 4, 2};

//�G�����Ƃ��f�ނ̔z���ł̃C���f�b�N�X
const int drop_item[] = { 2, 2, 2, 2, 3, 4, 5, 6, 7, 8, 9, 2, 2, 7, 8, 9 };

const int CREATE_SUM = 40;//���镐�푍��
const int WEAPON_SUM = 100;//����̑���
const int MATERIAL_SUM = 10;//�f�ނ̑���

const int LEVEL_MAX = 99;
const int LOVE_MAX = 1000;//�e���x�̍ő�l
const int EVENT_SUM = 5;//�e���C�x���g�̑����i�P�L�����́j10

//�e�G�̔ԍ�
enum {
	FING = 0,
	HANG,
	ACE,
	TEHIRA,
	TAKO,
	KUMO,
	MIZU,
	KUTI,
	UM01,
	UM02,
	UM03,
	TESSIE,
	GU,
	UM01B,
	UM02B,
	UM03B
};

//"�A�J�c�L" �` "�g���`"�܂ł̕�����
extern const char* st_name[];
extern const char* st_kanji_name[];
extern const char* st_full_name[];

//"�N����"�`"�~�Y�E��"�܂ł̕�����
extern const char* other_name[];
extern const char* other_kanji_name[];
extern const char* other_full_name[];

//�G"fing" �` �̕�����
extern const char* en_name[];

//�G�̍ő�HP�i���x��1�̂Ƃ��j
const int en_hp[] = { 70,120,60,100,80,100,70,100,100,100,50,400,600,800,800,1000 };

//�f�ނ̖��O
extern const char* material_name[];

//���q�p�V���[�Y�̕���̖��O
extern const char* start_weapon_name[];

//�_�C�������h�V���[�Y�̕���̖��O
extern const char* rare_weapon_name[];


#define DOT_EXTEND 5//�h�b�g�G�̊g��{��

//�F
const int BLACK = GetColor(0, 0, 0);
const int GRAY = GetColor(100, 100, 100);
const int GRAY2 = GetColor(200, 200, 200);
const int WHITE = GetColor(255, 255, 255);

const int BLUE = GetColor(0, 0, 255);
const int DARK_BLUE = GetColor(0, 0, 150);
const int LIGHT_BLUE = GetColor(200, 200, 255);
const int LIGHT_BLUE2 = GetColor(100, 100, 255);

const int SAFE_COLOR = GetColor(153, 153, 204);

const int RED = GetColor(255, 0, 0);
const int DARK_RED = GetColor(100, 0, 0);
const int LIGHT_RED = GetColor(255, 100, 100);

const int PINK = GetColor(238, 91, 166);

const int GREEN = GetColor(0, 255, 0);
const int DARK_GREEN = GetColor(0, 100, 0);
const int LIGHT_GREEN = GetColor(200, 255, 200);
const int LIGHT_GREEN2 = GetColor(100, 255, 100);

const int YELLOW = GetColor(255, 255, 0);
const int DARK_YELLOW = GetColor(100, 100, 0);
const int LIGHT_YELLOW = GetColor(255, 255, 200);
const int LIGHT_YELLOW2 = GetColor(255, 255, 100);

//�t�H���g �T�C�Y�Ƒ���
//const int FREE_FONT1 = CreateFontToHandle(NULL, 100, 10);

#endif