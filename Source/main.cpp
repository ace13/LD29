#include "Application.hpp"
#include "Util/FontFinder.hpp"

int main(int argc, char** argv)
{
	FontFinder::init();
	Application app;

	app.init(argc, argv);

	return app.run();
}
