/*
* gCanvas.cpp
*
*  Created on: May 6, 2020
*      Author: noyan
*/


#include "gCanvas.h"


gCanvas::gCanvas(gApp* root) : gBaseCanvas(root) {
	this->root = root;
}

gCanvas::~gCanvas() {
}

void gCanvas::setup() {
	oyunmodu = OYUN_LOAD;
	gorusmesafesi = 2;
	loadprogress = 0;
	loadekraniSetup();
	startfont.loadFont("VampireWarsItalic.ttf", 60);

	//scoreKaydiSetup();
}

void gCanvas::update() {
	if(oyunmodu == OYUN_LOAD){
		switch(loadprogress){
		case 0:
			skySetup();
			mapSetup();
			loadprogress = 10;
			break;
		case 10:
			karakterSetup();
			cameraSetup();
			loadprogress = 30;
			break;
		case 30:
			ayarbutonSetup();
			pausemenuSetup();
			engelSetup();
			loadprogress = 70;
			break;
		case 70:
			oyunsonuSetup();
			loadGame();
			coinSetup();
			loadprogress = 90;
			break;
		case 90:
			coinPanelSetup();
			startLineSetup();
			finishLineSetup();
			finishmeterSetup();
			loadprogress = 100;
			break;
		case 100:
			oyunmodu = OYUN_START;
			root->muzikCal(root->MUZIK_MENU);
			root->muzikSeviyesi(1);
			break;
		}
	}
	if(oyunmodu == OYUN_START){
		if(zamansayaci < 60 * 3) {
			zamansayaci++;
			camera.boom(-6.0f / 180);
		}else if (zamansayaci >= 60 * 3) oyunmodu = OYUN_NORMAL;
	}
	if(oyunmodu != OYUN_NORMAL) return;
	cameraHareketi();
	skylight.setPosition(camera.getPosition());
	skylight.boom(10.0f);
	camera.setPosition(karakter.getPosition());
	camera.boom(2.5f);
	camera.dolly(5.0f);
	karakterUpdate();
	karakterHangiYolda();
	karakterYolDegistirme();
	karakterVsEngel();
	coinToplandi();
	isGameFinish();
}

void gCanvas::draw() {
	if(oyunmodu == OYUN_LOAD){
		setColor(255,255,255,125);
		gDrawRectangle(0, 0, getWidth(), getHeight(), true);
		setColor(255,0,0,125);
		gDrawTriangle(0, 0, 0, getHeight(), getWidth() / 2, getHeight(), true);
		setColor(0,0,255,125);
		gDrawTriangle(getWidth(), 0, getWidth(), getHeight(), getWidth() / 2, getHeight(), true);
		setColor(0,255,0,125);
		gDrawTriangle(0, 0, 0, getHeight() / 2, getWidth() / 2, 0, true);
		setColor(255,255,0,125);
		gDrawTriangle(getWidth(), 0, getWidth(), getHeight() / 2, getWidth() / 2, 0, true);
		setColor(255,255,255);
		loadekrani.draw(loadekranix - getWidth() / 8, loadekraniy, loadekraniw * 2, loadekranih * 1.5f);
		setColor(0,0,0);
		startfont.drawText("" + gToStr(loadprogress), loadekranix + loadekraniw, loadekraniy + loadekranih * 1.5f);
		setColor(255,255,255);
		return;
	}
	enableDepthTest();//3 boyutlu cizilecek
	camera.begin();
	skylight.enable();
	sky.draw();
	mapDraw();
	karakterDraw();
	engelDraw();
	coinDraw();
	coinPanelDraw();
	startLineDraw();
	finishLineDraw();
	skylight.disable();
	camera.end();
	disableDepthTest();//3 boyutun bittigi yer
	ayarbutonDraw();
	pausemenuDraw();
	oyunsonuDraw();
	finishmeterDraw();
	if(oyunmodu == OYUN_START){
		if((3 - zamansayaci / 60) != 0)startfont.drawText("" + gToStr(3 - (zamansayaci / 60)),
						   getWidth() / 2 - startfont.getStringWidth("" + gToStr(3 - (zamansayaci / 60))),
						   getHeight() / 2);
	}
}

void gCanvas::keyPressed(int key) {
//	gLogi("gCanvas") << "keyPressed:" << key;
	if(oyunmodu == OYUN_NORMAL){
		cameraKeyMap(key, PRESSED);
		karakterKeyMap(key);
	}
	if(key == G_KEY_ESC){
		if(oyunmodu == OYUN_NORMAL) oyunmodu = OYUN_PAUSE;
		else if(oyunmodu == OYUN_PAUSE) oyunmodu = OYUN_NORMAL;
	}
	if(key == G_KEY_M){
		//Gonderilecek degerler hazirlaniyor
		kaydedilecekbaslik.push_back("Score");
		kaydedilecekdeger.push_back(coinscore);
		kaydedilecekbaslik.push_back("Score2");
		kaydedilecekdeger.push_back(coinscore + 1000);
		//score deðeri kayýda gönderiliyor
		saveGame(kaydedilecekbaslik, kaydedilecekdeger);
		kaydedilecekbaslik.clear();
		kaydedilecekdeger.clear();
	}
}

