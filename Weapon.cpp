#include"Weapon.h"
#include"Chara.h"
#include"DxLib.h"
#include"SDvalue.h"
#include"Button.h"
#include"Control.h"

#include<cmath>
#include<algorithm>
#include<string>
#include<sstream>

using namespace std;

//弾の種類と初期座標設定
Bullet::Bullet(int k, int sx, int sy) {
	x = sx; y = sy;
	kind = k;
	distance = 0;

	//デフォルト設定
	rx = 10; ry = 10;
	dx = 10; dy = 0;
	damage = 10;
}

bool Bullet::operator> (const Bullet &other) const {
	return y > other.get_y();
}

//向き(スピード)の設定
void Bullet::set_dir(int srx, int sry) {
	rx = srx; ry = sry;
}
//大きさの設定
void Bullet::set_scale(int sdx, int sdy) {
	dx = sdx; dy = sdy;
}
//ダメージの設定
void Bullet::set_damage(int after_damage) {
	damage = after_damage;
}

//各変数の取得
int Bullet::get_x() const {
	return x;
}
int Bullet::get_y() const {
	return y;
}
int Bullet::get_dx() const {
	return dx;
}
int Bullet::get_dy() const {
	return dy;
}
int Bullet::get_damage() const {
	return damage;
}

int Bullet::get_kind() const {
	return kind;
}

int Bullet::get_distance() const {
	return distance;
}

//移動
void Bullet::move() {
	x += rx;
	y += ry;
	distance += (int)sqrt((double)(rx*rx + ry*ry));
}

///////////ここからWeaponクラス////////////

//デストラクタ
//ステージ上の弾をすべて消す
Weapon::~Weapon() {
	delete wi;
	reset();
}

void Weapon::reset() {
	while (!bullets.empty()) {
		delete bullets.front();
		bullets.pop();
	}
}

//連射力を取得
int Weapon::get_interval() {
	return wi->INTERVAL;
}

//追加の防御力を取得
int Weapon::get_defense() {
	return wi->DEFENSE;
}

//速度低下時間を取得
int Weapon::get_debuff() {
	return wi->DEBUFF;
}

//武器の性能補正 敵用
void Weapon::cus(int level) {
	//攻撃力などを変更する
	//wi->DAMAGE += level * WEAPON_DAMAGE_UP;
	wi->DAMAGE *= level * WEAPON_DAMAGE_UP;
}

//弾の移動とステージ外に行ったかの確認
void Weapon::limit(int left, int right, int up, int down) {
	unsigned int size = bullets.size();
	//全ての弾を１回ずつ確認する
	for (unsigned int i = 0; i < size; i++) {
		Bullet* b = bullets.front();
		bullets.pop();
		b->move();//弾の移動
		if (b->get_distance() > wi->DISTANCE) {
			delete b;//射程外
		}
		else if (b->get_x()+ wi->DX < left || b->get_x()- wi->DX > right || b->get_y()+ wi->DY < up || b->get_y()- wi->DY > down) {
			//ステージ外
			delete b;
		}
		else {//ステージ内
			bullets.push(b);
		}
	}
}

//弾の発射 clickは攻撃の入力がされているかどうか
bool Weapon::attack(bool click, int x, int y) {
	if (cnt > 0) {//インターバル中
		cnt--;
	}
	if (!click) {//攻撃していない
		
	}
	else {//攻撃中
		if (cnt == 0) {
			//発射
			Bullet* b = new Bullet(wi->KIND, x, y);
			b->set_damage(wi->DAMAGE);//ダメージ設定
			//向き（スピード）設定
			if (wi->RX == 0 && wi->RY == 0) {//ランダムな方向
				random_r();
				b->set_dir(wi->RX, wi->RY);
				wi->RX = 0; wi->RY = 0;
			}
			else {//決まった方向
				b->set_dir(wi->RX, wi->RY);
			}		
			b->set_scale(wi->DX, wi->DY);//大きさ設定
			bullets.push(b);//弾のセット完了
			//次撃てるまでの時間
			cnt = MAX_INTERVAL - wi->INTERVAL + 1;
			return true;
		}
	}
	return false;
}

