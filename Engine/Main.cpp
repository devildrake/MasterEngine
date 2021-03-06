#include <stdlib.h>
#include "Application.h"
#include "Modules/ModuleRender.h"
#include <Globals.h>

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/lib/x86/SDL2.lib" )
#pragma comment( lib, "SDL/lib/x86/SDL2main.lib" )
#include <Leaks.h>
#include <Brofiler.h>

enum main_states {
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = NULL;

void DumpLeaks(void) {
	_CrtDumpMemoryLeaks(); //Show leaks with file and line where allocation was made
}

int main(int argc, char** argv) {
	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;

	while (state != MAIN_EXIT) {
		switch (state) {
		case MAIN_CREATION:

			LOG("Application Creation --------------");
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG("Application Init --------------");
			if (App->Init() == false) {
				LOG("Application Init exits with error -----");
				state = MAIN_EXIT;
			} else if (App->Start() == false) {

			} else {
				state = MAIN_UPDATE;
				LOG("Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			BROFILER_FRAME("YourThreadName");

			int update_return = App->Update();

			if (update_return == UPDATE_ERROR) {
				LOG("Application Update exits with error -----");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
		break;

		case MAIN_FINISH:

			LOG("Application CleanUp --------------");
			if (App->CleanUp() == false) {
				LOG("Application CleanUp exits with error -----");
			} else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}

	}
	LOG("Bye :)\n");
	delete App;
	DumpLeaks();
	return main_return;
}
