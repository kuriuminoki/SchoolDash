#include"DxLib.h"
#include"Chara.h"
#include"Weapon.h"
#include"Control.h"
#include"SDvalue.h"

#include<string>
using namespace std;

static const int SPEED_DEFENSE = 2;//防御中の走るスピード低下
static const int SPEED_DEBUFF = 2;//デバフを食らっているときのスピード

//デストラクタ
Character::~Character() {
	//武器の削除
	delete weapon;
	//画像の削除
	DeleteGraph(run[0]);
	DeleteGraph(run[1]);
	DeleteGraph(debuff_handle);
}

//キャラのドット絵を描画する bright_flagがfalseなら輝度を変更しない
void Character::draw_dot(bool bright_flag) {
	if (damage_flag > 0 && bright_flag) {//ダメージを受けたところなら赤くなる
		SetDrawBright(255, 50, 50);
	}
	DrawRotaGraph2(x, y, 0, 0, DOT_EXTEND, 0, run[((cnt + 3) / 4) % 2], TRUE, FALSE);
	if (damage_flag > 0 && bright_flag) { 
		SetDrawBright(255, 255, 255);
		damage_flag--;
	}
}

//AIのテンプレ　エリアの左、中央、右に移動する確率、上下移動の頻度, 左端の座標
void Character::ai_normal(int left, int middle, int right, int updown, int left_limit) {
	//右に移動
	if (input_stick != LEFT_KEY && x < next_x) {
		input_stick = RIGHT_KEY;
	}//左に移動
	else if (input_stick != RIGHT_KEY && x > next_x) {
		input_stick = LEFT_KEY;
	}
	//目標地点を超えた場合
	if ((input_stick == RIGHT_KEY && x > next_x) || (input_stick == LEFT_KEY && x < next_x)) {
		next_x = x;//目標地点に到達したことにする
	}
	//止まる
	if (next_x == x) { input_stick = FALSE; }
	//左右の移動開始
	if (next_x == x && GetRand(1000) < 20) {
		int r = GetRand(left + middle + right - 1);
		if (r < left) {
			next_x = GetRand(180) + left_limit;
		}
		else if (r < left + middle) {
			next_x = GetRand(180) + 180 + left_limit;
		}
		else {
			next_x = GetRand(180) + 360 + left_limit;
		}
	}

	//上に移動
	if (input_stick2 != DOWN_KEY && y > next_y) {
		input_stick2 = UP_KEY;
	}//左に移動
	else if (input_stick2 != UP_KEY && y < next_y) {
		input_stick2 = DOWN_KEY;
	}
	//目標地点を超えた場合
	if ((input_stick2 == UP_KEY && y < next_y) || (input_stick2 == DOWN_KEY && y > next_y)) {
		next_y = y;//目標地点に到達したことにする
	}
	//止まる
	if (next_y == y) { input_stick2 = FALSE; }
	//上下の移動開始
	if (next_y == y && GetRand(1000) < updown) {
		next_y = 265 + GetRand(380) - y_size;
	}
}

//各変数取得用
int Character::get_cx() const {
	return x + x_size / 2;
}

int Character::get_cy() const {
	return y + y_size / 2;
}

int Character::get_x() const {
	return x;
}

int Character::get_y() const {
	return y;
}

int Character::get_xsize() const {
	return x_size;
}

int Character::get_ysize() const {
	return y_size;
}

int Character::get_max_hp() const {
	return max_hp;
}

int Character::get_hp() const {
	return hp;
}

int Character::get_defense() const {
	//攻撃中は防御不可
	if (input_right > 0 && input_left == 0) {
		//生徒自身の防御力＋武器の防御力
		return defense + weapon->get_defense();
	}
	return 0;
}

//cntをインクリメント
void Character::run_cnt(){
	cnt++;
}

//武器で弾を発射
void Character::attack() {
	if (debuff_time > 0) {//デバフ中は攻撃不可
		weapon->attack(false, x + (x_size / 2), y + (y_size / 2));
	}
	else {
		weapon->attack(input_left, x + (x_size / 2), y + (y_size / 2));
	}
}

//発射した弾がステージ外で消える
void Character::attack_limit() {
	weapon->limit(0, GAME_WIDE, 0, BATTLE_DOWN);
}

//描画用のキューへ弾を入れる
void Character::attack_push(std::priority_queue<Bullet, std::vector<Bullet>, std::greater<Bullet> >& que) {
	weapon->push_draw(que);
}

