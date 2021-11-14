#include"DxLib.h"
#include"SDvalue.h"
#include<string>

using namespace std;

//テスト用に使う
int week_num(int m_year, int m_month, int m_week);

int loading_haikei;

//本ゲームで使用する定数

////セーブするデータ//////////
int SoundVolume = 50;	//ゲームの音量(デフォルトは５０)
int weapon_list[100];	//各武器の所持数を保存
int material[10];		//各素材の所持数を保存
int year = 1;			//今何年生
int month = 4;			//今何月
int week = 1;			//今月の何周目
int level[10];			//各生徒のレベル
int exp_sum[10];		//各生徒の経験値
int love[10];			//各生徒の親密度（アカツキは空白）
int item_num = 0;		//回復薬の所持数
int money = 0;			//所持金
int have_weapon[10];	//各生徒の装備中武器 武器の番号 1～
int classmate[10];		//各生徒がどこのクラスか
int max_wave = 0;		//最高で何waveまで行ったか
int finish_event[10];	//見終わった親密度イベントの数0-5
int finish_story;		//見終わったストーリーの数(見逃したものも含む)
bool class_event;		//今日、クラスメイトとのイベントをやったかどうか（抽選の結果関係なく）
bool sub_event_flag;	//今日、サブイベントをやったかどうか（存在するかは関係なく）
bool story_list[120];	//各メインストーリを観たことがあるか
////////////////////////////

