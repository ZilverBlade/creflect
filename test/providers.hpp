#pragma once
#include <rapidcheck/gtest.h>

constexpr int MAX_PROVIDED_STR_LENGTH = 50;

rc::Gen<char> ArbitraryAlphaChar() {
    std::vector<rc::Gen<char>> validChar;
    validChar.push_back(rc::gen::inRange('a', char('z' + 1)));
    validChar.push_back(rc::gen::inRange('A', char('Z' + 1)));

    return rc::gen::join(rc::gen::elementOf(validChar));
}

char ProvideAlphaCharOnly() {
    return *ArbitraryAlphaChar();
}

rc::Gen<char> ArbitraryNumericChar() {
    return rc::gen::inRange('0', char('9' + 1));
}
char ProvideNumericCharOnly() {
    return *ArbitraryNumericChar();
}

char ProvideInvalidCharOnly() {
    return '\t';
}

std::string ProvideAlphabeticString() {
    return *rc::gen::map(rc::gen::inRange(1, MAX_PROVIDED_STR_LENGTH), [](int length) -> std::string {
        std::string str(length, 'x');
        for (int i = 0; i < length; ++i) {
            str[i] = ProvideAlphaCharOnly();
        }
        return str;
        });
}

std::string ProvideValidAlphanumbericString() {
    return *rc::gen::map(rc::gen::inRange(1, MAX_PROVIDED_STR_LENGTH), [](int length) -> std::string {
        std::string str(length, 'x');
        for (int i = 0; i < length; ++i) {
            str[i] = (i >= 1 && (rand() % 2 == 1)) ? ProvideNumericCharOnly() : ProvideAlphaCharOnly();
        }
        return str;
        });
}

std::string ProvideValidMemberNameNoNumbers() {
    return *rc::gen::map(rc::gen::inRange(1, MAX_PROVIDED_STR_LENGTH), [](int length) -> std::string {
        std::string str(length, 'x');
        for (int i = 0; i < length; ++i) {
            str[i] = (i >= 1 && (rand() % 2 == 1)) ? '_' : ProvideAlphaCharOnly();
        }
        return str;
    });
}

std::string ProvideValidMemberNameWithNumbers() {
    return *rc::gen::map(rc::gen::inRange(1, MAX_PROVIDED_STR_LENGTH), [](int length) -> std::string {
        std::string str(length, 'x');
        for (int i = 0; i < length; ++i) {
            int s = rand() % 3;
            if (i >= 1 && s == 1) {
                str[i] = ProvideNumericCharOnly();
            }
            else if (s == 2) {
                str[i] = '_';
            }
            else {
                str[i] = ProvideAlphaCharOnly();
            }
        }
        return str;
    });
}
std::string ProvideInvalidMemberName() {
    return *rc::gen::map(rc::gen::inRange(1, MAX_PROVIDED_STR_LENGTH), [](int length) -> std::string {
        std::string str(length, 'x');
        for (int i = 0; i < length; ++i) {
            str[i] = ProvideInvalidCharOnly();
        }
        return str;
        });
}
std::string ProvideValidInvalidNameMix() {
    return *rc::gen::map(rc::gen::inRange(1, MAX_PROVIDED_STR_LENGTH), [](int length) -> std::string {
        std::string str(length, 'x');
        for (int i = 0; i < length; ++i) {
            if (i % 2) {
                str[i] = ProvideInvalidCharOnly();
            } else {
                str[i] = ProvideAlphaCharOnly();
            }
        }
        return str;
        });
}