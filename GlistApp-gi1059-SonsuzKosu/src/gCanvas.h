 /*
 * gCanvas.h
 *
 *  Created on: May 6, 2024
 *      Author: elif
 */

#ifndef GCANVAS_H_
#define GCANVAS_H_

#include "gBaseCanvas.h"
#include "gApp.h"
#include "gImage.h"
#include "gCamera.h"
#include "gBox.h"
#include "gLight.h"
#include "gSkybox.h"
#include "gModel.h"
#include "MenuCanvas.h"
#include "gFile.h"
#include "gFont.h"


class gCanvas : public gBaseCanvas {
public:
	gCanvas(gApp* root);
	virtual ~gCanvas();

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void charPressed(unsigned int codepoint);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseScrolled(int x, int y);
	void mouseEntered();
	void mouseExited();
	void windowResized(int w, int h);

	void showNotify();
	void hideNotify();

private:
	gApp* root;
	gImage logo;

	int oyunmodu;
	int loadprogress;
	static const int OYUN_NORMAL = 0, OYUN_PAUSE = 1, OYUN_LOAD = -1, OYUN_BITTI = 2, OYUN_START = 3,
					 OYUN_WIN = 4;

	//Camera
	static const int PRESSED = 0, RELEASED = 1, DRAGGED = 2;
	gCamera camera;
	void cameraSetup();
	void cameraHareketi();
	void cameraKeyMap(int key, int eventType);
	bool cameraforward, camerabackward, cameraleft, cameraright, cameraup, cameradown;
	bool cameraturnleft, cameraturnright, cameraturnup, cameraturndown, camerarollleft, camerarollright;

	//Skybox
	gLight skylight;
	gSkybox sky;
	std::vector<std::string> skyimagelist;
	void skySetup();

	//Harita
	static const int MAP_X = 0, MAP_Y = 1, MAP_Z = 2, MAP_W = 3, MAP_H = 4, MAP_D = 5;
	gModel map;
	void mapSetup();
	void mapDraw();
	static const int mapsayisi = 6;
	std::vector<float> mapkonum;
	std::vector<std::vector<float>> maplistesi;
	float mapw, maph, mapd;
	gBox maphitbox[mapsayisi];
	int gorusmesafesi;

	//Karakter
	gModel karakter;
	float karakterw, karakterh, karakterd;
	void karakterSetup();
	void karakterDraw();
	gBox karakterhitbox;
	void karakterUpdate();
	int karakterdurum;
	static const int karakteranimsayisi = 6;
	static const int ROBOT_KOS = 0, ROBOT_OLDU = 1, ROBOT_DUR = 2, ROBOT_TAKLA = 3, ROBOT_EGIL = 4,
					 ROBOT_YURU = 5;
	int karaktersonframe[karakteranimsayisi], karakterilkframe[karakteranimsayisi];
	void karakterSonrakiFrame();
	int framegecikmesayaci;
	float karakterhiz, karakterivme;
	float karakterhizy;
	bool isGround;
	int karaktereskidurum;
	int karakterhangiyol;
	void karakterHangiYolda();
	float karakterhizx;
	static const int SOL = 0, ORTA = 1, SAG = 2;
	int karakteryol, karaktereskiyol;
	static const int seritsayisi = 3;
	bool isChanging;
	float karakterdegisim, karakterdegisimhizi;
	float serit[seritsayisi];
	void karakterYolDegistirme();
	void karakterKeyMap(int key);

	//Arayuz Nesneleri
	//Ayarbuton
	int ayarbutondurum;
	gImage ayarbuton[2];
	int ayarbutonx, ayarbutony, ayarbutonw, ayarbutonh;
	void ayarbutonSetup();
	void ayarbutonDraw();
	void ayarbutonTikla(int x, int y, int eventType);

	//Pause Menu
	gImage pausemenu;
	int pausemenux, pausemenuy, pausemenuw, pausemenuh;
	void pausemenuSetup();
	void pausemenuDraw();
	gImage devambuton[2], restartbuton[2], anamenubuton[2];
	int devambutonx, devambutony, devambutonw, devambutonh;
	int restartbutonx, restartbutony, restartbutonw, restartbutonh;
	int anamenubutonx, anamenubutony, anamenubutonw, anamenubutonh;
	int devambutondurum, restartbutondurum, anamenubutondurum;
	void pausebutonlariTikla(int x, int y, int eventType);

	//Engeller
	static const int ENGEL_W = 0, ENGEL_H = 1, ENGEL_D = 2, ENGEL_X = 3, ENGEL_Y = 4, ENGEL_Z = 5, ENGEL_MAPID = 6;
	gModel engel;
	float engelw, engelh, engeld;
	void engelSetup();
	std::deque<float> yeniengel;
	std::deque<std::deque<float>> engellistesi;
	void engelUret();
	void engelDraw();
	void karakterVsEngel();
	gBox engelhitbox;

	//Oyun Sonu
	gImage oyunsonu;
	int oyunsonux, oyunsonuy, oyunsonuw, oyunsonuh;
	void oyunsonuSetup();
	void oyunsonuDraw();

	//Dosya Kaydi
	gFile scorekaydi;
	void scoreKaydiSetup();
	void saveGame(std::vector<std::string>& kaydedilecekBaslik, std::vector<int>& kaydedilecekDeger);
	void loadGame();
	std::vector<int> kaydedilecekdeger;
	std::vector<std::string> kaydedilecekbaslik;
	std::string savefiletext, savefilesrc;
	std::vector<std::string> kayitparametreleri;
	std::vector<int> loadcoin;

	//Coin
	static const int COIN_W = 0, COIN_H = 1, COIN_D = 2, COIN_X = 3, COIN_Y = 4, COIN_Z = 5, COIN_MAPID = 6,
					 COIN_ROTATE = 7;
	gModel coin;
	float coinx , coiny, coinw, coinh, coind;
	std::deque<float> yenicoin;
	std::deque<std::deque<float>> coinlistesi;
	gBox coinhitbox;
	void coinSetup();
	void coinUret();
	void coinDraw();
	gImage coinpanel;
	float coinpanelx, coinpanely, coinpanelw, coinpanelh;
	int coinscore;
	gFont coinyazi;
	void coinPanelSetup();
	void coinToplandi();
	void coinPanelDraw();

	//Star Line
	gModel startline;
	void startLineSetup();
	void startLineDraw();
	gBox startlineHitbox;
	float startlinew, startlineh, startlined;
	int zamansayaci;
	gFont startfont;

	//Finish Line
	gModel finishline;
	void finishLineSetup();
	void finishLineDraw();
	gBox finishlineHitbox;
	float finishlinew, finishlineh, finishlined;
	void isGameFinish();

	//Finish meter
	gImage finishmeter;
	int finishmeterx, finishmetery, finishmeterw, finishmeterh;
	float finishmeterpx, finishmeterpy, finishmeterpw, finishmeterph;
	float maplength, maptometer, karakterkonumorani;
	void finishmeterSetup();
	void finishmeterDraw();

	//Load ekrani
	gImage loadekrani;
	int loadekranix, loadekraniy, loadekraniw, loadekranih;
	void loadekraniSetup();
};

#endif /* GCANVAS_H_ */
