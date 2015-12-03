#include "ClassDemoApp.h"

#include <SDL.h>

int main(int argc, char *argv[])
{
	ClassDemoApp app;
	while (!app.UpdateAndRender());
	return 0;
}
