#include "TerminalUI.h"
#include <iostream>
#include <algorithm>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif

void TerminalUI::init() {
#ifdef _WIN32
    // Bật ANSI escape code cho Windows 10+
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}

TermSize TerminalUI::getSize() const {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return {24, 80};
    }
    int cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return {rows, cols};
#else
    winsize w{};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) return {24, 80};
    return {static_cast<int>(w.ws_row), static_cast<int>(w.ws_col)};
#endif
}

void TerminalUI::clear() const {
    // clear + home
    std::cout << "\x1b[2J\x1b[H";
    std::cout.flush();
}

void TerminalUI::moveCursor(int row, int col) const {
    row = std::max(1, row);
    col = std::max(1, col);
    std::cout << "\x1b[" << row << ";" << col << "H";
}

void TerminalUI::hideCursor(bool hide) const {
    std::cout << (hide ? "\x1b[?25l" : "\x1b[?25h");
    std::cout.flush();
}

void TerminalUI::printAt(int row, int col, const std::string& s) const {
    moveCursor(row, col);
    std::cout << s;
    std::cout.flush();
}

void TerminalUI::printCentered(int row, const std::string& s) const {
    TermSize ts = getSize();
    int col = (ts.cols - static_cast<int>(s.size())) / 2 + 1;
    printAt(row, col, s);
}

int TerminalUI::centerTop(int boxHeight) const {
    TermSize ts = getSize();
    return std::max(1, (ts.rows - boxHeight) / 2 + 1);
}

int TerminalUI::centerLeft(int boxWidth) const {
    TermSize ts = getSize();
    return std::max(1, (ts.cols - boxWidth) / 2 + 1);
}

void TerminalUI::drawBox(int top, int left, int height, int width) const {
    if (height < 3 || width < 3) return;

    // góc + cạnh
    std::string topLine    = "+" + std::string(width - 2, '-') + "+";
    std::string midLine    = "|" + std::string(width - 2, ' ') + "|";
    std::string bottomLine = "+" + std::string(width - 2, '-') + "+";

    printAt(top, left, topLine);
    for (int r = 1; r <= height - 2; r++) {
        printAt(top + r, left, midLine);
    }
    printAt(top + height - 1, left, bottomLine);
}
