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

//イベントの名前を返す
string get_event_name(string kind, int num) {
	//csvファイルからデータを入手
	int fp;
	char buff[256];
	string pass;
	pass = "text/" + kind + ".csv";
	fp = FileRead_open(pass.c_str());
	//ファイルポインタを移動
	for (int i = 1; i <= num; i++) {
		FileRead_gets(buff, 256, fp);//最後以外読み飛ばし
	}
	FileRead_close(fp);
	//情報をテキストで入手
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

//csvファイル：[イベント名, 年, 月, 週, 同じクラスに必要な生徒]
//csvファイルからイベントの発生条件確認、見れるならtrue
bool check_event_file(string kind, int num) {
	//csvファイルからデータを入手
	int fp;
	char buff[256];
	string pass;
	pass = "text/" + kind + ".csv";
	fp = FileRead_open(pass.c_str());
	//ファイルポインタを移動
	for (int i = 1; i <= num; i++) {
		FileRead_gets(buff, 256, fp);//最後以外読み飛ばし
	}
	//情報をテキストで入手
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
	//ファイルクローズ
	FileRead_close(fp);
	int y, m, w;
	y = atoi(data[1].c_str());
	m = atoi(data[2].c_str());
	w = atoi(data[3].c_str());
	//サブイベントの存在有無を調べる場合
	if (kind == "sub") {
		if (data[4] == "サブ") {
			return true;
		}
		else {
			return false;
		}
	}
	//キャラ名に対応するキャラ番号を調べる
	int n = 0;//いない場合は0、絶対に条件を満たす
	for (int i = 1; i < 10; i++) {
		if (data[4] == st_name[i]) {//一致するキャラの名前発見
			n = i;
			break;
		}
	}
	//日付とクラスメイトの条件を満たしているか
	if (week_num(year, month, week) >= week_num(y, m, w) && classmate[0] == classmate[n]) {
		return true;
	}
	return false;
}

//クラスメイトとのイベント選択
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

//描画位置の設定
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

//立ち絵のロード
void Stand::load(const char *name) {
	if (handle != -1) {//前の画像があるなら消去しておく
		DeleteGraph(handle);
	}
	handle = LoadGraph(name);
	GetGraphSize(handle, &x_size, &y_size);
}

//立ち絵の描画
void Stand::draw(int dx, int dy) {
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	DrawRotaGraph2(position + dx, 0 + dy, 0, 0, STAND_EXTEND, 0, handle, TRUE, reverse);
	SetDrawMode(DX_DRAWMODE_NEAREST);
}

Stand::~Stand() {
	if (handle != -1) {//前の画像があるなら消去しておく
		DeleteGraph(handle);
	}
}



//////////テキストイベントクラス/////////////////
TextEvent::TextEvent(EVENT_KIND kind, int num, bool bright, string folder) {
	first_flag = false;
	event_name = "";
	finish_text = false;
	bright_value = 255;//もともとは明るいままの設定
	bright_flag = bright;
	if (bright_flag) { bright_value = 0; }//真っ暗スタート
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
	string num_s;//番号の文字列
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
	//立ち絵
	stand = new Stand[2];
	stand[0].set_position(true);
	stand[1].set_position(false);
	//フキダシ
	huki = LoadGraph("picture/フキダシ.png");
	//背景
	char sentence[256];
	string str;
	//ファイルを開く
	file = FileRead_open(file_name.c_str());
	FileRead_gets(sentence, 256, file);//1行読み込む
	str = sentence;
	haikei = LoadGraph(("picture/haikei/" + str).c_str());
	//効果音
	kettei = LoadSoundMem("sound/システム音/決定.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, kettei);
	say_left = false;
	say_none = false;
	textfont = CreateFontToHandle(NULL, 30, 8);
	//画面表示のずれ
	left_dx = -1 * START_DX; right_dx = START_DX; text_dy = START_DY;
	effect_sound = -1;
}

//デストラクタ
TextEvent::~TextEvent() {
	FileRead_close(file);//ファイルを閉じる
	delete[] stand;//右と左の２人を消す
	DeleteGraph(haikei);
	DeleteGraph(huki);
	DeleteSoundMem(kettei);
	DeleteFontToHandle(textfont);
	if (effect_sound != -1) {
		DeleteSoundMem(effect_sound);
	}
}

//メイン 描画もする
bool TextEvent::play() {
	//処理フェーズ
	if (next_cnt > 0) {//誤クリック防止
		next_cnt--;
	}
	if (bright_value == 255) {
		//画面表示のずれ
		if (left_dx != 0) { left_dx = (int)(left_dx * 0.75); }
		if (right_dx != 0) { right_dx = (int)(right_dx * 0.75); }
		if (text_dy != 0) { text_dy = (int)(text_dy * 0.75); }
		//左クリックされたとき or セリフが空の時（最初）
		if (left_click() == 1 || control_space() == TRUE || saying.size() == 0) {
			if (next()) {
				finish_text = true;//ファイルが終端だから終了
				//暗くしていく
				if (bright_flag) { bright_value = 254; }
			}
		}
		deal_text();//セリフを１文字ずつ表示する調整
	}
	if (bright_value != 255) {
		if (finish_text) {//終了時、暗くする
			bright_value = (int)(bright_value * 0.75);
		}
		else {//開始または背景変更時、明るくする
			if (bright_value < 4) { bright_value = 4; }
			bright_value = (int)(bright_value * 1.25);
			if (bright_value > 255) { bright_value = 255; }
		}
	}

	//描画フェーズ
	draw();
	if (control_quit() == TRUE) {
		finish_text = true;//終了
		//暗くしていく
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

//セリフを１文字ずつ表示するように調整
void TextEvent::deal_text() {
	if (text_now < (int)saying.size()) {
		text_now += 2;
		if (text_now >(int)saying.size()) {
			text_now = (int)saying.size();
		}
	}
}

//左クリックされたら呼ばれる
bool TextEvent::next() {
	//セリフの途中だった場合：セリフを最後までスキップする
	if (text_now > 0 && text_now != (int)saying.size()) {
		text_now = (int)saying.size();
		next_cnt = 0;
	}
	else if (next_cnt > 0) {
		next_cnt = 0;
		return false;//誤クリックとみなして次のテキストに移行しない
	}//セリフが終了済みだった：次のブロックへ移動
	else if(FileRead_eof(file) == 0){//ファイルが最後でないなら
		if (saying.size() != 0) {
			first_flag = true;
		}
		if (left_click() == 1 || control_space() == TRUE) {
			PlaySoundMem(kettei, DX_PLAYTYPE_BACK);//効果音
		}
		left_dx = 0; right_dx = 0;
		file_read();//次のセリフ
	}
	else {//終了
		return true;
	}
	return false;
}

//テキストファイルのスクリプト言語の解釈
void TextEvent::file_read() {
	char sentence[256];
	string str;
	do {//ファイルの終端または空行までループ
		FileRead_gets(sentence, 256, file);//1行読み込む
		str = sentence;
		if (str == "@name") {//キャラの立ち絵変更
			say_none = false;//誰もしゃべらない状態解除
			FileRead_gets(sentence, 256, file);//1行読み込む
			str = sentence;//-left or -right or キャラの名前
			//立ち絵変更するのに、左右を指定した場合
			if (str == "-left") {
				say_left = false;//今は右がしゃべっていることにする
				FileRead_gets(sentence, 256, file);//1行読み込む
			}
			else if (str == "-right") {
				say_left = true;
				FileRead_gets(sentence, 256, file);//1行読み込む
			}
			//立ち絵をスライドさせて登場させる場合
			str = sentence;
			if (str == "-move") {
				if (say_left) { right_dx = START_DX; }
				else{ left_dx = -1 * START_DX; }
				FileRead_gets(sentence, 256, file);//1行読み込む
			}
			//ここから立ち絵のロード
			string stand_name;//(キャラの名前)
			stand_name = "picture/stand/";
			stand_name += sentence;// [/キャラ名] のパス追加
			stand_name += "/";
			stand_name += sentence;// [キャラ名/キャラ名表情]の形に
			speaker = sentence;//発言者の名前更新
			FileRead_gets(sentence, 256, file);//1行読み込む(キャラの表情)
			if (month >= 6 && month <= 10) { stand_name += "夏"; }//衣替え
			stand_name += sentence;// [表情] のファイル名を追加
			stand_name += ".png";
			stand[say_left].load(stand_name.c_str());
			say_left = !say_left;
		}
		else if (str == "@get") {//武器ゲット
			FileRead_gets(sentence, 256, file);//1行読み込む(武器の番号)
			int num = atoi(sentence);
			weapon_list[num - 1]++;//武器ゲット
		}
		else if (str == "@music") {//BGM変更
			FileRead_gets(sentence, 256, file);//1行読み込む
			string pitch = sentence;
			bool high = false;
			if (pitch == "-high") { //"-high"以外なら音ファイルの名前
				high = true;
				FileRead_gets(sentence, 256, file);//1行読み込む(音ファイルの名前)
			}
			change_music(sentence, high, true);
		}
		else if (str == "@love") {//好感度アップ
			FileRead_gets(sentence, 256, file);//1行読み込む(好感度アップの生徒苗字)
			str = sentence;
			for (int i = 0; i < 10; i++) {//指定された苗字の生徒の番号を調べる
				if (str == st_name[i]) {
					FileRead_gets(sentence, 256, file);//1行読み込む(上がる数値)
					love[i] += atoi(sentence); 
					if (love[i] > LOVE_MAX) { love[i] = LOVE_MAX; }
					break;
				}
			}
		}
		else if (str == "@sound") {
			//効果音のデータを消す
			if (effect_sound != -1) { DeleteSoundMem(effect_sound); }
			FileRead_gets(sentence, 256, file);//1行読み込む
			effect_sound = LoadSoundMem(sentence);
			ChangeVolumeSoundMem(255 * SoundVolume / 100, effect_sound);
			PlaySoundMem(effect_sound, DX_PLAYTYPE_BACK);
		}
		else if (str == "@none") {//誰もしゃべらない
			say_none = true;
		}
		else if (str == "@haikei") {
			if (haikei != -1) { DeleteGraph(haikei); }
			FileRead_gets(sentence, 256, file);//1行読み込む
			str = sentence;
			haikei = LoadGraph(("picture/haikei/" + str).c_str());
			bright_value = 0;//画面を暗くする
			text_dy = START_DY;//フキダシをずらす
		}
		else if(sentence[0] != 0){//単にセリフをセットするだけ
			text_now = 0;
			saying = str;
			if (!say_none) {
				saying = "「" + saying + "」";
			}
		}
	} while (FileRead_eof(file) == 0 && sentence[0] != 0);
}

//セリフを３分割する
static const int MAX_SAYING = 33;//一行に全角で何文字までか
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

//背景、立ち絵、フキダシの描画
void TextEvent::draw() {
	//画面を暗くする
	SetDrawBright(bright_value, bright_value, bright_value);
	//背景
	//DrawBox(0, 0, 1280, 1024, GRAY, TRUE);
	DrawRotaGraph2(0, 0, 0, 0, 1.0, 0, haikei, TRUE, FALSE);//背景画像

	stand[0].draw(left_dx, 0);//左の人
	stand[1].draw(right_dx, 0);//右の人

	//イベント名(左上に表示)
	if (event_name.size() > 0) {
		DrawBox(0, 0, 38 + event_name.size() * 16, 60, GRAY2, TRUE);
		DrawStringToHandle(20, 18, event_name.c_str(), BLACK, textfont);
	}

	//画面の明るさを元に戻す
	SetDrawBright(255, 255, 255);
	//画面が暗くなっているときはセリフ表示しない
	if (bright_value != 255) { return; }

	//フキダシとセリフ表示
	static const int tLEFT = 250;
	static const int tRIGHT = 1280 - tLEFT - 100;
	//フキダシ
	DrawRotaGraph2(83, 710 + text_dy, 0, 0, 1, 0, huki, TRUE, FALSE);
	//フキダシのずれなし　かつ　誰かの発言中なら矢印
	if (text_dy == 0 && !say_none) {
		if (say_left) {//左の人が発言中
			DrawTriangle(tLEFT - 5, 721, tLEFT + 105, 721, tLEFT + 50, 655, BLACK, TRUE);
			DrawTriangle(tLEFT, 721, tLEFT + 100, 721, tLEFT + 50, 660, WHITE, TRUE);
		}
		else {//右の人が発言中
			DrawTriangle(tRIGHT - 5, 721, tRIGHT + 105, 721, tRIGHT + 50, 655, BLACK, TRUE);
			DrawTriangle(tRIGHT, 721, tRIGHT + 100, 721, tRIGHT + 50, 660, WHITE, TRUE);
		}
	}
	//セリフ
	saying_divide();
	for (int i = 0; i < 3; i++) {//770 + i * 70
		DrawStringToHandle(127, 810 + i * 60 + text_dy, sub_saying[i].c_str(), BLACK, textfont);
	}
	//発言者の名前
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
			DrawStringToHandle(800, 950, "Ｑキー長押しでスキップ", BLACK, textfont);
		}
	}
}



/////////////////////////////////////////////////////
//////////親密度を上げて見るイベントの画面///////////
StEvent::StEvent(int st_dot[], int stand_handle[]) {
	te_flag = false;
	dx2 = -1 * START_DX;
	dx1 = 0;
	m_st_select = -1;
	for (int i = 0; i < 10; i++) {
		m_stand_handle[i] = stand_handle[i];
	}
	//フォント
	font_handle = CreateFontToHandle(NULL, 50, 10);
	font_small_handle = CreateFontToHandle(NULL, 30, 8);
	//戻るボタン
	button_cancel = new Button("戻る", 30, 20, 150, 80, LIGHT_GREEN, GREEN, font_handle, BLACK);
	button_st = new StButton(9, st_dot, 200);
	button_decide = new Button("開始", 1100, 20, 150, 80, LIGHT_BLUE, BLUE, font_handle, BLACK);
	//イベントを見れるかのチェック
	check_event();
	//効果音
	decide_sound = LoadSoundMem("sound/システム音/決定.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, decide_sound);
}

StEvent::~StEvent() {
	DeleteFontToHandle(font_handle);
	DeleteFontToHandle(font_small_handle);
	delete button_cancel;
	delete button_st;
	DeleteSoundMem(decide_sound);
}

//各生徒のイベントが観れるか調べる
void StEvent::check_event() {
	for (int i = 1; i < 10; i++) {
		//まずはwaveと親密度と進行状況
		if (max_wave / (100 / EVENT_SUM) >= finish_event[i] && love[i] / (LOVE_MAX / EVENT_SUM) > finish_event[i]) {
			//csvファイルを確認
			able_event[i] = check_event_file(st_name[i], finish_event[i] + 1);
		}
		else {
			able_event[i] = false;
		}
	}
}

bool StEvent::play() {
	if (te_flag) {//テキストイベント中
		bool f = te->play();
		if (f) { //終了
			finish_event[m_st_select]++;
			delete te;
		}
		return f;
	}
	GetMousePoint(&hand_x, &hand_y);
	//立ち絵のずらし
	if (dx1 != 0) {
		dx1 = (int)(dx1 * 0.75);
	}
	if (dx2 != 0) {
		dx2 = (int)(dx2 * 0.75);
	}
	//左クリック時
	if (left_click() == 1) {
		if (button_cancel->overlap(hand_x, hand_y)) {
			return true;
		}
		//キャラを選んでいて、イベントを見れるなら
		if (button_decide->overlap(hand_x, hand_y) && m_st_select != -1 && able_event[m_st_select]) {
			te_flag = true;
			//テキストイベント作成
			te = new TextEvent(TEXT_EVENT, finish_event[m_st_select] + 1, true, st_name[m_st_select]);
		}
		//生徒の選択
		int n = button_st->overlap(hand_x, hand_y);
		if (n != -1) {
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
			if (m_st_select != -1) {//外れるキャラ
				button_st->change_flag(m_st_select - 1, true);
			}
			m_st_select = n + 1;//キャラ番号をセット
			button_st->change_flag(n, false);//セットするキャラ
			dx1 = START_DX;
		}
	}
	return false;
}

void StEvent::draw() {
	if (te_flag) {//テキストイベント中は描画しない
		return;
	}
	//背景
	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, GRAY2, TRUE);
	//生徒の立ち絵
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	//アカツキ
	DrawRotaGraph2(600 - dx2, -50, 0, 0, STAND_EXTEND, 0, m_stand_handle[0], TRUE, FALSE);
	if (m_st_select != -1) {
		DrawRotaGraph2(-50 - dx1, -50, 0, 0, STAND_EXTEND, 0, m_stand_handle[m_st_select], TRUE, FALSE);
	}
	SetDrawMode(DX_DRAWMODE_NEAREST);
	//イベント詳細
	if (m_st_select != -1) {
		if (able_event[m_st_select]) {
			DrawBox(300, 620, 980, 690, LIGHT_BLUE, TRUE);
			DrawStringToHandle(430, 630, "イベント開始可能", DARK_GREEN, font_handle);
		}
		else {
			DrawBox(300, 620, 980, 720, LIGHT_BLUE, TRUE);
			DrawStringToHandle(430, 630, "イベント開始不可", RED, font_handle);
			if (finish_event[m_st_select] == EVENT_SUM) {
				DrawStringToHandle(432, 680, "(全てのイベントを終了済み)", RED, font_small_handle);
			}
			else if (max_wave / (100 / EVENT_SUM) < finish_event[m_st_select]) {
				DrawStringToHandle(440, 680, "(より高Waveの探索が必要)", RED, font_small_handle);
			}
			else if (love[m_st_select] / (LOVE_MAX / EVENT_SUM) <= finish_event[m_st_select]) {
				DrawStringToHandle(490, 680, "(親密度が足りない)", RED, font_small_handle);
			}
			else {//親密度イベントにクラスメイトによる制限はなし
				DrawStringToHandle(417, 680, "(まだ開始できる時期ではない)", RED, font_small_handle);
			}
		}
	}
	//ボタン
	button_cancel->draw(hand_x, hand_y);
	//キャラを選んでいて、イベントを見れるなら
	if (m_st_select != -1 && able_event[m_st_select]) {
		button_decide->draw(hand_x, hand_y);
	}
	button_st->draw(hand_x, hand_y);
	//イベント閲覧可能なら分かりやすいしるしをつける
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