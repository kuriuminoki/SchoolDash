#include"DxLib.h"
#include"Dash.h"
#include"Chara.h"
#include"Weapon.h"
#include"Control.h"
#include"SDvalue.h"
#include"Button.h"
#include"Text.h"

#include<algorithm>
#include<sstream>

using namespace std;

//Y座標の小さい順にランク付けする
template <typename TYPE>
void y_sort(TYPE** a, int rank[], int size) {
	for (int i = 0; i < size; i++) {
		rank[i] = i;
	}
	for (int i = 0; i < size - 1; i++) {
		for (int j = i + 1; j < size; j++) {
			if (a[rank[i]]->get_y() > a[rank[j]]->get_y()) {
				int tmp = rank[i];
				rank[i] = rank[j];
				rank[j] = tmp;
			}
		}
	}
}

static const int ALL_HAIKEI = 1;//背景の画像の種類数

Dash::Dash(int start_wave, int st[], int weapon[], int item[], int dash_level[]) {
	//フォント
	FontHandle1 = CreateFontToHandle(NULL, 30, 8);
	FontHandle2 = CreateFontToHandle(NULL, 70, 10);

	//wave, 宝の座標
	wave = start_wave + 1;//以降、敵が全滅した際に＋１されていく
	treasure_x = 0;
	//生徒の準備
	sn = 0;
	//生徒の数を確認
	for (int i = 0; i < 3; i++) {
		if (st[i] >= 0) {
			sn++;
		}
	}
	//生徒の確保
	student = new Student*[sn];
	for (int i = 0; i < sn; i++) {
		student[i] = new Student(st[i], 100, 150 + (i * 150), weapon[i], item[i], dash_level[i]);
	}
	//初日はチュートリアルも
	if (year == 1 && month == 4 && week == 1) {
		te = new TextEvent(TEXT_STORY, 0, false);
		state = EVENT;
	}
	else {
		state = RUN;
	}

	//敵の準備
	enemy_create();

	//最初のBGM
	ostringstream mn;
	mn << "music/battle" << GetRand(4) + 1 << ".ogg";
	change_music(mn.str().c_str(), false, true);

	//ボタン
	button_decide = new Button("探索をやめる", 280, 400, 300, 200, LIGHT_YELLOW, YELLOW, FontHandle1, BLACK);
	button_cancel = new Button("探索を続ける", 700, 400, 300, 200, LIGHT_BLUE, BLUE, FontHandle1, BLACK);

	//効果音
	defense_handle = LoadSoundMem("sound/atari/防御.wav");
	ChangeVolumeSoundMem(255 * SoundVolume / 100, defense_handle);
	item_sound = LoadSoundMem("sound/atari/回復.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume + 10) / 100, item_sound);

	//背景の座標と種類の並び
	x = 0;
	now = 0; next = 0;
	
	//最後に報酬で受け取るもの
	for (int i = 0; i <sn; i++) {
		stand_handle[i] = -1;
	}
	dash_money = 0;//お金
	dash_exp = 0;
	for (int i = 1; i < sn; i++) {//親密度　アカツキは除く
		dash_love[i] = 0;
	}
	dash_weapon = new vector<int>(WEAPON_SUM, 0);
	dash_material = new vector<int>(MATERIAL_SUM, 0);

	//非同期読み込み
	SetUseASyncLoadFlag(TRUE);
	//背景、エフェクト、回復薬や宝箱の画像
	haikei_handle = new int[ALL_HAIKEI];
	haikei_handle[0] = LoadGraph("picture/背景1.png");
	szangeki_handle = LoadGraph("picture/生徒斬撃.png");
	ezangeki_handle = LoadGraph("picture/敵斬撃.png");
	item_handle = LoadGraph("picture/回復薬.png");
	treasure_handle = LoadGraph("picture/宝箱.png");
	//音楽
	load_music_final();
	SetUseASyncLoadFlag(FALSE);
	//非同期読み込み終了
}

Dash::~Dash() {
	//生徒の削除
	for (int i = 0; i < sn; i++) {
		delete student[i];
	}
	delete[] student;
	//敵の削除
	while (!enemy.empty()) {
		delete enemy.front();
		enemy.pop();
	}
	//その他
	for (int i = 0; i < ALL_HAIKEI; i++) {
		DeleteGraph(haikei_handle[i]);
	}
	delete[] haikei_handle;
	DeleteFontToHandle(FontHandle1);
	DeleteFontToHandle(FontHandle2);
	DeleteGraph(szangeki_handle);
	DeleteGraph(ezangeki_handle);
	DeleteGraph(item_handle);
	DeleteGraph(treasure_handle);
	DeleteSoundMem(defense_handle);
	DeleteSoundMem(item_sound);
	DeleteSoundMem(final_music);
	delete button_decide;
	delete button_cancel;
	for (int i = 0; i < sn; i++) {
		DeleteGraph(stand_handle[i]);
	}
	delete dash_weapon;
	delete dash_material;
	change_music("", false, true);
}

//次の状況を決める
void Dash::change_state() {
	create_event();//イベント発生（するかも）
	int r = GetRand(99) + 1;
	if (r <= GET_RAND) {//宝
		treasure_x = 1280 + 700;
	}
	else {//戦闘
		enemy_create();
	}
}

//アカツキ以外のメンバーごとにイベントの抽選
void Dash::create_event() {
	//やられているならなし
	if (dead()) { return; }
	string file_name = "";
	int event_num = GetRand(2) + 1;//イベントの番号(1～3)
	int n[2] = { -1, -1 };//一人目、二人目の生徒番号
	for (int i = 1; i < sn; i++) {
		if (GetRand(99) < EVENT_RAND) {
			n[i - 1] = student[i]->get_kind();
			//親密度上昇
			dash_love[i] += LOVE_UP;
		}
	}
	//キャラの番号でソート
	if (n[0] > n[1]) { swap(n[0], n[1]); }
	//３人での会話イベントなら１種類のみ
	if (n[1] != -1 && n[0] != -1) { event_num = 1; }
	//フォルダ名を特定
	for (int i = 0; i < 2; i++) {
		if (n[i] != -1) {
			file_name += st_name[n[i]];
		}
	}
	if (file_name != "") {
		te = new TextEvent(TEXT_BATTLE, event_num, false, file_name);
		state = EVENT;
	}
}

//探索終了時にtrue
bool Dash::play() {
	//処理フェーズ
	//音楽終了時
	if (!pause_flag && state != EVENT && state != GET && final_flag && !check_music()) {
		final_cnt++;
		if (final_cnt == 60) {
			return true;
		}
		return false;
	}
	//やられたとき
	if (dead()) {
		cnt++;//ゲームオーバーまでのカウント
		if (cnt == 60) {
			cnt = 0;
			return true;//探索終了
		}
		return false;//以降は無視
	}
	GetMousePoint(&hand_x, &hand_y);
	if (control_q() == TRUE && (state == BATTLE || state == RUN)) {
		pause_flag = !pause_flag;//一時停止
		if (final_flag) {
			if (pause_flag) {
				StopSoundMem(music);
			}
			else {
				PlaySoundMem(music, DX_PLAYTYPE_BACK, FALSE);
			}
		}
	}
	if (pause_flag) {//一時停止中　
		if (left_click() == 1) {
			if (button_decide->overlap(hand_x, hand_y)) {
				return true;//探索終了
			}
			if (button_cancel->overlap(hand_x, hand_y)) {
				pause_flag = false;
			}
		}
		if (!pause_flag && final_flag) {
			PlaySoundMem(music, DX_PLAYTYPE_BACK, FALSE);
		}
		return false;
	}//以降は一時停止中は無視

	//ゲットした素材の表示処理
	if (!que_material.empty()) {
		material_cnt++;
		if (material_cnt >= MATERIAL_CNT) {
			material_cnt = 0;
			que_material.pop();
		}
	}
	switch (state) {
	case RUN:
		move();//背景の移動
		break;
	case BATTLE://バトル
		if (battle() && state != EVENT) {
			state = RUN;
		}
		break;
	case EVENT:
		//なにもしない　描画関数に任せる
		break;
	case GET:
		get();
		break;
	}
	return false;
}

//描画
void Dash::draw() {
	//一時停止中は暗くなる
	if (pause_flag) { SetDrawBright(100, 100, 100); }
	//背景
	DrawRotaGraph2(x, 0, 0, 0, 1.0, 0, haikei_handle[now], TRUE, FALSE);
	DrawRotaGraph2(1280 + x, 0, 0, 0, 1.0, 0, haikei_handle[next], TRUE, FALSE);
	//陣地の境界線
	if (state == BATTLE) { DrawBox(638, 245, 642, 650, YELLOW, TRUE); }
	//生徒、敵、弾の描画
	draw_chara();
	if (treasure_x > 0) {//宝箱
		DrawRotaGraph2(treasure_x, 350, 0, 0, DOT_EXTEND, 0, treasure_handle, TRUE, FALSE);
	}
	//生徒の情報を下に表示
	draw_state();
	if(pause_flag){ //一時停止中
		SetDrawBright(255, 255, 255);
		button_decide->draw(hand_x, hand_y);
		button_cancel->draw(hand_x, hand_y);
		DrawBox(200, 680, 1080, 920, LIGHT_GREEN, TRUE);
		DrawStringToHandle(220, 700, "移動：ＷＡＳＤキー", BLACK, FontHandle1);
		DrawStringToHandle(220, 750, "攻撃：左クリック（長押し）", BLACK, FontHandle1);
		DrawStringToHandle(220, 800, "防御：右クリック（長押し）", BLACK, FontHandle1);
		DrawStringToHandle(220, 850, "回復：下のキャラステータスにカーソルを合わせてＥキー", BLACK, FontHandle1);
	}
	//宝ゲット時
	if (state == GET) {
		DrawBox(195, 45, 1085, 205, YELLOW, TRUE);
		DrawBox(200, 50, 1080, 200, WHITE, TRUE);
		DrawStringToHandle(220, 70, (treasure_name + "を手に入れた！").c_str(), BLACK, FontHandle1);
		DrawStringToHandle(770, 155, "左クリックで閉じる", BLACK, FontHandle1);
	}
	//イベント
	if (state == EVENT) {
		cnt++;//即イベントが始まるわけではない
		if (que_material.empty() && cnt > 30 && te->play()) {
			delete te;//終了
			cnt = 0;
			state = RUN;
		}
	}
	if (final_cnt != 0) {
		DrawStringToHandle(350, max(300, 400 - final_cnt*final_cnt), "探索終了！", YELLOW, FontHandle2);
	}
}

//宝ゲット
bool Dash::get() {
	if (left_click() == 1) {//終了、次へ
		treasure_x = -1;
		state = RUN;
		enemy_create();//次は必ず戦闘
		return true;
	}
	return false;
}

//宝箱の中身を決定
void Dash::decide_treasure() {
	ostringstream s;
	int r = GetRand(999);
	if (wave >= 51 && r < 50) {//ダイヤモンド武器
		(*dash_weapon)[50 + (r % 10)]++;
		treasure_name = rare_weapon_name[r % 10];
	}
	else {
		int r2 = GetRand(1000);
		int n = GetRand(wave) + 1;
		if (r2 < 800) {//素材 0 or 1
			(*dash_material)[r2 % 2] += n / 3 + wave / 10 + 1;
			treasure_name = material_name[r2 % 2];
			s << "×" << n / 3 + wave / 10 + 1;
			treasure_name += s.str();
		}
		else {//お金
			n++;//0ptになるのを防ぐ
			dash_money += 50 * n * n / 100;
			s << 50 * n * n / 100;
			treasure_name = s.str();
			treasure_name += "pt";
		}
	}
}

//バトル
bool Dash::battle() {
	//AI
	bool item_flag = false;//誰か一人でも回復しようとしたならtrue
	for (int i = 1; i < sn; i++) {
		student[i]->ai();//AI
		student[i]->cure_reset();
		if (item_flag) { continue; }//回復は同時に複数人がやらない
		for (int j = 0; j < sn; j++) {
			if (i == j) { continue; }//自分自身は回復できない
			student[i]->cure_ai(j, *student[j]);
		}
		if (student[i]->get_target() != -1) { item_flag = true; }
	}
	//自分のキーボード入力
	student[0]->control();
	//生徒の処理
	for (int i = 0; i < sn; i++) {
		if (student[i]->get_hp() > 0) {
			student[i]->move();//移動
			student[i]->limit();//移動制限
			student[i]->attack();//弾発射
		}
		student[i]->attack_limit();//弾の移動と場外判定
		student[i]->attack_push(draw_que);//描画準備
		for (int j = 0; j < en; j++) {//敵との当たり判定
			Enemy* e = enemy.front(); enemy.pop();
			if (student[i]->atari(*e)) {
				PlaySoundMem(defense_handle, DX_PLAYTYPE_BACK);
			}
			enemy.push(e);
		}
		//回復する
		if (student[i]->get_target() != -1) {
			if (student[i]->cure_friend(*(student[student[i]->get_target()]))) {
				//回復成功で効果音
				PlaySoundMem(item_sound, DX_PLAYTYPE_BACK);
				//親密度アップ 採用するかは未定
				dash_love[student[i]->get_target()] += LOVE_UP;
			}
		}
	}
	//敵の処理
	for (int i = 0; i < en; i++) {
		Enemy* e = enemy.front(); enemy.pop();
		e->ai();//AI
		e->move();//移動
		e->limit();//移動制限
		e->attack();//弾発射
		e->attack_limit();//弾の移動と場外判定
		e->attack_push(draw_que);//描画準備
		for (int j = 0; j < sn; j++) {//生徒との当たり判定
			if (e->atari(*student[j])) {
				PlaySoundMem(defense_handle, DX_PLAYTYPE_BACK);
			}
		}
		enemy.push(e);
	}
	enemy_dead();//死んだ敵を削除
	if (enemy.empty()) {//敵が全滅したとき
		//経験値獲得
		dash_exp += wave * EXP_RATE;
		//次のウェーブへ
		wave++; now_wave++;
		//親密度アップ 採用するかは未定
		dash_love[1] += LOVE_UP; dash_love[2] += LOVE_UP;
		//BGMが歌ものになる
		if (now_wave == 1) { 
			change_load_music(final_music, true, false);
			final_flag = true;
		}
		for (int i = 0; i < sn; i++) {
			student[i]->input_reset();//入力状態をリセット
		}
		change_state();
		return true;
	}
	return false;
}

//やられた生徒がいるかどうか調べる
bool Dash::dead() {
	for (int i = 0; i < sn; i++) {
		if (student[i]->get_hp() <= 0) {
			return true;
		}
	}
	return false;
}

//死んだ敵の処理
void Dash::enemy_dead() {
	for (int i = 0; i < en; i++) {
		Enemy* e = enemy.front(); enemy.pop();
		if (e->get_hp() > 0) {//まだやられていない
			enemy.push(e);
		}
		else {//やられた
			dash_money += e->get_money();//お金をゲット
			(*dash_material)[e->get_drop()]++;
			que_material.push(e->get_drop());
			delete e;//メモリの開放
		}
	}
	en = (int)enemy.size();
}

//敵の出現
void Dash::enemy_create() {
	int enemy_kind = 0;//敵の種類
	//ボス
	if (wave % 10 == 0) {
		if (wave / 10 % 5 == 1) {
			enemy_kind = TESSIE;
		}
		if (wave / 10 % 5 == 2) {
			enemy_kind = GU;
		}
		if (wave / 10 % 5 == 3) {
			enemy_kind = UM01B;
		}
		if (wave / 10 % 5 == 4) {
			enemy_kind = UM02B;
		}
		if (wave / 10 % 5 == 0) {
			enemy_kind = UM03B;
		}
		//敵のレベル＝Wave数
		Enemy* e = new Enemy(enemy_kind, 1280 + 700, 350, wave);
		enemy.push(e);
	}
	else {//ボス以外
		en = 3;//敵の数
		//weveによって出現する敵の種類数が違う
		if (wave < 10) {
			enemy_kind = HANG;
		}
		else if (wave < 20) {
			enemy_kind = TEHIRA;
		}
		else if (wave < 30) {
			enemy_kind = KUTI;
		}
		else {
			enemy_kind = UM03;
		}
		//敵を生成
		for (int i = 0; i < en; i++) {
			//敵のレベル＝Wave数
			Enemy* e = new Enemy(GetRand(enemy_kind), 1280 + 700, 200 + (i * 150), wave);
			enemy.push(e);
		}
	}
}

//背景の移動
bool Dash::move() {
	x -= 6;
	//生徒
	for (int i = 0; i < sn; i++) {
		//生徒は走るモーション
		student[i]->run_cnt();
	}
	//敵
	for (int i = 0; i < en; i++) {
		Enemy* e = enemy.front(); enemy.pop();
		e->dash(-6);
		enemy.push(e);
	}
	//宝箱
	if (treasure_x > 0) {
		treasure_x -= 6;
	}
	//終了時
	if (x <= -1 * GAME_WIDE) {
		for (int i = 0; i < sn; i++) {
			student[i]->cnt_reset();
		}
		x = 0;
		now = next;
		next = 0;//ここで次の背景を決定
		
		//次の状況に移行
		if (!enemy.empty()) {
			state = BATTLE;
		}
		else if(treasure_x > 0){
			state = GET;
			//宝決定
			decide_treasure();
		}
		else {
			state = EVENT;
		}
		return true;
	}
	return false;
}


//生徒、敵、弾の描画
void Dash::draw_chara() {
	int* srank = new int[sn];
	int* erank = new int[en];
	y_sort(student, srank, sn);
	Enemy** e_array = new Enemy*[en];
	for (int i = 0; i < en; i++) {
		Enemy* e = enemy.front(); enemy.pop();
		e_array[i] = e;
		enemy.push(e);
	}
	y_sort(e_array, erank, en);

	//弾
	while (!draw_que.empty()) {
		const Bullet b = draw_que.top();
		//生徒
		for (int i = 0; i < sn; i++) {
			if (srank[i] != -1 && student[srank[i]]->get_cy() < b.get_y()) {
				student[srank[i]]->draw(!pause_flag);
				srank[i] = -1;
			}
		}
		//敵
		for (int i = 0; i < en; i++) {
			if (erank[i] != -1 && e_array[erank[i]]->get_cy() < b.get_y()) {
				e_array[erank[i]]->draw(!pause_flag);
				erank[i] = -1;
			}
		}
		//弾
		draw_que.pop();
		draw_bullet(b.get_kind(), b.get_x(), b.get_y(), b.get_dx(), b.get_dy());
		//DrawCircle(b.get_x(), b.get_y(), b.get_dx(), BLUE, TRUE);
	}
	//まだ描画していないなら
	//生徒
	for (int i = 0; i < sn; i++) {
		if(srank[i] != -1)
		student[srank[i]]->draw(!pause_flag);
	}
	//敵
	for (int i = 0; i < en; i++) {
		if (erank[i] != -1)
		e_array[erank[i]]->draw(!pause_flag);
	}

	delete[] e_array;
	delete[] srank;
	delete[] erank;
}

//生徒の情報を下に表示
//四角形の横幅は400, 間は両端20 これでちょうど合計1280
//縦は650～1024で、四角は670～1004
//武器と回復薬の四角はそれぞれ横幅170 間20 縦800～984
static const int HP_BAR_LEN = 360;//HPバーの長さ
void Dash::draw_state() {
	DrawBox(0, 650, 1280, 1024, GRAY, TRUE);
	int draw_x = 20;
	int border;
	char item_num[4] = { 'x', '0','0', '\0' };
	student[0]->set_target(-1);
	for (int i = 0; i < sn; i++) {
		if (i > 0 && hand_x > draw_x && hand_x < draw_x + 400 && hand_y > 670 && hand_y < 1004) {
			DrawBox(draw_x - 5, 670 - 5, draw_x + 400 + 5, 1004 + 5, WHITE, TRUE);
			student[0]->set_target(i);
		}
		DrawBox(draw_x, 670, draw_x + 400, 1004, LIGHT_BLUE, TRUE);
		//生徒の名前
		DrawBox(draw_x + 10, 680, draw_x + 390, 730, LIGHT_YELLOW2, TRUE);
		DrawStringToHandle(draw_x + 20, 690, st_name[student[i]->get_kind()], BLACK, FontHandle1);
		ostringstream w;
		w << "Lv " << level[student[i]->get_kind()];
		DrawStringToHandle(draw_x + 300, 690, w.str().c_str(), BLACK, FontHandle1);
		//HPバー表示
		draw_x += 20;
		border = HP_BAR_LEN * student[i]->get_hp() / student[i]->get_max_hp();
		DrawBox(draw_x, 745, draw_x + border, 785, GREEN, TRUE);
		DrawBox(draw_x + border, 745, draw_x + HP_BAR_LEN, 785, RED, TRUE);
		draw_x -= 20;
		//HPの数値表示
		ostringstream h;
		h << "ＨＰ： " << student[i]->get_hp() << " / " << student[i]->get_max_hp();
		DrawStringToHandle(draw_x + 20, 795, h.str().c_str(), BLACK, FontHandle1);
		//回復薬の数表示
		item_num[1] = '0' + student[i]->get_item() / 10;
		item_num[2] = '0' + student[i]->get_item() % 10;
		DrawBox(draw_x + 210, 850, draw_x + 380, 984, LIGHT_YELLOW, TRUE);
		DrawRotaGraph2(draw_x + 243, 845, 0, 0, DOT_EXTEND, 0, item_handle, TRUE, FALSE);
		DrawStringToHandle(draw_x + 267, 945, item_num, BLACK, FontHandle1);
		//武器の表示
		DrawBox(draw_x + 20, 850, draw_x + 190, 984, LIGHT_YELLOW, TRUE);
		DrawRotaGraph2(draw_x + 53, 865, 0, 0, DOT_EXTEND, 0, student[i]->get_weapon_dot(), TRUE, FALSE);
		draw_x += 20 + 400;//次のループに向けて
	}
	//獲得した素材を上に表示
	if (!que_material.empty()) {
		int my = 100 - min(material_cnt * material_cnt, 100);
		DrawBox(250, my, 980, my + 50, LIGHT_YELLOW, TRUE);
		ostringstream m;
		m << material_name[que_material.front()] << "を手に入れた！";
		DrawStringToHandle(270, my + 10, m.str().c_str(), BLACK, FontHandle1);
	}
	//現在のウェーブ数を左上に表示
	ostringstream s;
	s << "WAVE：" << wave;
	DrawBox(0, 0, 250, 50, GRAY2, TRUE);
	DrawStringToHandle(10, 10, s.str().c_str(), BLACK, FontHandle1);
	//操作説明
	DrawStringToHandle(5, 50, "Ｑキー：一時停止", BLACK, FontHandle1);
	//現在の報酬
	ostringstream r;
	r << "獲得金：" << dash_money << "pt" << endl <<  "経験値：" << dash_exp << endl;
	//r << "好感度１：" << dash_love[1] << "好感度２：" << dash_love[2];
	DrawBox(980, 0, GAME_WIDE, 80, GRAY2, TRUE);
	DrawStringToHandle(1000, 10, r.str().c_str(), DARK_YELLOW, FontHandle1);
}

void Dash::draw_bullet(int kind, int x, int y, int dx, int dy) {
	switch (kind) {
	case BLUE_BALL:
		DrawCircle(x, y, dx, BLUE, TRUE);
		break;
	case RED_BALL:
		DrawCircle(x, y, dx, RED, TRUE);
		break;
	case BLUE_ZAN:
		DrawModiGraph(x - dx, y - dy, x + dx, y - dy, x + dx, y + dy, x - dx, y + dy, szangeki_handle, TRUE);
		break;
	case RED_ZAN:
		DrawModiGraph(x + dx, y - dy, x - dx, y - dy, x - dx, y + dy, x + dx, y + dy, ezangeki_handle, TRUE);
		break;
	}
}

/////報酬を受け取る
bool Dash::play_reward() {
	//キャラの立ち絵を用意する
	if (stand_handle[0] == -1) {
		int k;
		string name;
		for (int i = 0; i < sn; i++) {
			k = student[i]->get_kind();
			name = "picture/stand/";
			name += st_name[k]; name += "/"; name += st_name[k];
			if (month >= 6 && month <= 10) { name += "夏"; }//衣替え
			stand_handle[i] = LoadGraph((name + "笑顔.png").c_str());
			if (stand_handle[i] == -1) {
				stand_handle[i] = LoadGraph((name + "通常.png").c_str());
			}
		}
	}
	//左クリック時
	if (left_click() == 1) {
		//ここで受け取る
		item_num = student[0]->get_item();//回復薬更新
		max_wave = max(max_wave, wave - 1);//Wave最高記録更新
		money += dash_money;//お金
		//経験値
		for (int i = 0; i < sn; i++) {
			int k = student[i]->get_kind();
			exp_sum[k] += dash_exp;
			level_up(k);//レベルアップ（できるなら）
		}
		//親密度　アカツキは除く
		for (int i = 1; i < sn; i++) {
			love[student[i]->get_kind()] += dash_love[i];
			if (love[student[i]->get_kind()] > LOVE_MAX) { love[student[i]->get_kind()] = LOVE_MAX; }
		}
		//武器
		for (unsigned int i = 0; i < WEAPON_SUM; i++) {
			weapon_list[i] += (*dash_weapon)[i];
		}
		//素材
		for (unsigned int i = 0; i < MATERIAL_SUM; i++) {
			material[i] += (*dash_material)[i];
		}
		return true;
	}
	return false;
}

void Dash::draw_reward() {
	//背景
	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, GRAY, TRUE);
	//キャラの立ち絵
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	for (int i = 0; i < sn; i++) {
		DrawRotaGraph2(-150 + (i * 400), -50, 0, 0, STAND_EXTEND, 0,stand_handle[i], TRUE, FALSE);
	}
	SetDrawMode(DX_DRAWMODE_NEAREST);
	int k;
	for (int i = 0; i < sn; i++) {
		k = student[i]->get_kind();
		if (exp_sum[k] + dash_exp >= need_exp(level[k]) && level[k] != LEVEL_MAX) {
			DrawBox(i * 400 + 120 - 3, 480 - 3, i * 400 + 370 + 3, 550 + 3, BLACK, TRUE);
			DrawBox(i * 400 + 120, 480, i * 400 + 370, 550, LIGHT_YELLOW, TRUE);
			DrawStringToHandle(140 + (i * 400), 500, "レベルアップ！", RED, FontHandle1);
		}
	}
	//現在の報酬
	ostringstream r1;
	ostringstream r2;
	r1 << "獲得金：" << dash_money;
	r2 << "経験値：" << dash_exp;
	DrawBox(0, 750, GAME_WIDE, 950, LIGHT_BLUE, TRUE);
	DrawStringToHandle(50, 800, r1.str().c_str(), YELLOW, FontHandle2);
	DrawStringToHandle(650, 800, r2.str().c_str(), DARK_GREEN, FontHandle2);
	DrawBox(0, 0, 550, 50, LIGHT_GREEN, TRUE);
	DrawStringToHandle(10, 10, "左クリックでセーブして終了", BLACK, FontHandle1);
}

void Dash::load_music_final() {
	const char* list[] = {
		"music/12345_full.mp3",
		"music/distance_full.mp3",
		"music/hana_full.mp3",
		"music/kobune_full.mp3",
		"music/noapusa_full.mp3",
		"music/skwya_full.mp3",
		"music/star_full.mp3",
		"music/zinnia_full.mp3"
	};
	final_music = LoadSoundMem(list[GetRand(7)]);
}