const int RANGE = 60;//弾が飛ぶ角度の範囲(°)
//弾の飛ぶ方向をランダムに決定する
static const double PI = 3.14159;
void Weapon::random_r() {
	int r = GetRand(RANGE) - (RANGE / 2);
	wi->RX = (int)(wi->SPEED * cos(PI * r / 180));
	wi->RY = (int)(wi->SPEED * sin(PI * r / 180));
}

//２つのオブジェクトが重なっているか判別するのに使う
bool atari_judge(int x1, int x2, int dx1, int dx2) {
	if (abs(x1 - x2) < dx1 + dx2) {
		return true;
	}
	return false;
}
//弾のあたり判定 防御音を鳴らすときはtrueを返す
bool Weapon::atari(Character& c) {
	bool flag = false;
	//相手の座標や大きさを取得
	int cx = c.get_cx();
	int x_size = c.get_xsize();
	int cy = c.get_cy();
	int y_size = c.get_ysize();
	int d = c.get_defense();//相手の防御力

	unsigned int size = bullets.size();
	//全ての弾をみる
	for (unsigned int i = 0; i < size; i++) {
		Bullet* b = bullets.front();
		bullets.pop();
		//キューにY座標と弾の種類を入れる
		if (atari_judge(b->get_x(), cx, b->get_dx(), x_size/2) && atari_judge(b->get_y(), cy, b->get_dy(), y_size/2)) {
			//当たった
			c.change_hp((d - 100)*wi->DAMAGE / 100);//DAMAGEはd%減少
			c.set_debuff(wi->DEBUFF);
			if (d > 0) { flag = true; }	
			else{ PlaySoundMem(wi->atari_handle, DX_PLAYTYPE_BACK); }
			delete b;
		}
		else {//当たっていない
			bullets.push(b);
		}
	}
	return flag;
}

//描画用の優先度キューに弾の情報を入れる
void Weapon::push_draw(std::priority_queue<Bullet, std::vector<Bullet>, std::greater<Bullet> >& que) {
	unsigned int size = bullets.size();
	//DrawFormatString(10, 720, GetColor(255,255,255), "tama_num = %d", bullets.size());
	//全ての弾を入れる
	for (unsigned int i = 0; i < size; i++) {
		Bullet* b = bullets.front();
		bullets.pop();
		//キューに弾を入れる
		que.push(*b);
		bullets.push(b);
	}
}

int Weapon::get_dot() const {
	return wi->get_dot();
}

//武器の作成
Weapon::Weapon(int kind) {
	wi = new WeaponInfo(kind);
}

//武器のダメージなどをセットする
void Weapon::set_value(int da, int sdx, int sdy, int in, int di, int s, int srx, int sry) {
	wi->DAMAGE = da;
	wi->DX = sdx; wi->DY = sdy;
	wi->INTERVAL = in;
	wi->DISTANCE = di;
	wi->SPEED = s;
	wi->RX = srx; wi->RY = sry;
}
//武器の種類などをセットする
void Weapon::set_looks(int kind, int defense, int debuff, const char tama_name[]) {
	wi->KIND = kind;
	wi->DEFENSE = defense;
	wi->DEBUFF = debuff;
	string s;
	s = "sound/atari/";
	s += tama_name;
	wi->atari_handle = LoadSoundMem(s.c_str());//当たり時の効果音
	ChangeVolumeSoundMem(255 * (SoundVolume - 5) / 100, wi->atari_handle);
}


////////WeaponInfoクラス/////////////

