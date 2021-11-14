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

//Y���W�̏��������Ƀ����N�t������
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

static const int ALL_HAIKEI = 1;//�w�i�̉摜�̎�ސ�

Dash::Dash(int start_wave, int st[], int weapon[], int item[], int dash_level[]) {
	//�t�H���g
	FontHandle1 = CreateFontToHandle(NULL, 30, 8);
	FontHandle2 = CreateFontToHandle(NULL, 70, 10);

	//wave, ��̍��W
	wave = start_wave + 1;//�ȍ~�A�G���S�ł����ۂɁ{�P����Ă���
	treasure_x = 0;
	//���k�̏���
	sn = 0;
	//���k�̐����m�F
	for (int i = 0; i < 3; i++) {
		if (st[i] >= 0) {
			sn++;
		}
	}
	//���k�̊m��
	student = new Student*[sn];
	for (int i = 0; i < sn; i++) {
		student[i] = new Student(st[i], 100, 150 + (i * 150), weapon[i], item[i], dash_level[i]);
	}
	//�����̓`���[�g���A����
	if (year == 1 && month == 4 && week == 1) {
		te = new TextEvent(TEXT_STORY, 0, false);
		state = EVENT;
	}
	else {
		state = RUN;
	}

	//�G�̏���
	enemy_create();

	//�ŏ���BGM
	ostringstream mn;
	mn << "music/battle" << GetRand(4) + 1 << ".ogg";
	change_music(mn.str().c_str(), false, true);

	//�{�^��
	button_decide = new Button("�T������߂�", 280, 400, 300, 200, LIGHT_YELLOW, YELLOW, FontHandle1, BLACK);
	button_cancel = new Button("�T���𑱂���", 700, 400, 300, 200, LIGHT_BLUE, BLUE, FontHandle1, BLACK);

	//���ʉ�
	defense_handle = LoadSoundMem("sound/atari/�h��.wav");
	ChangeVolumeSoundMem(255 * SoundVolume / 100, defense_handle);
	item_sound = LoadSoundMem("sound/atari/��.wav");
	ChangeVolumeSoundMem(255 * (SoundVolume + 10) / 100, item_sound);

	//�w�i�̍��W�Ǝ�ނ̕���
	x = 0;
	now = 0; next = 0;
	
	//�Ō�ɕ�V�Ŏ󂯎�����
	for (int i = 0; i <sn; i++) {
		stand_handle[i] = -1;
	}
	dash_money = 0;//����
	dash_exp = 0;
	for (int i = 1; i < sn; i++) {//�e���x�@�A�J�c�L�͏���
		dash_love[i] = 0;
	}
	dash_weapon = new vector<int>(WEAPON_SUM, 0);
	dash_material = new vector<int>(MATERIAL_SUM, 0);

	//�񓯊��ǂݍ���
	SetUseASyncLoadFlag(TRUE);
	//�w�i�A�G�t�F�N�g�A�񕜖��󔠂̉摜
	haikei_handle = new int[ALL_HAIKEI];
	haikei_handle[0] = LoadGraph("picture/�w�i1.png");
	szangeki_handle = LoadGraph("picture/���k�a��.png");
	ezangeki_handle = LoadGraph("picture/�G�a��.png");
	item_handle = LoadGraph("picture/�񕜖�.png");
	treasure_handle = LoadGraph("picture/��.png");
	//���y
	load_music_final();
	SetUseASyncLoadFlag(FALSE);
	//�񓯊��ǂݍ��ݏI��
}

