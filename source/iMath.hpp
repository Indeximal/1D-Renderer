#pragma once

namespace ima {
    template <typename T>
    T clamp(T v, T lo, T hi) {
        return v > hi ? hi : (v < lo ? lo : v);
    }

}