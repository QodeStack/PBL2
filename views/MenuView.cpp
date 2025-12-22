#include "MenuView.h"
#include <iostream>
#include <limits>
#include "TerminalUI.h"

#include <iomanip>
#include <sstream>

// helper đọc choice an toàn
static int readChoice()
{
    int choice;
    while (true)
    {
        if (std::cin >> choice)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // tránh dính newline
            return choice;
        }
        // nhập sai (ký tự), reset
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Nhap sai. Vui long nhap so!\nChon: ";
    }
}

int MenuView::showMainMenu() const
{
    TerminalUI ui;
    ui.init();
    ui.clear();

    const int boxW = 60;
    const int boxH = 11;
    int top = ui.centerTop(boxH);
    int left = ui.centerLeft(boxW);

    ui.drawBox(top, left, boxH, boxW);

    ui.printCentered(top + 1, "HE THONG QUAN LY SAN BONG");
    ui.printAt(top + 3, left + 3, "1. Dang nhap");
    ui.printAt(top + 4, left + 3, "2. Dang ky (khach hang)");
    ui.printAt(top + 5, left + 3, "0. Thoat");
    ui.printAt(top + 7, left + 3, "Chon: ");

    ui.moveCursor(top + 7, left + 9);
    return readChoice();
}

int MenuView::showAdminMenu() const
{
    TerminalUI ui;
    ui.init();
    ui.clear();

    const int boxW = 70;
    const int boxH = 15;
    int top = ui.centerTop(boxH);
    int left = ui.centerLeft(boxW);

    ui.drawBox(top, left, boxH, boxW);

    ui.printCentered(top + 1, "MENU ADMIN");
    ui.printAt(top + 3, left + 3, "1. Xem danh sach san");
    ui.printAt(top + 4, left + 3, "2. Them san (Create)");
    ui.printAt(top + 5, left + 3, "3. Sua san (Update)");
    ui.printAt(top + 6, left + 3, "4. Xoa san (Delete)");
    ui.printAt(top + 7, left + 3, "5. Dat san OFFLINE tai quay");
    ui.printAt(top + 8, left + 3, "6. Xem lich dat CHUA tinh tien");
    ui.printAt(top + 9, left + 3, "7. Tinh tien san");
    ui.printAt(top + 10, left + 3, "0. Dang xuat");
    ui.printAt(top + 12, left + 3, "Chon: ");

    ui.moveCursor(top + 12, left + 9);
    return readChoice();
}

int MenuView::showCustomerMenu() const
{
    TerminalUI ui;
    ui.init();
    ui.clear();

    const int boxW = 60;
    const int boxH = 12;
    int top = ui.centerTop(boxH);
    int left = ui.centerLeft(boxW);

    ui.drawBox(top, left, boxH, boxW);

    ui.printCentered(top + 1, "MENU KHACH HANG");
    ui.printAt(top + 3, left + 3, "1. Xem tat ca san");
    ui.printAt(top + 4, left + 3, "2. Xem san trong");
    ui.printAt(top + 5, left + 3, "3. Dat san");
    ui.printAt(top + 6, left + 3, "0. Dang xuat");
    ui.printAt(top + 8, left + 3, "Chon: ");

    ui.moveCursor(top + 8, left + 9);
    return readChoice();
}

// GIAO DIỆN
std::pair<std::string, std::string> MenuView::showLoginForm() const
{
    TerminalUI ui;
    ui.init();
    ui.clear();

    const int boxW = 70;
    const int boxH = 12;
    int top = ui.centerTop(boxH);
    int left = ui.centerLeft(boxW);

    ui.drawBox(top, left, boxH, boxW);

    ui.printCentered(top + 1, "DANG NHAP");
    ui.printAt(top + 3, left + 3, "Ten Nguoi Dung: ");
    ui.printAt(top + 5, left + 3, "Mat Khau : ");
    ui.printAt(top + 8, left + 3, "Nhap 0 o Username de quay lai");

    std::string username, password;

    // ✅ Đưa con trỏ vào ô nhập Username
    ui.moveCursor(top + 3, left + 19);
    std::getline(std::cin >> std::ws, username);

    if (username == "0")
        return {"0", ""};

    // ✅ Đưa con trỏ vào ô nhập Password
    ui.moveCursor(top + 5, left + 14);
    std::getline(std::cin, password);

    return {username, password};
}

