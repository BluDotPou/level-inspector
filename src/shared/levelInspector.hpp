#pragma once

#include <Geode/Geode.hpp>
#include <chrono>
#include <string>

namespace levelinspector {
    inline std::chrono::steady_clock::time_point g_levelStart{};
    inline bool g_levelStartSet = false;

    inline void markLevelStart() {
        g_levelStart = std::chrono::steady_clock::now();
        g_levelStartSet = true;
    }

    inline int elapsedSeconds() {
        if (!g_levelStartSet) return 0;
        return (int)std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - g_levelStart
        ).count();
    }

    inline std::string formatTime(int totalSeconds) {
        int minutes = totalSeconds / 60;
        int seconds = totalSeconds % 60;

        std::string out = std::to_string(minutes) + ":";
        if (seconds < 10) out += "0";
        out += std::to_string(seconds);
        return out;
    }

    inline std::string toStdString(gd::string const& s) {
        return std::string(s.c_str());
    }
}