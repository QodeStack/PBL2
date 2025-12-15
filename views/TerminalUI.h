#pragma once
#include <string>


struct TermSize {
    int rows;
    int cols;
};

class TerminalUI {
public:
    // gọi 1 lần đầu chương trình (Windows cần bật ANSI)
    void init();

    TermSize getSize() const;

    void clear() const;
    void moveCursor(int row, int col) const; // 1-based
    void hideCursor(bool hide) const;

    // vẽ khung
    void drawBox(int top, int left, int height, int width) const;

    // in text
    void printAt(int row, int col, const std::string& s) const;
    void printCentered(int row, const std::string& s) const;

    // tiện ích căn giữa box
    int centerTop(int boxHeight) const;
    int centerLeft(int boxWidth) const;
};
