# Labyrinth Game

Labyrinth is a cross-platform game for Terminal/Command Prompt.

Tested under Ubuntu 16.04, Windows 10 and macOS 10.12.5.

*__Please note:__* The game *may* as well work under other operating systems.


## Installation

As of now, there are two ways to run the game:

* Build the game from source
* Run the pre-built binary

*__Please note__* The game is still in development, so at some point binaries may be unavailable. Sorry for the inconvenience.

### Installation: Build program
Before you start make sure you have a C compiler and ```cmake``` utility installed on your computer. You will also need ```pdcurses``` library (Windows) or ```ncurses``` library (Linux, macOS). Once you have everything on your computer, simply clone the repository and build it with the following:

    git clone https://github.com/paper-lark/labyrinth && cd labyrinth && make
    
The executable can be found in the **bin/** folder.

### Installation: Run prebuilt executable
Simply find the appropriate executable in the **bin/** folder: **labyrinth.mac** for macOS, 
                                                               **labyrinth.linux** for Linux,
                                                               **labyrinth.exe** for Windows.
                                                                 
*__Please note:__* The program depends on ```pdcurses``` or ```ncurses``` library depending on your OS as mentioned in the previous section.
Thus, if you are going to run the program on Unix-like system install ```nucurses``` library,
or if you're running on Windows just make sure that ```pdcurses.dll``` library (which can be found in ```bin/``` folder) is located in the same folder as the executable.

## Bugs
If you find any bug, please report it at https://github.com/paper-lark/Labyrinth/issues

## Suggestion
If you have any suggestions about the project or new features, feel free to contact me.

## License
Labyrinth is available under the terms of the MIT License. See ```LICENSE``` for more.
