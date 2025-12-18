#include "CustomerController.h"
#include <iostream>
#include <limits>  // để dùng std::numeric_limits
#include "../views/TerminalUI.h"

void CustomerController::viewAllPitches(const std::vector<Pitch>& pitches) const {
    TerminalUI ui;
    ui.showCustomerAllPitches(pitches);
}


void CustomerController::viewFreePitches(const std::vector<Pitch>& pitches,
                                         const std::vector<Booking>& bookings) const
{
    TerminalUI ui;

    // ===== (1) UI nhập timeSlot trong khung =====
    TermSize ts = ui.getSize();

    int boxWidth  = std::min(110, ts.cols - 4);
    int boxHeight = std::min(30,  ts.rows - 4);
    if (boxWidth < 70)  boxWidth  = std::min(ts.cols - 2, 70);
    if (boxHeight < 14) boxHeight = std::min(ts.rows - 2, 14);

    int top  = ui.centerTop(boxHeight);
    int left = ui.centerLeft(boxWidth);

    ui.clear();
    ui.drawBox(top, left, boxHeight, boxWidth);

    int innerLeft = left + 1;
    int innerW    = boxWidth - 2;

    std::string title = "SAN TRONG THEO KHUNG GIO";
    int titleCol = innerLeft + std::max(0, (innerW - (int)title.size()) / 2);
    ui.printAt(top + 1, titleCol, title);
    ui.drawHLine(top + 2, left, boxWidth, '-');

    std::string ask = "Nhap khung gio (VD: 2025-12-01 18:00-19:00):";
    ui.printAt(top + 4, innerLeft + 2, ui.fitText(ask, innerW - 4));
    ui.printAt(top + 6, innerLeft + 2, "> ");

    // Xóa '\n' còn lại trong buffer trước khi getline (đúng logic cũ)
    std::cin.clear();
    

    ui.moveCursor(top + 6, innerLeft + 4);
    std::string timeSlot;
    std::getline(std::cin >> std::ws, timeSlot);
    

    // ===== (2) LỌC đúng logic cũ: TRÙNG CHUỖI timeSlot =====
    std::vector<const Pitch*> freeList;
    freeList.reserve(pitches.size());

    for (const auto& p : pitches) {
        bool occupied = false;
        for (const auto& b : bookings) {
            if (b.getPitchId() == p.getId()
                && b.getTimeSlot() == timeSlot
                && b.getStatus() == BookingStatus::Active)
            {
                occupied = true;
                break;
            }
        }
        if (!occupied) freeList.push_back(&p);
    }

    // ===== (3) Render kết quả UI bảng 4 cột cách đều =====
    ui.clear();
    ui.drawBox(top, left, boxHeight, boxWidth);

    std::string title2 = "SAN TRONG O KHUNG GIO: " + timeSlot;
    int title2Col = innerLeft + std::max(0, (innerW - (int)title2.size()) / 2);
    ui.printAt(top + 1, title2Col, ui.fitText(title2, innerW));
    ui.drawHLine(top + 2, left, boxWidth, '-');

    if (freeList.empty()) {
        std::string msg = "Khong co san trong o khung gio nay.";
        int msgCol = innerLeft + std::max(0, (innerW - (int)msg.size()) / 2);
        ui.printAt(top + 5, msgCol, msg);

        std::string hint = "Nhan ENTER de quay lai...";
        ui.printAt(top + boxHeight - 2, innerLeft + 2, ui.fitText(hint, innerW - 4));
        return; // main sẽ view.pause()
    }

    int colStart = innerLeft + 1;
    int usableW  = innerW - 2;

    // 4 cột cách đều nhau
    const int sepTotal = 3 * 3; // " | " * 3
    int contentW = usableW - sepTotal;
    if (contentW < 20) contentW = 20;

    int base = contentW / 4;
    int rem  = contentW % 4;

    int wId    = base + (rem > 0 ? 1 : 0);
    int wName  = base + (rem > 1 ? 1 : 0);
    int wPrice = base + (rem > 2 ? 1 : 0);
    int wSize  = base;

    auto pad = [](const std::string& s, int w) {
        if ((int)s.size() >= w) return s.substr(0, w);
        return s + std::string(w - (int)s.size(), ' ');
    };

    auto printRow = [&](int r,
                        const std::string& id,
                        const std::string& name,
                        const std::string& price,
                        const std::string& size)
    {
        std::string sId    = pad(ui.fitText(id,    wId),    wId);
        std::string sName  = pad(ui.fitText(name,  wName),  wName);
        std::string sPrice = pad(ui.fitText(price, wPrice), wPrice);
        std::string sSize  = pad(ui.fitText(size,  wSize),  wSize);

        std::string line = sId + " | " + sName + " | " + sPrice + " | " + sSize;
        if ((int)line.size() > usableW) line = line.substr(0, usableW);

        ui.printAt(r, colStart, line);
    };

    int row = top + 3;
    printRow(row, "ID", "Ten san", "Gia", "Loai");
    row++;

    ui.drawHLine(row, left, boxWidth, '-');
    row++;

    int maxRows = top + boxHeight - 2;
    for (const auto* pp : freeList) {
        if (row > maxRows) break;

        const Pitch& p = *pp;
        printRow(row,
                 std::to_string(p.getId()),
                 p.getName(),
                 std::to_string((long long)p.getPrice()),
                 std::to_string(p.getSize()) + " nguoi");
        row++;
    }

    std::string hint = "Nhan ENTER de quay lai...";
    ui.printAt(top + boxHeight - 2, innerLeft + 2, ui.fitText(hint, innerW - 4));
}

