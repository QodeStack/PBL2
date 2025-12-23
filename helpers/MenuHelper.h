#ifndef MENU_HELPER_H
#define MENU_HELPER_H

#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <algorithm>
#include "../models/Pitch.h"

namespace MenuHelper {

// Dừng màn hình: chờ người dùng nhấn ENTER đúng 1 lần
inline void pause()
{
    std::cin.clear();

    // Nếu buffer đang có sẵn ký tự (thường là '\n' dư), bỏ nó đi trước
    if (std::cin.rdbuf()->in_avail() > 0)
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Chờ ENTER
    std::string dummy;
    std::getline(std::cin, dummy);
}

// Tách chuỗi theo '\n' (bỏ qua '\r' nếu có)
inline std::vector<std::string> splitLines(const std::string &s)
{
    std::vector<std::string> lines;
    std::string cur;

    for (char c : s)
    {
        if (c == '\r') continue;
        if (c == '\n')
        {
            lines.push_back(cur);
            cur.clear();
        }
        else cur.push_back(c);
    }
    lines.push_back(cur);
    return lines;
}

} // namespace MenuHelper

namespace PitchSort {
inline std::vector<Pitch> sortedById(const std::vector<Pitch>& pitches, bool asc = true)
{
    std::vector<Pitch> v = pitches; // copy
    std::sort(v.begin(), v.end(), [&](const Pitch& a, const Pitch& b){
        return asc ? (a.getId() < b.getId()) : (a.getId() > b.getId());
    });
    return v;
}

} // namespace PitchSort

#endif

