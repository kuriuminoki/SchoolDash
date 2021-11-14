#ifndef INCLUDED_BUTTON_H
#define INCLUDED_BUTTON_H

#include<string>

class WeaponInfo;
class Dash;

class Button {
private:
	bool m_flag = true;//機能しないボタンはfalse
	std::string m_tag = "";
	int m_x, m_y;//ボタンの位置
	int m_wide, m_height;
	int m_color;//四角の中の色
	int m_color2;//マウスが重なっているときに使う色
	int m_font_color;//文字の色
	int m_font;
	int m_dx, m_dy;//文字を表示する座標
	int m_graph_handle = -1;//絵
	int m_graph_ex;//絵の拡大率
public:
	Button(std::string, int x, int y, int wide, int height, int color, int color2, int font, int font_color);
public:
	void change_flag(bool f, int new_color);
	bool get_flag();
	int get_handle();//画像を取得
	bool overlap(int hand_x, int hand_y);
	void draw(int hand_x, int hand_y);
	void set_graph(int handle, int ex);
	void set_string(std::string new_string);//タグをつけなおす
};

//複数の生徒のボタンを管理する
class StButton {
private:
	int m_size;
	Button** st;
public:
	StButton(int size, int handle[], int height);
	~StButton();
public:
	//n番目の画像ハンドルを取得
	int get_handle(int num);
	//n番目のフラグを取得
	bool get_flag(int num);
	//num番目の生徒のフラグ変更
	void change_flag(int num, bool f);
	//画像ハンドルを変更（仲間じゃない生徒を-1にする用）
	void change_pic(int num, int handle, int ex);
	//マウスカーソルが重なっているなら ０～ 違うなら-1
	int overlap(int hand_x, int hand_y);
	//ボタンの描画
	void draw(int hand_x, int hand_y);
private:
	void st_absent();//日付を見て、まだ仲間になっていない生徒を決定
};

//探索に出撃する生徒を選ぶ画面
class StDecide {
private:
	int hand_x, hand_y;
	const int START_DX = 400;
	int dx[3];
	int m_start_wave = 0;//何ウェーブ目からスタートするか
	int m_st_num[3];//現在選ばれている３枠の生徒番号
	int m_stand_handle[10];
	int m_dot_handle[10];
	int m_run_handle[10][2];
	Button* m_st_button[3];//選択中の生徒が入る
	Button* m_button_right;//矢印ボタン
	Button* m_button_left;
	Button* button_decide;//右上の決定ボタン
	Button* button_cancel;//左上の戻るボタン
	StButton* m_button_st;//生徒９人のボタン
	int m_right_handle, m_left_handle;//矢印の画像ハンドル
	int m_run_cnt;
	WeaponInfo* m_wi[10];//装備中の武器
	int font;
	int font_small_handle;
	int decide_sound;
public:
	StDecide(int stand_handle[], int dot_handle[], int run_handle[][2]);
	~StDecide();
public:
	int play();
	void draw();
	Dash* get_dash();//探索クラスの作成
private:
	int set_st(int new_num);//生徒をセットする 外れたキャラ番号を返す
	int cancel_st();//生徒を外す
};

#endif