Dash::~Dash() {
	//���k�̍폜
	for (int i = 0; i < sn; i++) {
		delete student[i];
	}
	delete[] student;
	//�G�̍폜
	while (!enemy.empty()) {
		delete enemy.front();
		enemy.pop();
	}
	//���̑�
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

//���̏󋵂����߂�
void Dash::change_state() {
	create_event();//�C�x���g�����i���邩���j
	int r = GetRand(99) + 1;
	if (r <= GET_RAND) {//��
		treasure_x = 1280 + 700;
	}
	else {//�퓬
		enemy_create();
	}
}

//�A�J�c�L�ȊO�̃����o�[���ƂɃC�x���g�̒��I
void Dash::create_event() {
	//����Ă���Ȃ�Ȃ�
	if (dead()) { return; }
	string file_name = "";
	int event_num = GetRand(2) + 1;//�C�x���g�̔ԍ�(1�`3)
	int n[2] = { -1, -1 };//��l�ځA��l�ڂ̐��k�ԍ�
	for (int i = 1; i < sn; i++) {
		if (GetRand(99) < EVENT_RAND) {
			n[i - 1] = student[i]->get_kind();
			//�e���x�㏸
			dash_love[i] += LOVE_UP;
		}
	}
	//�L�����̔ԍ��Ń\�[�g
	if (n[0] > n[1]) { swap(n[0], n[1]); }
	//�R�l�ł̉�b�C�x���g�Ȃ�P��ނ̂�
	if (n[1] != -1 && n[0] != -1) { event_num = 1; }
	//�t�H���_�������
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

//�T���I������true
bool Dash::play() {
	//�����t�F�[�Y
	//���y�I����
	if (!pause_flag && state != EVENT && state != GET && final_flag && !check_music()) {
		final_cnt++;
		if (final_cnt == 60) {
			return true;
		}
		return false;
	}
	//���ꂽ�Ƃ�
	if (dead()) {
		cnt++;//�Q�[���I�[�o�[�܂ł̃J�E���g
		if (cnt == 60) {
			cnt = 0;
			return true;//�T���I��
		}
		return false;//�ȍ~�͖���
	}
	GetMousePoint(&hand_x, &hand_y);
	if (control_q() == TRUE && (state == BATTLE || state == RUN)) {
		pause_flag = !pause_flag;//�ꎞ��~
		if (final_flag) {
			if (pause_flag) {
				StopSoundMem(music);
			}
			else {
				PlaySoundMem(music, DX_PLAYTYPE_BACK, FALSE);
			}
		}
	}
	if (pause_flag) {//�ꎞ��~���@
		if (left_click() == 1) {
			if (button_decide->overlap(hand_x, hand_y)) {
				return true;//�T���I��
			}
			if (button_cancel->overlap(hand_x, hand_y)) {
				pause_flag = false;
			}
		}
		if (!pause_flag && final_flag) {
			PlaySoundMem(music, DX_PLAYTYPE_BACK, FALSE);
		}
		return false;
	}//�ȍ~�͈ꎞ��~���͖���

	//�Q�b�g�����f�ނ̕\������
	if (!que_material.empty()) {
		material_cnt++;
		if (material_cnt >= MATERIAL_CNT) {
			material_cnt = 0;
			que_material.pop();
		}
	}
	switch (state) {
	case RUN:
		move();//�w�i�̈ړ�
		break;
	case BATTLE://�o�g��
		if (battle() && state != EVENT) {
			state = RUN;
		}
		break;
	case EVENT:
		//�Ȃɂ����Ȃ��@�`��֐��ɔC����
		break;
	case GET:
		get();
		break;
	}
	return false;
}

//�`��
void Dash::draw() {
	//�ꎞ��~���͈Â��Ȃ�
	if (pause_flag) { SetDrawBright(100, 100, 100); }
	//�w�i
	DrawRotaGraph2(x, 0, 0, 0, 1.0, 0, haikei_handle[now], TRUE, FALSE);
	DrawRotaGraph2(1280 + x, 0, 0, 0, 1.0, 0, haikei_handle[next], TRUE, FALSE);
	//�w�n�̋��E��
	if (state == BATTLE) { DrawBox(638, 245, 642, 650, YELLOW, TRUE); }
	//���k�A�G�A�e�̕`��
	draw_chara();
	if (treasure_x > 0) {//��
		DrawRotaGraph2(treasure_x, 350, 0, 0, DOT_EXTEND, 0, treasure_handle, TRUE, FALSE);
	}
	//���k�̏������ɕ\��
	draw_state();
	if(pause_flag){ //�ꎞ��~��
		SetDrawBright(255, 255, 255);
		button_decide->draw(hand_x, hand_y);
		button_cancel->draw(hand_x, hand_y);
		DrawBox(200, 680, 1080, 920, LIGHT_GREEN, TRUE);
		DrawStringToHandle(220, 700, "�ړ��F�v�`�r�c�L�[", BLACK, FontHandle1);
		DrawStringToHandle(220, 750, "�U���F���N���b�N�i�������j", BLACK, FontHandle1);
		DrawStringToHandle(220, 800, "�h��F�E�N���b�N�i�������j", BLACK, FontHandle1);
		DrawStringToHandle(220, 850, "�񕜁F���̃L�����X�e�[�^�X�ɃJ�[�\�������킹�Ăd�L�[", BLACK, FontHandle1);
	}
	//��Q�b�g��
	if (state == GET) {
		DrawBox(195, 45, 1085, 205, YELLOW, TRUE);
		DrawBox(200, 50, 1080, 200, WHITE, TRUE);
		DrawStringToHandle(220, 70, (treasure_name + "����ɓ��ꂽ�I").c_str(), BLACK, FontHandle1);
		DrawStringToHandle(770, 155, "���N���b�N�ŕ���", BLACK, FontHandle1);
	}
	//�C�x���g
	if (state == EVENT) {
		cnt++;//���C�x���g���n�܂�킯�ł͂Ȃ�
		if (que_material.empty() && cnt > 30 && te->play()) {
			delete te;//�I��
			cnt = 0;
			state = RUN;
		}
	}
	if (final_cnt != 0) {
		DrawStringToHandle(350, max(300, 400 - final_cnt*final_cnt), "�T���I���I", YELLOW, FontHandle2);
	}
}

//��Q�b�g
bool Dash::get() {
	if (left_click() == 1) {//�I���A����
		treasure_x = -1;
		state = RUN;
		enemy_create();//���͕K���퓬
		return true;
	}
	return false;
}

//�󔠂̒��g������
void Dash::decide_treasure() {
	ostringstream s;
	int r = GetRand(999);
	if (wave >= 51 && r < 50) {//�_�C�������h����
		(*dash_weapon)[50 + (r % 10)]++;
		treasure_name = rare_weapon_name[r % 10];
	}
	else {
		int r2 = GetRand(1000);
		int n = GetRand(wave) + 1;
		if (r2 < 800) {//�f�� 0 or 1
			(*dash_material)[r2 % 2] += n / 3 + wave / 10 + 1;
			treasure_name = material_name[r2 % 2];
			s << "�~" << n / 3 + wave / 10 + 1;
			treasure_name += s.str();
		}
		else {//����
			n++;//0pt�ɂȂ�̂�h��
			dash_money += 50 * n * n / 100;
			s << 50 * n * n / 100;
			treasure_name = s.str();
			treasure_name += "pt";
		}
	}
}

//�o�g��
bool Dash::battle() {
	//AI
	bool item_flag = false;//�N����l�ł��񕜂��悤�Ƃ����Ȃ�true
	for (int i = 1; i < sn; i++) {
		student[i]->ai();//AI
		student[i]->cure_reset();
		if (item_flag) { continue; }//�񕜂͓����ɕ����l�����Ȃ�
		for (int j = 0; j < sn; j++) {
			if (i == j) { continue; }//�������g�͉񕜂ł��Ȃ�
			student[i]->cure_ai(j, *student[j]);
		}
		if (student[i]->get_target() != -1) { item_flag = true; }
	}
	//�����̃L�[�{�[�h����
	student[0]->control();
	//���k�̏���
	for (int i = 0; i < sn; i++) {
		if (student[i]->get_hp() > 0) {
			student[i]->move();//�ړ�
			student[i]->limit();//�ړ�����
			student[i]->attack();//�e����
		}
		student[i]->attack_limit();//�e�̈ړ��Ə�O����
		student[i]->attack_push(draw_que);//�`�揀��
		for (int j = 0; j < en; j++) {//�G�Ƃ̓����蔻��
			Enemy* e = enemy.front(); enemy.pop();
			if (student[i]->atari(*e)) {
				PlaySoundMem(defense_handle, DX_PLAYTYPE_BACK);
			}
			enemy.push(e);
		}
		//�񕜂���
		if (student[i]->get_target() != -1) {
			if (student[i]->cure_friend(*(student[student[i]->get_target()]))) {
				//�񕜐����Ō��ʉ�
				PlaySoundMem(item_sound, DX_PLAYTYPE_BACK);
				//�e���x�A�b�v �̗p���邩�͖���
				dash_love[student[i]->get_target()] += LOVE_UP;
			}
		}
	}
	//�G�̏���
	for (int i = 0; i < en; i++) {
		Enemy* e = enemy.front(); enemy.pop();
		e->ai();//AI
		e->move();//�ړ�
		e->limit();//�ړ�����
		e->attack();//�e����
		e->attack_limit();//�e�̈ړ��Ə�O����
		e->attack_push(draw_que);//�`�揀��
		for (int j = 0; j < sn; j++) {//���k�Ƃ̓����蔻��
			if (e->atari(*student[j])) {
				PlaySoundMem(defense_handle, DX_PLAYTYPE_BACK);
			}
		}
		enemy.push(e);
	}
	enemy_dead();//���񂾓G���폜
	if (enemy.empty()) {//�G���S�ł����Ƃ�
		//�o���l�l��
		dash_exp += wave * EXP_RATE;
		//���̃E�F�[�u��
		wave++; now_wave++;
		//�e���x�A�b�v �̗p���邩�͖���
		dash_love[1] += LOVE_UP; dash_love[2] += LOVE_UP;
		//BGM���̂��̂ɂȂ�
		if (now_wave == 1) { 
			change_load_music(final_music, true, false);
			final_flag = true;
		}
		for (int i = 0; i < sn; i++) {
			student[i]->input_reset();//���͏�Ԃ����Z�b�g
		}
		change_state();
		return true;
	}
	return false;
}

//���ꂽ���k�����邩�ǂ������ׂ�
bool Dash::dead() {
	for (int i = 0; i < sn; i++) {
		if (student[i]->get_hp() <= 0) {
			return true;
		}
	}
	return false;
}

//���񂾓G�̏���
void Dash::enemy_dead() {
	for (int i = 0; i < en; i++) {
		Enemy* e = enemy.front(); enemy.pop();
		if (e->get_hp() > 0) {//�܂�����Ă��Ȃ�
			enemy.push(e);
		}
		else {//���ꂽ
			dash_money += e->get_money();//�������Q�b�g
			(*dash_material)[e->get_drop()]++;
			que_material.push(e->get_drop());
			delete e;//�������̊J��
		}
	}
	en = (int)enemy.size();
}

//�G�̏o��
void Dash::enemy_create() {
	int enemy_kind = 0;//�G�̎��
	//�{�X
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
		//�G�̃��x����Wave��
		Enemy* e = new Enemy(enemy_kind, 1280 + 700, 350, wave);
		enemy.push(e);
	}
	else {//�{�X�ȊO
		en = 3;//�G�̐�
		//weve�ɂ���ďo������G�̎�ސ����Ⴄ
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
		//�G�𐶐�
		for (int i = 0; i < en; i++) {
			//�G�̃��x����Wave��
			Enemy* e = new Enemy(GetRand(enemy_kind), 1280 + 700, 200 + (i * 150), wave);
			enemy.push(e);
		}
	}
}

//�w�i�̈ړ�
bool Dash::move() {
	x -= 6;
	//���k
	for (int i = 0; i < sn; i++) {
		//���k�͑��郂�[�V����
		student[i]->run_cnt();
	}
	//�G
	for (int i = 0; i < en; i++) {
		Enemy* e = enemy.front(); enemy.pop();
		e->dash(-6);
		enemy.push(e);
	}
	//��
	if (treasure_x > 0) {
		treasure_x -= 6;
	}
	//�I����
	if (x <= -1 * GAME_WIDE) {
		for (int i = 0; i < sn; i++) {
			student[i]->cnt_reset();
		}
		x = 0;
		now = next;
		next = 0;//�����Ŏ��̔w�i������
		
		//���̏󋵂Ɉڍs
		if (!enemy.empty()) {
			state = BATTLE;
		}
		else if(treasure_x > 0){
			state = GET;
			//�󌈒�
			decide_treasure();
		}
		else {
			state = EVENT;
		}
		return true;
	}
	return false;
}


//���k�A�G�A�e�̕`��
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

	//�e
	while (!draw_que.empty()) {
		const Bullet b = draw_que.top();
		//���k
		for (int i = 0; i < sn; i++) {
			if (srank[i] != -1 && student[srank[i]]->get_cy() < b.get_y()) {
				student[srank[i]]->draw(!pause_flag);
				srank[i] = -1;
			}
		}
		//�G
		for (int i = 0; i < en; i++) {
			if (erank[i] != -1 && e_array[erank[i]]->get_cy() < b.get_y()) {
				e_array[erank[i]]->draw(!pause_flag);
				erank[i] = -1;
			}
		}
		//�e
		draw_que.pop();
		draw_bullet(b.get_kind(), b.get_x(), b.get_y(), b.get_dx(), b.get_dy());
		//DrawCircle(b.get_x(), b.get_y(), b.get_dx(), BLUE, TRUE);
	}
	//�܂��`�悵�Ă��Ȃ��Ȃ�
	//���k
	for (int i = 0; i < sn; i++) {
		if(srank[i] != -1)
		student[srank[i]]->draw(!pause_flag);
	}
	//�G
	for (int i = 0; i < en; i++) {
		if (erank[i] != -1)
		e_array[erank[i]]->draw(!pause_flag);
	}

	delete[] e_array;
	delete[] srank;
	delete[] erank;
}