//武器の番号は"1"スタート
//csvファイルから武器の情報を得る
WeaponInfo::WeaponInfo(int kind) {
	//武器の画像を読み込む
	string dot_name = "";
	int num = kind;
	while (num > 0) {
		dot_name += '0' + (num % 10);
		num /= 10;
	}
	reverse(dot_name.begin(), dot_name.end());
	dot_name = "picture/weapon/weapon" + dot_name + ".png";
	//csvファイルから性能を入手
	int fp;
	char buff[256];
	fp = FileRead_open("text/weapon.csv");
	//ファイルポインタを移動
	for (int i = 1; i <= kind; i++) {
		FileRead_gets(buff, 256, fp);//最後以外読み飛ばし
	}
	//武器の情報をテキストで入手
	string s = "";
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

	name = data[0];
	for (unsigned int i = 1; i < data.size(); i++) {
		value.push_back(atoi(data[i].c_str()));
	}
	DAMAGE = value[0];
	DX = value[1]; DY = value[2];
	INTERVAL = value[3];
	DISTANCE = value[4];
	SPEED = value[5];
	RX = value[6]; RY = value[7];
	DEFENSE = value[8];
	DEBUFF = value[9];

	//弾の見た目と効果音を性能を見て判断
	if (SPEED > 0) {
		if (DX == DY) {
			KIND = BLUE_BALL;
		}
		else {
			KIND = BLUE_ZAN;
		}
	}
	else {
		if (DX == DY) {
			KIND = RED_BALL;
		}
		else {
			KIND = RED_ZAN;
		}
	}

	//効果音
	if (KIND == BLUE_ZAN || KIND == RED_ZAN) {
		atari_handle = LoadSoundMem("sound/atari/斬撃当たり.wav");
		ChangeVolumeSoundMem(255 * (SoundVolume - 5) / 100, atari_handle);
	}
	else {
		atari_handle = LoadSoundMem("sound/atari/弾当たり.wav");
		ChangeVolumeSoundMem(255 * SoundVolume / 100, atari_handle);
	}

	//武器のドット絵
	dot_handle = LoadGraph(dot_name.c_str());

	FileRead_close(fp);
}

WeaponInfo::~WeaponInfo() {
	if (dot_handle != -1) {
		DeleteGraph(dot_handle);
	}
	DeleteSoundMem(atari_handle);
}

int WeaponInfo::get_dot() const {
	return dot_handle;
}

void WeaponInfo::get_name(std::string& s) const {
	s = name;
}

void WeaponInfo::draw(int x, int y, int color, int font, int font_color) {
	static const int WIDE = 600, HEIGHT = 300;
	DrawBox(x, y, x + WIDE, y + HEIGHT, color, TRUE);
	DrawRotaGraph2(x + 350, y + 70, 0, 0, DOT_EXTEND*2, 0, dot_handle, TRUE, FALSE);
	DrawStringToHandle(x + 10, y + 10, name.c_str(), BLACK, font);
	ostringstream s;
	s << DAMAGE << endl << SPEED << endl << INTERVAL << endl;
	s << DISTANCE << endl << DY << endl << DEFENSE << endl << DEBUFF;
	DrawStringToHandle(x + 250, y + 60, s.str().c_str(), DARK_YELLOW, font);

	ostringstream n;
	n << "攻撃力" << endl << "弾速" << endl << "連射力" << endl;
	n << "射程" << endl << "大きさ" << endl << "防御力" << endl << "デバフ時間";
	DrawStringToHandle(x + 10, y + 60, n.str().c_str(), font_color, font);
}



/////////武器変更画面クラス///////////////

//生徒１０人のドット絵を受け取る
WeaponChange::WeaponChange(int handle[]) {
	GetMouseWheelRotVol();//ホイールの回転量リセット
	//フォント
	font_handle = CreateFontToHandle(NULL, 50, 10);
	font_small_handle = CreateFontToHandle(NULL, 30, 8);
	//生徒10人のボタン
	button_st = new StButton(10, handle, 200);
	//戻るボタン
	button_cancel = new Button("戻る", 30, 20, 150, 80, LIGHT_GREEN, GREEN, font_handle, BLACK);
	//矢印の画像ハンドル
	up_handle = LoadGraph("picture/上矢印.png");
	down_handle = LoadGraph("picture/下矢印.png");
	//矢印ボタン
	button_up = new Button("", 50, 150, 500, 70, GRAY2, YELLOW, font_handle, BLACK);
	button_up->change_flag(false, GRAY);
	button_down = new Button("", 50, 650, 500, 70, GRAY2, YELLOW, font_handle, BLACK);
	button_up->set_graph(up_handle, 3);
	button_down->set_graph(down_handle, 3);

	//武器の用意
	wi = new WeaponInfo*[WEAPON_SUM];
	for (int i = 0; i < WEAPON_SUM; i++) {
		wi[i] = new WeaponInfo(i + 1);//kind = １～
	}
	button_weapon = new Button*[MAX_WEAPON];
	for (int i = 0; i < MAX_WEAPON; i++) {
		button_weapon[i] = new Button("", 50, 240+(i*50), 500, 40, LIGHT_BLUE, BLUE, font_small_handle, BLACK);
	}
	weapon_num = new int[MAX_WEAPON];

	//その他
	now = 0;
	weapon_reset();//ボタンに武器を設定
}