std::pair<std::string, std::string> MenuView::showRegisterForm()
{
    TerminalUI ui;
    TermSize ts = ui.getSize();

    int boxWidth  = std::min(110, ts.cols - 4);
    int boxHeight = std::min(22,  ts.rows - 4);
    if (boxWidth < 70)  boxWidth  = std::min(ts.cols - 2, 70);
    if (boxHeight < 14) boxHeight = std::min(ts.rows - 2, 14);

    int top  = ui.centerTop(boxHeight);
    int left = ui.centerLeft(boxWidth);

    int innerLeft = left + 1;
    int innerW    = boxWidth - 2;

    ui.clear();
    ui.drawBox(top, left, boxHeight, boxWidth);

    std::string title = "DANG KY";
    int titleCol = innerLeft + std::max(0, (innerW - (int)title.size()) / 2);
    ui.printAt(top + 1, titleCol, title);
    ui.drawHLine(top + 2, left, boxWidth, '-');

    // Labels
    std::string lblUser = "Ten nguoi dung: ";
    std::string lblPass = "Mat khau     : ";

    ui.printAt(top + 4, innerLeft + 2, lblUser);
    ui.printAt(top + 6, innerLeft + 2, lblPass);

    ui.printAt(top + boxHeight - 3, innerLeft + 2, "Nhap 0 o Username de quay lai");

    // Input (nhap ngay sau label)
    std::string username, password;

    ui.moveCursor(top + 4, innerLeft + 2 + (int)lblUser.size());
    std::getline(std::cin >> std::ws, username);

    if (username == "0") return {"0", ""};

    ui.moveCursor(top + 6, innerLeft + 2 + (int)lblPass.size());
    std::getline(std::cin >> std::ws, password);

    return {username, password};
}


void MenuView::pause() const {
    // chỉ chờ Enter, không in thêm thông báo
    if (std::cin.peek() == '\n') std::cin.get();
    std::cin.get();
}

void MenuView::showPitchesScreen(const std::vector<Pitch>& pitches) const {
    TerminalUI ui;
    ui.init();
    ui.clear();

    const int boxW = 110;
    const int boxH = 22;
    int top  = ui.centerTop(boxH);
    int left = ui.centerLeft(boxW);

    ui.drawBox(top, left, boxH, boxW);
    ui.printCentered(top + 1, "DANH SACH SAN");

    // ====== CĂN LỀ + GAP ======
    const int marginL = 4;
    const int marginR = 4;
    const int gap     = 6;

    const int innerW   = boxW - 2;
    const int contentW = innerW - marginL - marginR;
    const int gapsW    = gap * 3;

    // In 1 dòng vào khung, có lề trái/phải
    auto printRow = [&](int r, const std::string& rowText) {
        std::string line = std::string(marginL, ' ') + rowText;

        if ((int)line.size() < marginL + contentW)
            line += std::string(marginL + contentW - line.size(), ' ');

        line += std::string(marginR, ' ');

        if ((int)line.size() < innerW) line += std::string(innerW - line.size(), ' ');
        if ((int)line.size() > innerW) line = line.substr(0, innerW);

        ui.printAt(r, left + 1, line);
    };

    // ✅ đường gạch ngang dưới title (giờ đã có printRow + contentW)
    ui.printAt(top + 2, left + 1, std::string(innerW, '-'));

    // ====== ĐỘ RỘNG CỘT (4 cột bằng nhau + xử lý phần dư) ======
    int base = (contentW - gapsW) / 4;
    int rem  = (contentW - gapsW) % 4;

    int idW    = base + (rem > 0 ? 1 : 0);
    int nameW  = base + (rem > 1 ? 1 : 0);
    int priceW = base + (rem > 2 ? 1 : 0);
    int typeW  = base;

    if (nameW < 15) nameW = 15;

    auto cut = [](const std::string& s, int w) {
        if ((int)s.size() <= w) return s;
        if (w <= 3) return s.substr(0, w);
        return s.substr(0, w - 3) + "...";
    };

    int row = top + 4; // ✅ bắt đầu nội dung dưới đường title

    // ====== HEADER ======
    {
        std::ostringstream oss;
        oss << std::left
            << std::setw(idW)    << "ID"      << std::string(gap, ' ')
            << std::setw(nameW)  << "TEN SAN" << std::string(gap, ' ')
            << std::setw(priceW) << "GIA"     << std::string(gap, ' ')
            << std::setw(typeW)  << "LOAI";
        printRow(row++, oss.str());
        printRow(row++, std::string(contentW, '-'));
    }

    // ====== BODY ======
    if (pitches.empty()) {
        printRow(row++, "Chua co san nao.");
    } else {
        for (const auto& p : pitches) {
            std::ostringstream oss;
            oss << std::left
                << std::setw(idW)    << p.getId() << std::string(gap, ' ')
                << std::setw(nameW)  << cut(p.getName(), nameW) << std::string(gap, ' ')
                << std::setw(priceW) << (long long)p.getPrice() << std::string(gap, ' ')
                << std::setw(typeW)  << (std::to_string(p.getSize()) + " nguoi");

            printRow(row++, oss.str());
            if (row >= top + boxH - 3) break;
        }
    }

    // ====== FOOTER ======
    printRow(top + boxH - 2, "Nhan ENTER de quay lai...");
    ui.moveCursor(top + boxH - 2, left + 1 + marginL + (int)std::string("Nhan ENTER de quay lai...").size());
}