void gCanvas::keyReleased(int key) {
//	gLogi("gCanvas") << "keyReleased:" << key;
	cameraKeyMap(key, RELEASED);
}

void gCanvas::charPressed(unsigned int codepoint) {
//	gLogi("gCanvas") << "charPressed:" << gCodepointToStr(codepoint);
}

void gCanvas::mouseMoved(int x, int y) {
//	gLogi("gCanvas") << "mouseMoved" << ", x:" << x << ", y:" << y;
}

void gCanvas::mouseDragged(int x, int y, int button) {
//	gLogi("gCanvas") << "mouseDragged" << ", x:" << x << ", y:" << y << ", b:" << button;
	ayarbutonTikla(x, y, DRAGGED);
	if(oyunmodu == OYUN_PAUSE || oyunmodu == OYUN_BITTI || oyunmodu == OYUN_WIN){
		pausebutonlariTikla(x, y, DRAGGED);
	}
}

void gCanvas::mousePressed(int x, int y, int button) {
//	gLogi("gCanvas") << "mousePressed" << ", x:" << x << ", y:" << y << ", b:" << button;
	ayarbutonTikla(x, y, PRESSED);
	if(oyunmodu == OYUN_PAUSE || oyunmodu == OYUN_BITTI || oyunmodu == OYUN_WIN){
		pausebutonlariTikla(x, y, PRESSED);
	}
}

void gCanvas::mouseReleased(int x, int y, int button) {
//	gLogi("gCanvas") << "mouseReleased" << ", button:" << button;
	ayarbutonTikla(x, y, RELEASED);
	if(oyunmodu == OYUN_PAUSE || oyunmodu == OYUN_BITTI || oyunmodu == OYUN_WIN){
		pausebutonlariTikla(x, y, RELEASED);
	}
}

void gCanvas::mouseScrolled(int x, int y) {
//	gLogi("gCanvas") << "mouseScrolled" << ", x:" << x << ", y:" << y;
}

void gCanvas::mouseEntered() {

}

void gCanvas::mouseExited() {

}

void gCanvas::windowResized(int w, int h) {

}

void gCanvas::showNotify() {

}

void gCanvas::hideNotify() {

}

void gCanvas::cameraSetup() {
	//KeyMap
	cameraforward = false;
	camerabackward = false;
	cameraleft = false;
	cameraright = false;
	cameraup = false;
	cameradown = false;

	//Camera Rotateler (Cameranýn Kendisi)
	cameraturnleft = false;
	cameraturnright = false;
	cameraturnup = false;
	cameraturndown = false;
	camerarollleft = false;
	camerarollright = false;

	camera.panDeg(90.0f);

}

void gCanvas::cameraHareketi() {
	if(cameraforward)camera.dolly(-2.2f); //z ekseni ileri
	if(camerabackward)camera.dolly(2.2f); //z ekseni geri
	if(cameraleft)camera.truck(-0.2f); //x ekseni sol
	if(cameraright)camera.truck(0.2f); //x ekseni sað
	if(cameradown)camera.boom(-0.2f); //y ekseni yukarý
	if(cameraup)camera.boom(0.2f); //y ekseni asagi
	if(cameraturnright)camera.pan(-0.05f); //y ekseni sola don
	if(cameraturnleft)camera.pan(0.05f); //y ekseni sola sag
	if(cameraturnup)camera.tilt(0.05f); //y ekseni sola yukari
	if(cameraturndown)camera.tilt(-0.05f); //y ekseni sola asagi
	if(camerarollleft)camera.roll(0.1f); //z ekseninde sola dondur
	if(camerarollright)camera.roll(-0.1f); //z ekseninde saga dondur
}

void gCanvas::cameraKeyMap(int key, int eventType) {
	if(eventType == PRESSED){
		if(key == 87)cameraforward = true;
		if(key == 83)camerabackward = true;
		if(key == 65)cameraleft = true;
		if(key == 68)cameraright = true;
		if(key == 81)cameraup = true;
		if(key == 69)cameradown = true;
		if(key == 74)cameraturnleft = true;
		if(key == 76)cameraturnright = true;
		if(key == 73)cameraturnup = true;
		if(key == 75)cameraturndown = true;
		if(key == 85)camerarollleft = true;
		if(key == 79)camerarollright = true;

	}
	if(eventType == RELEASED){
		if(key == 87)cameraforward = false;
		if(key == 83)camerabackward = false;
		if(key == 65)cameraleft = false;
		if(key == 68)cameraright = false;
		if(key == 81)cameraup = false;
		if(key == 69)cameradown = false;
		if(key == 74)cameraturnleft = false;
		if(key == 76)cameraturnright = false;
		if(key == 73)cameraturnup = false;
		if(key == 75)cameraturndown = false;
		if(key == 85)camerarollleft = false;
		if(key == 79)camerarollright = false;
	}
}

