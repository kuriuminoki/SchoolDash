#ifndef DASH_H_INCLUDED
#define DASH_H_INCLUDED

//学校の探索をするパート

#include<string>
#include<queue>
#include<vector>
#include<functional>

class Student;
class Enemy;
class Bullet;
class Button;
class TextEvent;

struct CompareBulletGreater;

enum {
	RUN,		//走る
	BATTLE,		//戦闘
	EVENT,		//イベント
	GET,		//宝物ゲット
};

class Dash {
private:
	static const int LOVE_UP = 5;//親密度が一度のイベントでどれだけ上がるか
	static const int EVENT_RAND = 40;//親密度イベントが発生する確率
	static const int EXP_RATE = 7;//もらえる経験値　WAVE×
	static const int GET_RAND = 50;//宝箱の出現確率50
	static const int MATERIAL_CNT = 60;//ゲットした素材の通知の表示時間
	int hand_x, hand_y;//マウスの座標
	Button* button_decide;//終了ボタン
	Button* button_cancel;//再開ボタン
	TextEvent* te;//イベントクラス
private:
	bool pause_flag = false;//trueなら一時停止
	int state;//戦闘中？ダッシュ中？
	int wave = 0;//何ウェーブ目か
	int now_wave = 0;//今回の探索で何ウェーブクリアしたか
	int cnt = 0;//カウント用
	std::queue<int> que_material;//ゲットした素材名を表示する用 素材番号を入れる
	int material_cnt = 0;
private:
	int sn;//生徒の数
	int en;//敵の数
	Student** student;
	std::queue<Enemy*> enemy;
private:
	//描画する弾や斬撃のキュー
	std::priority_queue<Bullet, std::vector<Bullet>, std::greater<Bullet> > draw_que;
	int szangeki_handle;//生徒斬撃の画像
	int ezangeki_handle;//敵斬撃の画像
	int defense_handle;//防御の効果音
	int item_handle;//回復薬の画像
	int item_sound;//回復の効果音
private:
	int* haikei_handle;//背景画像
	int now, next;//今描画する背景、次に描画する背景
	int x;//背景の描画位置
	int treasure_x;
	int treasure_handle;
	std::string treasure_name;
private:
	bool final_flag = false;//クライマックスの音楽が流れたらtrue
	int final_cnt = 0;//時間切れの通知時間
	int final_music;//クライマックスの音楽（あらかじめロード）
	int FontHandle1;//フォントデータ
	int FontHandle2;//フォントデータ(超でかい)
private://リザルトとして提供するデータ
	int stand_handle[3];//キャラの立ち絵（笑顔）
	int dash_exp;//生徒が１回の探索で獲得した経験値
	int dash_love[3];//アカツキが一回の探索で獲得した親密度
	std::vector<int>* dash_weapon;//１回の探索で獲得した武器
	std::vector<int>* dash_material;//１回の探索で獲得した素材
	int dash_money;//１回の探索で獲得したお金
public:
	//各生徒の種類と武器の種類と回復薬の個数とレベル
	Dash(int start_wave, int st[], int weapon[], int item[], int dash_level[]);
	~Dash();
public:
	bool play();//メイン
	void draw();//描画
	bool play_reward();//報酬を受け取る
	void draw_reward();//報酬を受け取る
private:
	void load_music_final();//クライマックスの音楽をロードしておく
	void change_state();//次は戦闘？宝物？
	bool get();//宝ゲット
	void decide_treasure();//宝箱の中身を決定
	bool battle();//バトル
	bool dead();//生徒の誰かがやられたらtrue
	void enemy_dead();//死んだ敵の処理
	void enemy_create();//敵の出現(waveの最初)
	bool move();//背景の移動
	void create_event();//イベントクラス作成
	void draw_chara();//生徒、敵、弾の描画
	void draw_state();//生徒の情報を下に表示
	void draw_bullet(int kind, int x, int y, int dx, int dy);//弾の種類によって描写を変える
};


#endif