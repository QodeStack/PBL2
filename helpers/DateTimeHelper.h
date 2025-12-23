#ifndef DATE_TIME_HELPER_H
#define DATE_TIME_HELPER_H
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace DateTimeHelper
{

    // Hàm lấy thời gian hiện tại dạng "YYYY-MM-DD HH:MM"
    inline std::string getCurrentTimeString()
    {
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

    // Hàm parse "YYYY-MM-DD HH:MM" thành tm
    inline bool parseDateTime(const std::string &s, std::tm &outTm)
    {
        std::istringstream iss(s);
        iss >> std::get_time(&outTm, "%Y-%m-%d %H:%M");
        return !iss.fail();
    }

    // Tính số giờ giữa 2 chuỗi datetime
    inline double diffHours(const std::string &start, const std::string &end)
    {
        std::tm tmStart{}, tmEnd{};
        if (!parseDateTime(start, tmStart) || !parseDateTime(end, tmEnd))
            return 0.0;

        std::time_t tStart = std::mktime(&tmStart);
        std::time_t tEnd = std::mktime(&tmEnd);
        if (tEnd <= tStart)
            return 0.0;

        double seconds = std::difftime(tEnd, tStart);
        return seconds / 3600.0;
    }

    // Tính số giờ từ timeSlot online "2025-12-01 18:00-20:00"
    inline double hoursFromTimeSlot(const std::string &timeSlot)
    {
        std::string datePart, timeRange;
        std::istringstream iss(timeSlot);
        if (!(iss >> datePart >> timeRange))
            return 0.0;

        auto pos = timeRange.find('-');
        if (pos == std::string::npos)
            return 0.0;

        std::string startTime = timeRange.substr(0, pos);
        std::string endTime = timeRange.substr(pos + 1);

        return diffHours(datePart + " " + startTime, datePart + " " + endTime);
    }

} // namespace DateTimeHelper


#endif