void gCanvas::skySetup() {
	//Gokyuzu kaplamasi yuklendi
	skyimagelist.push_back("skymap/right.jpg");
	skyimagelist.push_back("skymap/left.jpg");
	skyimagelist.push_back("skymap/top.jpg");
	skyimagelist.push_back("skymap/bottom.jpg");
	skyimagelist.push_back("skymap/front.jpg");
	skyimagelist.push_back("skymap/back.jpg");

	sky.loadTextures(skyimagelist);
	sky.scale(50.0f); //50 katina cikarildi

	//Skylight
	skylight.setAmbientColor(210, 210, 192, 255);
	skylight.setDiffuseColor(64, 64, 64, 255);
	skylight.setSpecularColor(16, 16, 8, 255);
	skylight.setPosition(0.0f, 10.0f, 0.0f);
	//skylight.setType(gLight::LIGHTTYPE_DIRECTIONAL);
}

void gCanvas::mapSetup() {
	serit[SOL] = 2.4f;
	serit[ORTA] = 0.0f;
	serit[SAG] = -2.4f;
	map.loadModel("street-final/street-final-doubletile.obj");
	mapw = map.getBoundingBox().getWidth();
	maph = map.getBoundingBox().getHeight();
	mapd = map.getBoundingBox().getDepth();
	//Map konumlari
	for (int sira = 0; sira < mapsayisi; ++sira) {
		mapkonum.push_back((-mapw + 2.1f) * sira); //x
		mapkonum.push_back(0); //y
		mapkonum.push_back(0); //z
		mapkonum.push_back(mapw); //w
		mapkonum.push_back(maph); //h
		mapkonum.push_back(mapd); //d
		maplistesi.push_back(mapkonum);
		mapkonum.clear();

		//Map hitbox ayari
		maphitbox[sira].setPosition(maplistesi[sira][MAP_X] - (mapw / 2),
									maplistesi[sira][MAP_Y] + 1.5f,
									maplistesi[sira][MAP_Z]);
		maphitbox[sira].scale(mapw / 2, 2, 3);
	}
}

void gCanvas::mapDraw() {
	for (int sira = karakterhangiyol; sira < mapsayisi; ++sira) {
		map.setPosition(maplistesi[sira][MAP_X], maplistesi[sira][MAP_Y], maplistesi[sira][MAP_Z]);
		map.draw();
		//maphitbox[sira].draw();
		if(sira >= karakterhangiyol + gorusmesafesi / 2) break;
	}
}

void gCanvas::karakterSetup() {
	karakterdegisim = 0.0f;
	isChanging = false;
	karakteryol = ORTA;
	karaktereskiyol = karakteryol;
	karakterdegisimhizi = 0.07f;
	karakterhizx = 0;
	karakterhangiyol = 0;
	isGround = true;
	karakterhizy = 0;
	karakterhiz = 0.1f;
	karakterivme = 0.00025f;
	framegecikmesayaci = 0;
	karakter.loadModel("kilavuz-yeni-robot-4.dae");
	karakter.scale(0.4f);
	karakter.panDeg(-90.0f);
	karakter.dolly(10.0f);
	karakter.boom(1.8f);
	karakter.setPosition(karakter.getPosX(), karakter.getPosY(), serit[karakteryol]);

	//karakter.setPosition(karakter.getPosX() - mapw * 5, karakter.getPosY(), serit[karakteryol]);

	//Karakter animasyonu
	karakter.makeVertexAnimated();
	//karakter.setAnimationFrameNo(436);

	karakterdurum = ROBOT_YURU;
	karaktereskidurum = karakterdurum;
	karakterilkframe[ROBOT_KOS] = 421;
	karaktersonframe[ROBOT_KOS] = 437;
	karakterilkframe[ROBOT_OLDU] = 22;
	karaktersonframe[ROBOT_OLDU] = 100;
	karakterilkframe[ROBOT_DUR] = 135;
	karaktersonframe[ROBOT_DUR] = 177;
	karakterilkframe[ROBOT_TAKLA] = 196;
	karaktersonframe[ROBOT_TAKLA] = 255;
	karakterilkframe[ROBOT_EGIL] = 330;
	karaktersonframe[ROBOT_EGIL] = 370;
	karakterilkframe[ROBOT_YURU] = 383;
	karaktersonframe[ROBOT_YURU] = 415;
	karakter.setAnimationFrameNo(karakterilkframe[karakterdurum]);

	karakterw = karakter.getBoundingBox().getWidth();
	karakterh = karakter.getBoundingBox().getHeight();
	karakterd = karakter.getBoundingBox().getDepth();

	karakterhitbox.setPosition(karakter.getPosX(), karakter.getPosY() + karakterh / 2, karakter.getPosZ());
	karakterhitbox.setScale(karakterw / 2, karakterh / 2, karakterd / 2);
}

