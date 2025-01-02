
#include "gAppManager.h"
#include "gApp.h"


int main(int argc, char **argv) {

	gStartEngine(new gApp(argc, argv), "Sonsuz Kosu", G_WINDOWMODE_APP, 1920, 1080);

	return 0;
}