WeaponChange::~WeaponChange() {
	DeleteFontToHandle(font_handle);
	DeleteFontToHandle(font_small_handle);
	DeleteGraph(up_handle);
	DeleteGraph(down_handle);
	delete button_st;
	delete button_cancel;
	delete button_up;
	delete button_down;
	for (int i = 0; i < WEAPON_SUM; i++) {
		delete wi[i];
	}
	delete[] wi;
	for (int i = 0; i < MAX_WEAPON; i++) {
		delete button_weapon[i];
	}
	delete[] button_weapon;
	delete[] weapon_num;
}

//num番目の生徒を武器変更不可にする
void WeaponChange::cancel_st(int num) {
	button_st->change_flag(num, false);
}

//メイン
bool WeaponChange::play() {
	GetMousePoint(&hand_x, &hand_y);
	//生徒選択済み
	if (st_select != -1) {
		play_wi();
	}

	//生徒選択処理
	return play_st();
}

//描画
void WeaponChange::draw() {
	//背景
	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, DARK_GREEN, TRUE);
	//生徒選択済み
	if (st_select != -1) {
		draw_wi();
	}

	//生徒ボタン表示
	button_st->draw(hand_x, hand_y);
	//戻るボタン表示
	button_cancel->draw(hand_x, hand_y);
}

//生徒選択画面
bool WeaponChange::play_st() {
	//左クリックされたとき
	if (left_click() == 1) {
		//戻るボタン
		if (button_cancel->overlap(hand_x, hand_y)) {
			return true;
		}
		int n = button_st->overlap(hand_x, hand_y);
		if (n != -1) {
			if (st_select != -1) {//もともと選んでいた生徒true
				button_st->change_flag(st_select, true);
			}
			st_select = n;//生徒選択
			button_st->change_flag(n, false);//選択した生徒false
		}
	}
	return false;
}

//武器変更処理
void WeaponChange::play_wi() {
	//武器ボタンにカーソルが重なっているか確認
	touch = -1;
	for (int i = 0; i < MAX_WEAPON; i++) {
		if (button_weapon[i]->overlap(hand_x, hand_y) && weapon_num[i] != -1) {
			touch = i;
		}
	}
	//マウスのホイール回転
	int rot = GetMouseWheelRotVol();
	if (rot > 0 && button_up->get_flag()) {//上へ
		now_up();
		weapon_reset();
	}//下へ
	else if (rot < 0 && button_down->get_flag()) {
		now_down();
		weapon_reset();
	}//左クリックされたとき 長押しも可
	else if (left_click() == 1 || (left_click() > 10 && left_click() % 2 == 1)) {
		//上矢印
		if (button_up->overlap(hand_x, hand_y)) {
			now_up();
			weapon_reset();
		}//下矢印
		else if (button_down->overlap(hand_x, hand_y)) {
			now_down();
			weapon_reset();
		}
	}
	//左クリックされたとき
	if (left_click() == 1) {
		//戻るボタン
		if (button_cancel->overlap(hand_x, hand_y)) {
			st_select = -1;
		}
		//武器ボタン
		if (touch != -1) {
			//装備中の武器を外す
			weapon_list[have_weapon[st_select]-1]++;
			//新たに武器を付け替える
			have_weapon[st_select] = weapon_num[touch];
			weapon_list[weapon_num[touch]-1]--;
			weapon_reset();
		}
	}
}

