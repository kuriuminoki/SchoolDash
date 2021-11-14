#include"Button.h"
#include"SDvalue.h"
#include"Control.h"
#include"Weapon.h"
#include"Dash.h"
#include"DxLib.h"
#include<string>
#include<sstream>

using namespace std;

/////ボタン/////////////
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

//ボタンの描画 下にラベルの文字列も表示できる
void Button::draw(int hand_x, int hand_y) {
	if (overlap(hand_x, hand_y)) {
		DrawBox(m_x - 5, m_y - 5, m_x + m_wide + 5, m_y + m_height + 5, m_color2, TRUE);
	}
	DrawBox(m_x, m_y, m_x + m_wide, m_y + m_height, m_color, TRUE);
	//絵の描画
	if (!m_flag) { SetDrawBright(100, 100, 100); }
	DrawRotaGraph(m_x + (m_wide / 2), m_y + (m_height / 2), m_graph_ex, 0, m_graph_handle, TRUE, FALSE);
	SetDrawBright(255,255,255);
	//文字の描画
	DrawStringToHandle(m_dx, m_dy, m_tag.c_str(), m_font_color, m_font);
	//DrawFormatString(m_x, m_y - 20, WHITE, "font_size = %d", m_font_size);
	//DrawFormatString(m_x, m_y - 40, WHITE, "string_size = %d", m_string_size);
}

//ボタンのオン・オフ
void Button::change_flag(bool f, int new_color) {
	m_flag = f;
	m_color = new_color;
}

//フラグを取得
bool Button::get_flag() {
	return m_flag;
}

//ボタンがマウスカーソルと重なっているか
bool Button::overlap(int hand_x, int hand_y) {
	if (!m_flag) {
		return false;
	}
	if (hand_x >= m_x && hand_x <= m_x + m_wide && hand_y >= m_y && hand_y <= m_y + m_height) {
		return true;
	}
	return false;
}

//ボタン内に文字ではなく絵を表示するようにセット
void Button::set_graph(int handle, int ex) {
	m_graph_handle = handle;
	m_graph_ex = ex;
	m_tag = "";
}

//タグをつけなおす
void Button::set_string(std::string tag) {
	m_tag = tag;
	//文字列が取る幅を文字の大きさと文字列の長さから計算
	int m_font_size;//一文字の大きさ
	int m_string_size;//文字列がとる幅
	GetFontStateToHandle(NULL, &m_font_size, NULL, m_font);
	m_string_size = m_font_size * (m_tag.size() / 2);
	m_dx = m_x + (m_wide - m_string_size) / 2 - (m_tag.size()/2);
	m_dy = m_y + (m_height - m_font_size) / 2;
}

//画像を取得
int Button::get_handle() {
	return m_graph_handle;
}

//複数の生徒のボタンを管理する
StButton::StButton(int size, int handle[], int height) {
	m_size = size;
	int distance = 10;//四角の間の幅
	int edge = 50;//両端の空幅
	int wide = (GAME_WIDE - (edge * 2)) / size;//四角の横幅
	int y = GAME_HEIGHT - height - edge;//y座標
	st = new Button*[size];
	for (int i = 0; i < size; i++) {
		st[i] = new Button("", edge + (wide * i) + distance, y, wide - distance, height, LIGHT_YELLOW, YELLOW, -1, BLACK);
		st[i]->set_graph(handle[i], DOT_EXTEND);
	}
	st_absent();//日付を見て、まだ仲間になっていない生徒を決定
}

StButton::~StButton() {
	for (int i = 0; i < m_size; i++) {
		delete st[i];
	}
	delete[] st;
}

//日付を見て、まだ仲間になっていない生徒や離脱中の生徒を決定
void StButton::st_absent() {
	int index = 0;//インデックス指定用
	if (m_size != 10) { index = 1; }//アカツキはいない
	//ハンダと喧嘩中
	if (year == 3 && (month == 1 || (month == 2 && week <= 3))) {
		st[1 - index]->set_graph(-1, 1);
		st[1 - index]->change_flag(false, BLACK);
	}
	//オオタがハンダとと喧嘩中
	if ((year == 2 && month == 7 && week == 4) || (year == 2 && month == 9 && week <= 3)) {
		st[2 - index]->set_graph(-1, 1);
		st[2 - index]->change_flag(false, BLACK);
	}
	//オオタからトモチまでの８人
	for (int i = 0; i < 8; i++) {
		if (week_num(year, month, week) < week_num(join_year[i], join_month[i], join_week[i])) {
			//アカツキ、ハンダの２人を足して、アカツキがいないならやっぱ１引く
			st[i + 2 - index]->set_graph(-1, 1);
			st[i + 2 - index]->change_flag(false, BLACK);
		}
	}
}

