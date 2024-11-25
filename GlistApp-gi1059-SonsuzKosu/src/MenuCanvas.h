 /*
 * MenuCanvas.h
 *
 *  Created on: May 6, 2020
 *      Author: noyan
 */

#ifndef MENUCANVAS_H_
#define MENUCANVAS_H_

#include "gBaseCanvas.h"
#include "gApp.h"
#include "gImage.h"
#include "gModel.h"
#include "gFont.h"
#include "gCamera.h"
#include "gLight.h"


class MenuCanvas : public gBaseCanvas {
public:
	MenuCanvas(gApp* root);
	virtual ~MenuCanvas();

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

	gModel karakter;
	gModel karakter2;
	float karakter2x, karakter2y, karakter2z;
	int karakterilkframe, karaktersonframe;
	int framegecikmesayaci;
	gModel map;
	int perderenkg, perderenkr, perderenkb;
	gFont baslikfont;
	gCamera camera;
	int nextscene;
	int clock;
	int modelchange;
	void nextsceneChange();
	void screenBright();
	void karakterSceneUpdate();
	void karakterSonrakiFrame();

	//Basla Buton
	gImage playbuton;
	int playbutonx, playbutony, playbutonw, playbutonh;
	void playbutonSetup();
	void playbutonDraw();
	void playbutonTikla(int x, int y);
};

#endif /* MENUCANVAS_H_ */