void gCanvas::karakterDraw() {
	karakter.draw();
	//karakterhitbox.draw();
	if(oyunmodu == OYUN_NORMAL || oyunmodu == OYUN_BITTI)karakterSonrakiFrame();
}

void gCanvas::karakterUpdate() {
	//Eger karakter durum degisirse karakterin yeni durumunun ilk frame'ini drawda gostermesi icin atandi
	if(karaktereskidurum != karakterdurum){
		karakter.setAnimationFrameNo(karakterilkframe[karakterdurum]);
		karaktereskidurum = karakterdurum;
	}

	//Karakter 0.4f hizina ulasana kadar karakterin hizi surekli olarak arttiriliyor
	if(karakterhiz < 0.4f)karakterhiz += karakterivme;
	if(karakterhiz > 0.4f)karakterhiz -= karakterivme * 12;

	//Zýplama hareketi
	//Karakteri Y ekseninde ilerlet
	karakter.boom(karakterhizy);
	if(karakter.getPosY() > 1.8f){
		karakterhizy -= 0.0125f;
	}else{
		karakterhizy = 0;
		karakter.setPosition(karakter.getPosX(), 1.8f, karakter.getPosZ());
		isGround = true;
		if(karakterhiz > 0.3f)karakterdurum = ROBOT_KOS;
		if(karakterhiz < 0.3f)karakterdurum = ROBOT_YURU;
	}

	//Karakteri ilerletiyor
	karakter.dolly(karakterhiz);

	//Karakter Hitbox Update
	karakterhitbox.setPosition(karakter.getPosX(), karakter.getPosY() + karakterh / 2, karakter.getPosZ());
}

void gCanvas::karakterSonrakiFrame() {
	//frame+1
	if(karakter.getAnimationFrameNo() < karaktersonframe[karakterdurum] - 1){
		if(framegecikmesayaci < root->getFramerate() / 48) framegecikmesayaci++;
		else{
			karakter.nextAnimationFrame();
			framegecikmesayaci = 0;
		}
	}
	else{
		if(karakter.getAnimationFrameNo() > karakterilkframe[ROBOT_OLDU] && karakter.getAnimationFrameNo() < karaktersonframe[ROBOT_OLDU]) return;
		karakter.setAnimationFrameNo(karakterilkframe[karakterdurum]);
	}
}

void gCanvas::ayarbutonSetup(){
	ayarbutondurum = 0;
	ayarbuton[0].loadImage("ayarlar_buton.png");
	ayarbuton[1].loadImage("ayarlar_buton2.png");
	ayarbutonw = ayarbuton[0].getWidth();
	ayarbutonh = ayarbuton[0].getHeight();
	ayarbutonx = getWidth() - ayarbutonw;
	ayarbutony = 0;
}

void gCanvas::ayarbutonDraw(){
	ayarbuton[ayarbutondurum].draw(ayarbutonx, ayarbutony);
}

void gCanvas::ayarbutonTikla(int x, int y, int eventType){
	if(x > ayarbutonx && x < ayarbutonx + ayarbutonw &&
	   y > ayarbutony && y < ayarbutony + ayarbutonh){
		if(eventType == PRESSED){
			ayarbutondurum = 1;
		}
		if(eventType == RELEASED){
			ayarbutondurum = 0;
			if(oyunmodu == OYUN_NORMAL) oyunmodu = OYUN_PAUSE;
			else if(oyunmodu == OYUN_PAUSE) oyunmodu = OYUN_NORMAL;
		}
		if(eventType == DRAGGED) ayarbutondurum = 1;
	}else ayarbutondurum = 0;
}

void gCanvas::karakterHangiYolda() {
	for (int mapID = 0; mapID < mapsayisi; ++mapID) {
		if(karakterhitbox.getBoundingBox().intersects(maphitbox[mapID].getBoundingBox())){
			karakterhangiyol = mapID;
			break;
		}
	}

}

