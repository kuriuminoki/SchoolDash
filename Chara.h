#ifndef CHARA_H_INCLUDED
#define CHARA_H_INCLUDED

//生徒と敵のオブジェクト

#include<queue>
#include<utility>
#include<functional>

class Weapon;
class Bullet;

//1Level上がるごとに体力がどれだけ増えるか
#define STUDENT_HP_UP 120
#define ENEMY_HP_UP 4

//基底クラス
class Character {
protected://入力関連
	int input_stick;//ASDWキーの左右(A,D)
	int input_stick2;//上下（S,W）
	int input_left;
	int input_right;
	int input_e;
	int input_q;
protected:
	int RUN_SPEED = 6;//走るスピード生徒はデフォルト6
	int level;
	int max_hp;
	int hp;
	int defense;//防御力
	int debuff_time = 0;//速度低下残り時間
	int x, y;
	int cnt;
	int damage_flag = 0;//ダメージを受けた時、ドット絵を赤くする用
	Weapon* weapon;//武器
protected:
	int run[2];//ドット絵
	int x_size, y_size;
	int debuff_handle;
public:
	~Character();
	int get_cx() const;//座標（中心）を取得
	int get_cy() const;//座標（中心）を取得
	int get_x() const;
	int get_y() const;
	int get_xsize() const;
	int get_ysize() const;
	int get_max_hp() const;
	int get_hp() const;
	int get_defense() const;//Eキーの入力状態によって0または防御力を返す
public:
	void change_hp(int value);//hpを+valueする
	void set_debuff(int value);//速度低下をくらう
	void cnt_reset();//cntを0にする
public:
	void move();//移動
	void run_cnt();//走らせるcntインクリメント
	void attack();//武器の使用
	void attack_limit();//弾のステージ外判定
	bool atari(Character& c);//敵に攻撃が当たったか
	//描画用のキューに弾を入れる
	void attack_push(std::priority_queue<Bullet, std::vector<Bullet>, std::greater<Bullet> >& que);
public:
	void ai_normal(int left, int middle, int right, int updown, int left_limit);
	virtual void ai() = 0;
	int next_x;//次に向かう座標
	int next_y;//次に向かう座標
protected:
	void draw_dot(bool bright_flag);
};

//生徒
class Student :
	public Character {
private:
	int kind;//生徒の種類 アカツキ、ハンダ ～ "0"スタート
	int yarare_handle;//やられたときのドット絵
	int item;//回復薬の個数
	int item_handle;//回復薬の画像
	int ase_handle;//汗の画像
	int heart_handle;//ハートの画像
	int icon_y_size;
private:
	int target = -1;//回復しようとしている生徒(０～２ or -1)
	int icon_cnt = 0;//ドット絵上部にアイコンを表示する残り時間
	bool heart = true;//回復された側ならtrue,した側ならfalse
public:
	void set_target(int t);
	int get_target();
	void cure_hp();//回復薬で回復する
	bool cure_friend(Student& s);//仲間に回復薬を使用する
	void cure_ai(int n, Student& s);//仲間を回復するか判断するAI
	void cure_reset();//target = -1
public:
	//種類、座標、武器の種類、回復薬の個数、レベル
	Student(int k, int sx, int sy, int wea, int it, int lev);
	~Student();
private:
	void load_dot();//ドット絵のロード
	void level_set();//レベルに応じてHP増加
public:
	int get_weapon_dot() const;
	int get_kind() const;
	int get_item() const;
public:
	void draw(bool bright_flag);//描画
public:
	void control();//キーボードで操作
	void input_reset();//入力状態をリセット
	void limit();//移動限界
	void ai();
	int attack_cnt = 0;//攻撃し続ける残り時間
	void ai_attack(int left, int middle, int right);//攻撃・防御の切り替え 攻撃の確率を引数に
};

//敵
static const int ENEMY_KIND = 16;//敵の種類数
const int drop_money[] = { 20, 25, 25, 25,
							30, 30, 30, 30,
							35, 35, 35, 50, 75, 
							100, 125, 150 };
class Enemy :
	public Character {
private:
	int kind;//敵の種類
	int hp_dx = 0;//敵の種類によってはHPバーの位置を調整する
public:
	//種類、座標、レベル
	Enemy(int k, int sx, int sy, int lev);
	~Enemy();
	void create_weapon();//kindに応じて武器用意
private:
	void load_dot();//ドット絵のロード
	void level_set();//レベルに応じてHP増加
public:
	void dash(int dx);//背景移動の際
	void draw(bool bright_flag);//描画
public:
	void limit();
	void ai();
public:
	int get_money();//倒したときに落とすお金
	int get_drop();//ドロップアイテムの番号を返す
};

#endif
