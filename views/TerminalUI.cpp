#include "TerminalUI.h"
#include <iostream>
#include <algorithm>
#include <limits> // NEW
#include <cstdlib> // dùng cho hàm clear
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

// Khởi tạo môi trường trên terminal. Môi trường này hổ trợ cho việc chỉnh sửa giao diện UI 
void TerminalUI::init()
{
#ifdef _WIN32
    // Bật ANSI escape code cho Windows 10+
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
        return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
        return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}

// Đọc kích thước cửa sổ terminal
TermSize TerminalUI::getSize() const
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {
        return {24, 80};
    }
    int cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return {rows, cols};
}

// Xóa giao diện hiện tại trên terminal
void TerminalUI::clear() const
{
#ifdef _WIN32
    std::system("cls");                 // ✅ Windows CMD/VSCode terminal: clear chắc chắn
#endif
    std::cout.flush();
}

// Di chuyển con trỏ chuột đến vị trí mình muốn 
void TerminalUI::moveCursor(int row, int col) const
{
    row = std::max(1, row);
    col = std::max(1, col);
    std::cout << "\x1b[" << row << ";" << col << "H";
}

//Hàm này dùng để in một chuỗi theo vị trí row,col
void TerminalUI::printAt(int row, int col, const std::string &s) const
{
    moveCursor(row, col);
    std::cout << s;
    std::cout.flush();
}

//Hàm này dùng để in một chuỗi theo hàng ngang , chuỗi này nằm giữa khung 
void TerminalUI::printCentered(int row, const std::string &s) const
{
    TermSize ts = getSize();
    int col = (ts.cols - static_cast<int>(s.size())) / 2 + 1;
    printAt(row, col, s);
}

// tính ra vị trí của row để vẽ chiều cao của khung box . Làm cho box cách đều trên và dưới 
int TerminalUI::centerTop(int boxHeight) const
{
    TermSize ts = getSize();
    return std::max(1, (ts.rows - boxHeight) / 2 + 1);
}

// tính ra vị trí của cột để vẽ chiều ngang của khung box . Làm cho box cách đều trái và phải 
int TerminalUI::centerLeft(int boxWidth) const
{
    TermSize ts = getSize();
    return std::max(1, (ts.cols - boxWidth) / 2 + 1);
}

// Hàm vẽ khung box
void TerminalUI::drawBox(int top, int left, int height, int width) const
{
    if (height < 3 || width < 3)
        return;

    // góc + cạnh
    std::string topLine = "+" + std::string(width - 2, '-') + "+";
    std::string midLine = "|" + std::string(width - 2, ' ') + "|";
    std::string bottomLine = "+" + std::string(width - 2, '-') + "+";

    printAt(top, left, topLine);
    for (int r = 1; r <= height - 2; r++)
    {
        printAt(top + r, left, midLine);
    }
    printAt(top + height - 1, left, bottomLine);
}

//Hàm dùng để fix lỗi khi nội dung tràn khung box
std::string TerminalUI::fitText(const std::string &s, int width) const
{
    if (width <= 0)
        return "";
    if ((int)s.size() <= width)
        return s;
    if (width <= 3)
        return s.substr(0, width);
    return s.substr(0, width - 3) + "...";
}

// Dùng để vẽ một đường Ngang. Trong dự án này là dùng để phân cách tiêu đề và các phần bên dưới 
void TerminalUI::drawHLine(int row, int left, int width, char ch) const
{
    if (width < 3)
        return;
    // kẻ ngang trong box, giữ 2 cạnh '|'
    std::string line;
    line.reserve(width);
    line.push_back('|');
    line += std::string(width - 2, ch);
    line.push_back('|');
    printAt(row, left, line);
}


////////////////////////////////////////// Chỉnh Màu 
void TerminalUI::setColor(Color c) const
{
    // ANSI color codes
    const char* code = "\x1b[0m"; // Default
    switch (c)
    {
    case Color::Green:  code = "\x1b[32m"; break;
    case Color::Red:    code = "\x1b[31m"; break;
    case Color::Yellow: code = "\x1b[33m"; break;
    case Color::Cyan:   code = "\x1b[36m"; break;
    case Color::White:  code = "\x1b[37m"; break;
    default:            code = "\x1b[0m";  break;
    }
    std::cout << code;
}

void TerminalUI::resetColor() const
{
    std::cout << "\x1b[0m";
}

void TerminalUI::printAtColor(int row, int col, Color c, const std::string& s) const
{
    moveCursor(row, col);
    setColor(c);
    std::cout << s;
    resetColor();
    std::cout.flush();
}

void TerminalUI::printCenteredInBoxColor(int row, int boxLeft, int boxWidth, Color c, const std::string& s) const
{
    // vùng trong box: left+1 .. left+width-2
    int innerLeft = boxLeft + 1;
    int innerW    = boxWidth - 2;

    // Căn giữa theo ĐỘ DÀI THẬT CỦA TEXT (không tính ANSI)
    int col = innerLeft + std::max(0, (innerW - (int)s.size()) / 2);

    printAtColor(row, col, c, s);
}

