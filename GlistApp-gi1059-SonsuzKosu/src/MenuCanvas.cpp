/*
* MenuCanvas.cpp
*
*  Created on: May 6, 2020
*      Author: noyan
*/


#include "MenuCanvas.h"
#include "gCanvas.h"


MenuCanvas::MenuCanvas(gApp* root) : gBaseCanvas(root) {
	this->root = root;
}

MenuCanvas::~MenuCanvas() {
}

void MenuCanvas::setup() {
	logo.loadImage("glistengine_logo.png");
	modelchange = 0;
	clock = 0;
	nextscene = 0;
	perderenkr = 0;
	perderenkg = 0;
	perderenkb = 0;

	//Donen karakter
	karakter.loadModel("kilavuz-yeni-robot-4.dae");
	karakter.dolly(-10.0f);
	karakter.truck(-5.0f);
	karakter.boom(-2.0f);

	//Haritadaki Karakter
	framegecikmesayaci = 0;
	karakterilkframe = 421;
	karaktersonframe = 437;
	karakter2.loadModel("kilavuz-yeni-robot-4.dae");
	karakter2.makeVertexAnimated();
	karakter2.setAnimationFrameNo(426);
	karakter2x = -0.1f; karakter2y = -0.2f; karakter2z = -5.0f;
	karakter2.scale(0.04f);

	//Map Yukleniyor
	map.loadModel("street-final/street-final-doubletile.obj");
	map.boom(-4.0f);
	map.pan(4.75f);
	baslikfont.loadFont("VampireWarsItalic.ttf", 60);
	playbutonSetup();
}

void MenuCanvas::update() {
	karakterSceneUpdate();
	//gLogi("MenuCanvas") << "clock:" << clock << " scene:" << nextscene;
	if(nextscene == 0 && clock < 420) clock += 1;
	    if(perderenkb < 255 && nextscene == 0 && clock < 100){
	        perderenkb += 3;
	        perderenkr += 2;
	        perderenkg += 2;
	    }
	    if(clock >= 420 && nextscene == 0){
	        nextsceneChange();
	    }
	    if(nextscene == 1){
	        if(clock < 40)clock+=1;
	        if(clock >= 40)nextsceneChange();
	    }
	    if(nextscene == 2){
	        if(clock < 420)clock+=1;
	        if(clock == 420)nextsceneChange();
	    }
	    if(nextscene == 3){
	        if(clock < 40)clock+=1;
	        if(clock == 40)clock = 40; nextsceneChange();
	    }
}

void MenuCanvas::draw() {
	setColor(perderenkr, perderenkg, perderenkb, 255);
	gDrawRectangle(0, 0, getWidth(), getHeight(), 1);
	setColor(255, 255, 255, 255);

	//Karakter
	enableDepthTest();
	camera.begin();
	//skylight.enable();
	if(modelchange == 0)karakter.draw();
	if(modelchange == 1){map.draw(); karakter2.draw(); karakterSonrakiFrame();}
	karakter2.setPosition(karakter2x, karakter2y, karakter2z);
	karakter.pan(0.01f);
	disableDepthTest();
	camera.end();
	//skylight.disable();

	//GUI
	setColor(265, 265, 10, 255);
	baslikfont.drawText("Sonsuz", getWidth() / 2, getHeight() / 8);
	setColor(265, 265, 10, 255);
	baslikfont.drawText("Kosu", getWidth() * 0.6f, getHeight() / 4);
	setColor(255, 255, 255, 255);
	playbutonDraw();
}

void MenuCanvas::keyPressed(int key) {
//	gLogi("MenuCanvas") << "keyPressed:" << key;
}

void MenuCanvas::keyReleased(int key) {
//	gLogi("MenuCanvas") << "keyReleased:" << key;
}

void MenuCanvas::charPressed(unsigned int codepoint) {
//	gLogi("MenuCanvas") << "charPressed:" << gCodepointToStr(codepoint);
}

void MenuCanvas::mouseMoved(int x, int y) {
//	gLogi("MenuCanvas") << "mouseMoved" << ", x:" << x << ", y:" << y;
}

