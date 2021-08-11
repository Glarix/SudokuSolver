# Sudoku Game in C with Solver
#### Project by Codreanu Dan 
---

## Description
With this Sudoku project you can choose and play Sudoku games out of 4 different categories:
* Easy
* Medium
* Hard
* Expert

You can either solve the entire board yourself in a normal way or press ENTER key at any moment to have the game solved automatically for you.
There are 80 locally saved levels ( 20 for each difficulty category) that you can choose to solve from and also you can choose Random levels from a specific category ( requires an internet connection to load the random level ).
It was developed on Ubuntu, and currently only compiles and runs in a Linux environment.

---
## Project's prerequisites and insallation process
---

Before compiling and running the program you must satisfy the following dependencies:

* The SDL2 library package. For Ubuntu you can get it by typing the following command in terminal:

    ```bash
    sudo apt-get install libsdl2-2.0
    ```
* The SDL2 development package. For Ubuntu you can get it by typing the following command in terminal:

    ```bash
    sudo apt-get install libsdl2-dev
    ```

* The SDL_ttf development library. For Ubuntu you can get it by typing the following command in terminal:
   
    ```bash
    sudo apt-get install libsdl2-ttf-dev
    ```

* The curl.h library ( OpenSSL variant). For Ubuntu you can get it by typing the following command in terminal:

    ```bash
    sudo apt-get install libcurl4-openssl-dev
    ```

After getting the prerequisites you can run make command for the program to compile.

---
## How to use the Program
---

To run the program you have to type the following command:
```bash
./Sudoku <PATH TO CURRENT DIRECTORY>
```    
You must include the path to the directory that contains the \<levels\> directory.

After the program runs you can select either a Random level or a pre-saved level by clicking one of the two buttons. 

Next the 4 possible difficulty categories will appear on the screen and you will have to sellect one of them:
* 1 - Easy
* 2 - Medium 
* 3 - Hard 
* 4 - Expert 

If Pre-saved level option was selected, you will now have the option to choose one out of the 20 possible levels.
If Random level option was selected the above step is not necessary.

Now the game should be loaded and you can play. 
Left-click on empty square to select and press a digit key to place the wanted number in the square.

If the number is good then it will appear with Green, else a RED X will appear on the bottom of the screen.

Three mistakes are allowed before losing the game.

To unselect a selected square you can right-click it.

At any moment you can press ENTER key and the remaining numbers will be automatically filled.

After each game is finished you have to close the window and rerun the program.

---

## Directory structure
* __src/__  ( source code files )
* __levels/__  ( the directories and files containing pre-saved levels data)
* __include/__  ( header files for src/ code)
* __src/obj/__  ( object files )