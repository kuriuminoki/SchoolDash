#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include<string>
#include<vector>

class Button;
class StButton;
class StDecide;
class Dash;
class TextEvent;
class StEvent;
class WeaponChange;
class WeaponCreate;
class Shop;
class Option;
class Data;
class Ed;

//ゲーム画面の種類
enum GAME_MODE {
	GAME_MENU,//最初のメニュー画面

	GAME_STORY,//ストーリー画面

	GAME_PRE_DASH,//探索直前
	GAME_DASH,//探索中
	GAME_END_DASH,//探索終了

	GAME_EVENT,//イベント

	GAME_DATA,//データ画面

	GAME_CUS,//武器変更画面

	GAME_CREATE,//武器の作成

	GAME_SHOP,//ショップ画面

	GAME_START,//日付変更画面

	GAME_OPTION,//オプション画面

	GAME_MOVIE,//ムービー画面
};

//ゲーム本体となる
class SchoolDash {
private:
	//音や画像
	int haikei;
	int st_handle[10];//生徒のドット立ち絵
	int run[10][2];//走るドット絵
	int stand_handle[10];//立ち絵
	int decide_sound;//決定音
	int cancel_sound;//戻る音
private:
	//ゲーム管理用
	int hand_x, hand_y;//マウスの座標
	int window_flag;
	int cnt;
	int big_font;
	int small_font;
	GAME_MODE state;//今ゲームがどの画面か
	TextEvent* te;//テキストイベント
private:
	//ボタンたち
	int button_font;
	Button* button_tansaku;//探索ボタン
	Button* button_event;//イベントボタン
	Button* button_cus;//準備ボタン
	Button* button_shop;//ショップボタン
	Button* button_create;//武器作成ボタン
	Button* button_data;//データボタン
	Button* button_option;//オプションボタン
public:
	SchoolDash();
	~SchoolDash();
	void load_all();
	void delete_all();
	void change_menu_sound();//メニュー画面の効果音の音量を再設定
private:
	//お役立ち関数群
	void draw_date(int draw__x, int draw__y);//左上に日付表示
	void next_date();//日付を進める
public:
	//メイン
	void play();
	bool create_event();
private:
	//最初のメニュー画面
	void play_menu();
	void draw_menu();
private:
	//ストーリー閲覧画面
	void play_story();
	void draw_story();
private:
	//日付変更画面
	void play_start();
	void draw_start();
private:
	//探索直前画面
	StDecide* sd;
	void play_pre_dash();
	void draw_pre_dash();
private:
	//探索中
	Dash* dash;//探索
	void play_dash();
	void draw_dash();
private:
	//探索終了
	void play_end_dash();
	void draw_end_dash();
private:
	//イベント画面
	StEvent* se;
	void play_event();
	void draw_event();
private:
	//データ
	Data* data;
	void play_data();
	void draw_data();
private:
	//武器変更画面
	WeaponChange* wc;//武器変更画面
	void play_cus();
	void draw_cus();
private:
	//武器作成画面
	WeaponCreate* wcre;
	void play_cre();
	void draw_cre();
private:
	//ショップ
	Shop* shop;
	void play_shop();
	void draw_shop();
private:
	//オプション
	Option* option;
	void play_option();
	void draw_option();
private:
	//エンディング
	Ed* ed;
};

class Option {
private:
	int hand_x, hand_y;
	int font_handle;
	Button* m_button_right;//矢印ボタン
	Button* m_button_left;
	Button* m_button_cancel;//戻るボタン
	int m_right_handle, m_left_handle;//矢印の画像ハンドル
public:
	Option();
	~Option();
public:
	bool play();
	void draw();
};


//どのデータを閲覧中か
enum {
	DATA_MENU,
	DATA_ST,
	DATA_OTHER,
	DATA_EVENT
};

class StData;
class OtherData;
class EventData;

//データ閲覧画面クラス
class Data {
private:
	int data_state;//どのデータを閲覧中か
	int hand_x, hand_y;
	int decide_sound;//決定音
	int cancel_sound;//戻る音
	int font_handle;//フォント
	int font_small_handle;//フォント
	Button* button_cancel;//戻るボタン
	Button* button_st;//生徒閲覧画面へ
	Button* button_other;//クルメ達の閲覧画面へ
	Button* button_event;//イベント閲覧画面へ
private:
	StData* m_st_data;//生徒閲覧画面用
	OtherData* m_other_data;
	EventData* m_event_data;
public:
	Data(int stand_handle[], int dot_handle[]);
	~Data();
public:
	bool play();
	void draw();
};

class StData {
private:
	int hand_x, hand_y;
	static const int START_DX = 600;//画面表示の最初のずれ
	int dx = 0;//画面表示のずれ
	int handle[10];//生徒の立ち絵
	std::vector<std::string> prof[10];//キャラの紹介文
	StButton* sb;//生徒１０人のボタン
	int st_select;//どの生徒を閲覧するか
	int decide_sound;//決定音
	int font_handle;//フォント
	int font_small_handle;//フォント
	Button* button_change;//紹介文の表示に切り替えるボタン
	bool draw_prof;//紹介文の表示中
public:
	StData(int stand_handle[], int dot_handle[]);
	~StData();
public:
	void play();
	void draw();
	void reset();
};

class OtherData {
private:
	int hand_x, hand_y;
	static const int START_DX = 600;//画面表示の最初のずれ
	int dx;//画面表示のずれ
	int handle[6];//クルメ達の立ち絵
	std::vector<std::string> prof[6];//キャラの紹介文
	int st_select;//何番目の生徒を閲覧中か
	int decide_sound;//決定音
	int font_handle;//フォント
	int font_small_handle;//フォント
	Button* button_next;//次へボタン
	Button* button_prev;//前へボタン
public:
	OtherData();
	~OtherData();
public:
	void play();
	void draw();
	void reset();
};


class EventData {
private:
	int hand_x, hand_y;
	static const int DRAW_TEXT_BUTTON = 6;//画面上に表示するストーリーボタンの数
	Button** button_story;//押すとストーリーが観れる
	Button* button_up;//上矢印
	Button* button_down;//下矢印
	int up_handle, down_handle;//矢印の画像
	int m_now;//一番上に表示中のストーリーは何番？
	bool te_now;//今イベント視聴中？
	TextEvent* te;
	int decide_sound;//決定音
	int font_handle;//フォント
	int font_small_handle;//フォント
	std::string story_name[120];//イベントの名前
public:
	EventData();
	~EventData();
public:
	void reflesh();//ボタンを更新
	bool play();//イベント視聴中はtrue
	bool draw();//イベント視聴中はtrue
};


#endif