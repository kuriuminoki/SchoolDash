#ifndef WEAPON_H_INCLUDED
#define WEAPON_H_INCLUDED

#include<queue>
#include<functional>
#include<string>
#include<vector>
class Character;
class Button;
class StButton;

//1LEVEL上がるごとにどれだけダメージが増えるか
#define WEAPON_DAMAGE_UP 1

//弾の種類一覧
enum {
	BLUE_BALL,//生徒の弾
	RED_BALL,//敵の弾
	BLUE_ZAN,//生徒の斬撃
	RED_ZAN//敵の斬撃
};

//攻撃の弾、斬撃のクラス
class Bullet {
	int kind;//弾の種類
	int x, y;//座標
	int dx, dy;//大きさ(一辺の1/2 or 円の半径) 円も四角とみなして
	int rx, ry;//飛んでいく向き(スピード)
	int damage;
	int distance;//飛んだ距離
public:
	Bullet(int k, int sx, int sy);//種類と初期座標
	bool operator> (const Bullet &other) const;
public:
	void set_dir(int srx, int sry);//飛んでいく向き(スピード)を設定
	void set_scale(int sdx, int sdy);//大きさを設定
	void set_damage(int after_damage);//ダメージを設定
public:
	int get_x() const;
	int get_y() const;
	int get_dx() const;
	int get_dy() const;
	int get_damage() const;
	int get_kind() const;
	int get_distance() const;
public:
	void move();//弾の移動
};

static const int MAX_INTERVAL = 90;//連射力の最大値


class WeaponInfo {
public:
	int dot_handle;//武器のドット絵
	std::string name;//武器の名前
	int KIND;//弾の種類
	int DAMAGE;//攻撃力
	int SPEED;//弾速 弾の方向がランダムのときに使う
	int INTERVAL;//連射力
	int DISTANCE;//射程
	int DX, DY;//弾の大きさ
	int RX, RY;//弾の移動方向 両方0ならランダムを意味する
	int DEFENSE;//防御力 装備するだけで防御力が上乗せされる仕組み
	int DEBUFF;//当たった時の速度低下時間
	int atari_handle;//当たり時の効果音
	std::vector<std::string> data;//csvファイルから読み込んだテキストデータ
	std::vector<int> value;//テキストを整数に変換したもの（name以外）
public:
	WeaponInfo(int kind);
	~WeaponInfo();
	int get_dot() const;//武器のドット絵を返す
	void get_name(std::string& tag) const;//武器の名前を取得
	void draw(int x, int y, int color, int font, int font_color);//武器の情報を描画
};

//武器の基底クラス (弾の管理も行う)
class Weapon {
protected:
	WeaponInfo* wi;
private:
	std::queue<Bullet*> bullets;//発射中の弾
	int cnt = 0;//連射用のカウント
protected:
	void set_value(int da, int sdx, int sdy, int in, int di, int s, int srx, int sry);
	void set_looks(int kind, int defense, int debuff, const char tama_name[]);
public:
	Weapon(int kind);
	~Weapon();
	int get_dot() const;
	int get_interval();
	int get_defense();
	int get_debuff();
public:
	void random_r();//弾の方向をSPEEDをもとにランダムに決定
	void cus(int level);//性能をレベルによって調整できる
	void limit(int left, int right, int up, int down);//ステージ外の弾を消す
	bool attack(bool click, int x, int y);//発射時にtrueを返す
	bool atari(Character& c);////弾のあたり判定 防御音を鳴らすときはtrueを返す
	void reset();//撃った弾をすべて消す
public:
	void push_draw(std::priority_queue<Bullet, std::vector<Bullet>, std::greater<Bullet> >& que);//描画キューに弾を入れる
};

//武器変更画面
class WeaponChange {
private:
	const int MAX_WEAPON = 8;//一度に表示する武器の数
	int sum = 0;//所持数１以上の武器の数
	int sum_rate = 0;//上から何番目の武器がnowか
	int hand_x, hand_y;
	int now = 0;//一番上に表示する武器の番号
	int touch = -1;//参照中の武器(つまり変更する武器候補)
	int st_select = -1;//生徒選び画面なら-1武器変更中なら生徒番号
	int* weapon_num;//武器ボタンの武器番号

	StButton* button_st;//どの生徒の武器を変更するか選ぶボタン
	Button* button_cancel;//左上の戻るボタン
	Button* button_up;//矢印ボタン
	Button* button_down;
	Button** button_weapon;//武器のボタン
	WeaponInfo** wi;//所持している武器すべて

	int font_handle;//フォント
	int font_small_handle;//フォント
	int up_handle, down_handle;//矢印の画像ハンドル
public:
	WeaponChange(int handle[]);//キャラのドット絵を受け取る
	~WeaponChange();
	void cancel_st(int num);//仲間になっていない生徒をキャンセル
public:
	bool play();//終了時true返却
	void draw();//描画フェーズ
private:
	bool play_st();//生徒選択画面
	void play_wi();//武器変更画面
	void draw_wi();
	void weapon_reset();//矢印が押されたとき
	void now_up();//nowを一番近い上の武器に合わせる
	void now_down();//nowを一番近い下の武器に合わせる
};

class WeaponCreate {
private:
	const int MAX_WEAPON = 8;//一度に表示する武器の数
	int hand_x, hand_y;
	int now = 0;//一番上に表示する武器のインデックス
	int touch = -1;
	Button* button_cancel;//左上の戻るボタン
	Button* button_up;//矢印ボタン
	Button* button_down;
	Button** button_weapon;//武器のボタン
	WeaponInfo** wi;//作れる武器
	int font_handle;//フォント
	int font_small_handle;//フォント
	int up_handle, down_handle;//矢印の画像ハンドル
	int** need;//必要素材
	bool* able;//武器をつくれるならtrue
public:
	WeaponCreate();
	~WeaponCreate();
public:
	bool play();
	void draw();
private:
	void weapon_reset();//武器ボタンの表示を更新
	void check_able();//武器を作れるか調べる
	void draw_material(int x, int y);//必要素材を表示
	void create();//素材を消費して武器を作る
};

#endif