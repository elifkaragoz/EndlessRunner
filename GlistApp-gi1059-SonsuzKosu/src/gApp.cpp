/*
 * gApp.cpp
 *
 *  Created on: May 6, 2024
 *      Author: elif
 */

#include "gApp.h"
#include "gCanvas.h"
#include "MenuCanvas.h"


gApp::gApp() {
}

gApp::gApp(int argc, char **argv) : gBaseApp(argc, argv) {

}

gApp::~gApp() {
}

void gApp::setup() {

	sesSetup();
	muzikSetup();
	MenuCanvas* cnv = new MenuCanvas(this);
	appmanager->setCurrentCanvas(cnv);
}

void gApp::update() {
}

void gApp::sesCal(int ID) {
	if(ID < 0 || ID >= sessayisi) return;
	sescalar[ID].play();
}

void gApp::sesDur(int ID) {
	if(ID < 0 || ID >= sessayisi) return;
	if(sescalar[ID].isPlaying() == true) sescalar[ID].stop();
}

void gApp::sesDur() {
	for (int var = 0; var < sessayisi; ++var) {
		sescalar[var].stop();
	}
}

void gApp::sesSeviyesi(int ID, int volume) {
	if(ID < 0 || ID >= sessayisi) return;
	if(volume < 0) volume = 0;
	if(volume > 10) volume = 10;
	sescalar[ID].setVolume(volume * 0.1f);
}

void gApp::sesSeviyesi(int volume) {
	if(volume < 0) volume = 0;
	if(volume > 10) volume = 10;
	for (int var = 0; var < sessayisi; ++var) {
		sescalar[var].setVolume(volume * 0.1f);
	}
}

void gApp::sesSetLoop() {
	for (int var = 0; var < sessayisi; ++var) {
		sescalar[var].setLoopType(gFmodSound::LOOPTYPE_NONE);
	}
}

void gApp::sesSetup() {
	sescalar[0].loadSound("click.wav");
	sescalar[1].loadSound("click.wav");
}

void gApp::muzikCal(int ID) {
	if(ID < 0 || ID >= muziksayisi) return;
	if(muzikcalar[ID].isPlaying() == false) muzikcalar[ID].play();
}

void gApp::muzikDur(int ID) {
	if(ID < 0 || ID >= muziksayisi) return;
	if(muzikcalar[ID].isPlaying() == true) muzikcalar[ID].stop();
}

void gApp::muzikPause(int ID) {
	if(ID < 0 || ID >= muziksayisi) return;
	if(muzikcalar[ID].isPaused() == false)muzikcalar[ID].setPaused(true);
	else if(muzikcalar[ID].isPaused() == true) muzikcalar[ID].setPaused(false);
}

void gApp::muzikSeviyesi(int volume) {
	if(volume < 0) volume = 0;
	if(volume > 10) volume = 10;
	for (int var = 0; var < muziksayisi; ++var) {
		muzikcalar[var].setVolume(volume * 0.1f);
	}
}

void gApp::muzikSetLoop(int value) {
	if(value < 0) value = 0;
	if(value > 2) value = gFmodSound::LOOPTYPE_DEFAULT;
	for (int var = 0; var < muziksayisi; ++var) {
		muzikcalar[var].setLoopType(gFmodSound::LOOPTYPE_NORMAL);
	}
}

void gApp::muzikSetup() {
	muzikcalar[0].loadSound("412348__kasdonatov__hip-hop-165-bpm.mp3");
}

void gApp::muzikDur() {
	for (int var = 0; var < muziksayisi; ++var) {
		muzikcalar[var].stop();
	}
}
