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

TermSize TerminalUI::getSize() const
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {
        return {24, 80};
    }
    int cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return {rows, cols};
#else
    winsize w{};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
        return {24, 80};
    return {static_cast<int>(w.ws_row), static_cast<int>(w.ws_col)};
#endif
}

void TerminalUI::clear() const
{
#ifdef _WIN32
    std::system("cls");                 // ✅ Windows CMD/VSCode terminal: clear chắc chắn
#else
    std::cout << "\x1b[2J\x1b[H";       // ✅ Linux/mac: ANSI ok
#endif
    std::cout.flush();
}

void TerminalUI::moveCursor(int row, int col) const
{
    row = std::max(1, row);
    col = std::max(1, col);
    std::cout << "\x1b[" << row << ";" << col << "H";
}

void TerminalUI::hideCursor(bool hide) const
{
    std::cout << (hide ? "\x1b[?25l" : "\x1b[?25h");
    std::cout.flush();
}

void TerminalUI::printAt(int row, int col, const std::string &s) const
{
    moveCursor(row, col);
    std::cout << s;
    std::cout.flush();
}

void TerminalUI::printCentered(int row, const std::string &s) const
{
    TermSize ts = getSize();
    int col = (ts.cols - static_cast<int>(s.size())) / 2 + 1;
    printAt(row, col, s);
}

int TerminalUI::centerTop(int boxHeight) const
{
    TermSize ts = getSize();
    return std::max(1, (ts.rows - boxHeight) / 2 + 1);
}

int TerminalUI::centerLeft(int boxWidth) const
{
    TermSize ts = getSize();
    return std::max(1, (ts.cols - boxWidth) / 2 + 1);
}

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

// =========================
// NEW: helper
// =========================
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

// =========================
// NEW: Customer screen - Xem tất cả sân
// =========================
void TerminalUI::showCustomerAllPitches(const std::vector<Pitch> &pitches) const
{
    TermSize ts = getSize();

    // Box size an toàn (tránh tràn màn hình)
    int boxWidth = std::min(110, ts.cols - 4);
    int boxHeight = std::min(28, ts.rows - 4);

    if (boxWidth < 60)
        boxWidth = std::min(ts.cols - 2, 60);
    if (boxHeight < 10)
        boxHeight = std::min(ts.rows - 2, 10);

    int top = centerTop(boxHeight);
    int left = centerLeft(boxWidth);

    clear();
    drawBox(top, left, boxHeight, boxWidth);

    // vùng in bên trong box: [left+1 .. left+boxWidth-2]
    int innerLeft = left + 1;
    int innerW = boxWidth - 2;

    // Title
    std::string title = "TAT CA SAN";
    int titleCol = innerLeft + std::max(0, (innerW - (int)title.size()) / 2);
    printAt(top + 1, titleCol, title);

    // separator
    drawHLine(top + 2, left, boxWidth, '-');

    if (pitches.empty())
    {
        std::string msg = "Chua co san nao.";
        int msgCol = innerLeft + std::max(0, (innerW - (int)msg.size()) / 2);
        printAt(top + 4, msgCol, msg);
        return;
    }

    // Chia cột theo boxWidth để không tràn
    // Nội dung thực tế mỗi dòng chỉ được phép nằm trong innerW-2 (vì còn 1 khoảng padding)
    // Mình chừa 1 ký tự padding bên trái trong box: bắt đầu in tại innerLeft + 1
    int colStart = innerLeft + 1;
    int usableW = innerW - 2;

    // 4 cột, giữa các cột có " | " (3 ký tự), tổng 3 lần => 9 ký tự
    const int sepTotal = 3 * 3;
    int contentW = usableW - sepTotal;
    if (contentW < 20)
        contentW = 20; // an toàn tối thiểu

    int base = contentW / 4;
    int rem = contentW % 4;

    // widths đều nhau (chênh tối đa 1 ký tự)
    int wId = base + (rem > 0 ? 1 : 0);
    int wName = base + (rem > 1 ? 1 : 0);
    int wPrice = base + (rem > 2 ? 1 : 0);
    int wSize = base;

    auto printRow = [&](int r,
                        const std::string &id,
                        const std::string &name,
                        const std::string &price,
                        const std::string &size)
    {
        // build 1 dòng chắc chắn <= usableW
        std::string line;
        line.reserve(usableW);

        auto pad = [](const std::string &s, int w)
        {
            if ((int)s.size() >= w)
                return s.substr(0, w);
            return s + std::string(w - (int)s.size(), ' ');
        };

        std::string sId = pad(id, wId);
        std::string sName = pad(fitText(name, wName), wName);
        std::string sPrice = pad(fitText(price, wPrice), wPrice);
        std::string sSize = pad(fitText(size, wSize), wSize);

        line = sId + " | " + sName + " | " + sPrice + " | " + sSize;

        // đảm bảo không tràn usableW
        if ((int)line.size() > usableW)
            line = line.substr(0, usableW);

        printAt(r, colStart, line);
    };

    // Header
    int row = top + 3;
    printRow(row, "ID", "Ten san", "Gia", "Loai");
    row++;

    // separator
    drawHLine(row, left, boxWidth, '-');
    row++;

    // Data rows (giới hạn theo boxHeight)
    int maxRows = top + boxHeight - 2; // trước dòng viền dưới
    for (const auto &p : pitches)
    {
        if (row > maxRows)
            break;

        std::string id = std::to_string(p.getId());
        std::string name = p.getName();
        std::string price = std::to_string((long long)p.getPrice()); // cho gọn
        std::string size = std::to_string(p.getSize()) + " nguoi";

        printRow(row, id, name, price, size);
        row++;
    }

    // hint cuối box
    std::string hint = "Nhan ENTER de quay lai...";
    printAt(top + boxHeight - 2, innerLeft + 2, fitText(hint, innerW - 4));
}