//装備中の武器と選べる武器を表示
void WeaponChange::draw_wi() {
	button_up->draw(hand_x, hand_y);//上矢印
	button_down->draw(hand_x, hand_y);//下矢印
	DrawStringToHandle(600, 20, "装備中の武器", GREEN, font_handle);
	wi[have_weapon[st_select]-1]->draw(600, 70, LIGHT_BLUE, font_small_handle, DARK_RED);
	if (touch != -1 && weapon_num[touch] > 0) {//カーソルが武器ボタンに重なっているとき
		DrawStringToHandle(600, 400, "選択中の武器", YELLOW, font_handle);
		wi[weapon_num[touch]-1]->draw(600, 450, LIGHT_BLUE, font_small_handle, DARK_RED);
	}

	//武器ボタン
	for (int i = 0; i < MAX_WEAPON; i++) {
		button_weapon[i]->draw(hand_x, hand_y);
	}
	//武器の数を表示
	ostringstream s;
	s << "所持している武器：" << sum << "/100";
	DrawStringToHandle(50, 110, s.str().c_str(), GREEN, font_small_handle);
	DrawBox(20, 240, 40, 630, BLACK, TRUE);
	if (sum != 0) {
		int rate1 = (int)(390 * sum_rate / sum);
		int rate2 = min(390, (int)(390 * (sum_rate + MAX_WEAPON) / sum));
		DrawBox(20, 240 + rate1, 40, 240 + rate2, WHITE, TRUE);
	}
	//テスト
	//DrawFormatString(0, 0, BLACK, "sum = %d", sum);
	//DrawFormatString(100, 0, BLACK, "rate2 = %d", rate2);
}

//武器ボタンの表示をリセット　矢印が押されたとき
void WeaponChange::weapon_reset() {
	sum = 0; sum_rate = 0;
	bool up_flag = false;//上矢印が押せるかどうか
	bool down_flag = false;//下矢印が押せるかどうか
	int d = 0;//ボタンのタグを何個決め終わったか
	for (int i = 0; i < WEAPON_SUM; i++) {
		if (weapon_list[i] == 0) {
			continue;
		}
		sum++;
		if (i < now) { sum_rate++; }
		if (!up_flag && i < now) {
			up_flag = true;//まだ上に武器がある
		}
		if (!down_flag && d >= MAX_WEAPON) {
			down_flag = true;//まだ下に武器がある
		}
		//ボタンに表示する武器
		if (i >= now && d < MAX_WEAPON) {
			weapon_num[d] = i+1;//武器番号をセット
			string n;
			ostringstream s;
			wi[i]->get_name(n);//武器の名前を取得
			s << n.c_str() << "×" << weapon_list[i];
			button_weapon[d]->set_string(s.str().c_str());//武器の名前をボタンにセット
			d++;
		}
	}
	//埋まらなかった部分を空白にする
	for (d; d < MAX_WEAPON; d++) {
		weapon_num[d] = -1;
		button_weapon[d]->set_string("");
	}
	//矢印ボタンの設定
	if (up_flag) {
		button_up->change_flag(true, GRAY2);
	}
	else {
		button_up->change_flag(false, GRAY);
	}
	if (down_flag) {
		button_down->change_flag(true, GRAY2);
	}
	else {
		button_down->change_flag(false, GRAY);
	}
}

void WeaponChange::now_up() {
	for (int i = now - 1; i >= 0; i--) {
		if (weapon_list[i] > 0) {
			now = i;
			break;
		}
	}
}

void WeaponChange::now_down() {
	for (int i = now + 1; i < WEAPON_SUM; i++) {
		if (weapon_list[i] > 0) {
			now = i;
			break;
		}
	}
}



///////////武器作成画面///////////////////