PitchFormInput MenuView::showCreatePitchForm() const {
    TerminalUI ui;
    ui.init();
    ui.clear();

    const int boxW = 80;
    const int boxH = 16;
    int top  = ui.centerTop(boxH);
    int left = ui.centerLeft(boxW);

    ui.drawBox(top, left, boxH, boxW);
    ui.printCentered(top + 1, "THEM SAN");

    ui.printAt(top + 3, left + 3, "ID san   : ");
    ui.printAt(top + 5, left + 3, "Ten san  : ");
    ui.printAt(top + 7, left + 3, "Gia      : ");
    ui.printAt(top + 9, left + 3, "Loai san (5/7/11) : ");

    PitchFormInput in{};

    ui.moveCursor(top + 3, left + 14);
    std::cin >> in.id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    ui.moveCursor(top + 5, left + 14);
    std::getline(std::cin, in.name);

    ui.moveCursor(top + 7, left + 14);
    std::cin >> in.price;

    ui.moveCursor(top + 9, left + 22);
    std::cin >> in.size;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    ui.printAt(top + boxH - 2, left + 3, "Nhan ENTER de luu va quay lai...");
    ui.moveCursor(top + boxH - 2, left + 3 + (int)std::string("Nhan ENTER de luu va quay lai...").size());
    return in;
}

UpdatePitchInput MenuView::showUpdatePitchForm() const {
    TerminalUI ui;
    ui.init();
    ui.clear();

    const int boxW = 85;
    const int boxH = 18;
    int top  = ui.centerTop(boxH);
    int left = ui.centerLeft(boxW);

    ui.drawBox(top, left, boxH, boxW);
    ui.printCentered(top + 1, "SUA SAN");

    ui.printAt(top + 3, left + 3, "ID san can sua        : ");
    ui.printAt(top + 5, left + 3, "Ten moi (bo trong=giu): ");
    ui.printAt(top + 7, left + 3, "Gia moi (0=giu)       : ");
    ui.printAt(top + 9, left + 3, "Loai moi (0=giu)      : ");

    UpdatePitchInput in{};
    // ID
    ui.moveCursor(top + 3, left + 28);
    std::cin >> in.id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Ten moi
    ui.moveCursor(top + 5, left + 28);
    std::getline(std::cin, in.newName);

    // Gia moi
    ui.moveCursor(top + 7, left + 28);
    std::cin >> in.newPrice;

    // Loai moi
    ui.moveCursor(top + 9, left + 28);
    std::cin >> in.newSize;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    ui.printAt(top + boxH - 2, left + 3, "Nhan ENTER de cap nhat va quay lai...");
    ui.moveCursor(top + boxH - 2, left + 3 + (int)std::string("Nhan ENTER de cap nhat va quay lai...").size());

    return in;
}

std::pair<int, bool> MenuView::showDeletePitchForm() const {
    TerminalUI ui;
    ui.init();
    ui.clear();

    const int boxW = 75;
    const int boxH = 14;
    int top  = ui.centerTop(boxH);
    int left = ui.centerLeft(boxW);

    ui.drawBox(top, left, boxH, boxW);
    ui.printCentered(top + 1, "XOA SAN");

    ui.printAt(top + 4, left + 3, "Nhap ID san muon xoa : ");

    int id;
    ui.moveCursor(top + 4, left + 27);
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    ui.printAt(top + 6, left + 3, "Ban chac chan muon xoa? (Y/N) : ");

    char yn;
    ui.moveCursor(top + 6, left + 38);
    std::cin >> yn;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    bool confirm = (yn == 'Y' || yn == 'y');

    ui.printAt(top + boxH - 2, left + 3, "Nhan ENTER de quay lai...");
    ui.moveCursor(top + boxH - 2, left + 3 + (int)std::string("Nhan ENTER de quay lai...").size());

    return {id, confirm};
}