//���k�̏������ɕ\��
//�l�p�`�̉�����400, �Ԃ͗��[20 ����ł��傤�Ǎ��v1280
//�c��650�`1024�ŁA�l�p��670�`1004
//����Ɖ񕜖�̎l�p�͂��ꂼ�ꉡ��170 ��20 �c800�`984
static const int HP_BAR_LEN = 360;//HP�o�[�̒���
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
		//���k�̖��O
		DrawBox(draw_x + 10, 680, draw_x + 390, 730, LIGHT_YELLOW2, TRUE);
		DrawStringToHandle(draw_x + 20, 690, st_name[student[i]->get_kind()], BLACK, FontHandle1);
		ostringstream w;
		w << "Lv " << level[student[i]->get_kind()];
		DrawStringToHandle(draw_x + 300, 690, w.str().c_str(), BLACK, FontHandle1);
		//HP�o�[�\��
		draw_x += 20;
		border = HP_BAR_LEN * student[i]->get_hp() / student[i]->get_max_hp();
		DrawBox(draw_x, 745, draw_x + border, 785, GREEN, TRUE);
		DrawBox(draw_x + border, 745, draw_x + HP_BAR_LEN, 785, RED, TRUE);
		draw_x -= 20;
		//HP�̐��l�\��
		ostringstream h;
		h << "�g�o�F " << student[i]->get_hp() << " / " << student[i]->get_max_hp();
		DrawStringToHandle(draw_x + 20, 795, h.str().c_str(), BLACK, FontHandle1);
		//�񕜖�̐��\��
		item_num[1] = '0' + student[i]->get_item() / 10;
		item_num[2] = '0' + student[i]->get_item() % 10;
		DrawBox(draw_x + 210, 850, draw_x + 380, 984, LIGHT_YELLOW, TRUE);
		DrawRotaGraph2(draw_x + 243, 845, 0, 0, DOT_EXTEND, 0, item_handle, TRUE, FALSE);
		DrawStringToHandle(draw_x + 267, 945, item_num, BLACK, FontHandle1);
		//����̕\��
		DrawBox(draw_x + 20, 850, draw_x + 190, 984, LIGHT_YELLOW, TRUE);
		DrawRotaGraph2(draw_x + 53, 865, 0, 0, DOT_EXTEND, 0, student[i]->get_weapon_dot(), TRUE, FALSE);
		draw_x += 20 + 400;//���̃��[�v�Ɍ�����
	}
	//�l�������f�ނ���ɕ\��
	if (!que_material.empty()) {
		int my = 100 - min(material_cnt * material_cnt, 100);
		DrawBox(250, my, 980, my + 50, LIGHT_YELLOW, TRUE);
		ostringstream m;
		m << material_name[que_material.front()] << "����ɓ��ꂽ�I";
		DrawStringToHandle(270, my + 10, m.str().c_str(), BLACK, FontHandle1);
	}
	//���݂̃E�F�[�u��������ɕ\��
	ostringstream s;
	s << "WAVE�F" << wave;
	DrawBox(0, 0, 250, 50, GRAY2, TRUE);
	DrawStringToHandle(10, 10, s.str().c_str(), BLACK, FontHandle1);
	//�������
	DrawStringToHandle(5, 50, "�p�L�[�F�ꎞ��~", BLACK, FontHandle1);
	//���݂̕�V
	ostringstream r;
	r << "�l�����F" << dash_money << "pt" << endl <<  "�o���l�F" << dash_exp << endl;
	//r << "�D���x�P�F" << dash_love[1] << "�D���x�Q�F" << dash_love[2];
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

