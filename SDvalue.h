#ifndef SDVALUE_H_INCLUDED
#define SDVALUE_H_INCLUDED

//本ゲームで使用する定数

#include"DxLib.h"
#include<string>

extern int loading_haikei;

////セーブするデータ//////////
extern int SoundVolume;//ゲームの音量
extern int weapon_list[100];//各武器の所持数を保存
extern int material[10];//各素材の所持数を保存
extern int year;//今何年生
extern int month;//今何月
extern int week;//今月の何周目
extern int level[10];//各生徒のレベル
extern int exp_sum[10];//各生徒の経験値
extern int love[10];//各生徒の親密度（アカツキは空白）
extern int item_num;//回復薬の所持数
extern int money;//所持金
extern int have_weapon[10];//各生徒の所持武器
extern int classmate[10];//各生徒がどこのクラスか
extern int max_wave;//最高で何waveまで行ったか
extern int finish_event[10];//見終わった親密度イベントの数
extern int finish_story;//見終わったストーリーの数(見逃したものも含む)
extern bool class_event;//今日、クラスメイトとのイベントをやったかどうか（抽選の結果関係なく）
extern bool sub_event_flag;//今日、サブイベントをやったかどうか（存在するかは関係なく）
extern bool story_list[120];	//各メインストーリを観たことがあるか
//////////////////////////////

//セーブとロード
bool game_save();
bool game_load();

//ループ時の初期化
void game_init();

//音楽
extern int music;
extern std::string music_name;
void change_music(const char* name, bool high, bool loop);
void change_load_music(int handle, bool high, bool loop);
void play_music();
int check_music();
void change_volume();

//次のレベルアップに必要な累計経験値
int need_exp(int now_level);
//生徒番号numの経験値が必要な経験値分あればレベルアップさせる
void level_up(int num);

//年月週を引数とし、入学から何週目かを返す
int week_num(int m_year, int m_month, int m_week);

//入学から何周目かを入力とし、日付を得る
void get_date(int week, int& m_year, int& m_month, int& m_week);

//画面の大きさ
#define GAME_WIDE 1280
#define GAME_HEIGHT 1024

#define BATTLE_UP 265
#define BATTLE_DOWN 645

const double STAND_EXTEND = 0.65;

//左右
#define LEFT 0
#define RIGHT 1

//各生徒の番号
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

//オオタからトモチまでの、仲間になる時期
const int join_year[] = { 1, 1, 1, 1, 2, 1, 2, 2};
const int join_month[] = { 6, 10, 10, 4, 6, 4, 5, 4};
const int join_week[] = { 4, 1, 1, 2, 4, 4, 4, 2};

//敵が落とす素材の配列上でのインデックス
const int drop_item[] = { 2, 2, 2, 2, 3, 4, 5, 6, 7, 8, 9, 2, 2, 7, 8, 9 };

const int CREATE_SUM = 40;//作れる武器総数
const int WEAPON_SUM = 100;//武器の総数
const int MATERIAL_SUM = 10;//素材の総数

const int LEVEL_MAX = 99;
const int LOVE_MAX = 1000;//親密度の最大値
const int EVENT_SUM = 5;//親密イベントの総数（１キャラの）10

//各敵の番号
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

//"アカツキ" ～ "トモチ"までの文字列
extern const char* st_name[];
extern const char* st_kanji_name[];
extern const char* st_full_name[];

//"クルメ"～"ミズウラ"までの文字列
extern const char* other_name[];
extern const char* other_kanji_name[];
extern const char* other_full_name[];

//敵"fing" ～ の文字列
extern const char* en_name[];

//敵の最大HP（レベル1のとき）
const int en_hp[] = { 70,120,60,100,80,100,70,100,100,100,50,400,600,800,800,1000 };

//素材の名前
extern const char* material_name[];

//自衛用シリーズの武器の名前
extern const char* start_weapon_name[];

//ダイヤモンドシリーズの武器の名前
extern const char* rare_weapon_name[];


#define DOT_EXTEND 5//ドット絵の拡大倍率

//色
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

//フォント サイズと太さ
//const int FREE_FONT1 = CreateFontToHandle(NULL, 100, 10);

#endif