OfflineBookingInput MenuView::showOfflineBookingForm() const {
    TerminalUI ui;
    ui.init();
    ui.clear();

    const int boxW = 85;
    const int boxH = 14;
    int top  = ui.centerTop(boxH);
    int left = ui.centerLeft(boxW);

    ui.drawBox(top, left, boxH, boxW);
    ui.printCentered(top + 1, "DAT SAN OFFLINE TAI QUAY");

    ui.printAt(top + 4, left + 3, "ID san              : ");
    ui.printAt(top + 6, left + 3, "Ten khach (bo trong) : ");

    OfflineBookingInput in{};

    ui.moveCursor(top + 4, left + 25);
    std::cin >> in.pitchId;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    ui.moveCursor(top + 6, left + 25);
    std::getline(std::cin, in.customerName);

    ui.printAt(top + boxH - 2, left + 3, "Nhan ENTER de tiep tuc...");
    ui.moveCursor(top + boxH - 2, left + 3 + (int)std::string("Nhan ENTER de tiep tuc...").size());

    return in;
}

// Hàm tách dòng cho update 
static std::vector<std::string> splitLines(const std::string& s) {
    std::vector<std::string> lines;
    std::string cur;
    for (char c : s) {
        if (c == '\r') continue;          // bỏ CR nếu có
        if (c == '\n') { lines.push_back(cur); cur.clear(); }
        else cur.push_back(c);
    }
    lines.push_back(cur);
    return lines;
}
void MenuView::showMessageBox(const std::string& title, const std::vector<std::string>& lines) const {
    TerminalUI ui;
    ui.init();
    ui.clear();

    const int boxW = 150;
    const int boxH = 20;
    int top  = ui.centerTop(boxH);
    int left = ui.centerLeft(boxW);

    ui.drawBox(top, left, boxH, boxW);
    ui.printCentered(top + 1, title);

    int r = top + 4;
for (const auto& s : lines) {
    auto parts = splitLines(s);               // tách theo '\n'
    for (const auto& oneLine : parts) {
        ui.printAt(r++, left + 3, oneLine);   // mỗi lần in 1 dòng, không có '\n'
        if (r >= top + boxH - 3) break;
    }
    if (r >= top + boxH - 3) break;
}

    ui.printAt(top + boxH - 2, left + 3, "Nhan ENTER de quay lai...");
    ui.moveCursor(top + boxH - 2, left + 3 + (int)std::string("Nhan ENTER de quay lai...").size());
}

