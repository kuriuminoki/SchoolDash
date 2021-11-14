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
	std::string weapon_str;//"����@�e����pt"�̕�����
	std::string weapon_str2;//"���A����@�e����pt"�̕�����
	int hand_x, hand_y;
	int focus = -1;//�ǂ̕���{�^���ɃJ�[�\�����d�Ȃ��Ă��邩
	int font_handle;
	int font_small_handle;
	bool rare_flag = false;//���A����w����ʂȂ�true
	Button* button_cancel;//�߂�{�^��
	Button* button_item;//�񕜖�w���{�^��
	Button* button_weapon[10];//���q�p����w���{�^��
	Button* button_weapon2[10];//���A����w���{�^��
	Button* button_change;//����؂�ւ��{�^��
	WeaponInfo* wi[10];
	WeaponInfo* wi2[10];
	int money_sound;
	int decide_sound;
public:
	Shop();
	~Shop();
public:
	bool play();//���C���@�I����true
	void draw();//�`��
};

#endif