//セーブ
bool game_save() {
	FILE *fp;
	if (fopen_s(&fp, "セーブデータ/savedata.dat", "wb") != 0) {
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

//ロード
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
	////最低でも1人はクラスメイトになる
	//while (true) {
	//	int r = GetRand(8) + 1;
	//	if (r != 8) {//イバラカワは例外
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
	if (fopen_s(&fp, "セーブデータ/savedata.dat", "rb") != 0) {
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

//卒業後、またループできるよう初期化
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
	//各生徒
	for (int i = 0; i < 10; i++) {
		finish_event[i] = 0;
		level[i] = 1;
		exp_sum[i] = 0;
		love[i] = 0;
		have_weapon[i] = i + 1;
	}
	//素材
	for (int i = 0; i < MATERIAL_SUM; i++) {
		material[i] = 0;
	}
	//武器
	for (int i = 0; i < 100; i++) {
		if (i % 10 >= 0 && i % 10 <= 9) {
			weapon_list[i] = 0;
		}
	}
}

int music = -1;
bool loop_flag = true;//ループ再生中ならtrue
int music_length = 0;//曲の長さ
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
	if (high) {//音を高くする
		SetFrequencySoundMem(60000, music);
	}
	if (loop_flag) {
		PlaySoundMem(music, DX_PLAYTYPE_LOOP);
	}
	else {
		PlaySoundMem(music, DX_PLAYTYPE_BACK);
	}
}
//あらかじめロードした音楽に変更
void change_load_music(int handle, bool high, bool loop) {
	if (music != -1) {
		DeleteSoundMem(music);
		music_name = "";
	}
	music = handle;
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, music);
	if (high) {//音を高くする
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

//音楽を再生する
void play_music() {
	if (!CheckSoundMem(music) && loop_flag) {//音楽
		//SetLoopStartTimePosSoundMem(music_length - 2200, music);
		PlaySoundMem(music, DX_PLAYTYPE_LOOP);
	}
}

//音楽が再生中かどうか
int check_music() {
	return CheckSoundMem(music);
}

//音量を変更する
void change_volume() {
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, music);
}

//次のレベルアップに必要な経験値（累計）
int need_exp(int now_level) {
	return ((1 + now_level) * now_level / 2) * 10;
}

//生徒番号numの経験値が必要な経験値分あればレベルアップさせる
void level_up(int num) {
	while (true) {
		if (level[num] == LEVEL_MAX) {//レベルは既に最大
			exp_sum[num] = need_exp(LEVEL_MAX);
			break;
		}
		if (exp_sum[num] >= need_exp(level[num])) {//１レベルアップ
			level[num]++;
		}
		else {//経験値が足りない
			break;
		}
	}
}

//年月週を引数とし、入学から何週目かを返す
int week_num(int m_year, int m_month, int m_week) {
	//m_year 入学から何年たったか
	//m_month 4月から何か月たったか
	//m_week 今月初めから何週目か
	m_year--;
	m_month -= 4;//4月入学
	if (m_month < 0) {//1-3月の場合
		m_month = 12 + m_month;
	}
	int res = m_year * 12 * 4 + m_month * 4 + m_week;
	res -= m_year * 8;//1年で休みの週は8
	if (m_month > 8 - 4) {
		res -= 4;//今年の夏休みは過ぎた
	}
	if (m_month > 12 + 1 - 4 || (m_month > 12 - 4 && m_week > 1)) {
		res -= 2;//今年の冬休みは過ぎた
	}
	return res;
}

//入学から何周目かを入力とし、日付を得る
void get_date(int sum_week, int& m_year, int& m_month, int& m_week) {
	if (sum_week % 40 == 0) {//学年最終日はコーナーケース
		m_year = sum_week / 40;
		m_month = 3;
		m_week = 2;
		return;
	}
	m_year = sum_week / 40 + 1;//まずは学年を確定
	sum_week %= 40;
	//次に月日を確定
	for (int m = 1; m <= 12; m++) {
		int now = m + 3;//4月スタートに調整
		int weeks;//今月は何週間ある？
		if (now == 8) { continue; }//夏休み
		if (now == 12) { weeks = 3;}//冬休み
		else if (now == 13) { weeks = 3; }//冬休み
		else if (now == 15) { weeks = 2; }//春休み
		else { weeks = 4; }//通常
		if (sum_week - weeks <= 0) {
			m_month = now;
			m_week = sum_week;
			break;
		}
		sum_week -= weeks;
	}
	if (m_month == 13) {//1月だけは2週目からスタート
		m_week++;
	}
	if (m_month >= 13) {//13月は1月
		m_month -= 12;
	}
}

const char* st_name[] = { "アカツキ", "ハンダ",
							"オオタ", "イシ", "タイラ",
							"オワダ", "ミヤトネ", "ウチモト",
							"イバラカワ", "トモチ" };

const char* st_kanji_name[] = { "赤月　亮也", "半田　裕斗",
								"太田　勝矢", "伊志　賢介", "台羅　功紀",
								"小和田　杏子", "宮利根　桃花", "内本　春香",
								"茨川　天子", "砥用　須恵" };

const char* st_full_name[] = { "アカツキ　リョウヤ", "ハンダ　ユウト",
								"オオタ　カツヤ", "イシ　ケンスケ", "タイラ　コウキ",
								"オワダ　アンコ", "ミヤトネ　モモカ", "ウチモト　ハルカ",
								"イバラカワ　テンコ", "トモチ　スエ" };

const char* other_name[] = { "クルメ", "マクタ", "シャシキ", "サイカ", "サキガケ", "ミズウラ"};

const char* other_kanji_name[] = { "来芽　輝子", "間九多　衣鶴",
									"斜式　啓太郎", "彩加　個乃実",
									"先崖　徹", "水浦　華菜"};

const char* other_full_name[] = { "クルメ　キコ", "マクタ　イヅル",
									"シャシキ　ケイタロウ", "サイカ　コノミ",
									"サキガケ　トオル", "ミズウラ　カナ"};

const char* en_name[] = { "fing", "hang", "ace", "tehira", "tako",
 "kumo", "mizu", "kuti", "um01", "um02", "um03", "tessie", "gu", "um01b", "um02b", "um03b" };


const char* material_name[] = { "高級木材", "硬い石", "黒い物体", 
								"きれいな石", "強力な糸", "謎の液体",
								"頑丈な物体", "赤い物体", "神秘の粉", 
								"地球の落とし物"};

const char* start_weapon_name[] = { "自衛用ツインソード", "自衛用マシンガン",
								"自衛用ビッグソード", "自衛用サンダー",
								"自衛用スナイパー", "自衛用ソード",
								"自衛用シールド・ガン", "自衛用クロ―",
								"自衛用ロングソード", "自衛用サイズ" };

const char* rare_weapon_name[] = { "ダイヤモンドツインソード", "ダイヤモンドマシンガン",
									"ダイヤモンドビッグソード", "ダイヤモンドサンダー",
									"ダイヤモンドスナイパー", "ダイヤモンドソード",
									"ダイヤモンドシールド・ガン", "ダイヤモンドクロ―",
									"ダイヤモンドロングソード", "ダイヤモンドサイズ" };