void MenuView::showUnpaidBookingsScreen(const std::vector<Booking>& bookings,const std::vector<Pitch>& pitches) const
{
    TerminalUI ui;
    ui.init();
    ui.clear();

    const int boxW = 125;
    const int boxH = 24;
    int top  = ui.centerTop(boxH);
    int left = ui.centerLeft(boxW);

    ui.drawBox(top, left, boxH, boxW);
    ui.printCentered(top + 1, "DANH SACH LICH DAT CHUA TINH TIEN");

    const int marginL = 3;
    const int marginR = 3;
    const int gap     = 3;

    const int innerW   = boxW - 2;
    const int contentW = innerW - marginL - marginR;

    auto cut = [](const std::string& s, int w) {
        if ((int)s.size() <= w) return s;
        if (w <= 3) return s.substr(0, w);
        return s.substr(0, w - 3) + "...";
    };

    auto printRow = [&](int r, const std::string& rowText) {
        std::string line = std::string(marginL, ' ') + rowText;

        if ((int)line.size() < marginL + contentW)
            line += std::string(marginL + contentW - line.size(), ' ');

        line += std::string(marginR, ' ');

        if ((int)line.size() < innerW) line += std::string(innerW - line.size(), ' ');
        if ((int)line.size() > innerW) line = line.substr(0, innerW);

        ui.printAt(r, left + 1, line);
    };

    // gạch ngang dưới title
    ui.printAt(top + 2, left + 1, std::string(innerW, '-'));

    // ====== 7 cột: BID | SAN | PITCH ID | KIEU | BAT DAU | KHACH | TONG
    const int cols = 7;
    const int gapsW = gap * (cols - 1);
    int base = (contentW - gapsW) / cols;
    int rem  = (contentW - gapsW) % cols;

    int wBID   = base + (rem > 0 ? 1 : 0);
    int wSAN   = base + (rem > 1 ? 1 : 0);
    int wPID   = base + (rem > 2 ? 1 : 0);
    int wKIEU  = base + (rem > 3 ? 1 : 0);
    int wSTART = base + (rem > 4 ? 1 : 0);
    int wKHACH = base + (rem > 5 ? 1 : 0);
    int wTONG  = base;

    int row = top + 4;

    // HEADER
    {
        std::ostringstream oss;
        oss << std::left
            << std::setw(wBID)   << "BID"      << std::string(gap, ' ')
            << std::setw(wSAN)   << "SAN"      << std::string(gap, ' ')
            << std::setw(wPID)   << "PITCH ID" << std::string(gap, ' ')
            << std::setw(wKIEU)  << "KIEU"     << std::string(gap, ' ')
            << std::setw(wSTART) << "BAT DAU"  << std::string(gap, ' ')
            << std::setw(wKHACH) << "KHACH"    << std::string(gap, ' ')
            << std::setw(wTONG)  << "TONG";
        printRow(row++, oss.str());
        printRow(row++, std::string(contentW, '-'));
    }

    // BODY
    int printed = 0;

    for (const auto& b : bookings)
    {
        // ✅ CHUA TINH TIEN = Active
        if (b.getStatus() != BookingStatus::Active || b.getTotalAmount() != 0.0)
    continue;


        int pitchId = b.getPitchId();

        // tìm tên sân
        std::string pitchName = "N/A";
        for (const auto& p : pitches) {
            if (p.getId() == pitchId) { pitchName = p.getName(); break; }
        }

        // KIEU: OFFLINE nếu timeSlot rỗng
        std::string type = b.getTimeSlot().empty() ? "OFFLINE" : "ONLINE";

        std::string start = b.getTimeSlot().empty()
                  ? b.getStartTime()
                  : b.getTimeSlot();   // ONLINE hiển thị TimeSlot
        std::string customer= b.getCustomerUsername();

        long long total = (long long)b.getTotalAmount();

        std::ostringstream oss;
        oss << std::left
            << std::setw(wBID)   << b.getId()                     << std::string(gap, ' ')
            << std::setw(wSAN)   << cut(pitchName, wSAN)          << std::string(gap, ' ')
            << std::setw(wPID)   << pitchId                      << std::string(gap, ' ')
            << std::setw(wKIEU)  << type                         << std::string(gap, ' ')
            << std::setw(wSTART) << cut(start, wSTART)            << std::string(gap, ' ')
            << std::setw(wKHACH) << cut(customer, wKHACH)         << std::string(gap, ' ')
            << std::setw(wTONG)  << total;

        printRow(row++, oss.str());
        printed++;

        if (row >= top + boxH - 3) break;
    }

    if (printed == 0) {
        printRow(row++, "Khong co lich dat CHUA tinh tien.");
    }

    // FOOTER
    printRow(top + boxH - 2, "Nhan ENTER de quay lai...");
    ui.moveCursor(top + boxH - 2, left + 1 + marginL + (int)std::string("Nhan ENTER de quay lai...").size());
}

// tính tiền 

int MenuView::showCheckoutChoosePitchForm() const {
    TerminalUI ui; ui.init(); ui.clear();
    const int boxW=75, boxH=12;
    int top=ui.centerTop(boxH), left=ui.centerLeft(boxW);
    ui.drawBox(top,left,boxH,boxW);
    ui.printCentered(top+1,"TINH TIEN SAN");
    ui.printAt(top+4,left+3,"Nhap ID san can tinh tien: ");
    int pitchId;
    ui.moveCursor(top+4,left+31);
    std::cin >> pitchId;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    ui.printAt(top+boxH-2,left+3,"Nhan ENTER de tiep tuc...");
    ui.moveCursor(top+boxH-2,left+3+(int)std::string("Nhan ENTER de tiep tuc...").size());
    return pitchId;
}

int MenuView::showCheckoutChooseBookingForm() const {
    TerminalUI ui; ui.init(); ui.clear();
    const int boxW=75, boxH=12;
    int top=ui.centerTop(boxH), left=ui.centerLeft(boxW);
    ui.drawBox(top,left,boxH,boxW);
    ui.printCentered(top+1,"CHON BOOKING DE TINH TIEN");
    ui.printAt(top+4,left+3,"Nhap Booking ID can tinh tien: ");
    int bookingId;
    ui.moveCursor(top+4,left+34);
    std::cin >> bookingId;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    ui.printAt(top+boxH-2,left+3,"Nhan ENTER de tiep tuc...");
    ui.moveCursor(top+boxH-2,left+3+(int)std::string("Nhan ENTER de tiep tuc...").size());
    return bookingId;
}




