#include "Shop.h"
#include "Button.h"
#include "Weapon.h"
#include "Control.h"
#include "DxLib.h"
#include "SDvalue.h"

#include<string>
#include<sstream>
using namespace std;

Shop::Shop() {
	//フォント
	font_handle = CreateFontToHandle(NULL, 50, 10);
	font_small_handle = CreateFontToHandle(NULL, 30, 8);
	//ボタン
	button_cancel = new Button("戻る", 30, 20, 150, 80, LIGHT_GREEN, GREEN, font_handle, BLACK);
	ostringstream n;
	n << "回復薬×１　" << ITEM_VALUE << "pt";
	button_item = new Button(n.str().c_str(), 40, 150, 550, 100, LIGHT_BLUE, BLUE, font_small_handle, BLACK);
	for (int i = 0; i < 10; i++) {//自衛用武器
		wi[i] = new WeaponInfo(i + 1);
		string s;
		wi[i]->get_name(s);
		button_weapon[i] = new Button(s.c_str() , 40, 400 + (i * 52), 450, 40, LIGHT_YELLOW, YELLOW, font_small_handle, BLACK);
	}
	for (int i = 0; i < 10; i++) {//レア武器
		wi2[i] = new WeaponInfo(40 + i + 1);
		string s;
		wi2[i]->get_name(s);
		button_weapon2[i] = new Button(s.c_str(), 40, 400 + (i * 52), 450, 40, LIGHT_YELLOW, YELLOW, font_small_handle, BLACK);
	}
	button_change = new Button("ページ切り替え", 65, 930, 400, 60, GRAY2, RED, font_handle, BLACK);
	//武器　各○○pt の文字列作成
	ostringstream p;
	p << "武器　各" << WEAPON_VALUE << "pt";
	weapon_str = p.str().c_str();
	ostringstream p2;
	p2 << "武器　各" << WEAPON_VALUE2 << "pt";
	weapon_str2 = p2.str().c_str();
	//効果音
	money_sound = LoadSoundMem("sound/システム音/money.mp3");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, money_sound);
	decide_sound = LoadSoundMem("sound/システム音/決定.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, decide_sound);
}
Shop::~Shop() {
	DeleteFontToHandle(font_handle);
	DeleteFontToHandle(font_small_handle);
	delete button_cancel;
	delete button_item;
	for (int i = 0; i < 10; i++) {
		delete wi[i];
		delete button_weapon[i];
		delete wi2[i];
		delete button_weapon2[i];
	}
	delete button_change;
	DeleteSoundMem(money_sound);
	DeleteSoundMem(decide_sound);
}

bool Shop::play() {
	GetMousePoint(&hand_x, &hand_y);
	//カーソルが武器ボタンに重なっているか確認
	focus = -1;
	for (int i = 0; i < 10; i++) {
		if (button_weapon[i]->overlap(hand_x, hand_y)) {
			focus = i;
		}
	}
	//左クリックされたとき
	if (left_click() == 1) {
		//戻るボタン
		if (button_cancel->overlap(hand_x, hand_y)) {
			return true;
		}
		//回復薬購入
		if (button_item->overlap(hand_x, hand_y)) {
			if (money >= ITEM_VALUE && item_num < 99) {
				money -= ITEM_VALUE;
				item_num++;
				PlaySoundMem(money_sound, DX_PLAYTYPE_BACK);//効果音
			}
		}
		//武器切り替え
		if (button_change->overlap(hand_x, hand_y)) {
			rare_flag = !rare_flag;
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//効果音
		}
		//武器購入
		if (focus != -1) {
			if (!rare_flag) {//自衛用武器購入
				if (money >= WEAPON_VALUE) {
					money -= WEAPON_VALUE;
					weapon_list[focus]++;
					PlaySoundMem(money_sound, DX_PLAYTYPE_BACK);//効果音
				}
			}
			else {//レア武器購入
				if (money >= WEAPON_VALUE2) {
					money -= WEAPON_VALUE2;
					weapon_list[40 + focus]++;
					PlaySoundMem(money_sound, DX_PLAYTYPE_BACK);//効果音
				}
			}
		}
	}
	return false;
}

void Shop::draw() {
	//背景
	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, DARK_GREEN, TRUE);
	//戻るボタン
	button_cancel->draw(hand_x, hand_y);
	//アイテムボタン
	DrawBox(0, 120, GAME_WIDE, 270, GRAY, TRUE);
	ostringstream s;
	s << "現在" << item_num << "個所持（最大99個）";
	DrawStringToHandle(650, 170, s.str().c_str(), LIGHT_GREEN2, font_handle);
	button_item->draw(hand_x, hand_y);
	//所持金表示
	ostringstream m;
	m << "所持金：" << money << "pt";
	DrawStringToHandle(700, 20, m.str().c_str(), YELLOW, font_handle);
	//ページ切り替えボタン
	button_change->draw(hand_x, hand_y);

	//武器ボタン
	DrawStringToHandle(500, 350, "所持数", BLACK, font_small_handle);
	if (!rare_flag) {//自衛用武器の画面
		//武器　各○○pt と表示する
		DrawStringToHandle(40, 340, weapon_str.c_str(), LIGHT_GREEN2, font_handle);
		ostringstream n;
		for (int i = 0; i < 10; i++) {
			button_weapon[i]->draw(hand_x, hand_y);
			n << weapon_list[i] << endl;
		}
		//武器の所持数
		DrawStringToHandle(515, 395, n.str().c_str(), LIGHT_GREEN2, font_handle);
		//武器詳細
		if (focus != -1) {
			wi[focus]->draw(650, 520, LIGHT_YELLOW, font_small_handle, DARK_RED);
		}
	}
	else {//レア武器の画面
		//武器　各○○pt と表示する
		DrawStringToHandle(40, 340, weapon_str2.c_str(), LIGHT_GREEN2, font_handle);
		ostringstream n;
		for (int i = 0; i < 10; i++) {
			button_weapon2[i]->draw(hand_x, hand_y);
			n << weapon_list[40 + i] << endl;
		}
		//武器の所持数
		DrawStringToHandle(515, 395, n.str().c_str(), LIGHT_GREEN2, font_handle);
		//武器詳細
		if (focus != -1) {
			wi2[focus]->draw(650, 520, LIGHT_YELLOW, font_small_handle, DARK_RED);
		}
	}
}