void gCanvas::pausemenuSetup() {
	pausemenu.loadImage("pause2/pause_ekran.png");
	pausemenuw = pausemenu.getWidth();
	pausemenuh = pausemenu.getHeight();
	pausemenux = (getWidth() / 2) - (pausemenuw / 2);
	pausemenuy = (getHeight() / 2) - (pausemenuh / 2);

	devambutondurum = 0;
	devambuton[0].loadImage("pause2/oyna.png");
	devambuton[1].loadImage("pause2/oyna_tiklandi.png");
	devambutonw = devambuton[0].getWidth();
	devambutonh = devambuton[0].getHeight();
	//devambutonx = (getWidth() / 3) - (devambutonw / 2) - 50;
	devambutonx = pausemenux + devambutonw / 2;
	devambutony = (getHeight() / 2) - (devambutonh / 2) + 50;

	restartbutondurum = 0;
	restartbuton[0].loadImage("pause2/restart0.png");
	restartbuton[1].loadImage("pause2/restart1.png");
	restartbutonw = restartbuton[0].getWidth();
	restartbutonh = restartbuton[0].getHeight();
	restartbutonx = devambutonx + restartbutonw + 50;
	restartbutony = devambutony;

	anamenubutondurum = 0;
	anamenubuton[0].loadImage("pause2/menu.png");
	anamenubuton[1].loadImage("pause2/menu_tiklandi.png");
	anamenubutonw = anamenubuton[0].getWidth();
	anamenubutonh = anamenubuton[0].getHeight();
	anamenubutonx = restartbutonx + anamenubutonw + 50;
	anamenubutony = devambutony;

}

void gCanvas::pausemenuDraw() {
	if(oyunmodu != OYUN_PAUSE) return;
	pausemenu.draw(pausemenux, pausemenuy);
	devambuton[devambutondurum].draw(devambutonx, devambutony);
	restartbuton[restartbutondurum].draw(restartbutonx, restartbutony);
	anamenubuton[anamenubutondurum].draw(anamenubutonx, anamenubutony);
}

void gCanvas::pausebutonlariTikla(int x, int y, int eventType){
	//Devam Et
	if(x > devambutonx && x < devambutonx + devambutonw &&
	   y > devambutony && y < devambutony + devambutonh && oyunmodu == OYUN_PAUSE){
		if(eventType == PRESSED){
			devambutondurum = 1;
			root->sesCal(gApp::SES_BUTON);
		}
		if(eventType == RELEASED){
			devambutondurum = 0;
			oyunmodu = OYUN_NORMAL;
		}
		if(eventType == DRAGGED) devambutondurum = 1;
	}else devambutondurum = 0;

	//Yeniden baslat
	if(x > restartbutonx && x < restartbutonx + restartbutonw &&
	   y > restartbutony && y < restartbutony + restartbutonh){
		if(eventType == PRESSED){
			restartbutondurum = 1;
			root->sesCal(gApp::SES_BUTON);
		}
		if(eventType == RELEASED){
			restartbutondurum = 0;
			root->sesDur();
			root->muzikDur();
			gCanvas* cnv = new gCanvas(root);
			appmanager->setCurrentCanvas(cnv);
		}
		if(eventType == DRAGGED) restartbutondurum = 1;
	}else restartbutondurum = 0;

	//Ana Menu
	if(x > anamenubutonx && x < anamenubutonx + anamenubutonw &&
	   y > anamenubutony && y < anamenubutony + anamenubutonh){
		if(eventType == PRESSED){
			anamenubutondurum = 1;
			root->sesCal(gApp::SES_BUTON);
		}
		if(eventType == RELEASED){
			anamenubutondurum = 0;
			MenuCanvas* cnv = new MenuCanvas(root);
			appmanager->setCurrentCanvas(cnv);
		}
		if(eventType == DRAGGED) anamenubutondurum = 1;
	}else anamenubutondurum = 0;
}


void gCanvas::karakterYolDegistirme() {
	//Sola Gidiyor
	if(karaktereskiyol > karakteryol){
		isChanging = true;
		if(karakterdegisim < serit[karakteryol]){
			karakter.setPosition(karakter.getPosX(), karakter.getPosY(), karakterdegisim);
			karakterdegisim += karakterdegisimhizi;
		}
		//Karakter Durdu
		if(karakterdegisim >= serit[karakteryol]){
			karaktereskiyol = karakteryol;
			if(karakteryol != SOL)karakterdegisim = 0.0f;
			else karakterdegisim = serit[SOL];
			isChanging = false;
			karakter.pan(-0.5f);
		}
	}

	//Saga Gidiyor
	if(karaktereskiyol < karakteryol){
		isChanging = true;
		if(karakterdegisim > serit[karakteryol]){
			karakter.setPosition(karakter.getPosX(), karakter.getPosY(), karakterdegisim);
			karakterdegisim -= karakterdegisimhizi;
		}
		//Karakter Durdu
		if(karakterdegisim <= serit[karakteryol]){
			karaktereskiyol = karakteryol;
			if(karakteryol != SAG)karakterdegisim = 0.0f;
			else karakterdegisim = serit[SAG];
			isChanging = false;
			karakter.pan(0.5f);
		}
	}
}

