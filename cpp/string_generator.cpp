#include <iostream>
#include <vector>
#include "string_generator.h"
#include "hexstring.h"

uint32_t round1(uint32_t &mem, const uint32_t var1, const uint32_t var2,
                const uint32_t init_s, const uint8_t init_n, const uint32_t var3, const uint32_t var4) {
    uint32_t ret;
    mem += ((var2 & var1) | (~var1 & var4)) + var3 + init_s;
    mem = (mem << init_n) | (mem >> (0x20U - init_n));
    ret = mem;
    mem += var1;
    return ret;
}

uint32_t round2(uint32_t &mem, const uint32_t var1, const uint32_t var2,
                const uint32_t init_s, const uint8_t init_n, const uint32_t var3, const uint32_t var4) {
    uint32_t ret;
    mem += init_s + var3 + ((var2 & ~var4) | (var4 & var1));
    mem = (mem << init_n) | (mem >> (0x20U - init_n));
    ret = mem;
    mem += var1;
    return ret;
}

uint32_t round3(uint32_t &mem, const uint32_t var1, const uint32_t var2,
                const uint32_t init_s, const uint8_t init_n, const uint32_t var3, const uint32_t var4) {
    uint32_t ret;
    mem += init_s + var3 + (var4 ^ var2 ^ var1);
    mem = (mem << init_n) | (mem >> (0x20U - init_n));
    ret = mem;
    mem += var1;
    return ret;
}

uint32_t round4(uint32_t &mem, const uint32_t var1, const uint32_t var2,
                const uint32_t init_s, const uint8_t init_n, const uint32_t var3, const uint32_t var4) {
    uint32_t ret;
    mem += init_s + var3 + (var2 ^ (var1 | ~var4));
    mem = (mem << init_n) | (mem >> (0x20U - init_n));
    ret = mem;
    mem += var1;
    return ret;
}

std::string genstr(std::string &s) {
    std::string str(INITSTR);
    uint32_t bits = s.size() * 8;
    s.append(1, 0x80);
    std::vector<uint32_t> str_mem(4, 0x00000000);
    std::vector<uint32_t> s_mem(16, 0x00000000);
    s_mem[14] = bits;
    hexstring2mem(str, str_mem);
    std::vector<uint32_t> str_mem_p = str_mem;
    string2mem(s, s_mem);
    for (size_t i = 0; i < 16; i++) {
        round1(str_mem[(16 - i) % 4], str_mem[(17 - i) % 4], str_mem[(18 - i) % 4], salt[i],
               (i % 4 == 0) ? 0x7 : (i % 4 == 1) ? 0xC : (i % 4 == 2) ? 0x11 : 0x16,
               s_mem[i], str_mem[(19 - i) % 4]);
    }
    for (size_t i = 0; i < 16; i++) {
        round2(str_mem[(16 - i) % 4], str_mem[(17 - i) % 4], str_mem[(18 - i) % 4], salt[i + 16],
               (i % 4 == 0) ? 0x5 : (i % 4 == 1) ? 0x9 : (i % 4 == 2) ? 0xE : 0x14,
               s_mem[(1 + i * 5) % 16], str_mem[(19 - i) % 4]);
    }
    for (size_t i = 0; i < 16; i++) {
        round3(str_mem[(16 - i) % 4], str_mem[(17 - i) % 4], str_mem[(18 - i) % 4], salt[i + 32],
               (i % 4 == 0) ? 0x4 : (i % 4 == 1) ? 0xB : (i % 4 == 2) ? 0x10 : 0x17,
               s_mem[(5 + i * 3) % 16], str_mem[(19 - i) % 4]);
    }
    for (size_t i = 0; i < 16; i++) {
        round4(str_mem[(16 - i) % 4], str_mem[(17 - i) % 4], str_mem[(18 - i) % 4], salt[i + 48],
               (i % 4 == 0) ? 0x6 : (i % 4 == 1) ? 0xA : (i % 4 == 2) ? 0xF : 0x15,
               s_mem[(i * 7) % 16], str_mem[(19 - i) % 4]);
    }
    for (size_t i = 0; i < 4; i++) {
        str_mem_p[i] += str_mem[i];
    }
    str.clear();
    for (auto value : str_mem_p) {
        std::string tmp = var2hexstring(value);
        str.append(tmp.substr(6, 2));
        str.append(tmp.substr(4, 2));
        str.append(tmp.substr(2, 2));
        str.append(tmp.substr(0, 2));
    }
    return str;
}