//n番目の画像ハンドルを取得
int StButton::get_handle(int num) {
	return st[num]->get_handle();
}

//n番目のフラグを取得
bool StButton::get_flag(int num) {
	return st[num]->get_flag();
}

//num番目の生徒のフラグを変更
void StButton::change_flag(int num, bool f) {
	if (num < 0 || num >= m_size) { return; }
	int c;
	if (f) { c = LIGHT_YELLOW; }
	else { c = DARK_YELLOW; }
	st[num]->change_flag(f, c);
}

//画像ハンドルを変更（仲間じゃない生徒を-1にする用）
void StButton::change_pic(int num, int handle, int ex) {
	if (num < 0 || num >= m_size) { return; }
	st[num]->set_graph(handle, ex);
}

//マウスカーソルが重なっているなら ０～
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


//////探索前の準備画面用クラス///////////
//探索に行く生徒の選択・ウェーブ数指定
StDecide::StDecide(int stand_handle[], int dot_handle[], int run_handle[][2]) {
	dx[0] = START_DX;
	dx[1] = 0; dx[2] = 0;
	font = CreateFontToHandle(NULL, 50, 10);
	font_small_handle = CreateFontToHandle(NULL, 30, 8);
	m_start_wave = max_wave;
	//生徒の画像
	m_run_cnt = 0;
	m_st_num[0] = 0;//一人目はアカツキで確定
	m_st_num[1] = -1;
	m_st_num[2] = -1;
	for (int i = 0; i < 10; i++) {
		m_stand_handle[i] = stand_handle[i];
		m_dot_handle[i] = dot_handle[i];
		m_run_handle[i][0] = run_handle[i][0];
		m_run_handle[i][1] = run_handle[i][1];
	}
	//ボタンの準備
	int distance = 50;//四角の間の幅
	int edge = 50;//両端の空幅 もと150
	int wide = 205;//四角の横幅
	int y = 500;//y座標
	//探索へ行く3人のボタン
	for (int i = 0; i < 3; i++) {
		m_st_button[i] = new Button("", edge + (wide * i) + distance, y, wide - distance, 250, LIGHT_BLUE2, BLUE, font_small_handle, BLACK);
	}
	button_decide = new Button("決定", 1100, 20, 150, 80, LIGHT_BLUE, BLUE, font, BLACK);
	button_cancel = new Button("戻る", 30, 20, 150, 80, LIGHT_GREEN, GREEN, font, BLACK);
	//生徒が装備中の武器
	for (int i = 0; i < 10; i++) {
		m_wi[i] = new WeaponInfo(have_weapon[i]);
	}
	//矢印の準備
	m_right_handle = LoadGraph("picture/右矢印.png");
	m_left_handle = LoadGraph("picture/左矢印.png");
	m_button_right = new Button("", 800, 20, 50, 50, LIGHT_YELLOW, YELLOW, font, BLACK);
	m_button_left = new Button("", 400, 20, 50, 50, LIGHT_YELLOW, YELLOW, font, BLACK);
	m_button_left->change_flag(false, DARK_YELLOW);
	m_button_right->set_graph(m_right_handle, 2);
	m_button_left->set_graph(m_left_handle, 2);
	//生徒９人のボタン
	int a[9];
	for (int i = 0; i < 9; i++) { a[i] = dot_handle[i + 1]; }
	m_button_st = new StButton(9, a, 200);
	//効果音
	decide_sound = LoadSoundMem("sound/システム音/決定.wav");
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

//生徒をセットする
int StDecide::set_st(int new_num) {
	int res;
	int num = 1;//左から詰めていく(２人目から)
	if (m_st_num[1] != -1) { num = 2; }//２人目も埋まっている
	res = m_st_num[num];
	m_st_num[num] = new_num;
	dx[num] = START_DX * num;
	return res;
}

//生徒を外す
int StDecide::cancel_st() {
	if (left_click() == 1) {//左クリックされたとき
		for (int i = 1; i < 3; i++) {
			//カーソルが重なっているボタン
			if (m_st_button[i]->overlap(hand_x, hand_y)) {
				PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
				int res = m_st_num[i];
				if (res == -1) { break; }//既に空白
				m_st_num[i] = -1;//外す
				if (i == 1) {//左に詰める
					m_st_num[1] = m_st_num[2];
					dx[1] = -1 * START_DX;
					m_st_num[2] = -1;//外す1;
				}
				return res;//外したキャラ番号を返す
			}
		}
	}
	return -1;
}

//探索クラスを作成する
Dash* StDecide::get_dash() {;
	int d_weapon[3];
	int d_item[3];
	int d_level[3];
	for (int i = 0; i < 3; i++) {
		d_weapon[i] = have_weapon[m_st_num[i]];
		//味方が持つ回復薬の個数
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
		//戻るボタン
		if (button_cancel->overlap(hand_x, hand_y)) {
			return -1;
		}
		//決定ボタン
		if (button_decide->overlap(hand_x, hand_y)) {
			return 1;
		}
	}
	m_run_cnt++;//ドットの走るアニメ用
	//ウェーブ指定の矢印の上限下限
	if (m_start_wave == max_wave) { m_button_right->change_flag(false, DARK_YELLOW); }
	else { m_button_right->change_flag(true, LIGHT_YELLOW); }
	if (m_start_wave == 0) { m_button_left->change_flag(false, DARK_YELLOW); }
	else { m_button_left->change_flag(true, LIGHT_YELLOW); }
	//ウェーブ数変更
	if (left_click() == 1 || (left_click() > 10 && left_click() % 2 == 1)) {
		if (m_button_right->overlap(hand_x, hand_y)) {
			m_start_wave++;
		}
		if (m_button_left->overlap(hand_x, hand_y)) {
			m_start_wave--;
		}
	}
	//生徒を入れる・入れ替える
	int n;
	if (left_click() == 1) {
		//キャラを入れる
		n = m_button_st->overlap(hand_x, hand_y);//nは選択中のキャラ番号-1
		if (n != -1) {//生徒を選択した
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
			int x = set_st(n + 1);//生徒をセット xは外れたキャラ番号
			//入れ替わりで外れた生徒のボタン
			if (x > 0) { m_button_st->change_flag(x - 1, true); }
			//選択された生徒のボタン
			m_button_st->change_flag(n, false);
		}
	}
	//生徒を外す
	n = cancel_st();
	if (n > 0) {
		m_button_st->change_flag(n - 1, true);
	}
	return 0;
}

void StDecide::draw() {
	//背景
	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, GRAY2, TRUE);
	//選択済みキャラの立ち絵
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	for (int i = 0; i < 3; i++) {
		if (m_st_num[i] == -1) {//キャラ未選択
			break;
		}
		DrawRotaGraph2(-150 + (i * 400) - dx[i], -50, 0, 0, STAND_EXTEND, 0, m_stand_handle[m_st_num[i]], TRUE, FALSE);	
	}
	SetDrawMode(DX_DRAWMODE_NEAREST);
	//選択済みキャラのドット絵と武器詳細
	for (int i = 0; i < 3; i++) {
		if (m_st_num[i] != -1) {//キャラ選択済み
			m_st_button[i]->set_graph(m_run_handle[m_st_num[i]][((m_run_cnt + 3) / 4) % 2], DOT_EXTEND);
			m_st_button[i]->draw(hand_x, hand_y);
			m_st_button[i]->draw(hand_x, hand_y);
			//生徒の名前
			DrawStringToHandle(100 + 205 * i, 505, st_name[m_st_num[i]], BLACK, font_small_handle);
		}
		else {//キャラ未選択
			m_st_button[i]->set_graph(-1, DOT_EXTEND);
			m_st_button[i]->set_string("未選択");
			m_st_button[i]->draw(hand_x, hand_y);
		}
		//生徒が装備中の武器の詳細
		if (m_st_num[i] != -1 && m_st_button[i]->overlap(hand_x, hand_y)) {
			m_wi[m_st_num[i]]->draw(700, 450, LIGHT_BLUE, font_small_handle, DARK_RED);
		}
	}
	int n = m_button_st->overlap(hand_x, hand_y);
	if (n >= 0) {
		m_wi[n + 1]->draw(700, 450, LIGHT_BLUE, font_small_handle, DARK_RED);
	}
	//ウェーブ数指定
	DrawBox(420, 20, 820, 70, LIGHT_BLUE, TRUE);
	ostringstream w;
	w << "Wave" << m_start_wave+1 << "からスタート";
	DrawStringToHandle(480, 30, w.str().c_str(), BLACK, font_small_handle);
	m_button_right->draw(hand_x, hand_y);
	m_button_left->draw(hand_x, hand_y);
	//生徒９人
	m_button_st->draw(hand_x, hand_y);
	//生徒のレベル
	for (int i = 0; i < 9; i++) {
		if (m_button_st->get_handle(i) == -1) {
			continue;
		}
		DrawBox(70 + (i * 131), 983, 171 + (i * 131), 1015, LIGHT_RED, TRUE);
		ostringstream w;
		w << "Lv " << level[i+1];
		DrawStringToHandle(80 + (i*131), 985, w.str().c_str(), BLACK, font_small_handle);
	}
	//決定ボタン・戻るボタン
	button_decide->draw(hand_x, hand_y);
	button_cancel->draw(hand_x, hand_y);
}