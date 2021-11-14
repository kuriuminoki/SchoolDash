#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include<string>

class Button;
class StButton;

static const int STAND_LEFT = 250;
static const int STAND_RIGHT = 930;

//csvファイルからイベントの発生条件確認、見れるならtrue
bool check_event_file(std::string kind, int num);

//クラスメイトとのイベントの番号決定
int decide_sub(int st_num);

//基底クラス
class Stand {
private:
	bool reverse = false;//立ち絵を左右反転するかどうか
	int position = 0;//描画位置
	int handle = -1;//画像ハンドル
	int x_size, y_size;
public:
	void set_position(bool left);
	void load(const char *name);
	void draw(int dx, int dy);
	~Stand();
};

//ストーリー、強制、雑談
enum EVENT_KIND {
	TEXT_STORY,
	TEXT_SUB,
	TEXT_OTHER,
	TEXT_EVENT,
	TEXT_BATTLE
};

//キャラが会話するイベント
class TextEvent {
private:
	std::string event_name;
	std::string speaker;//発言者の名前
	int file;//ファイル開閉用
	bool bright_flag;//trueなら開始時と終了時にbright_valueを変える
	int bright_value;//画面の暗さ
	bool finish_text;//trueなら終了 画面を暗くするならこのとき暗くする
	Stand* stand;//キャラの立ち絵配列 2人分
	bool first_flag;//一つ目のセリフが終わったかどうか
private:
	int text_now = 0;//セリフを何文字目まで表示するか
	static const int TEXT_NEXT_CNT = 30;
	int next_cnt = 0;//誤クリック防止用
	std::string saying;//セリフ
	std::string sub_saying[3];//セリフを3分割した結果
	bool say_left;//左の人がしゃべっているならtrue
	bool say_none;//誰もしゃべらないならtrue
private:
	int haikei;//背景
	int huki;//フキダシ
	int textfont;//テキストのフォント
	int kettei;//左クリックの効果音
	int effect_sound;//効果音（ファイルを読み込んだときに使う指示が出る）
public:
	static const int START_DX = 800;//400
	static const int START_DY = 400;
	int left_dx, right_dx, text_dy;//表示のずれ
public:
	//ファイルを開く //イベントの種類、番号、開始と終了時に暗転するか
	TextEvent(EVENT_KIND kind, int num, bool bright, std::string folder = "");
	//ファイルを閉じる
	~TextEvent();
public:
	bool play();//メイン 終了ならtrue
private:
	void deal_text();//セリフを画面に１文字ずつ表示するように
	bool next();//次のブロックを処理 ファイルの終わりならtrue
	void file_read();//ファイルのスクリプト言語処理
	void saying_divide();//セリフを３分割する
	void draw();//描画
};

//好感度を上げて見るイベント画面
class StEvent {
private:
	int hand_x, hand_y;
	int dx1, dx2;//立ち絵の表示ずらし用
	const int START_DX = 600;
	int m_st_select;//現在選択中の生徒
	int m_stand_handle[10];
	Button* button_cancel;
	Button* button_decide;
	StButton* button_st;
	int font_handle;
	int font_small_handle;
	bool able_event[10];//各生徒のイベントを見れるか
	int decide_sound;
private:
	TextEvent* te;
	bool te_flag;//テキストイベント中か(true)、選択画面か(false)
public:
	StEvent(int st_dot[], int stand_handle[]);
	~StEvent();
public:
	bool play();
	void draw();
	bool get_flag();//テキストイベントを見ているかどうか
private:
	void check_event();//各生徒のイベントを見れるか調べる（親密度と進行状況により）
};

#endif