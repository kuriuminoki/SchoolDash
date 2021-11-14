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


//生徒の立ち絵、ドット絵、走るドット絵をロードする
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
		if (month >= 6 && month <= 10) { name += "夏"; }
		run_handle[i][0] = LoadGraph((name + "走り1.png").c_str());
		run_handle[i][1] = LoadGraph((name + "走り2.png").c_str());
		dot_handle[i] = LoadGraph((name + ".png").c_str());
		name = "picture/stand/";
		name += st_name[i]; name += "/"; name += st_name[i];
		if (month >= 6 && month <= 10) { name += "夏"; }//衣替え
		stand_handle[i] = LoadGraph((name + "通常.png").c_str());
	}
}


////////ゲーム本体//////////////
SchoolDash::SchoolDash() {
	game_load();//ロード
	//初期セーブデータ作成用
	game_save();//セーブ
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
	decide_sound = LoadSoundMem("sound/システム音/決定.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, decide_sound);
	cancel_sound = LoadSoundMem("sound/システム音/キャンセル.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, cancel_sound);
	haikei = LoadGraph("picture/背景1.png");
	stand_handle[0] = -1;
	//大きいフォント
	big_font = CreateFontToHandle(NULL, 80, 10);
	//小さいフォント
	small_font = CreateFontToHandle(NULL, 30, 8);
	//ボタンの用意
	button_font = CreateFontToHandle(NULL, 50, 10);
	button_tansaku = new Button("探索", 95, 300, 300, 300, LIGHT_BLUE, BLUE, button_font, BLACK);
	button_event = new Button("イベント", 490, 300, 300, 300, LIGHT_BLUE, BLUE, button_font, BLACK);
	button_data = new Button("データ", 885, 300, 300, 300, LIGHT_GREEN2, GREEN, button_font, BLACK);
	button_cus = new Button("武器変更", 95, 700, 300, 300, LIGHT_YELLOW, YELLOW, button_font, BLACK);
	button_create = new Button("武器作成", 490, 700, 300, 300, LIGHT_YELLOW, YELLOW, button_font, BLACK);
	button_shop = new Button("ショップ", 885, 700, 300, 300, LIGHT_YELLOW, YELLOW, button_font, BLACK);
	button_option = new Button("オプション", 970, 10, 270, 80, LIGHT_YELLOW2, RED, button_font, BLACK);
}

//効果音の音量を再設定
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

//ゲーム本体のメイン関数
void SchoolDash::play() {
	//各処理へ分岐
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

//イベント発生するなら作成し、trueを返す
bool SchoolDash::create_event() {
	if (!sub_event_flag) {
		sub_event_flag = true;//サブイベント終了
		if (check_event_file("sub", finish_story + 1)) {//サブイベントが存在するなら
			state = GAME_STORY;//サブイベントへ移行
			change_music("none", false, true);//BGMはいったん消す
			te = new TextEvent(TEXT_SUB, finish_story + 1, true, "");
			return true;//サブイベントを実行するからいったんここまで
		}
	}
	if (!class_event) {
		//クラスメイトとのイベント
		//抽選は１日１回
		class_event = true;//クラスメイトのイベント終了
		//4月1日と卒業式はなし
		if (!(month == 4 && week == 1) && !(year == 3 && month == 3 && week == 2)) {
			int r = GetRand(8) + 1;//ハンダからトモチのなかから一人を抽選
			//特定の生徒、時期なら中止
			if (year == 1 && r == 8) {}//イバラカワ転校前
			else if (year == 1 && month < 7 && r == 2) {}//オオタ病み時期
			else if (((year == 1 && month >= 1 && month <= 3) || (year == 2 && month >= 4 && month <= 6)) && r == 6) {}//ミヤトネ病み時期
			else if (year == 3 && month >= 1 && month <= 2 && r == 1) {}//ハンダと喧嘩中
			//クラスイベントの作成
			else if (classmate[0] == classmate[r]) {//同じクラスなら
				state = GAME_STORY;//クラスイベントへ移行
				change_music("none", false, true);//BGMはいったん消す
				te = new TextEvent(TEXT_SUB, decide_sub(r), true, st_name[r]);
				return true;//イベントを作成したなら、いったんここまで
			}
		}
		game_save();//セーブ
	}
	//まだストーリーを観ていないなら
	if (week_num(year, month, week) > finish_story) {
		finish_story++;//ストーリーを１つ見たことになる
		//ストーリー
		//クラスメートの条件確認
		if (check_event_file("story", finish_story)) {
			state = GAME_STORY;
			change_music("none", false, true);//BGMはいったん消す
			te = new TextEvent(TEXT_STORY, finish_story, true);
			story_list[finish_story - 1] = true;//ストーリを観たことがある状態にする
			return true;//ストーリーを観るからいったんここまで
		}
	}
	return false;
}

//最初のメニュー画面
void SchoolDash::play_menu() {
	//////イベント発生（最優先）/////////////////
	if (create_event()) {
		return;//イベントが発生するからメニュー画面は表示しない
	}

	//卒業式のみ例外（探索なし、ＥＤへ）
	if (year == 3 && month == 3 && week == 2) {
		game_init();
		ed = new Ed();
		state = GAME_MOVIE;
	}

	//////////メニュー画面の処理////////////////
	GetMousePoint(&hand_x, &hand_y);
	//左クリックされたとき
	if (left_click() == 1) {
		//探索ボタン
		if (button_tansaku->overlap(hand_x, hand_y)) {
			state = GAME_PRE_DASH;
			sd = new StDecide(stand_handle, st_handle, run);
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
		}
		//イベントボタン
		if (button_event->overlap(hand_x, hand_y)) {
			state = GAME_EVENT;
			int a[9];//アカツキ以外のドット絵
			for (int i = 0; i < 9; i++) { a[i] = st_handle[i + 1]; }
			se = new StEvent(a, stand_handle);
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
		}
		//データボタン
		if (button_data->overlap(hand_x, hand_y)) {
			state = GAME_DATA;
			data = new Data(stand_handle, st_handle);
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
		}
		//武器変更ボタン
		if (button_cus->overlap(hand_x, hand_y)) {
			state = GAME_CUS;
			wc = new WeaponChange(st_handle);
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
		}
		//武器作成ボタン
		if (button_create->overlap(hand_x, hand_y)) {
			state = GAME_CREATE;
			wcre = new WeaponCreate();
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
		}
		//ショップボタン
		if (button_shop->overlap(hand_x, hand_y)) {
			state = GAME_SHOP;
			shop = new Shop;
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
		}
		//オプションボタン
		if (button_option->overlap(hand_x, hand_y)) {
			state = GAME_OPTION;
			option = new Option();
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
		}
	}
	//メニュー画面の音楽
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
		////マウス関連////
		SetMouseDispFlag(TRUE);//マウス表示
		SetDrawMode(DX_DRAWMODE_NEAREST);
		//データ削除
		delete_all();
		//ロードしなおす
		load_all();
		change_music("none", false, true);
		//ロードしなおす
		load_st_graph(stand_handle, st_handle, run);
	}
}

void SchoolDash::draw_menu() {
	if (state == GAME_STORY || state == GAME_MOVIE) { return; }//イベントを視聴中
	GetMousePoint(&hand_x, &hand_y);
	//背景
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
		DrawStringToHandle(10, 100, "学校を探索します。", BLACK, small_font);
		DrawStringToHandle(10, 150, "（日付が進みます）", BLACK, small_font);
	}
	else if (button_event->overlap(hand_x, hand_y)) {
		DrawBox(0, 90, 760, 210, LIGHT_GREEN, TRUE);
		DrawStringToHandle(10, 100, "親密度を上げたキャラとのイベントを視聴します。", BLACK, small_font);
		DrawStringToHandle(10, 150, "（日付が進みます）", BLACK, small_font);
	}
	DrawBox(800, 110, 1280, 170, LIGHT_RED, TRUE);
	DrawStringToHandle(820, 120, "スペースキーで画面表示の切替", BLACK, small_font);
}

//日付を表示
void SchoolDash::draw_date(int draw_x, int draw_y) {
	ostringstream s;
	s << year << "年生 " << month << "月 " << week << "週目 " << "所持金：" << money << "pt";
	DrawBox(draw_x, draw_y, draw_x + 900, draw_y + 70, GRAY2, TRUE);
	//DrawFormatString(draw_x + 10, draw_y + 10, BLACK, s.str().c_str());
	DrawStringToHandle(draw_x + 10, draw_y + 10, s.str().c_str(), BLACK, button_font);
}

//日付を進める データをすべてリセットする
//データのセーブを行う！！
void SchoolDash::next_date() {
	//日図家を進める
	week++;
	if (week == 5) {//次の月へ
		week = 1;//一週目から
		month++;//次の月
		if (month == 13) {//12月の次は１月
			month = 1;
		}
	}
	//長期休暇を飛ばす
	if (month == 8) { month = 9; }//夏休み
	if (month == 12 && week == 4) { month = 1; week = 2; }//冬休み
	if (month == 3 && week == 3) { //春休み
		month = 4; week = 1;//4月1日になる
		year++;//年が変わる
	}
	class_event = false;
	sub_event_flag = false;
	state = GAME_START;
	//セーブ
	game_save();
	//データ削除
	delete_all();
	load_all();
	change_music("none", false, true);
	//衣替えがあったかもしれないからロードしなおす
	load_st_graph(stand_handle, st_handle, run);
}

//ストーリー
void SchoolDash::play_story() {
	//新学期のクラスメイト紹介用 1:ハンダ～9:トモチ
	static int story_loop = 1;
	//ストーリー再生終了後
	if (te->play()) {
		state = GAME_MENU;
		delete te;
		///////クラス替え（４月１週目のみ）//////////
		//&新学期のクラスメイト紹介
		if (class_event && month == 4 && week == 1) {
			//まずは全生徒のクラスの決定
			if (story_loop == 1) {//初めに一回だけクラス抽選
				for (int i = 0; i < 10; i++) {//各生徒
					//新しいクラスの抽選
					classmate[i] = GetRand(3) + 1;
					//classmate[i] = classmate[0];//テスト　全員同じクラス
				}
				//最低でも１人は同じクラスになる
				while (true) {
					int r = GetRand(8) + 1;
					if (year != 1 || r != 8) {//1年ではイバラカワ（転校生）が例外
						classmate[r] = classmate[0];
						break;
					}
				}
			}
			//クラスが決まった後はクラスメイトの紹介を順に行う
			for (story_loop; story_loop < 10; story_loop++) {
				if (year == 1 && story_loop == 8) { continue; }//イバラカワ転校前
				if (classmate[0] == classmate[story_loop]) {//同じクラスなら
					state = GAME_STORY;//ストーリーは続く
					te = new TextEvent(TEXT_OTHER, year, true, st_name[story_loop]);
					story_loop++;
					break;//いったん止めて、紹介の後続きから
				}
			}
		}
	}
	if (state == GAME_MENU) { 
		story_loop = 1;
		game_save();//セーブ
	}//完全終了時
}
void SchoolDash::draw_story() {

}

//日付変更画面
void SchoolDash::play_start() {
	cnt++;
	if (cnt == 150) {//終了
		state = GAME_MENU;
		cnt = 0;
	}
}
void SchoolDash::draw_start() {
	int c = min((int)(cnt*2.0), 255);
	ostringstream s;
	s << year << "年生 " << month << "月 " << week << "週目 ";
	DrawStringToHandle(300, 420, s.str().c_str(), GetColor(c, c, c), big_font);
}

//探索直前
void SchoolDash::play_pre_dash() {
	int sd_result = sd->play();//戻るときは-1 探索へ行くときは1
	if (sd_result == -1) {//戻るボタン
		state = GAME_MENU;
		PlaySoundMem(cancel_sound, DX_PLAYTYPE_BACK);//効果音
	}
	else if (sd_result == 1) {//決定ボタン
		//state = GAME_MENU;//報酬確認終了メモリリークテスト
		//next_date();//日付を進めるメモリリークテストテスト
		state = GAME_DASH;//探索中へ移行
		dash = sd->get_dash();//探索クラスの作成
		PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
	}	
}
void SchoolDash::draw_pre_dash() {
	sd->draw();
	//終了しているなら
	if (state != GAME_PRE_DASH) {
		delete sd;
	}
}

//探索中
void SchoolDash::play_dash() {
	if (dash->play()) {
		state = GAME_END_DASH;//探索終了
	}
}
void SchoolDash::draw_dash() {
	dash->draw();
}

//探索終了
void SchoolDash::play_end_dash() {
	//左クリックされたら
	if (dash->play_reward()) {
		state = GAME_MENU;//報酬確認終了
		PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
		next_date();//日付を進める
	}
}
void SchoolDash::draw_end_dash() {
	if (state == GAME_END_DASH) {
		dash->draw_reward();
	}
	//探索の完全終了
	if (state != GAME_END_DASH) {
		delete dash;
	}
}

//イベント
void SchoolDash::play_event() {
	if (se->play()) {
		state = GAME_MENU;
		if (se->get_flag()) {//テキストイベントを見たうえでの終了なら
			next_date();//日付を進める
		}
		else {
			PlaySoundMem(cancel_sound, DX_PLAYTYPE_BACK);//戻る効果音
		}
	}
}
void SchoolDash::draw_event() {
	se->draw();
	if (state != GAME_EVENT) {
		delete se;
	}
}


//武器変更画面
void SchoolDash::play_cus() {
	if (wc->play()) {
		state = GAME_MENU;
		PlaySoundMem(cancel_sound, DX_PLAYTYPE_BACK);//効果音
	}
}
void SchoolDash::draw_cus() {
	wc->draw();
	if (state != GAME_CUS) {
		delete wc;
	}
}

//武器作成画面
void SchoolDash::play_cre() {
	if (wcre->play()) {
		state = GAME_MENU;
		PlaySoundMem(cancel_sound, DX_PLAYTYPE_BACK);//効果音
	}
}
void SchoolDash::draw_cre() {
	wcre->draw();
	if (state != GAME_CREATE) {
		delete wcre;
	}
}

//ショップ画面
void SchoolDash::play_shop() {
	if (shop->play()) {
		state = GAME_MENU;
		PlaySoundMem(cancel_sound, DX_PLAYTYPE_BACK);//効果音
	}
}
void SchoolDash::draw_shop() {
	shop->draw();
	if (state != GAME_SHOP) {
		delete shop;
	}
}

//オプション画面
void SchoolDash::play_option() {
	if (option->play()) {
		state = GAME_MENU;
		change_menu_sound();
		PlaySoundMem(cancel_sound, DX_PLAYTYPE_BACK);//効果音
	}
}
void SchoolDash::draw_option() {
	option->draw();
	if (state != GAME_OPTION) {
		delete option;
	}
}

//データ画面
void SchoolDash::play_data() {
	if (data->play()) {
		state = GAME_MENU;
		PlaySoundMem(cancel_sound, DX_PLAYTYPE_BACK);//効果音
	}
}
void SchoolDash::draw_data() {
	data->draw();
	if (state != GAME_DATA) {
		delete data;
	}
}

///////////オプション画面////////////
Option::Option() {
	//フォント
	font_handle = CreateFontToHandle(NULL, 50, 10);
	//戻るボタン
	m_button_cancel = new Button("戻る", 30, 20, 150, 80, LIGHT_GREEN, GREEN, font_handle, BLACK);
	//矢印の準備
	m_right_handle = LoadGraph("picture/右矢印.png");
	m_left_handle = LoadGraph("picture/左矢印.png");
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
	//左クリックされたとき
	if (left_click() == 1 || (left_click() > 10 && left_click() % 2 == 1)) {
		//矢印ボタン
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
		//戻るボタン
		if (m_button_cancel->overlap(hand_x, hand_y)) {
			return true;//オプション画面終了
		}
	}
	return false;
}
void Option::draw() {
	//背景
	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, GRAY2, TRUE);
	//戻るボタン
	m_button_cancel->draw(hand_x, hand_y);
	//矢印ボタン
	DrawBox(300, 270, 1050, 370, LIGHT_BLUE, TRUE);
	m_button_right->draw(hand_x, hand_y);
	m_button_left->draw(hand_x, hand_y);
	//現在の音量表示
	ostringstream s;
	s << "現在の音量：" << SoundVolume << "％";
	DrawStringToHandle(470, 300, s.str().c_str(), BLACK, font_handle);
}


