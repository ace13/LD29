#include "Application.hpp"
#include "Util/FontFinder.hpp"
#include "Settings.hpp"

int main(int argc, char** argv)
{
	Settings::loadSettings();

	FontFinder::init();
	Application app;

	app.init(argc, argv);

	int ret = app.run();

	Settings::saveSettings();

	return ret;
}