WeaponCreate::WeaponCreate() {
	GetMouseWheelRotVol();//ホイールの回転量リセット
	font_handle = CreateFontToHandle(NULL, 50, 10);
	font_small_handle = CreateFontToHandle(NULL, 30, 8);
	//戻るボタン
	button_cancel = new Button("戻る", 30, 20, 150, 80, LIGHT_GREEN, GREEN, font_handle, BLACK);
	//矢印の画像ハンドル
	up_handle = LoadGraph("picture/上矢印.png");
	down_handle = LoadGraph("picture/下矢印.png");
	//矢印ボタン
	button_up = new Button("", 50, 150, 500, 70, GRAY2, YELLOW, font_handle, BLACK);
	button_up->change_flag(false, GRAY);
	button_down = new Button("", 50, 650, 500, 70, GRAY2, YELLOW, font_handle, BLACK);
	button_up->set_graph(up_handle, 3);
	button_down->set_graph(down_handle, 3);

	//武器の用意
	wi = new WeaponInfo*[CREATE_SUM];
	for (int i = 0; i < CREATE_SUM; i++) {
		wi[i] = new WeaponInfo(i + 1 + (WEAPON_SUM - CREATE_SUM));
	}
	//武器ボタン
	button_weapon = new Button*[MAX_WEAPON];
	for (int i = 0; i < MAX_WEAPON; i++) {
		button_weapon[i] = new Button("", 50, 240 + (i * 50), 500, 40, LIGHT_BLUE, BLUE, font_small_handle, BLACK);
	}
	//必要素材の情報
	need = new int*[CREATE_SUM];
	for (int i = 0; i < CREATE_SUM; i++) {
		need[i] = new int[MATERIAL_SUM];
	}
	//csvファイルから性能を入手
	int fp;
	char buff[256];
	fp = FileRead_open("text/material.csv");
	//１行ずつ読んでいく
	for (int i = 0; i < CREATE_SUM; i++) {
		string s = "";
		FileRead_gets(buff, 256, fp);
		int n = 0;
		//武器の情報をテキストで入手
		for (int j = 0; buff[j] != '\0'; j++) {
			if (buff[j] == ',' && n < MATERIAL_SUM) {
				need[i][n] = atoi(s.c_str());
				n++;
				s = "";
			}
			else {
				s += buff[j];
			}
		}
		need[i][n] = atoi(s.c_str());
	}
	FileRead_close(fp);//ファイルクローズ
	able = new bool[CREATE_SUM];
	check_able();//武器が作れるか確認
	weapon_reset();//武器ボタンを設定
}

WeaponCreate::~WeaponCreate() {
	DeleteFontToHandle(font_handle);
	DeleteFontToHandle(font_small_handle);
	DeleteGraph(up_handle);
	DeleteGraph(down_handle);
	delete button_cancel;
	delete button_up;
	delete button_down;
	for (int i = 0; i < CREATE_SUM; i++) {
		delete wi[i];
		delete[] need[i];
	}
	delete[] wi;
	delete[] need;
	for (int i = 0; i < MAX_WEAPON; i++) {
		delete button_weapon[i];
	}
	delete[] button_weapon;
	delete[] able;
}

bool WeaponCreate::play() {
	GetMousePoint(&hand_x, &hand_y);
	//武器ボタンにカーソルが重なっているか確認
	touch = -1;
	for (int i = 0; i < MAX_WEAPON; i++) {
		if (button_weapon[i]->overlap(hand_x, hand_y)) {
			touch = i;
		}
	}

	//マウスのホイール回転
	int rot = GetMouseWheelRotVol();
	if (rot > 0 && button_up->get_flag()) {//上へ
		now--;
		weapon_reset();
	}
	else if (rot < 0 && button_down->get_flag()) {//下へ
		now++;
		weapon_reset();
	}
	else if (left_click() == 1) {//左クリック時
		if (button_up->overlap(hand_x, hand_y) && button_up->get_flag()) {
			now--;
			weapon_reset();
		}
		if (button_down->overlap(hand_x, hand_y) && button_down->get_flag()) {
			now++;
			weapon_reset();
		}
		if (button_cancel->overlap(hand_x, hand_y)) {
			return true;
		}
		if (touch != -1 && able[now + touch]) {//作る
			create();
			check_able();//素材が減るので再度チェック
			weapon_reset();//それに合わせて武器ボタンも更新
		}
	}
	return false;
}