/////��V���󂯎��
bool Dash::play_reward() {
	//�L�����̗����G��p�ӂ���
	if (stand_handle[0] == -1) {
		int k;
		string name;
		for (int i = 0; i < sn; i++) {
			k = student[i]->get_kind();
			name = "picture/stand/";
			name += st_name[k]; name += "/"; name += st_name[k];
			if (month >= 6 && month <= 10) { name += "��"; }//�ߑւ�
			stand_handle[i] = LoadGraph((name + "�Ί�.png").c_str());
			if (stand_handle[i] == -1) {
				stand_handle[i] = LoadGraph((name + "�ʏ�.png").c_str());
			}
		}
	}
	//���N���b�N��
	if (left_click() == 1) {
		//�����Ŏ󂯎��
		item_num = student[0]->get_item();//�񕜖�X�V
		max_wave = max(max_wave, wave - 1);//Wave�ō��L�^�X�V
		money += dash_money;//����
		//�o���l
		for (int i = 0; i < sn; i++) {
			int k = student[i]->get_kind();
			exp_sum[k] += dash_exp;
			level_up(k);//���x���A�b�v�i�ł���Ȃ�j
		}
		//�e���x�@�A�J�c�L�͏���
		for (int i = 1; i < sn; i++) {
			love[student[i]->get_kind()] += dash_love[i];
			if (love[student[i]->get_kind()] > LOVE_MAX) { love[student[i]->get_kind()] = LOVE_MAX; }
		}
		//����
		for (unsigned int i = 0; i < WEAPON_SUM; i++) {
			weapon_list[i] += (*dash_weapon)[i];
		}
		//�f��
		for (unsigned int i = 0; i < MATERIAL_SUM; i++) {
			material[i] += (*dash_material)[i];
		}
		return true;
	}
	return false;
}

void Dash::draw_reward() {
	//�w�i
	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, GRAY, TRUE);
	//�L�����̗����G
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
			DrawStringToHandle(140 + (i * 400), 500, "���x���A�b�v�I", RED, FontHandle1);
		}
	}
	//���݂̕�V
	ostringstream r1;
	ostringstream r2;
	r1 << "�l�����F" << dash_money;
	r2 << "�o���l�F" << dash_exp;
	DrawBox(0, 750, GAME_WIDE, 950, LIGHT_BLUE, TRUE);
	DrawStringToHandle(50, 800, r1.str().c_str(), YELLOW, FontHandle2);
	DrawStringToHandle(650, 800, r2.str().c_str(), DARK_GREEN, FontHandle2);
	DrawBox(0, 0, 550, 50, LIGHT_GREEN, TRUE);
	DrawStringToHandle(10, 10, "���N���b�N�ŃZ�[�u���ďI��", BLACK, FontHandle1);
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