/*
    spectrum - A real-time command line audio visualizer
    Copyright (C) 2026 Joe R. (@johnmilson125-png)
    Copyright (C) 2026 Majock Bim (@majockbim)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#if defined(_MSC_VER) && defined(__clang__)
#pragma once
#endif

#ifndef __Settings_H__
#define __Settings_H__

#include <stdio.h>
#include <vector>
#include <windows.h>
#include <io.h>
#include <string>
#include <conio.h>
#include <iostream>
#include "../third_party/inc/yyjson.h"

#define MAX_THEME_NAME 64
#define AsciiRgb(k, r, g, b) "\033[" #k ";2;" #r ";" #g ";" #b "m"

using FilePtr = FILE*;

enum ThemeMode {
    Mode0 = 0,
    Mode1 = 1,
    Mode2 = 2,
    Mode3 = 3,
    Mode4 = 4
};

enum ValueType {
    VT_Double = 1,
    VT_Character = 2,
    VT_Boolean = 3,
    VT_String = 4,
    VT_Int = 5
};

struct JsonValue {
    ValueType valueType;
    union {
        bool booleanValue;
        double doubleValue;
        char characterValue;
        int intValue;
        const char* stringValue;
    } defaultValue;
    yyjson_val* pointer;
    void* pointer1;
    const char* name;
    const char* root;
};

struct ThemeModeManager {
    const char* themeModeName;
    const ThemeMode themeMode;
};

struct Theme {
    char customCharacter = '*';
    double colorRed = 1.0;
    double colorGreen = 1.0;
    double colorBlue = 1.0;
    bool useRandomCharacters = false;
    char themeName[MAX_THEME_NAME];
    char themeId[MAX_THEME_NAME];
    char themeMode[MAX_THEME_NAME];
    int key;
};

struct ColorRGB {
    unsigned char r, g, b;
};

struct Option {
    int index;
    std::string text;
    std::string prefix;
    ColorRGB foregroundColor, backgroundColor;
};

int __cdecl ShowOption(_In_ struct Option* option);
void __cdecl ClearConsole(void);

class JsonFileReader final
{
private:
    template <typename T>
    void __cdecl CopyValue(T& _Left, T _Right, void* _Ptr) {
        _Left = _Right;
        if (_Ptr) {
            T* temporary = reinterpret_cast<T*>(_Ptr);
            *temporary = _Left;
        }
    }

public:
    struct Theme currentTheme;
    std::vector<struct Theme> themes;
    bool noBgColor = false;
    bool showMenu = false;
    JsonFileReader(void) = default;
    ~JsonFileReader() = default;

    int __cdecl ReadSettings(_In_ FilePtr jsonFile, _In_ struct JsonValue* jsonThemeOptions, _In_ size_t _Size);
    void __cdecl WriteJsonFile(void);
};

class JsonFileFinder final
{
public:
    JsonFileFinder(void) = default;
    ~JsonFileFinder() = default;

    int __cdecl FindJsonFiles(_In_ JsonFileReader* fileReader);
};

#endif
