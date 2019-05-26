#ifndef CRACKME_KEYGEN_HEXSTRING_H
#define CRACKME_KEYGEN_HEXSTRING_H

#define ERROR 0xFFFFFFFF

uint32_t hexstring2var(const std::string &);
std::string var2hexstring(uint32_t);
uint32_t string2var(const std::string &);
void string2mem(const std::string &, std::vector<uint32_t> &);
void hexstring2mem(const std::string &, std::vector<uint32_t> &);

#endif //CRACKME_KEYGEN_HEXSTRING_H
