/*
 * gApp.h
 *
 *  Created on: May 6, 2024
 *      Author: elif
 */

#ifndef GAPP_H_
#define GAPP_H_

#include "gBaseApp.h"
#include "gFmodSound.h"

class gApp : public gBaseApp {
public:
	gApp();
	gApp(int argc, char **argv);
	~gApp();

	void setup();
	void update();

	void sesCal(int ID);
	void sesDur(int ID);
	void sesDur();
	void sesSeviyesi(int ID, int volume);
	void sesSeviyesi(int volume);
	void sesSetLoop();
	static const int MUZIK_MENU = 0;
	void muzikCal(int ID);
	void muzikDur(int ID);
	void muzikDur();
	void muzikPause(int ID);
	void muzikSeviyesi(int volume);
	void muzikSetLoop(int value);
	static const int SES_BUTON = 0, SES_X = 1;

private:
	static const int sessayisi = 2;
	static const int muziksayisi = 1;
	gFmodSound sescalar[sessayisi];
	void sesSetup();
	gFmodSound muzikcalar[muziksayisi];
	void muzikSetup();
};

#endif /* GAPP_H_ */
