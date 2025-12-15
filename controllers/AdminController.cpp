#include "AdminController.h"
#include <iostream>
#include <limits>   // <-- THÊM DÒNG NÀY
/// Set thời gian 
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
//// Set thời gian 

using std::cout;
using std::cin;
using std::string;

// Hàm lấy thời gian hiện tại dạng "YYYY-MM-DD HH:MM"
static std::string getCurrentTimeString() {
    auto now = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(now);
    std::tm localTm{};
#ifdef _WIN32
    localtime_s(&localTm, &tt);
#else
    localtime_r(&tt, &localTm);
#endif

    std::ostringstream oss;
    oss << std::put_time(&localTm, "%Y-%m-%d %H:%M");
    return oss.str();
} 
// Hàm parse "YYYY-MM-DD HH:MM" thành time_t
static bool parseDateTime(const std::string& s, std::tm& outTm) {
    std::istringstream iss(s);
    iss >> std::get_time(&outTm, "%Y-%m-%d %H:%M");
    return !iss.fail();
}
//Tính số giờ giữa 2 chuỗi datetime
static double diffHours(const std::string& start, const std::string& end) {
    std::tm tmStart{}, tmEnd{};
    if (!parseDateTime(start, tmStart) || !parseDateTime(end, tmEnd)) {
        return 0.0;
    }

    std::time_t tStart = std::mktime(&tmStart);
    std::time_t tEnd   = std::mktime(&tmEnd);
    if (tEnd <= tStart) return 0.0;

    double seconds = std::difftime(tEnd, tStart);
    return seconds / 3600.0;
}
//Tính số giờ từ timeSlot online "2025-12-01 18:00-20:00"
static double hoursFromTimeSlot(const std::string& timeSlot) {
    // Format: "YYYY-MM-DD HH:MM-HH:MM"
    // tách thành: date = "YYYY-MM-DD", start = "HH:MM", end = "HH:MM"
    std::string datePart, timeRange;
    std::istringstream iss(timeSlot);
    if (!(iss >> datePart >> timeRange)) {
        return 0.0;
    }

    auto pos = timeRange.find('-');
    if (pos == std::string::npos) return 0.0;

    std::string startTime = timeRange.substr(0, pos);       // "HH:MM"
    std::string endTime   = timeRange.substr(pos + 1);      // "HH:MM"

    std::string fullStart = datePart + " " + startTime;
    std::string fullEnd   = datePart + " " + endTime;

    return diffHours(fullStart, fullEnd);
}


void AdminController::listPitches(const std::vector<Pitch>& pitches) const {
    std::cout << "\n=== DANH SACH SAN ===\n";
    if (pitches.empty()) {
        std::cout << "Chua co san nao.\n";
        return;
    }
    for (const auto& p : pitches) {
        std::cout << "ID: " << p.getId()
                  << " | Ten: " << p.getName()
                  << " | Gia: " << p.getPrice()
                  << " | Loai san: " << p.getSize() << " nguoi";
        std::cout << '\n';
    }
}

void AdminController::createPitch(std::vector<Pitch>& pitches) {
    int id;
    std::string name;
    double price;
    int size;

    std::cout << "Nhap ID san: ";
    std::cin >> id;
    std::cin.ignore();
    std::cout << "Nhap ten san: ";
    std::getline(std::cin, name);
    std::cout << "Nhap gia san (VD: 200000): ";
    std::cin >> price;
    std::cout << "Nhap loai san (5, 7, 11): ";
    std::cin >> size;

    if (size != 5 && size != 7 && size != 11) {
        std::cout << "Loai san khong hop le! Chi chap nhan 5, 7, 11.\n";
        return;
    }
    // kiểm tra trùng ID
    for (const auto& p : pitches) {
        if (p.getId() == id) {
            std::cout << "ID san da ton tai!\n";
            return;
        }
    }
    pitches.emplace_back(id, name,price,size);
    std::cout << "Them san thanh cong!\n";
}