void gCanvas::engelSetup() {
	engel.loadModel("barikat/barikat.obj");
	engelw = engel.getBoundingBox().getWidth();
	engelh = engel.getBoundingBox().getHeight();
	engeld = engel.getBoundingBox().getDepth();
	engelUret();
	engelhitbox.scale(engelw / 2, engelh / 2, engeld / 2);
}

void gCanvas::engelUret() {
	for (int mapId = 0; mapId < mapsayisi; ++mapId) {
		for (float konumx = -20.0f; konumx > -mapw; konumx -= 20.0f) {
			yeniengel.push_back(engelw);//w
			yeniengel.push_back(engelh);//h
			yeniengel.push_back(engeld);//d
			yeniengel.push_back(konumx - (mapw * mapId));//x
			yeniengel.push_back(karakter.getPosY());//y
			yeniengel.push_back(serit[(int)gRandom(seritsayisi)]);//z
			yeniengel.push_back(mapId);//mapId
			engellistesi.push_back(yeniengel);
			yeniengel.clear();
		}
	}
}

void gCanvas::engelDraw() {
	for (int sira = 0; sira < engellistesi.size(); ++sira) {
		if(engellistesi[sira][ENGEL_MAPID] < karakterhangiyol)continue;
		if(engellistesi[sira][ENGEL_MAPID] > karakterhangiyol + 1) break;
		engel.setPosition(engellistesi[sira][ENGEL_X], engellistesi[sira][ENGEL_Y], engellistesi[sira][ENGEL_Z]);
		engel.draw();
		//engelhitbox.draw();
	}
}

void gCanvas::karakterVsEngel() {
	for (int sira = 0; sira < engellistesi.size(); ++sira) {
		if(engellistesi[sira][ENGEL_MAPID] != karakterhangiyol)continue;
		engelhitbox.setPosition(engellistesi[sira][ENGEL_X],engellistesi[sira][ENGEL_Y] + engelh / 2,engellistesi[sira][ENGEL_Z]);
		if(karakterhitbox.getBoundingBox().intersects(engelhitbox.getBoundingBox())){
			//karakter olecek yada caný azalacak
			karakterdurum = ROBOT_OLDU;
			oyunmodu = OYUN_BITTI;
			//karakter yavaslayacak
			karakterhiz = 0.0f;
		}
	}
}

void gCanvas::karakterKeyMap(int key) {
	if(key == 32 && isGround == true){
		karakterhizy = 0.5f;
		isGround = false;
		karakterdurum = ROBOT_TAKLA;
	}
	if(key == G_KEY_A && !isChanging){
		if(karakteryol != SOL){
			karakteryol--;
			karakter.pan(0.5f);
		}

	}
	if(key == G_KEY_D && !isChanging){
		if(karakteryol != SAG){
			karakteryol++;
			karakter.pan(-0.5f);
		}
	}

	if(key == G_KEY_W && !isChanging){
		if(karakterhiz >= 0.36f){
			karakterhiz = 0.6f;
		}
	}
}

void gCanvas::oyunsonuSetup() {
	oyunsonu.loadImage("pause2/gameover_ekran.png");
	oyunsonuw = oyunsonu.getWidth();
	oyunsonuh = oyunsonu.getHeight();
	oyunsonux = getWidth() / 2 - oyunsonuw / 2;
	oyunsonuy = getHeight() / 2 - oyunsonuh / 2;
}

void gCanvas::oyunsonuDraw() {
	if(oyunmodu != OYUN_BITTI && oyunmodu != OYUN_WIN) return;
	oyunsonu.draw(oyunsonux, oyunsonuy);
	devambuton[devambutondurum].draw(devambutonx, devambutony);
	restartbuton[restartbutondurum].draw(restartbutonx, restartbutony);
	anamenubuton[anamenubutondurum].draw(anamenubutonx, anamenubutony);
}

void gCanvas::scoreKaydiSetup() {
	//Dosyayi yukle
	scorekaydi.loadFile("highscore.txt", gFile::FILEMODE_READWRITE);
	std::string data;
	data = "Highscore;" + gToStr(coinscore) + ";";
	scorekaydi.write(data);
	scorekaydi.close();
	scorekaydi.loadFile("highscore.txt", gFile::FILEMODE_READONLY);
	data = scorekaydi.getText(); //Dosyadaki butun veri kaydedildi
	std::vector<std::string> datalistesi; //Bolunecek kisimlarin yerlestirilecegi vector
	datalistesi = gSplitString(data, ";"); //Belirtilen ozel karakter (;) kullanýlarak metni parcalýyor ve vectore atýyor
	int score = 0;
	for (int var = 0; var < datalistesi.size(); ++var) {
		if((var % 2) == 1)score = gToInt(datalistesi[var]);
		gLogi("gCanvas") << datalistesi[0] << score;
	}
}