void WeaponCreate::weapon_reset() {
	for (int i = 0; i < MAX_WEAPON; i++) {
		string n;
		wi[i + now]->get_name(n);//武器の名前を取得
		button_weapon[i]->set_string(n.c_str());//武器の名前をボタンにセット
		if (able[i + now]) {
			button_weapon[i]->change_flag(true, LIGHT_BLUE);//作れる武器
		}
		else {
			button_weapon[i]->change_flag(true, LIGHT_BLUE2);//作れない武器
		}
	}
	if (now == 0) {
		button_up->change_flag(false, GRAY);
	}
	else {
		button_up->change_flag(true, GRAY2);
	}
	if (now == CREATE_SUM - MAX_WEAPON) {
		button_down->change_flag(false, GRAY);
	}
	else {
		button_down->change_flag(true, GRAY2);
	}
}

void WeaponCreate::check_able() {
	for (int i = 0; i < CREATE_SUM; i++) {
		able[i] = true;
		if (weapon_list[i % 10] <= 0) {//自衛用の武器素材
			able[i] = false;
			continue;
		}
		for (int j = 0; j < MATERIAL_SUM; j++) {//アイテム素材
			if (need[i][j] > material[j]) {
				able[i] = false;
				break;
			}
		}
	}
}

void WeaponCreate::draw() {
	//背景
	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, DARK_GREEN, TRUE);
	//戻るボタン
	button_cancel->draw(hand_x, hand_y);
	button_up->draw(hand_x, hand_y);
	button_down->draw(hand_x, hand_y);
	//カーソルが武器ボタンに重なっているとき
	if (touch != -1) {
		ostringstream s;
		s << "選択中の武器：" << weapon_list[now + touch + (WEAPON_SUM - CREATE_SUM)];
		s << "個所持";
		DrawStringToHandle(600, 100, s.str().c_str(), YELLOW, font_handle);
		wi[now + touch]->draw(600, 150, LIGHT_BLUE, font_small_handle, DARK_RED);
		//必要素材表示
		draw_material(600, 500);
	}
	//武器ボタン
	for (int i = 0; i < MAX_WEAPON; i++) {
		button_weapon[i]->draw(hand_x, hand_y);
	}
	DrawBox(20, 240, 40, 630, BLACK, TRUE);
	int rate1 = (int)(390 * now / CREATE_SUM);
	int rate2 = min(390, (int)(390 * (now + MAX_WEAPON) / CREATE_SUM));
	DrawBox(20, 240 + rate1, 40, 240 + rate2, WHITE, TRUE);
}

void WeaponCreate::draw_material(int x, int y) {
	const int height = 40;
	DrawStringToHandle(x, y, "必要素材", YELLOW, font_handle);
	y += 60;
	//自衛用武器素材
	int k = (now + touch) % 10;
	if (weapon_list[k] > 0) {
		DrawBox(x, y, x + 600, y + height, LIGHT_GREEN, TRUE);
	}
	else {
		DrawBox(x, y, x + 600, y + height, LIGHT_RED, TRUE);
	}
	ostringstream z;
	z << start_weapon_name[k];
	z << "：" << weapon_list[k] << " / 1";
	DrawStringToHandle(x + 10, y + 5, z.str().c_str(), BLACK, font_small_handle);
	y += height + 5;
	//素材
	for (int i = 0; i < MATERIAL_SUM; i++) {
		if (need[now + touch][i] == 0) { continue; }
		if (need[now + touch][i] <= material[i]) {
			DrawBox(x, y, x + 600, y + height, LIGHT_GREEN, TRUE);
		}
		else {
			DrawBox(x, y, x + 600, y + height, LIGHT_RED, TRUE);
		}
		ostringstream s;
		s << material_name[i] << "：" << material[i] << " / " << need[now + touch][i];
		DrawStringToHandle(x+10, y+5, s.str().c_str(), BLACK, font_small_handle);
		y += height + 5;
	}
}

void WeaponCreate::create() {
	//素材を消費
	weapon_list[(now + touch) % 10]--;
	for (int i = 0; i < MATERIAL_SUM; i++) {
		material[i] -= need[now + touch][i];
	}
	//作った武器
	weapon_list[(WEAPON_SUM - CREATE_SUM) + now + touch]++;
}