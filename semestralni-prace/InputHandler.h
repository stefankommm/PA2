//
// Created by stefam on 15. 5. 2023.
//

#pragma once


#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

enum class Key {
    Up,
    Down,
    Left,
    Right,
    Enter,
    Space,
    W,
    A,
    S,
    D,
    None
};

class InputHandler {
public:
    static Key getInput() {
        char buf[3] = {0};
        struct termios old = {0};

        if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 0;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");

        int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

        int nbytes = read(0, buf, sizeof(buf));

        // discard extra characters
        char ch;
        while (read(0, &ch, 1) > 0) {
            // discard extra characters
        }

        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
            perror("tcsetattr ~ICANON");

        fcntl(STDIN_FILENO, F_SETFL, oldf);

        if (nbytes == 3 && buf[0] == '\033' && buf[1] == '[') {  // escape sequence for arrow keys
            std::cout << "\r";  // Move cursor to start of line
            switch (buf[2]) {
                case 'A':
                    return Key::Up;
                case 'B':
                    return Key::Down;
                case 'C':
                    return Key::Right;
                case 'D':
                    return Key::Left;
            }
        } else if (nbytes == 1) {
            switch (buf[0]) {
                case '\n':
                    return Key::Enter;
                case ' ':
                    return Key::Space;
                case 'w':
                    return Key::W;
                case 'a':
                    return Key::A;
                case 's':
                    return Key::S;
                case 'd':
                    return Key::D;
            }
        }
        return Key::None;
    }


    std::string getString() {
        this->disableNonCanonicalMode();
        std::string input;
        std::getline(std::cin, input);
        this->enableNonCanonicalMode();
        return input;
    }

    static void enableNonCanonicalMode() {
        struct termios term;
        if (tcgetattr(STDIN_FILENO, &term) == -1) {
            perror("tcgetattr");
            return;
        }

        // Modify the terminal attributes
        term.c_lflag &= ~(ICANON | ECHO);
        term.c_cc[VMIN] = 0;
        term.c_cc[VTIME] = 0;

        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) == -1) {
            perror("tcsetattr");
            return;
        }
    }

// Disable non-canonical mode and restore terminal settings
   static void disableNonCanonicalMode() {
        struct termios term;
        if (tcgetattr(STDIN_FILENO, &term) == -1) {
            perror("tcgetattr");
            return;
        }

        // Restore the terminal attributes to their original values
        term.c_lflag |= (ICANON | ECHO);

        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) == -1) {
            perror("tcsetattr");
            return;
        }
    }
};