void CustomerController::bookPitch(const std::vector<Pitch>& pitches,
                                   std::vector<Booking>& bookings,
                                   const std::string& username)
{
    TerminalUI ui;
    TermSize ts = ui.getSize();

    int boxWidth  = std::min(110, ts.cols - 4);
    int boxHeight = std::min(26,  ts.rows - 4);
    if (boxWidth < 70)  boxWidth  = std::min(ts.cols - 2, 70);
    if (boxHeight < 14) boxHeight = std::min(ts.rows - 2, 14);

    int top  = ui.centerTop(boxHeight);
    int left = ui.centerLeft(boxWidth);

    auto showMsg = [&](const std::string& title, const std::string& msg) {
        ui.clear();
        ui.drawBox(top, left, boxHeight, boxWidth);

        int innerLeft = left + 1;
        int innerW    = boxWidth - 2;

        int titleCol = innerLeft + std::max(0, (innerW - (int)title.size()) / 2);
        ui.printAt(top + 1, titleCol, title);
        ui.drawHLine(top + 2, left, boxWidth, '-');

        ui.printAt(top + 5, innerLeft + 2, ui.fitText(msg, innerW - 4));

        std::string hint = "Nhan ENTER de quay lai...";
        ui.printAt(top + boxHeight - 2, innerLeft + 2, ui.fitText(hint, innerW - 4));
    };

    // ====== FORM NHAP ID SAN ======
    ui.clear();
    ui.drawBox(top, left, boxHeight, boxWidth);

    int innerLeft = left + 1;
    int innerW    = boxWidth - 2;

    std::string title = "DAT SAN (CUSTOMER)";
    int titleCol = innerLeft + std::max(0, (innerW - (int)title.size()) / 2);
    ui.printAt(top + 1, titleCol, title);
    ui.drawHLine(top + 2, left, boxWidth, '-');

    std::string uline = "Tai khoan: " + username;
    ui.printAt(top + 4, innerLeft + 2, ui.fitText(uline, innerW - 4));

    ui.printAt(top + 6, innerLeft + 2, "Nhap ID san muon dat: ");
    ui.printAt(top + 7, innerLeft + 2, "> ");

    // nhập ID (đúng logic cũ: cin >> id)
    int id;
    ui.moveCursor(top + 7, innerLeft + 4);
    std::cin >> id;

    // ====== TIM SAN ======
    const Pitch* selectedPitch = nullptr;
    for (const auto& p : pitches) {
        if (p.getId() == id) {
            selectedPitch = &p;
            break;
        }
    }

    if (!selectedPitch) {
        showMsg("KET QUA DAT SAN", "Khong tim thay san.");
        return;
    }

    // ====== NHAP TIMESLOT ======
    // (quan trọng) dùng getline với std::ws để tránh lỗi "lần 2 mới ăn"
    ui.clear();
    ui.drawBox(top, left, boxHeight, boxWidth);

    ui.printAt(top + 1, titleCol, title);
    ui.drawHLine(top + 2, left, boxWidth, '-');

    std::string info1 = "San: " + selectedPitch->getName() +
                        " | Gia: " + std::to_string((long long)selectedPitch->getPrice()) +
                        " | Loai: " + std::to_string(selectedPitch->getSize()) + " nguoi";
    ui.printAt(top + 4, innerLeft + 2, ui.fitText(info1, innerW - 4));

    std::string ask = "Nhap khung gio muon dat (VD: 2025-12-01 18:00-19:00):";
    ui.printAt(top + 6, innerLeft + 2, ui.fitText(ask, innerW - 4));
    ui.printAt(top + 7, innerLeft + 2, "> ");

    ui.moveCursor(top + 7, innerLeft + 4);
    std::string timeSlot;
    std::getline(std::cin >> std::ws, timeSlot); // ✅ fix lỗi nhập lần 2 mới lấy

    // ====== CHECK TRUNG (Y LOGIC CU) ======
    for (const auto& b : bookings) {
        if (b.getPitchId() == id &&
            b.getTimeSlot() == timeSlot &&
            b.getStatus() == BookingStatus::Active)
        {
            showMsg("KET QUA DAT SAN",
                    "Khung gio nay cho san nay da duoc dat. Vui long chon khung gio khac.");
            return;
        }
    }

    // ====== TAO BOOKING (Y LOGIC CU) ======
    bookings.emplace_back(
        id,
        username,
        timeSlot,
        "",
        "",
        BookingStatus::Active,
        0.0
    );

    showMsg("KET QUA DAT SAN",
            "Dat san thanh cong! San " + selectedPitch->getName() + " vao khung gio: " + timeSlot);
}