void AdminController::updatePitch(std::vector<Pitch>& pitches) {
    if (pitches.empty()) {
        std::cout << "Chua co san nao de sua.\n";
        return;
    }

    int id;
    std::cout << "Nhap ID san muon sua: ";
    std::cin >> id;
    // ⚠ Quan trọng: xóa hết ký tự còn trong buffer trước khi dùng getline
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (auto& p : pitches) {
        if (p.getId() == id) {
            std::cout << "\nThong tin hien tai:\n";
            std::cout << "ID: " << p.getId()
                      << " | Ten: " << p.getName()
                      << " | Gia: " << p.getPrice()
                      << " | Loai san: " << p.getSize() << " nguoi\n";

            // ====== SỬA TÊN SÂN ======
            std::string newName;
            std::cout << "\nNhap ten moi (de trong neu muon giu nguyen): ";
            std::getline(std::cin, newName);
            if (!newName.empty()) {
                p.setName(newName);
            }

            // ====== SỬA GIÁ SÂN ======
            std::cout << "Nhap gia moi (nhap -1 neu muon giu nguyen): ";
            double newPrice;
            std::cin >> newPrice;

            if (!std::cin.fail() && newPrice >= 0) {
                p.setPrice(newPrice);
            } else if (newPrice != -1) {
                std::cout << "Gia khong hop le, giu nguyen gia cu.\n";
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            // ====== SỬA LOẠI SÂN ======
            std::cout << "Nhap loai san moi (5, 7, 11; nhap -1 neu giu nguyen): ";
            int newSize;
            std::cin >> newSize;

            if (!std::cin.fail() && (newSize == 5 || newSize == 7 || newSize == 11)) {
                p.setSize(newSize);
            } else if (newSize != -1) {
                std::cout << "Loai san khong hop le, giu nguyen loai san cu.\n";
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "\n✅ Cap nhat thanh cong!\n";
            std::cout << "Thong tin moi:\n";
            std::cout << "ID: " << p.getId()
                      << " | Ten: " << p.getName()
                      << " | Gia: " << p.getPrice()
                      << " | Loai san: " << p.getSize() << " nguoi\n";
            return;
        }
    }

    std::cout << "Khong tim thay san.\n";
}

void AdminController::deletePitch(std::vector<Pitch>& pitches) {
    int id;
    std::cout << "Nhap ID san muon xoa: ";
    std::cin >> id;

    for (auto it = pitches.begin(); it != pitches.end(); ++it) {
        if (it->getId() == id) {
            pitches.erase(it);
            std::cout << "Xoa san thanh cong!\n";
            return;
        }
    }
    std::cout << "Khong tim thay san.\n";
}

///////////////////////////////////////////

void AdminController::bookPitchOffline(const std::vector<Pitch>& pitches,
                                       std::vector<Booking>& bookings) {
    if (pitches.empty()) {
        std::cout << "Chua co san nao trong he thong.\n";
        return;
    }

    int id;
    std::cout << "\n=== DAT SAN OFFLINE TAI QUAY ===\n";
    std::cout << "Nhap ID san muon dat: ";
    std::cin >> id;

    const Pitch* selectedPitch = nullptr;
    for (const auto& p : pitches) {
        if (p.getId() == id) {
            selectedPitch = &p;
            break;
        }
    }

    if (!selectedPitch) {
        std::cout << "Khong tim thay san.\n";
        return;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string customerName;
    std::cout << "Nhap ten khach (co the bo trong neu khong can luu ten): ";
    std::getline(std::cin, customerName);
    if (customerName.empty()) {
        customerName = "OFFLINE";
    }

    // 🌟 Với offline: ta KHÔNG dùng timeSlot để tính giờ, mà dùng startTime & endTime
    std::string start = getCurrentTimeString();

    // Tạo booking offline: timeSlot để trống, status = Active
    bookings.emplace_back(
        id,                // pitchId
        customerName,
        "",                // timeSlot
        start,             // startTime = thời điểm đặt sân
        "",                // endTime chưa có
        BookingStatus::Active,
        0.0
    );

    std::cout << "Dat san OFFLINE thanh cong cho khach: " << customerName
              << " | San: " << selectedPitch->getName()
              << " | Bat dau luc: " << start << "\n";
}

void AdminController::checkoutPitch(std::vector<Pitch>& pitches,
                                    std::vector<Booking>& bookings) {
    if (bookings.empty()) {
        std::cout << "Chua co booking nao.\n";
        return;
    }

    int pitchId;
    std::cout << "\n=== TINH TIEN SAN ===\n";
    std::cout << "Nhap ID san can tinh tien: ";
    std::cin >> pitchId;

    // 1. Tìm sân
    Pitch* pitchPtr = nullptr;
    for (auto& p : pitches) {
        if (p.getId() == pitchId) {
            pitchPtr = &p;
            break;
        }
    }

    if (!pitchPtr) {
        std::cout << "Khong tim thay san.\n";
        return;
    }

    // 2. Lọc các booking đang Active của sân này
    std::vector<Booking*> candidateBookings;
    for (auto& b : bookings) {
        if (b.getPitchId() == pitchId &&
            b.getStatus() == BookingStatus::Active &&
            b.getTotalAmount() == 0.0) { // chưa tính tiền
            candidateBookings.push_back(&b);
        }
    }

    if (candidateBookings.empty()) {
        std::cout << "Khong co booking nao dang hoat dong cho san nay.\n";
        return;
    }

    // 3. In danh sách các booking để Admin chọn
    std::cout << "\nCac booking dang hoat dong cho san ID " << pitchId << ":\n";
    for (auto* b : candidateBookings) {
        std::cout << "- Booking ID: " << b->getId();

        if (!b->getTimeSlot().empty()) {
            // ONLINE
            std::cout << " | Kieu: ONLINE"
                      << " | TimeSlot: " << b->getTimeSlot();
        } else {
            // OFFLINE
            std::cout << " | Kieu: OFFLINE"
                      << " | Bat dau: " << b->getStartTime();
        }

        std::cout << " | Khach: " << b->getCustomerUsername()
                  << " | Tong tien hien tai: " << b->getTotalAmount()
                  << "\n";
    }

    // 4. Cho admin chọn Booking ID cần tính tiền
    int bookingId;
    std::cout << "\nNhap Booking ID can tinh tien: ";
    std::cin >> bookingId;

    Booking* target = nullptr;
    for (auto* b : candidateBookings) {
        if (b->getId() == bookingId) {
            target = b;
            break;
        }
    }

    if (!target) {
        std::cout << "Booking ID khong hop le.\n";
        return;
    }

    double hours = 0.0;
    double amount = 0.0;

    // 5. Phân biệt Online / Offline để tính giờ
    if (!target->getTimeSlot().empty()) {
        // 👉 Khách ONLINE: dùng timeSlot để tính
        hours = hoursFromTimeSlot(target->getTimeSlot());
        if (hours <= 0) {
            std::cout << "Khong the tinh duoc thoi gian tu timeSlot.\n";
            return;
        }
        amount = hours * pitchPtr->getPrice();
    } else {
        // 👉 Khách OFFLINE: dùng startTime -> now
        std::string start = target->getStartTime();
        std::string end   = getCurrentTimeString();

        if (start.empty()) {
            std::cout << "Booking offline khong co startTime hop le.\n";
            return;
        }

        hours = diffHours(start, end);
        if (hours <= 0) {
            std::cout << "Thoi gian choi <= 0, kiem tra lai.\n";
            return;
        }

        amount = hours * pitchPtr->getPrice();
        target->setEndTime(end);
    }

    // 6. Cập nhật trạng thái booking + sân
    target->setTotalAmount(amount);
    target->setStatus(BookingStatus::Finished);
    pitchPtr->setBooked(false); // nếu bạn đang dùng cờ này

    std::cout << "\n=== HOA DON DON GIAN ===\n";
    std::cout << "San: " << pitchPtr->getName() << " (ID: " << pitchId << ")\n";
    std::cout << "Booking ID: " << target->getId() << "\n";
    std::cout << "Khach: " << target->getCustomerUsername() << "\n";
    std::cout << "So gio tinh tien: " << hours << "\n";
    std::cout << "Don gia/1 gio: " << pitchPtr->getPrice() << "\n";
    std::cout << "Tong tien: " << amount << "\n";
}

/////////////////////////////////////////


void AdminController::viewUnpaidBookings(const std::vector<Booking>& bookings,
                                         const std::vector<Pitch>& pitches) const {
    std::cout << "\n=== DANH SACH LICH DAT CHUA TINH TIEN ===\n";

    bool found = false;

    for (const auto& b : bookings) {
        // 📌 Điều kiện "chưa tính tiền":
        // - status vẫn còn Active
        // - totalAmount == 0
        if (b.getStatus() == BookingStatus::Active &&
            b.getTotalAmount() == 0.0) {

            // Tìm sân tương ứng
            const Pitch* pitchPtr = nullptr;
            for (const auto& p : pitches) {
                if (p.getId() == b.getPitchId()) {
                    pitchPtr = &p;
                    break;
                }
            }

            std::cout << "Booking ID: " << b.getId();

            if (pitchPtr) {
                std::cout << " | San: " << pitchPtr->getName()
                          << " (ID: " << pitchPtr->getId()
                          << ", " << pitchPtr->getSize() << " nguoi"
                          << ", Gia/gio: " << pitchPtr->getPrice() << ")";
            } else {
                std::cout << " | San ID: " << b.getPitchId();
            }

            // Phân biệt online/offline để in thông tin thời gian
            if (!b.getTimeSlot().empty()) {
                std::cout << " | Kieu: ONLINE"
                          << " | TimeSlot: " << b.getTimeSlot();
            } else {
                std::cout << " | Kieu: OFFLINE"
                          << " | Bat dau: " << b.getStartTime();
            }

            std::cout << " | Khach: " << b.getCustomerUsername()
                      << " | Tong tien: " << b.getTotalAmount()
                      << "\n";

            found = true;
        }
    }

    if (!found) {
        std::cout << "Hien khong co lich dat nao chua tinh tien.\n";
    }
}

void AdminController::createPitch(std::vector<Pitch>& pitches, int id, const std::string& name, double price, int size) {
    pitches.emplace_back(id, name, price, size);
}

bool AdminController::updatePitch(std::vector<Pitch>& pitches, int id, const std::string& newName, double newPrice, int newSize) {
    for (auto& p : pitches) {
        if (p.getId() == id) {
            if (!newName.empty()) p.setName(newName);
            if (newPrice > 0)     p.setPrice(newPrice);
            if (newSize > 0)      p.setSize(newSize);
            return true;
        }
    }
    return false;
}

bool AdminController::deletePitchById(std::vector<Pitch>& pitches, int id) {
    for (auto it = pitches.begin(); it != pitches.end(); ++it) {
        if (it->getId() == id) {
            pitches.erase(it);
            return true;
        }
    }
    return false;
}

bool AdminController::bookPitchOffline(const std::vector<Pitch>& pitches,std::vector<Booking>& bookings,int pitchId,const std::string& customerNameInput,std::string& outMsg)
{
    if (pitches.empty()) {
        outMsg = "Chua co san nao trong he thong.";
        return false;
    }

    const Pitch* selectedPitch = nullptr;
    for (const auto& p : pitches) {
        if (p.getId() == pitchId) {
            selectedPitch = &p;
            break;
        }
    }

    if (!selectedPitch) {
        outMsg = "Khong tim thay san co ID = " + std::to_string(pitchId);
        return false;
    }

    std::string customerName = customerNameInput;
    if (customerName.empty()) customerName = "OFFLINE";

    std::string start = getCurrentTimeString();

    bookings.emplace_back(
        pitchId,
        customerName,
        "",
        start,
        "",
        BookingStatus::Active,
        0.0
    );

    outMsg = "Dat san OFFLINE thanh cong! Khach: " + customerName +
             " | San: " + selectedPitch->getName() +
             " | Bat dau luc: " + start;
    return true;
}

