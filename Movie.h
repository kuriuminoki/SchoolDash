#ifndef MOVIE_H_INCLUDED
#define MOVIE_H_INCLUDED

static const int run_x[] = { 800, 900, 200, 350, 250, 750, 580, 650, 100, 460 };
static const int run_y[] = { 270, 320, 170, 380, 400, 470, 240, 400, 240, 310 };

class Ed {
private:
	int font_handle;
private:
	int run[10][2];
	int other[6];
	int haikei;
public:
	int cnt;
	int haikei_x;
	int* other_x;
public:
	Ed();
	~Ed();
public:
	bool play();
};

#endif