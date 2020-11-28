# MasterEngine
This is a repository to keep working versions and progression of the Engine I'll be developing for my AAA videogame deveolpment masters degree at UPC

The engine currently works as a rather simple Model viewer, allowing user to load textured models, and place upon them different textures as diffuse 

The engine assignment features include:

- An automatically loaded Baker House with its texture on Start
- A main window which, upon modifying its size, does not alter the view on the geometry displayed. (recalculating horizontal FOV and Aspect Ratio)
- Drag & Drop files: files dropped form anywhere inside the hard drive will be processed and models will substitute the currently loaded model, 
  deleting the previous one, while textures on the other hand will subtitute the current model's texture. Any other file type will generate a log and nothing more.
- Logging of all load events via the Console Window
- Unity like camera control, including WASD movement while pressing Right Click on the mouse, orbiting around the current model by holding left Alt and 
  moving the mouse, zooming in and out with the scrollwheel, fcusing the camera view on the current model by pressing the F key and multiplying the 
  movement speed by 3 by holding the left shift button. Holding the Left mouse button and dragging it will move the user's camera along the X and Y axis.
- The camera adapts its focusing distance to the loaded model's size and scaling, both on loading a new model and on pressing the F key.
- A main menu on the top left that displays options to toggle the ImGui windows on/off (including about window), as well as allowing the user to exit the 
application and navigating to the Github repository.
- Said windows include:
   - A properties window, displaying information about the current model (transform, model vertices, model faces and texture size)
   - A configuration window, displaying information about the hardware, the frame rates, the main window, the rendering options, the camera and some debugging features.
   - A console window, display all logged information.
   - An about window, displaying information about the author, the engine, the used libraries and the project's license.
- On startup, the main window's size will scale relatively to the desktop's size. The bigger the desktop, the bigger the window.
- Two additional working models can be found inside the folder Resources/Models
- Every step of the assimp and texture loading is logged onto the console window.

Additional features include:

- Right clicking on any ImGuiWindow's name bar will display the option to close it.
- All ImGuiWindows feature a toggle on/off shortcut: ALT + Y for console, ALT + A for about, ALT + U for configuration, ALT + P for properties.
- Application may be shut down by pressing the top right corner X button or pressing ALT + F4.
- The transform section of the Properties window is not read-only, but it also allows user interaction, modifying the models position, rotation and scale.
- The application section of the configuration window allows users to lock/unlock FPS capping, as well as modifying its value
- The window section of the configuration window allows users to toggle window options and modify the window size and brightness (aspect ratio is read only)
- The renderer section of the configuration window allows some features to be toggled on/off (face culling, depth testing, wireframe mode).
- The camera section of the configuration window allows users to modify the speeds of most of the movement features, as well as the editor's background color.
- The debug section of the configuration window allows users to modify the grids length, color, step and height.
- The console window allows users to toggle the autoscroll option, as well as featuring a scroll down button and the right click option on the log to clear all messages.
- While moving the camera with the Orbit mode or with the WASD feature, whenever the mouse cursor gets out of the window bounds, it will reset its position to the opposite 
  side of the window. The cursor will be invisible while any of these two features are on use.
- The scrollwheel and left mouse button movement options for the camera will be disabled if the initial click or the mouse wheel motion takes place while the cursor was 
  over any ImGui window/menu. This is so to prevent conflicts between functionalities.