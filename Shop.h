#ifndef SHOP_H_INCLUDED
#define SHOP_H_INCLUDED
#include<string>
class Button;
class WeaponInfo;

class Shop {
private:
	static const int ITEM_VALUE = 500;
	static const int WEAPON_VALUE = 750;
	static const int WEAPON_VALUE2 = 10000;
	std::string weapon_str;//"武器　各○○pt"の文字列
	std::string weapon_str2;//"レア武器　各○○pt"の文字列
	int hand_x, hand_y;
	int focus = -1;//どの武器ボタンにカーソルが重なっているか
	int font_handle;
	int font_small_handle;
	bool rare_flag = false;//レア武器購入画面ならtrue
	Button* button_cancel;//戻るボタン
	Button* button_item;//回復薬購入ボタン
	Button* button_weapon[10];//自衛用武器購入ボタン
	Button* button_weapon2[10];//レア武器購入ボタン
	Button* button_change;//武器切り替えボタン
	WeaponInfo* wi[10];
	WeaponInfo* wi2[10];
	int money_sound;
	int decide_sound;
public:
	Shop();
	~Shop();
public:
	bool play();//メイン　終了時true
	void draw();//描画
};

#endif