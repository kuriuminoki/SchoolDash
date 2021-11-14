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
	//�t�H���g
	font_handle = CreateFontToHandle(NULL, 50, 10);
	font_small_handle = CreateFontToHandle(NULL, 30, 8);
	//�{�^��
	button_cancel = new Button("�߂�", 30, 20, 150, 80, LIGHT_GREEN, GREEN, font_handle, BLACK);
	ostringstream n;
	n << "�񕜖�~�P�@" << ITEM_VALUE << "pt";
	button_item = new Button(n.str().c_str(), 40, 150, 550, 100, LIGHT_BLUE, BLUE, font_small_handle, BLACK);
	for (int i = 0; i < 10; i++) {//���q�p����
		wi[i] = new WeaponInfo(i + 1);
		string s;
		wi[i]->get_name(s);
		button_weapon[i] = new Button(s.c_str() , 40, 400 + (i * 52), 450, 40, LIGHT_YELLOW, YELLOW, font_small_handle, BLACK);
	}
	for (int i = 0; i < 10; i++) {//���A����
		wi2[i] = new WeaponInfo(40 + i + 1);
		string s;
		wi2[i]->get_name(s);
		button_weapon2[i] = new Button(s.c_str(), 40, 400 + (i * 52), 450, 40, LIGHT_YELLOW, YELLOW, font_small_handle, BLACK);
	}
	button_change = new Button("�y�[�W�؂�ւ�", 65, 930, 400, 60, GRAY2, RED, font_handle, BLACK);
	//����@�e����pt �̕�����쐬
	ostringstream p;
	p << "����@�e" << WEAPON_VALUE << "pt";
	weapon_str = p.str().c_str();
	ostringstream p2;
	p2 << "����@�e" << WEAPON_VALUE2 << "pt";
	weapon_str2 = p2.str().c_str();
	//���ʉ�
	money_sound = LoadSoundMem("sound/�V�X�e����/money.mp3");
	ChangeVolumeSoundMem(255 * (SoundVolume) / 100, money_sound);
	decide_sound = LoadSoundMem("sound/�V�X�e����/����.wav");
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
	//�J�[�\��������{�^���ɏd�Ȃ��Ă��邩�m�F
	focus = -1;
	for (int i = 0; i < 10; i++) {
		if (button_weapon[i]->overlap(hand_x, hand_y)) {
			focus = i;
		}
	}
	//���N���b�N���ꂽ�Ƃ�
	if (left_click() == 1) {
		//�߂�{�^��
		if (button_cancel->overlap(hand_x, hand_y)) {
			return true;
		}
		//�񕜖�w��
		if (button_item->overlap(hand_x, hand_y)) {
			if (money >= ITEM_VALUE && item_num < 99) {
				money -= ITEM_VALUE;
				item_num++;
				PlaySoundMem(money_sound, DX_PLAYTYPE_BACK);//���ʉ�
			}
		}
		//����؂�ւ�
		if (button_change->overlap(hand_x, hand_y)) {
			rare_flag = !rare_flag;
			PlaySoundMem(decide_sound, DX_PLAYTYPE_BACK);//���ʉ�
		}
		//����w��
		if (focus != -1) {
			if (!rare_flag) {//���q�p����w��
				if (money >= WEAPON_VALUE) {
					money -= WEAPON_VALUE;
					weapon_list[focus]++;
					PlaySoundMem(money_sound, DX_PLAYTYPE_BACK);//���ʉ�
				}
			}
			else {//���A����w��
				if (money >= WEAPON_VALUE2) {
					money -= WEAPON_VALUE2;
					weapon_list[40 + focus]++;
					PlaySoundMem(money_sound, DX_PLAYTYPE_BACK);//���ʉ�
				}
			}
		}
	}
	return false;
}

void Shop::draw() {
	//�w�i
	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, DARK_GREEN, TRUE);
	//�߂�{�^��
	button_cancel->draw(hand_x, hand_y);
	//�A�C�e���{�^��
	DrawBox(0, 120, GAME_WIDE, 270, GRAY, TRUE);
	ostringstream s;
	s << "����" << item_num << "�����i�ő�99�j";
	DrawStringToHandle(650, 170, s.str().c_str(), LIGHT_GREEN2, font_handle);
	button_item->draw(hand_x, hand_y);
	//�������\��
	ostringstream m;
	m << "�������F" << money << "pt";
	DrawStringToHandle(700, 20, m.str().c_str(), YELLOW, font_handle);
	//�y�[�W�؂�ւ��{�^��
	button_change->draw(hand_x, hand_y);

	//����{�^��
	DrawStringToHandle(500, 350, "������", BLACK, font_small_handle);
	if (!rare_flag) {//���q�p����̉��
		//����@�e����pt �ƕ\������
		DrawStringToHandle(40, 340, weapon_str.c_str(), LIGHT_GREEN2, font_handle);
		ostringstream n;
		for (int i = 0; i < 10; i++) {
			button_weapon[i]->draw(hand_x, hand_y);
			n << weapon_list[i] << endl;
		}
		//����̏�����
		DrawStringToHandle(515, 395, n.str().c_str(), LIGHT_GREEN2, font_handle);
		//����ڍ�
		if (focus != -1) {
			wi[focus]->draw(650, 520, LIGHT_YELLOW, font_small_handle, DARK_RED);
		}
	}
	else {//���A����̉��
		//����@�e����pt �ƕ\������
		DrawStringToHandle(40, 340, weapon_str2.c_str(), LIGHT_GREEN2, font_handle);
		ostringstream n;
		for (int i = 0; i < 10; i++) {
			button_weapon2[i]->draw(hand_x, hand_y);
			n << weapon_list[40 + i] << endl;
		}
		//����̏�����
		DrawStringToHandle(515, 395, n.str().c_str(), LIGHT_GREEN2, font_handle);
		//����ڍ�
		if (focus != -1) {
			wi2[focus]->draw(650, 520, LIGHT_YELLOW, font_small_handle, DARK_RED);
		}
	}
}