//武器で発射した弾のあたり判定
bool Character::atari(Character& c) {
	return weapon->atari(c);
}

//hpを+valueする
void Character::change_hp(int value) {
	hp += value;
	if (value < 0) { damage_flag = 5; }//ダメージを受けた
	if (hp < 0) { hp = 0; }
}

//デバフをくらう
void Character::set_debuff(int value) {
	//防御中はデバフくらわない
	if (input_right > 0 && input_left == 0) {
		return;
	}
	debuff_time = value;//上書きされる 上乗せはされない
}

//cntを0にする
void Character::cnt_reset() {
	cnt = 0;
}

//移動する
void Character::move() {
	//歩くモーション
	if (input_stick == FALSE && input_stick2 == FALSE) { cnt = 0; }
	else { run_cnt(); }

	int run_speed = RUN_SPEED;

	//デバフによる速度低下
	if (debuff_time > 0) {
		debuff_time--;
		run_speed = SPEED_DEBUFF;
	}
	else if (get_defense() > 0) {//防御中の速度低下
		run_speed -= SPEED_DEFENSE;
	}

	double ex = 1;
	if (input_stick != FALSE && input_stick2 != FALSE) {
		ex = 1.414;//斜め移動、ルート２でわるため
	}
	switch (input_stick) {
	case LEFT_KEY:
		x -= (int)(run_speed / ex);
		break;
	case RIGHT_KEY:
		x += (int)(run_speed / ex);
		break;
	}
	switch (input_stick2) {
	case UP_KEY:
		y -= (int)(run_speed / ex);
		break;
	case DOWN_KEY:
		y += (int)(run_speed / ex);
		break;
	}
}


/////////ここからStudent///////////////////

Student::Student(int k, int sx, int sy, int wea, int it, int lev) {
	kind = k;
	x = sx;
	next_x = x;
	y = sy;
	next_y = y;
	//武器
	weapon = new Weapon(wea);
	item = it;//回復薬の個数
	level = lev;

	max_hp = 1000;
	defense = 50;//防御力デフォルトは50% 敵は常に0
	//ここでレベルに応じてステータスを設定
	level_set();
	hp = max_hp;//デフォルトは1000

	load_dot();//ドット絵のロード

	//回復薬のロード
	item_handle = LoadGraph("picture/回復薬.png");
	int icon_x_size;
	GetGraphSize(item_handle, &icon_x_size, &icon_y_size);
	icon_y_size *= DOT_EXTEND;

	//非同期読み込み
	SetUseASyncLoadFlag(TRUE);
	ase_handle = LoadGraph("picture/汗.png");
	heart_handle = LoadGraph("picture/ハート.png");
	debuff_handle = LoadGraph("picture/デバフ.png");
	SetUseASyncLoadFlag(FALSE);
	//非同期読み込み終了
}

Student::~Student() {
	DeleteGraph(yarare_handle);
	DeleteGraph(item_handle);
	DeleteGraph(ase_handle);
	DeleteGraph(heart_handle);
}

//レベルに応じてステータス調整
void Student::level_set() {
	max_hp += (level - 1) * STUDENT_HP_UP;
}

int Student::get_weapon_dot() const{
	return weapon->get_dot();
}

int Student::get_kind() const {
	return kind;
}

int Student::get_item() const {
	return item;
}

//ドット絵のロード
void Student::load_dot() {
	string name = "picture/dot/";
	name += st_name[kind];
	if (month >= 6 && month <= 10) { name += "夏"; }
	run[0] = LoadGraph((name + "走り1.png").c_str());
	run[1] = LoadGraph((name + "走り2.png").c_str());
	GetGraphSize(run[0], &x_size, &y_size);
	x_size *= DOT_EXTEND;
	y_size *= DOT_EXTEND;
	yarare_handle = LoadGraph((name + "やられ.png").c_str());
}

//キーボードで操作する
void Student::control() {
	input_stick = control_stick();
	input_stick2 = control_stick2();
	input_left = left_click();
	input_right = right_click();
	input_e = control_cure();
	//input_q = 
}

//画面外へは行けない
void Student::limit() {
	if (x < 0) { x = 0; next_x = x;}
	if(x + x_size > GAME_WIDE / 2){
		x = GAME_WIDE / 2 - x_size;
		next_x = x;
	}
	if (y + y_size < 265) {//120
		y = 265 - y_size;
		next_y = y;
	}
	if (y + y_size > 645 ) {//500
		y = 645 - y_size;
		next_y = y;
	}
}