void gCanvas::coinSetup() {
	coin.loadModel("coinnew/coin.obj");
	coinw = coin.getBoundingBox().getWidth();
	coinh = coin.getBoundingBox().getHeight();
	coind = coin.getBoundingBox().getDepth();
	coin.scale(0.4f);
	coinhitbox.scale(coinw / 2, coinh / 2, coind / 2);
	coinUret();
}

void gCanvas::coinUret() {
	for (int mapId = 0; mapId < mapsayisi; ++mapId) {
		for (float konumx = -30.0f; konumx > -mapw; konumx -= 20.0f) {
			yenicoin.push_back(coinw);//w
			yenicoin.push_back(coinh);//h
			yenicoin.push_back(coind);//d
			yenicoin.push_back(konumx - (mapw * mapId));//x
			yenicoin.push_back(karakter.getPosY() + coinh / 3);//y
			yenicoin.push_back(serit[(int)gRandom(seritsayisi)]);//z
			yenicoin.push_back(mapId);//mapId
			yenicoin.push_back(0); //Rotate
			coinlistesi.push_back(yenicoin);
			yenicoin.clear();
		}
	}
}

void gCanvas::coinDraw() {
	for (int sira = 0; sira < coinlistesi.size(); ++sira) {
		coin.setPosition(coinlistesi[sira][COIN_X], coinlistesi[sira][COIN_Y], coinlistesi[sira][COIN_Z]);
		coin.draw();
		coinhitbox.setPosition(coinlistesi[sira][COIN_X], coinlistesi[sira][COIN_Y], coinlistesi[sira][COIN_Z]);
		//coinhitbox.draw();
	}
	coin.panDeg(3);
}

void gCanvas::coinPanelSetup() {
	coinpanel.loadImage("altinpanel1.png");
	coinpanelw = coinpanel.getWidth();
	coinpanelh = coinpanel.getHeight();
	coinpanelx = 0;
	coinpanely = 0;
	coinyazi.loadFont("FreeSans.ttf", 18);
}

void gCanvas::coinToplandi() {
	for (int coinID = 0; coinID < coinlistesi.size(); ++coinID) {
		if(coinlistesi[coinID][COIN_MAPID] > karakterhangiyol) break;
		if(coinlistesi[coinID][COIN_MAPID] < karakterhangiyol) continue;
		//Coin Hitbox Guncellendi
		coinhitbox.setPosition(coinlistesi[coinID][COIN_X], coinlistesi[coinID][COIN_Y], coinlistesi[coinID][COIN_Z]);
		if(karakterhitbox.getBoundingBox().intersects(coinhitbox.getBoundingBox())){
			coinscore++;
			coinlistesi.erase(coinlistesi.begin() + coinID);
			break;
		}
	}
}

void gCanvas::coinPanelDraw() {
	coinpanel.draw(coinpanelx, coinpanely);
	coinyazi.drawText("" + gToStr(coinscore), coinpanelx + coinpanelw * 0.4f, coinpanely + coinpanelh * 0.75f);
}

void gCanvas::startLineSetup() {
	startline.loadModel("street-final/basla/start.obj");
	startlinew = startline.getBoundingBox().getWidth();
	startlineh = startline.getBoundingBox().getHeight();
	startlined = startline.getBoundingBox().getDepth();
	startline.setScale(0.2f);
	startline.boom(10.0f);
	startline.truck(karakter.getPosX());
	startline.truck(-10.0f);
	//Start Camera Efekti Baslangic Konumu
	camera.setPosition(karakter.getPosition());
	camera.boom(8.5f);
	camera.dolly(5.0f);
	//Gecen Sureyi Toplayan Degisken
	zamansayaci = 0;
}

void gCanvas::startLineDraw() {
	startline.draw();
}

void gCanvas::saveGame(std::vector<std::string> &kaydedilecekBaslik,
					   std::vector<int> &kaydedilecekDeger) {
	gFile savefile;
	//Kayit dosyasini ac (yoksa olusturacak)
	savefile.loadFile("save.txt", gFile::FILEMODE_WRITEONLY);
	//Dosyaya degerleri yazdiriyor (Yazim formati ile birlikte)
	for (int kayitID = 0; kayitID < kaydedilecekDeger.size(); ++kayitID) {
		savefilesrc += kaydedilecekBaslik[kayitID] + " = ";
		savefilesrc += gToStr(kaydedilecekDeger[kayitID]);
		//Eger son kayit satirinda degil ise ";" aksi takdirde bir alt satira gec
		if((kaydedilecekDeger.size() - 1) == kayitID)savefilesrc += ";";
		else savefilesrc += ";\n";
	}
	//Dosyaya kaydet
	savefile.write(savefilesrc);
	//Dosyayi kapat
	savefile.close();
}