void MenuCanvas::mouseDragged(int x, int y, int button) {
//	gLogi("MenuCanvas") << "mouseDragged" << ", x:" << x << ", y:" << y << ", b:" << button;
}

void MenuCanvas::mousePressed(int x, int y, int button) {
//	gLogi("MenuCanvas") << "mousePressed" << ", x:" << x << ", y:" << y << ", b:" << button;
	playbutonTikla(x, y);
}

void MenuCanvas::mouseReleased(int x, int y, int button) {
//	gLogi("MenuCanvas") << "mouseReleased" << ", button:" << button;
}

void MenuCanvas::mouseScrolled(int x, int y) {
//	gLogi("MenuCanvas") << "mouseScrolled" << ", x:" << x << ", y:" << y;
}

void MenuCanvas::mouseEntered() {

}

void MenuCanvas::mouseExited() {

}

void MenuCanvas::windowResized(int w, int h) {

}

void MenuCanvas::showNotify() {

}

void MenuCanvas::hideNotify() {

}

void MenuCanvas::playbutonSetup() {
	playbuton.loadImage("play.png");
	playbutonw = playbuton.getWidth();
	playbutonh = playbuton.getHeight();
	playbutonx = getWidth() / 2;
	playbutony = getHeight() / 2;
}

void MenuCanvas::playbutonDraw() {
	playbuton.draw(playbutonx, playbutony);
}

void MenuCanvas::playbutonTikla(int x, int y) {
	if(x > playbutonx && x < playbutonx + playbutonw &&
	   y > playbutony && y < playbutony + playbutonh){
		gCanvas* cnv = new gCanvas(root);
		appmanager->setCurrentCanvas(cnv);
	}
}

void MenuCanvas::nextsceneChange() {
	if(nextscene == 0){
		if(perderenkr > 0)perderenkr -= 2;
		if(perderenkg > 0)perderenkg -= 2;
		if(perderenkb > 0)perderenkb -= 2;
		if(perderenkb < 50) modelchange = -1;
		if(perderenkr < 1 && perderenkg < 1 && perderenkb < 1){nextscene = 1; clock = 0;}
	}
	if(nextscene == 1){
		screenBright();
	}
	if(nextscene == 2){
		if(perderenkr > 0)perderenkr -= 2;
		if(perderenkg > 0)perderenkg -= 2;
		if(perderenkb > 0)perderenkb -= 2;
		if(perderenkb < 50) modelchange = -1;
		if(perderenkr < 1 && perderenkg < 1 && perderenkb < 1){nextscene = 3; clock = 0;}
	}
	if(nextscene == 3){
		screenBright();
	}
}

void MenuCanvas::screenBright() {
	if(perderenkb < 255 && nextscene == 1){
		perderenkr += 2;
		perderenkg += 2;
		perderenkb += 3;
	}
	if(perderenkr > 150 && nextscene == 1){ nextscene = 2; clock = 0; modelchange = 1;}

	if(perderenkb < 255 && nextscene == 3){
		perderenkr += 2;
		perderenkg += 2;
		perderenkb += 3;
	}
	if(perderenkr > 150 && nextscene == 3){ nextscene = 0; clock = 0; modelchange = 0;}
}

void MenuCanvas::karakterSceneUpdate() {
	if(modelchange == 1){
		//Animasyon calisiyor
		//Karakter yakýnlasiyor
		if(karakter2z < -0.5){
			karakter2z += 0.03;
		}
	}
	if(modelchange == 0)karakter2z = -8.5;
}

void MenuCanvas::karakterSonrakiFrame() {
    //frame+1
    if(karakter2.getAnimationFrameNo() < karaktersonframe - 1){
        if(framegecikmesayaci < root->getFramerate() / 48) framegecikmesayaci++;
        else{
            karakter2.nextAnimationFrame();
            framegecikmesayaci = 0;
        }
    }
    else{
        karakter2.setAnimationFrameNo(karakterilkframe);
    }
}