//キャラのドット絵を描画
void Student::draw(bool bright_flag) {
	if (hp <= 0) {
		DrawRotaGraph2(x, y, 0, 0, DOT_EXTEND, 0, yarare_handle, TRUE, FALSE);
		return;
	}
	draw_dot(bright_flag);//キャラのドット絵
	//ピンチ時の汗
	if (hp * 100 / max_hp <= 30) {
		DrawRotaGraph2(x - 20, y - icon_y_size + 60, 0, 0, DOT_EXTEND - 2, 0, ase_handle, TRUE, FALSE);
	}
	if (icon_cnt > 0) {
		icon_cnt--;
		if (heart) {
			DrawRotaGraph2(x, y - icon_y_size - 20 + icon_cnt, 0, 0, DOT_EXTEND, 0, heart_handle, TRUE, FALSE);
		}
		else {
			DrawRotaGraph2(x, y - icon_y_size - 20 + icon_cnt, 0, 0, DOT_EXTEND, 0, item_handle, TRUE, FALSE);
		}
	}

	//防御中
	if (get_defense() > 0) {
		DrawBox(x + x_size, y, x + x_size + 10, y + y_size, LIGHT_BLUE2, TRUE);
	}
	if (debuff_time > 0) {
		DrawRotaGraph2(x, y, 0, 0, DOT_EXTEND, 0, debuff_handle, TRUE, FALSE);
	}
	//DrawFormatString(10, 10, GetColor(0, 0, 0), "x = %d", x);
	//DrawFormatString(10, 40, GetColor(0, 0, 0), "y_size = %d", y_size);
}

//AI
void Student::ai() {
	switch (kind) {
	case HANDA:
		ai_attack(100, 80, 30);
		ai_normal(30, 40, 30, 100, 0);
		break;
	case OOTA:
		ai_attack(100, 100, 90);
		ai_normal(0, 20, 80, 50, 0);
		break;
	case ISHI:
		ai_attack(100, 90, 70);
		ai_normal(20, 60, 20, 130, 0);
		break;
	case TAIRA:
		ai_attack(100, 100, 100);
		ai_normal(100, 0, 0, 100, 0);
		break;
	case OWADA:
		ai_attack(100, 70, 60);
		ai_normal(10, 60, 30, 50, 0);
		break;
	case MIYATONE:
		ai_attack(100, 50, 10);
		ai_normal(10, 30, 60, 30, 0);
		break;
	case UCHIMOTO:
		ai_attack(100, 100, 100);
		ai_normal(0, 40, 60, 130, 0);
		break;
	case IBARAKAWA:
		ai_attack(100, 90, 90);
		ai_normal(30, 40, 30, 180, 0);
		break;
	case TOMOCHI:
		ai_attack(100, 100, 50);
		ai_normal(20, 40, 40, 50, 0);
		break;
	}
}

void Student::ai_attack(int left, int middle, int right) {
	if (attack_cnt > 0) {//攻撃中
		attack_cnt--;
		input_right = 0;
		input_left = 1;
	}
	else if (attack_cnt < 0) {//防御中
		attack_cnt++;
		input_right = 1;
		input_left = 0;
	}
	else {
		input_right = 0;
		input_left = 0;
		int r = GetRand(99);
		int i = MAX_INTERVAL - weapon->get_interval() + 1;
		if (x < 180 && r < left) {
			attack_cnt = GetRand(5) * i;
		}
		else if (x >= 180 && x < 360 && r < middle) {
			attack_cnt = GetRand(5) * i;
		}
		else if(x >= 360 && r < right){
			attack_cnt = GetRand(5) * i;
		}
		else {//防御
			attack_cnt = -1 * (GetRand(60) + 30);
		}
	}
}

void Student::input_reset() {
	input_stick = FALSE;
	input_stick2 = FALSE;
	input_left = FALSE;
	input_right = FALSE;
	input_e = FALSE;
	debuff_time = 0;//デバフを終了
	weapon->reset();//弾をすべて消す
}

//回復するターゲットを設定
void Student::set_target(int n) {
	target = n;
}

//回復するターゲットを取得
int Student::get_target() {
	return target;
}

//味方に回復してもらう
void Student::cure_hp() {
	hp = max_hp;
	heart = true;
	icon_cnt = 30;
}

