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
	//tHg
	font_handle = CreateFontToHandle(NULL, 50, 10);
	font_small_handle = CreateFontToHandle(NULL, 30, 8);
	//{^
	button_cancel = new Button("ßé", 30, 20, 150, 80, LIGHT_GREEN, GREEN, font_handle, BLACK);
	ostringstream n;
	n << "ñò~P@" << ITEM_VALUE << "pt";
	button_item = new Button(n.str().c_str(), 40, 150, 550, 100, LIGHT_BLUE, BLUE, font_small_handle, BLACK);
	for (int i = 0; i < 10; i++) {//©qpí
		wi[i] = new WeaponInfo(i + 1);
		string s;
		wi[i]->get_name(s);
		button_weapon[i] = new Button(s.c_str() , 40, 400 + (i * 52), 450, 40, LIGHT_YELLOW, YELLOW, font_small_handle, BLACK);
	}
	for (int i = 0; i < 10; i++) {//Aí
		wi2[i] = new WeaponInfo(40 + i + 1);
		string s;
		wi2[i]->get_name(s);
		button_weapon2[i] = new Button(s.c_str(), 40, 400 + (i * 52), 450, 40, LIGHT_YELLOW, YELLOW, font_small_handle, BLACK);
	}
	button_change = new Button("y[WØèÖ¦", 65, 930, 400, 60, GRAY2, RED, font_handle, BLACK);
	//í@ept Ì¶ñì¬
	ostringstream p;
	p << "í@e" << WEAPON_VALUE << "pt";
	weapon_str = p.str().c_str();
	ostringstream p2;
	p2 << "í@e" << WEAPON_VALUE2 << "pt";
	weapon_str2 = p2.str().c_str();
	//øÊ¹
	money_sound = LoadSoundMem("sound/VXe¹/money.mp3");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, money_sound);
	decide_sound = LoadSoundMem("sound/VXe¹/è.wav");
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
	//J[\ªí{^ÉdÈÁÄ¢é©mF
	focus = -1;
	for (int i = 0; i < 10; i++) {
		if (button_weapon[i]->overlap(hand_x, hand_y)) {
			focus = i;
		}
	}
	//¶NbN³ê½Æ«
	if (left_click() == 1) {
		//ßé{^
		if (button_cancel->overlap(hand_x, hand_y)) {
			return true;
		}
		//ñòwü
		if (button_item->overlap(hand_x, hand_y)) {
			if (money >= ITEM_VALUE && item_num < 99) {
				money -= ITEM_VALUE;
				item_num++;
				PlaySoundMem(money_sound, DX_PLAYTYPE_BACK);//øÊ¹
			}
		}
		//íØèÖ¦
		if (button_change->overlap(hand_x, hand_y)) {
			rare_flag = !rare_flag;
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//øÊ¹
		}
		//íwü
		if (focus != -1) {
			if (!rare_flag) {//©qpíwü
				if (money >= WEAPON_VALUE) {
					money -= WEAPON_VALUE;
					weapon_list[focus]++;
					PlaySoundMem(money_sound, DX_PLAYTYPE_BACK);//øÊ¹
				}
			}
			else {//Aíwü
				if (money >= WEAPON_VALUE2) {
					money -= WEAPON_VALUE2;
					weapon_list[40 + focus]++;
					PlaySoundMem(money_sound, DX_PLAYTYPE_BACK);//øÊ¹
				}
			}
		}
	}
	return false;
}

void Shop::draw() {
	//wi
	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, DARK_GREEN, TRUE);
	//ßé{^
	button_cancel->draw(hand_x, hand_y);
	//ACe{^
	DrawBox(0, 120, GAME_WIDE, 270, GRAY, TRUE);
	ostringstream s;
	s << "»Ý" << item_num << "ÂiÅå99Âj";
	DrawStringToHandle(650, 170, s.str().c_str(), LIGHT_GREEN2, font_handle);
	button_item->draw(hand_x, hand_y);
	//à\¦
	ostringstream m;
	m << "àF" << money << "pt";
	DrawStringToHandle(700, 20, m.str().c_str(), YELLOW, font_handle);
	//y[WØèÖ¦{^
	button_change->draw(hand_x, hand_y);

	//í{^
	DrawStringToHandle(500, 350, "", BLACK, font_small_handle);
	if (!rare_flag) {//©qpíÌæÊ
		//í@ept Æ\¦·é
		DrawStringToHandle(40, 340, weapon_str.c_str(), LIGHT_GREEN2, font_handle);
		ostringstream n;
		for (int i = 0; i < 10; i++) {
			button_weapon[i]->draw(hand_x, hand_y);
			n << weapon_list[i] << endl;
		}
		//íÌ
		DrawStringToHandle(515, 395, n.str().c_str(), LIGHT_GREEN2, font_handle);
		//íÚ×
		if (focus != -1) {
			wi[focus]->draw(650, 520, LIGHT_YELLOW, font_small_handle, DARK_RED);
		}
	}
	else {//AíÌæÊ
		//í@ept Æ\¦·é
		DrawStringToHandle(40, 340, weapon_str2.c_str(), LIGHT_GREEN2, font_handle);
		ostringstream n;
		for (int i = 0; i < 10; i++) {
			button_weapon2[i]->draw(hand_x, hand_y);
			n << weapon_list[40 + i] << endl;
		}
		//íÌ
		DrawStringToHandle(515, 395, n.str().c_str(), LIGHT_GREEN2, font_handle);
		//íÚ×
		if (focus != -1) {
			wi2[focus]->draw(650, 520, LIGHT_YELLOW, font_small_handle, DARK_RED);
		}
	}
}