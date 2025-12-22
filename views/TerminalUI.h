#pragma once
#include <string>
#include <vector>              

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
    void moveCursor(int row, int col) const; 

    // vẽ khung
    void drawBox(int top, int left, int height, int width) const;

    // in text
    void printAt(int row, int col, const std::string& s) const;
    void printCentered(int row, const std::string& s) const;

    // tiện ích căn giữa box
    int centerTop(int boxHeight) const;
    int centerLeft(int boxWidth) const;

    std::string fitText(const std::string& s, int width) const;   // cắt chữ tránh tràn
    void drawHLine(int row, int left, int width, char ch = '-') const; // kẻ ngang trong box

};