//味方に回復薬を使用
bool Student::cure_friend(Student& s) {
	if (hp <= 0) {//自身がやられている
		return false;
	}
	//回復されている途中
	if (heart == false && icon_cnt > 0) {
		return false;
	}
	//回復ボタンを押していない、または相手の体力がマックス、または回復薬がない
	if (input_e != CURE_KEY || s.get_hp() <= 0 || s.get_hp() == s.get_max_hp() || item == 0) {
		return false;
	}
	else {
		item--;
		s.cure_hp();
		heart = false;
		icon_cnt = 30;
	}
	return true;
}

const int CURE_RATE = 25;//HPが何％以下なら回復するか
//仲間を回復するか判断するAI
void Student::cure_ai(int n, Student& s) {
	if (hp <= 0) { return; }//自身がやられたなら回復できない
	if (s.get_hp() * 100 / s.get_max_hp() <= CURE_RATE && item > 0 && GetRand(99) < 3) {
		target = n;
		input_e = CURE_KEY;
	}
}

//回復の中止
void Student::cure_reset() {
	target = -1;
	input_e = FALSE;
}


/////////敵///////

Enemy::Enemy(int k, int sx, int sy, int lev) {
	//RUN_SPEED = 6;
	hp_dx = 0;
	kind = k;//敵の種類
	create_weapon();
	x = sx;
	next_x = x;
	y = sy;
	next_y = y;
	level = lev;
	defense = 0;//防御力デフォルトは50 敵は0
	//ここでレベルに応じてステータスを設定
	level_set();
	hp = max_hp;//デフォルトは100
	weapon->cus(level);//武器の強さ調整
	load_dot();//ドット絵のロード

	debuff_handle = LoadGraph("picture/デバフ.png");
}

Enemy::~Enemy() {

}

//敵の種類に応じて武器を用意 & HP設定
void Enemy::create_weapon() {
	weapon = new Weapon(WEAPON_SUM + kind + 1);
	max_hp = en_hp[kind];
	//キャラ別のあたり判定調整 (見た目よりあたり判定を緩くする)
	switch (kind) {
	case TAKO:
		RUN_SPEED = 9;
		break;
	case TESSIE:
		x_size = (37 * x_size) / 50;
		hp_dx = 50;
		break;
	case UM01B:
		x_size = (37 * x_size) / 50;
		break;
	}
}

//レベルに応じてステータス調整
void Enemy::level_set() {
	max_hp += level * ENEMY_HP_UP;
}

//ドット絵のロード
void Enemy::load_dot() {
	string name = "picture/enemy/";
	name += en_name[kind];
	run[0] = LoadGraph((name + "1.png").c_str());
	run[1] = LoadGraph((name + "2.png").c_str());
	GetGraphSize(run[0], &x_size, &y_size);
	x_size *= DOT_EXTEND;
	y_size *= DOT_EXTEND;
}


//HPのバーの高さ
const int ENEMY_HP_HEIGHT = 20;
//敵のドット絵を描画
void Enemy::draw(bool bright_flag) {
	draw_dot(bright_flag);//キャラのドット絵
	//HP表示
	int border = x_size * hp / max_hp;
	DrawBox(x + hp_dx , y - ENEMY_HP_HEIGHT, x + border + hp_dx, y - 10, GREEN, TRUE);
	DrawBox(x + border + hp_dx, y - ENEMY_HP_HEIGHT, x + x_size + hp_dx, y - 10, RED, TRUE);
	if (debuff_time > 0) {//デバフエフェクト
		DrawRotaGraph2(x, y, 0, 0, DOT_EXTEND, 0, debuff_handle, TRUE, FALSE);
	}
	//DrawFormatString(10, 10, GetColor(0, 0, 0), "x = %d, y = %d", x, y);
	//DrawFormatString(10, 40, GetColor(0, 0, 0), "x_size = %d", x_size);
}

//背景移動の際に合わせて動かす用
void Enemy::dash(int dx) {
	x += dx;
	next_x += dx;
}

//移動限界座標
void Enemy::limit() {
	if (x < GAME_WIDE / 2) { 
		x = GAME_WIDE / 2;
		next_x = x;
	}
	if (x + x_size > GAME_WIDE) {
		x = GAME_WIDE - x_size;
		next_x = x;
	}
	if (y + y_size < 265) {//120
		y = 265 - y_size;
		next_y = y;
	}
	if (y + y_size > 645) {//500
		y = 645 - y_size;
		next_y = y;
	}
}

//AI
void Enemy::ai() {
	input_left = 1;
	ai_normal(30, 30, 30, 70, GAME_WIDE / 2);
}

int Enemy::get_money() {
	return drop_money[kind];
}

int Enemy::get_drop() {
	return drop_item[kind];
}