//////////データ閲覧画面//////////////////
Data::Data(int stand_handle[], int dot_handle[]) {
	data_state = DATA_MENU;
	//フォント
	font_handle = CreateFontToHandle(NULL, 50, 10);
	font_small_handle = CreateFontToHandle(NULL, 30, 8);
	//効果音
	decide_sound = LoadSoundMem("sound/システム音/決定.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, decide_sound);
	cancel_sound = LoadSoundMem("sound/システム音/キャンセル.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, cancel_sound);
	//戻るボタン
	button_cancel = new Button("戻る", 30, 20, 150, 80, LIGHT_GREEN, GREEN, font_handle, BLACK);
	//生徒閲覧ボタン
	button_st = new Button("アカツキと仲間", 200, 200, 800, 100, LIGHT_YELLOW, YELLOW, font_handle, BLACK);
	//クルメ達閲覧ボタン
	button_other = new Button("出会ったその他の生徒", 200, 350, 800, 100, LIGHT_YELLOW, YELLOW, font_handle, BLACK);
	if (week_num(year, month, week) < week_num(1, 5, 1)) {
		//クルメすら未登場
		button_other->change_flag(false, DARK_YELLOW);
	}
	//イベント閲覧ボタン
	button_event = new Button("過去のイベント閲覧", 200, 500, 800, 100, LIGHT_YELLOW, YELLOW, font_handle, BLACK);
	//各データのオブジェクト
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
	//左クリックされたとき
	if (left_click() == 1) {
		//戻るボタン
		if (button_cancel->overlap(hand_x, hand_y)) {
			if (data_state == DATA_MENU) {
				return true;//データ閲覧画面終了
			}
			else {//データ閲覧のメニュー画面へ戻る
				data_state = DATA_MENU;
				m_st_data->reset();
				m_other_data->reset();
				PlaySoundMem(cancel_sound, DX_PLAYTYPE_BACK);//効果音
			}
		}
	}
	//各データ閲覧中の処理
	switch (data_state) {
	case DATA_MENU://データ閲覧のメニュー画面中
		if (left_click() == 1) {
			//生徒閲覧画面へ
			if (button_st->overlap(hand_x, hand_y)) {
				data_state = DATA_ST;
				PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
			}
			//クルメ達閲覧画面へ
			if (button_other->overlap(hand_x, hand_y)) {
				data_state = DATA_OTHER;
				PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
			}
			//イベント閲覧画面へ
			if (button_event->overlap(hand_x, hand_y)) {
				GetMouseWheelRotVol();//ホイールの回転量リセット
				data_state = DATA_EVENT;
				PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
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
	//背景
	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, LIGHT_BLUE, TRUE);
	switch (data_state) {
	case DATA_MENU:
		//各ボタン
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
	//戻るボタン
	button_cancel->draw(hand_x, hand_y);
}

//テキストファイルからキャラの説明文をロードする関数//
void load_prof(string file_name, int num, vector<string> prof[]) {
	//ファイルを開く
	int file = FileRead_open(file_name.c_str());
	for (int i = 0; i < num; i++) {
		char sentence[256];
		do {
			FileRead_gets(sentence, 256, file);//1行読み込む
			prof[i].push_back(sentence);
		} while (FileRead_eof(file) == 0 && sentence[0] != 0);
	}
	FileRead_close(file);//ファイルを閉じる
}


//////生徒閲覧画面////////////
StData::StData(int stand_handle[], int dot_handle[]) {
	//どの生徒を選んでいるか　初期値は-1
	st_select = -1;
	//生徒の立ち絵
	for (int i = 0; i < 10; i++) {
		handle[i] = stand_handle[i];
	}
	//生徒ボタン
	sb = new StButton(10, dot_handle, 200);
	//フォント
	font_handle = CreateFontToHandle(NULL, 50, 10);
	font_small_handle = CreateFontToHandle(NULL, 30, 8);
	//効果音
	decide_sound = LoadSoundMem("sound/システム音/決定.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, decide_sound);
	//キャラ説明
	load_prof("text/生徒紹介.txt", 10, prof);
	//ボタン
	button_change = new Button("表示切替", 1000, 20, 250, 80, LIGHT_YELLOW, YELLOW, font_handle, BLACK);
	draw_prof = false;
}

StData::~StData() {
	//フォント
	DeleteFontToHandle(font_handle);
	DeleteFontToHandle(font_small_handle);
	//効果音
	DeleteSoundMem(decide_sound);
	//生徒ボタン
	delete sb;
	delete button_change;
}

void StData::play() {
	GetMousePoint(&hand_x, &hand_y);
	//表示ずれを戻していく
	if (dx != 0) {
		dx = (int)(dx * 0.75);
	}
	//左クリックされたとき
	if (left_click() == 1) {
		//表示切替
		if (st_select != -1 && button_change->overlap(hand_x, hand_y)) {
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
			draw_prof = !draw_prof;
		}
		//生徒のボタン
		int n = sb->overlap(hand_x, hand_y);
		if (n != -1) {
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
			dx = START_DX;
			if (st_select != -1) {//もともと選んでいた生徒true
				sb->change_flag(st_select, true);
			}
			st_select = n;//生徒選択
			sb->change_flag(n, false);//選択した生徒false
		}
	}
}

void StData::draw() {
	if (st_select != -1) {//生徒情報閲覧中
		//立ち絵表示
		SetDrawMode(DX_DRAWMODE_BILINEAR);
		DrawRotaGraph2(0 - dx, -50, 0, 0, STAND_EXTEND, 0, handle[st_select], TRUE, FALSE);
		SetDrawMode(DX_DRAWMODE_NEAREST);
		//生徒情報
		DrawBox(800 + dx, 150, GAME_WIDE, GAME_HEIGHT, SAFE_COLOR, TRUE);
		//名前
		DrawBox(810 + dx, 160, GAME_WIDE, 260, WHITE, TRUE);
		DrawStringToHandle(820 + dx, 170, st_full_name[st_select], GRAY, font_small_handle);
		DrawStringToHandle(820 + dx, 200, st_kanji_name[st_select], BLACK, font_handle);
		if (draw_prof) {
			//キャラ紹介文
			for (unsigned int i = 0; i < prof[st_select].size(); i++) {
				DrawStringToHandle(850 + dx, 350 + i * 50, prof[st_select][i].c_str(), BLACK, font_small_handle);
			}
		}
		else {
			//レベル
			ostringstream s;
			s << "Level：" << level[st_select] << " / " << LEVEL_MAX;
			DrawStringToHandle(850 + dx, 300, s.str().c_str(), RED, font_handle);
			ostringstream e;
			e << "経験値：" << exp_sum[st_select] << " / " << need_exp(level[st_select]);
			DrawStringToHandle(870 + dx, 380, e.str().c_str(), LIGHT_RED, font_small_handle);
			//クラス
			ostringstream c;
			c << "クラス：" << year << "年" << classmate[st_select] << "組";
			if (classmate[0] == classmate[st_select]) {
				DrawStringToHandle(850 + dx, 500, c.str().c_str(), YELLOW, font_handle);
			}
			else {
				DrawStringToHandle(850 + dx, 500, c.str().c_str(), DARK_GREEN, font_handle);
			}
			//親密度
			if (st_select != 0) {//アカツキは除外
				DrawStringToHandle(850 + dx, 600, "親密度", PINK, font_handle);
				DrawBox(845 + dx, 675, 1205 + dx, 735, GRAY, TRUE);
				DrawBox(850 + dx, 680, 850 + (350 * love[st_select] / LOVE_MAX) + dx, 730, PINK, TRUE);
				DrawBox(850 + dx + (350 * love[st_select] / LOVE_MAX), 680, 350 + 850 + dx, 730, BLACK, TRUE);
			}
		}	
	}
	//下の生徒ボタン
	sb->draw(hand_x, hand_y);
	//表示切替ボタン
	if (st_select != -1) {
		button_change->draw(hand_x, hand_y);
	}
}

void StData::reset() {
	if (st_select != -1) {//もともと選んでいた生徒true
		sb->change_flag(st_select, true);
	}
	st_select = -1;
	draw_prof = false;
}

//その他の生徒閲覧画面
OtherData::OtherData() {
	string name;
	for (int i = 0; i < 6; i++) {
		name = "picture/stand/";
		name += other_name[i]; name += "/"; name += other_name[i];
		if (month >= 6 && month <= 10) { name += "夏"; }//衣替え
		handle[i] = LoadGraph((name + "通常.png").c_str());
		//handle[i] = LoadGraph("picture/stand/クルメ/クルメ通常.png");
	}
	st_select = 0;
	dx = START_DX;
	//フォント
	font_handle = CreateFontToHandle(NULL, 50, 10);
	font_small_handle = CreateFontToHandle(NULL, 30, 8);
	//効果音
	decide_sound = LoadSoundMem("sound/システム音/決定.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, decide_sound);
	//ボタン
	button_next = new Button("次へ", 1100, 20, 150, 80, LIGHT_YELLOW, YELLOW, font_handle, BLACK);
	button_prev = new Button("前へ", 900, 20, 150, 80, LIGHT_YELLOW, YELLOW, font_handle, BLACK);
	button_prev->change_flag(false, DARK_YELLOW);
	if (st_select == 0 && week_num(year, month, week) < week_num(1, 7, 1)) {
		//マクタ未登場
		button_next->change_flag(false, DARK_YELLOW);
	}
	
	//キャラ説明
	load_prof("text/その他紹介.txt", 6, prof);
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
	//左クリックされたとき
	if (left_click() == 1) {
		//次へボタン
		if (button_next->overlap(hand_x, hand_y)) {
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
			st_select++;
			dx = START_DX;
			button_prev->change_flag(true, LIGHT_YELLOW);
		}
		//前へボタン
		if (button_prev->overlap(hand_x, hand_y)) {
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
			st_select--;
			dx = START_DX;
			button_next->change_flag(true, LIGHT_YELLOW);
		}
		//次へボタン、前へボタンの無効化のチェック
		if (st_select == 0) {
			//左端まで来た
			button_prev->change_flag(false, DARK_YELLOW);
		}
		if (st_select == 0 && week_num(year, month, week) < week_num(1, 7, 1)) {
			//マクタ未登場
			button_next->change_flag(false, DARK_YELLOW);
		}
		else if (st_select == 1 && week_num(year, month, week) < week_num(1, 9, 3)) {
			//シャシキ未登場
			button_next->change_flag(false, DARK_YELLOW);
		}
		else if (st_select == 2 && week_num(year, month, week) < week_num(1, 11, 4)) {
			//サイカ未登場
			button_next->change_flag(false, DARK_YELLOW);
		}
		else if (st_select == 3 && week_num(year, month, week) < week_num(2, 4, 3)) {
			//サキガケ未登場
			button_next->change_flag(false, DARK_YELLOW);
		}
		else if (st_select == 4 && week_num(year, month, week) < week_num(2, 5, 2)) {
			//ミズウラ未登場
			button_next->change_flag(false, DARK_YELLOW);
		}
		else if (st_select == 5) {
			//右端まで来た
			button_next->change_flag(false, DARK_YELLOW);
		}
	}
}

void OtherData::draw() {
	//ボタン
	button_next->draw(hand_x, hand_y);
	button_prev->draw(hand_x, hand_y);
	//表示ずれを戻していく
	if (dx != 0) {
		dx = (int)(dx * 0.75);
	}
	//立ち絵表示
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	DrawRotaGraph2(0 - dx, -50, 0, 0, STAND_EXTEND, 0, handle[st_select], TRUE, FALSE);
	SetDrawMode(DX_DRAWMODE_NEAREST);
	//生徒情報
	DrawBox(800 + dx, 150, GAME_WIDE, GAME_HEIGHT, SAFE_COLOR, TRUE);
	//名前
	DrawBox(810 + dx, 160, GAME_WIDE, 260, WHITE, TRUE);
	DrawStringToHandle(820 + dx, 170, other_full_name[st_select], GRAY, font_small_handle);
	DrawStringToHandle(820 + dx, 200, other_kanji_name[st_select], BLACK, font_handle);
	//キャラ紹介
	for (unsigned int i = 0; i < prof[st_select].size(); i++) {
		DrawStringToHandle(850 + dx, 400 + i * 50, prof[st_select][i].c_str(), BLACK, font_small_handle);
	}
}

void OtherData::reset() {
	st_select = 0;
	button_next->change_flag(true, LIGHT_YELLOW);
	if (st_select == 0 && week_num(year, month, week) < week_num(1, 7, 1)) {
		//マクタ未登場
		button_next->change_flag(false, DARK_YELLOW);
	}
	button_prev->change_flag(false, DARK_YELLOW);
	dx = START_DX;
}

EventData::EventData() {
	m_now = 1;
	te_now = false;
	//フォント
	font_handle = CreateFontToHandle(NULL, 50, 10);
	font_small_handle = CreateFontToHandle(NULL, 30, 8);
	//効果音
	decide_sound = LoadSoundMem("sound/システム音/決定.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, decide_sound);
	//矢印の画像ハンドル
	up_handle = LoadGraph("picture/上矢印.png");
	down_handle = LoadGraph("picture/下矢印.png");
	//矢印ボタン
	button_up = new Button("", 200, 150, 800, 70, GRAY2, YELLOW, font_handle, BLACK);
	button_up->change_flag(false, GRAY);
	button_down = new Button("", 200, 850, 800, 70, GRAY2, YELLOW, font_handle, BLACK);
	button_up->set_graph(up_handle, 3);
	button_down->set_graph(down_handle, 3);
	//イベントの名前を取得
	int file;
	char buff[256];
	file = FileRead_open("text/story.csv");
	for (int i = 0; i < 120; i++) {
		FileRead_gets(buff, 256, file);//最後まで見ていく
		for (int j = 0; buff[j] != '\0'; j++) {
			if (buff[j] == ',') {
				break;
			}
			else {
				story_name[i] += buff[j];
			}
		}
	}
	FileRead_close(file);//ファイルクローズ
	//ストーリーボタン
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

//ストーリーボタンを更新
void EventData::reflesh() {
	for (int i = 0; i < DRAW_TEXT_BUTTON; i++) {
		ostringstream s;
		int y, m, w;
		get_date(i + m_now, y, m, w);
		s << y << "年生" << m << "月" << w << "週目：";
		if (!story_list[i + m_now - 1]) {//見たことない奴は見れない
			button_story[i]->change_flag(false, DARK_YELLOW);
			s << "？？？";
		}
		else {
			button_story[i]->change_flag(true, LIGHT_YELLOW);
			s << story_name[i + m_now - 1];
		}
		button_story[i]->set_string(s.str().c_str());
	}
}

bool EventData::play() {
	if (te_now) {//イベント視聴中
		return true;
	}
	GetMousePoint(&hand_x, &hand_y);

	//マウスのホイール回転
	int rot = GetMouseWheelRotVol();
	if (rot > 0 && button_up->get_flag()) {//上へ
		m_now--;
		reflesh();
		button_down->change_flag(true, GRAY2);
	}//下へ
	else if (rot < 0 && button_down->get_flag()) {
		m_now++;
		reflesh();
		button_up->change_flag(true, GRAY2);
	}//クリックでも上下できる
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
	//左クリックがされたとき
	if (left_click() == 1) {
		//ストーリーを観る
		for (int i = 0; i < DRAW_TEXT_BUTTON; i++) {
			if (button_story[i]->overlap(hand_x, hand_y)) {
				PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
				change_music("none", false, true);//BGMはいったん消す
				te = new TextEvent(TEXT_STORY, m_now + i, true);
				te_now = true;
			}
		}
	}
	//矢印ボタンの状態更新
	if (m_now == 1) {
		button_up->change_flag(false, GRAY);
	}
	if (m_now == 120 - DRAW_TEXT_BUTTON + 1) {
		button_down->change_flag(false, GRAY);
	}
	return false;
}

bool EventData::draw() {
	if (te_now) {//イベント視聴中
		if (te->play()) {
			te_now = false;
			delete te;
			//メニュー画面の音楽に戻しておく
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