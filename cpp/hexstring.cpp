#include <iostream>
#include <vector>
#include "hexstring.h"

uint32_t hexstring2var(const std::string &s) {
    uint32_t rt = 0x00000000;
    size_t count = 0;
    for (auto c : s) {
        uint32_t tmp;
        if (isdigit(c)) {
            tmp = c - 0x00000030;

        }
        else if (isupper(c) && c < 'G') {
            tmp = c - 0x00000037;
        }
        else {
            return ERROR;
        }
        tmp <<= (s.size() - count) * 4 - 4;
        rt |= tmp;
        count++;
    }
    return rt;
}

std::string var2hexstring(const uint32_t var) {
    std::string str;
    size_t count = 8;
    while (count) {
        uint32_t tmp = var >> (count * 4 - 4);
        tmp &= 0x0000000FU;
        if (tmp < 10) {
            str.append(1, tmp + 0x30);
        }
        else {
            str.append(1, tmp + 0x37);
        }
        count--;
    }
    return str;
}

uint32_t string2var(const std::string &s) {
    if (s.size() > 4) {
        return ERROR;
    }
    uint32_t rt = 0x00000000;
    size_t count = 0;
    for (uint32_t c : s) {
        uint32_t tmp = c;
        tmp <<= 8 * (s.size() - count - 1);
        rt |= tmp;
        count++;
    }
    rt &= (0xFFFFFFFF >> ((4 - s.size()) * 8));
    return rt;
}

void string2mem(const std::string &s, std::vector<uint32_t> &mem) {
    size_t sub_start = 0;
    while (sub_start / 4 < mem.size() && sub_start < s.size()) {
        std::string sub = s.substr(sub_start, 4);
        std::reverse(sub.begin(), sub.end());
        mem[sub_start / 4] = string2var(sub);
        sub_start += 4;
    }
}

void hexstring2mem(const std::string &s, std::vector<uint32_t> &mem) {
    size_t sub_start = 0;
    while (sub_start / 8 < mem.size()) {
        std::string sub = s.substr(sub_start, 8);
        std::string tmp;
        tmp.append(sub.substr(6, 2));
        tmp.append(sub.substr(4, 2));
        tmp.append(sub.substr(2, 2));
        tmp.append(sub.substr(0, 2));
        mem[sub_start / 8] = hexstring2var(tmp);
        sub_start += 8;
    }
}
