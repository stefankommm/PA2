//
// Created by stefam on 15. 5. 2023.
//

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
    Esc,
    W,
    A,
    S,
    D,
    None
};

/**
 * @brief Handles incoming keypresses from the terminal, loads strings from cin
 * Class handling incoming communication from the terminal
 */
class InputHandler {
public:

    // Vypne kanonicky mod, umozni nacitanie znaku neblokovane
    /**
     * Function which non-blockingly gets the last character written to the buffer.
     * @return Returns the appropriate Key to the inputted character/string of characters for keys like Enter,Esc...
     */
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

        char ch;
        while (read(0, &ch, 1) > 0) {
            // vymaz zvysny buffer, aby sa nenacital zase dalsi krat
        }

        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
            perror("tcsetattr ~ICANON");

        fcntl(STDIN_FILENO, F_SETFL, oldf);

        if (nbytes == 3 && buf[0] == '\033' && buf[1] == '[') {  // escape chars pre sipky
            std::cout << "\r";  // prehod kurzor na zaciatok, nepomaha velmi
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
                case '\033':
                    return Key::Esc;
            }
        }
        return Key::None;
    }

    /**
     * Function which handles loading strings from the user input.
     * Used for inputting names, names of files where the map is.
     * @return Returns the appropriate string
     */
    static std::string getString() {
        disableNonCanonicalMode();
        std::string input;
        std::getline(std::cin, input);
        enableNonCanonicalMode();
        return input;
    }

    /**
     * Enables nonCanonicalMode which if disabled allows user to get non-blocking
     * input from the console terminal.
     */
    static void enableNonCanonicalMode() {
        struct termios term;
        if (tcgetattr(STDIN_FILENO, &term) == -1) {
            perror("tcgetattr");
            return;
        }
        //Flags in the terminal corresponding to the Canonical mode
        term.c_lflag &= ~(ICANON | ECHO);
        term.c_cc[VMIN] = 0;
        term.c_cc[VTIME] = 0;

        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) == -1) {
            perror("tcsetattr");
            return;
        }
    }

    /**
    * Disables nonCanonicalMode which if disabled allows user to get non-blocking
    * input from the console terminal.
    */
    static void disableNonCanonicalMode() {
        struct termios term;
        if (tcgetattr(STDIN_FILENO, &term) == -1) {
            perror("tcgetattr");
            return;
        }

        // nastav naspat kanonicky mod
        term.c_lflag |= (ICANON | ECHO);

        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) == -1) {
            perror("tcsetattr");
            return;
        }
    }
};
