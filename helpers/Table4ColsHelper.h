// Canh đều và in bảng 4 cột trong terminal
#ifndef TABLE_4COLS_HELPER_H
#define TABLE_4COLS_HELPER_H

#include <string>
#include <algorithm>
#include "../views/TerminalUI.h" // để dùng ui.fitText()

#include <string>
#include <cctype>

namespace Table4
{

    struct ColWidths
    {
        int w1{}, w2{}, w3{}, w4{};
    };

    // pad phải (đủ độ rộng cột)
    inline std::string padRight(const std::string &s, int w)
    {
        if ((int)s.size() >= w)
            return s.substr(0, w);
        return s + std::string(w - (int)s.size(), ' ');
    }

    // Tính độ rộng 4 cột theo logic bạn đang dùng
    inline ColWidths calcWidths(int usableW)
    {
        const int sepTotal = 3 * 3; // " | " * 3
        int contentW = usableW - sepTotal;
        if (contentW < 20)
            contentW = 20;

        int base = contentW / 4;
        int rem = contentW % 4;

        ColWidths w;
        w.w1 = base + (rem > 0 ? 1 : 0);
        w.w2 = base + (rem > 1 ? 1 : 0);
        w.w3 = base + (rem > 2 ? 1 : 0);
        w.w4 = base;
        return w;
    }

    // Build 1 dòng 4 cột (đã fitText + pad + cắt theo usableW)
    inline std::string buildRow(const TerminalUI &ui, const ColWidths &w,
                                int usableW,const std::string &c1,const std::string &c2,
                                const std::string &c3, const std::string &c4)
    {
        std::string s1 = padRight(ui.fitText(c1, w.w1), w.w1);
        std::string s2 = padRight(ui.fitText(c2, w.w2), w.w2);
        std::string s3 = padRight(ui.fitText(c3, w.w3), w.w3);
        std::string s4 = padRight(ui.fitText(c4, w.w4), w.w4);

        std::string line = s1 + " | " + s2 + " | " + s3 + " | " + s4;
        if ((int)line.size() > usableW)
            line = line.substr(0, usableW);
        return line;
    }

} // namespace Table4

namespace CheckCharacter
{
    inline bool containsAlphaAZ(const std::string &s)
    {
        for (unsigned char c : s)
        {
            if (std::isalpha(c))
                return true;
        }
        return false;
    }
}

#endif
