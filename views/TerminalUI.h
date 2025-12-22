#pragma once
#include <string>
#include <vector>

struct TermSize
{
    int rows;
    int cols;
};
enum class Color
{
    Default,
    Green,
    Red,
    Yellow,
    Cyan,
    White
};

class TerminalUI
{
public:
    // gọi 1 lần đầu chương trình (Windows cần bật ANSI)
    void init();

    TermSize getSize() const;

    void clear() const;
    void moveCursor(int row, int col) const;

    // vẽ khung
    void drawBox(int top, int left, int height, int width) const;

    // in text
    void printAt(int row, int col, const std::string &s) const;
    void printCentered(int row, const std::string &s) const;

    // tiện ích căn giữa box
    int centerTop(int boxHeight) const;
    int centerLeft(int boxWidth) const;

    std::string fitText(const std::string &s, int width) const;        // cắt chữ tránh tràn
    void drawHLine(int row, int left, int width, char ch = '-') const; // kẻ ngang trong box

    /////////////////////////////////////////////////// Chỉnh Màu 
    //Chức năng: đổi màu chữ hiện tại của terminal.
    void setColor(Color c) const;

    //Chức năng: đưa màu chữ về mặc định (thường là trắng/xám).
    void resetColor() const;

    //Chức năng: in một chuỗi s tại đúng tọa độ (row, col) với màu c.
    void printAtColor(int row, int col, Color c, const std::string &s) const;

    // Căn giữa theo box (đúng UI của bạn đang vẽ box)
    // boxLeft = left của box, boxWidth = width của box
    void printCenteredInBoxColor(int row, int boxLeft, int boxWidth, Color c, const std::string &s) const;

};