void gCanvas::loadGame() {
	gFile savefile;
	savefile.loadFile("save.txt", gFile::FILEMODE_READWRITE);
	//Dosya hasarli mi degil mi kontrolu ile gorevli degisken
	bool savefilecrashed = false;
	//Dosya var mi yok mu kontrolu
	if(!savefile.exists()){
		savefilecrashed = true;
	}else{
		//Butun dosya bir metin degiskenine aktarýlýyor
		savefiletext = savefile.getText();
		//Veri bos mu degil mi onu kontrol edecek
		if(savefiletext.empty()){
			savefilecrashed = true;
		}
	}
	//Ustteki sonuca gore takip edilecek islem
	//Hata varsa
	if(savefilecrashed){
		coinscore = 0;
		kaydedilecekbaslik.push_back("Score");
		kaydedilecekdeger.push_back(coinscore);
		kaydedilecekbaslik.push_back("Score2");
		kaydedilecekdeger.push_back(coinscore + 1000);
		saveGame(kaydedilecekbaslik, kaydedilecekdeger);
		kaydedilecekdeger.clear();
		kaydedilecekbaslik.clear();
	}
	gLogi("save.txt") << savefiletext;
	//Dosyadaki metinleri verilere bolecek
	kayitparametreleri = gSplitString(savefiletext, ";\n");
	//Her bir satirdan "deger" ayristirilarak alinir
	for (int satirno = 0; satirno < kayitparametreleri.size(); ++satirno) {
		//Satiralardaki butun bosluklar silinecek
		kayitparametreleri[satirno].erase(std::remove(kayitparametreleri[satirno].begin(),
							kayitparametreleri[satirno].end(), ' '), kayitparametreleri[satirno].end());
		//Esittirin oldugu yeri bulma
		size_t pos = kayitparametreleri[satirno].find('=');
		//Esittiri bulduktan sonraki islem
		if(pos != std::string::npos){
			//Esittirdikten sonraki degeri al
			std::string parcano = kayitparametreleri[satirno].substr(pos + 1);
			//Aldigimiz degeri int e donustur
			int deger = std::atoi(parcano.c_str());
			//int e donusen degeri degiskene aktarma
			loadcoin.push_back(deger);
			//gLogi("kayitparamterleri") << kayitparametreleri[satirno];
		}
	}
	//Score degeri gerekli degiskene atandi
	coinscore = loadcoin[0];
	loadcoin.clear();
}

void gCanvas::finishLineSetup() {
	finishline.loadModel("street-final/basla/start.obj");
	finishlinew = finishline.getBoundingBox().getWidth();
	finishlineh = finishline.getBoundingBox().getHeight();
	finishlined = finishline.getBoundingBox().getDepth();
	finishline.setScale(0.2f);
	finishline.boom(10.0f);
	finishline.truck(-mapw * 5.5f);
	finishlineHitbox.setScale(finishlinew / 8, finishlineh / 2, finishlined / 8);
}

void gCanvas::finishLineDraw() {
	finishline.draw();
	//finishlineHitbox.draw();
}

void gCanvas::isGameFinish() {
	finishlineHitbox.setPosition(finishline.getPosX() - 2.0f, 1.8f, finishline.getPosZ());
	if(karakterhitbox.getBoundingBox().intersects(finishlineHitbox.getBoundingBox())){
		oyunmodu = OYUN_WIN;
	}
}

void gCanvas::finishmeterSetup() {
	finishmeter.loadImage("pause2/ses_cerceve.png");
	finishmeterw = finishmeter.getWidth();
	finishmeterh = finishmeter.getHeight();
	finishmeterx = getWidth() / 2 - finishmeterw / 2;
	finishmetery = 20;

	finishmeterpw = 0;
	finishmeterph = finishmeterh;
	finishmeterpx = finishmeterx + 5;
	finishmeterpy = finishmetery;

	maplength = map.getBoundingBox().getWidth() * mapsayisi;
	maptometer = finishmeterw / maplength;
}

void gCanvas::finishmeterDraw() {
	gDrawRectangle(finishmeterpx, finishmeterpy, finishmeterpw, finishmeterph, true);
	finishmeter.draw(finishmeterx, finishmetery);

	karakterkonumorani = (karakter.getPosX() / finishline.getPosX()) * 100;
	finishmeterpw = (maptometer * karakterkonumorani) * ((finishmeterw - 10) / (maptometer * 100));
}

void gCanvas::loadekraniSetup() {
	loadekrani.loadImage("gui_loadingtext3.png");
	loadekraniw = loadekrani.getWidth();
	loadekranih = loadekrani.getHeight();
	loadekranix = getWidth() / 2 - loadekraniw / 2;
	loadekraniy = getHeight() / 2 - loadekranih / 2;
}
