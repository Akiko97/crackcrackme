#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "string_generator.h"
#include "hexstring.h"

void usage(char *);

void usage(char *file) {
    std::cout << "Usage: " << file << " [chk|gen] name [key(only needed during chk)]" << std::endl;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        usage(argv[0]);
        exit(0);
    }
    const std::string right("4E26E075");
    std::string func(argv[1]);
    if (func == "chk" && argc == 4) {
        std::string name(argv[2]);
        std::string key(argv[3]);
        std::string key07 = genstr(name).substr(0, 8);
        if (key07 != key.substr(0, 8)) {
            std::cout << "WRONG KEY!" << std::endl;
            exit(0);
        }
        std::string subkey1 = key.substr(0, 16);
        std::string key_tail = genstr(subkey1).substr(3, 4);
        if (key_tail != key.substr(16, 4)) {
            std::cout << "WRONG KEY!" << std::endl;
            exit(0);
        }
        std::string subkey2 = var2hexstring(hexstring2var(key.substr(0, 8)) ^ hexstring2var(key.substr(8, 8)));
        std::string key8F = genstr(subkey2).substr(4, 8);
        if (key8F != right) {
            std::cout << "WRONG KEY!" << std::endl;
            exit(0);
        }
        std::cout << "RIGHT KEY!" << std::endl;
    }
    else if (func == "gen" && argc == 3) {
        std::string name(argv[2]);
        std::string key07(genstr(name).substr(0, 8));
        uint32_t num = (0x00000069U ^ 0xC30F5EE8U);
        std::string key8F = var2hexstring(num ^ hexstring2var(key07));
        std::string subkey(key07 + key8F);
        std::string key_tail(genstr(subkey).substr(3, 4));
        std::string key(key07 + key8F + key_tail);
        std::cout << "Key: " << key << std::endl;
    }
    else {
        usage(argv[0]);
    }
    return 0;
}
