# bnkchip8
Simple Chip8 emulator written in C++.
picture 1
picture 2
### Building the project
In order to build this project you must have these tools installed:
- **C++ compiler** - Any compiler will work however on Windows I would recommend using MSVC.
- **CMake** - This is our meta build tool which keeps track of all of the files, dependencies, configs and scripts that we will need to build. 
- **Ninja** - This is what we are using to actually build our project.
- **Git** - This is to clone the repo.

This project does rely on 2 dependencies which are:
- **GLFW** - This is for window and input handling.
- **GLAD** - This is for modern OpenGL functions.

However these are included in the `/external` directory so we do not need to download these and link them.

##### Commands
Once you have these installed, open a command prompt (you will need to use the Developer Command Prompt/Powershell if you are compiling with MSVC) and cd into the folder where you want to store the repo. Then run this command:
`git clone https://github.com/shaythorn97/bnkchip8.git`

Next you will need to run this command in order to generate the Ninja build files:
`cmake -B build -G Ninja`

Finally we run this command to build the project:
`ninja -C build`

Once everything is built we can run this command to run the project:
`ninja -C build run`

Its important that we run the project using this command as it makes sure that we are using the correct `/roms` directory. If you want to run this project using something like Visual Studio you will need to create your own `/roms` directory inside the build folder where the .exe file can see it.

##### Running ROMs
When you run the project it will run a test ROM that will check if all the instructions are working correctly. If you want to run some other ROMs there are several supplied in the `/roms` directory. You can swap them out by opening `/src/main.cpp` and changing the file name of the the current ROM to match an existing one inside the `/roms` directory. We could change it to use the `pong.ch8` file for example.
```
int main()
{
    // we can change the ROM here
    ROM rom("pong.ch8");
    Chip8 chip8(rom);

    while (chip8.IsRunning())
    {
        chip8.EmulateCycle();
        
        chip8.Display();
    }
    
    return 0;
}
```

If you want to use your own ROMs all you need to do is just add them into the `/roms` directory and then change the file name inside the code and then run the project. A list of Chip8 ROMs can be found in this repo here:
`https://github.com/kripod/chip8-roms`
