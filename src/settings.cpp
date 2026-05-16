#include "../inc/settings/settings.h"
#include <cstdlib>
#include <processenv.h>
#include <stdlib.h>
#include <string>
#include <direct.h>

#define MAX_DATA_SIZE 1024

template <typename T> 
constexpr inline size_t __cdecl ToIndex(T index) noexcept 
{
    if (index > 0) {return(size_t(index));} return(size_t(0));
}

void __cdecl ClearConsole(void) 
{
    HANDLE StdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written;
    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    GetConsoleScreenBufferInfo(StdOutput, &csbi);
    FillConsoleOutputCharacter(StdOutput, ' ', csbi.dwSize.X * csbi.dwSize.Y, {0, 0}, &written);
    FillConsoleOutputAttribute(StdOutput, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, {0, 0}, &written);
    SetConsoleCursorPosition(StdOutput, {0, 0});
}

int __cdecl ShowOption(_In_ struct Option* option) 
{
    if (option) {
        std::cout << "\033[48;2;" << std::to_string(option->backgroundColor.r) + ";" + std::to_string(option->backgroundColor.g) + ";" + std::to_string(option->backgroundColor.b) + "m";
        std::cout << "\033[38;2;" << std::to_string(option->foregroundColor.r) + ";" + std::to_string(option->foregroundColor.g) + ";" + std::to_string(option->foregroundColor.b) + "m" << option->prefix << " ";
        std::cout << "\033[97m" << option->text;
        std::cout << "\033[48;2;" << std::to_string(option->backgroundColor.r) + ";" + std::to_string(option->backgroundColor.g) + ";" + std::to_string(option->backgroundColor.b) + "m";
        std::cout << "\033[38;2;" << std::to_string(option->foregroundColor.r) + ";" + std::to_string(option->foregroundColor.g) + ";" + std::to_string(option->foregroundColor.b) + "m " << option->prefix << std::endl;
        std::cout << "\033[0m";
    } else {
        return(-1);
    }
    return(0);
}

bool SpecialKey = false;
int __cdecl GetKeyPressed(_In_ int* Pressed) 
{
    *Pressed = 0;
    *Pressed = _getch();
    if (*Pressed == 224) {
        SpecialKey = true;
    }

    if (*Pressed == 72 && SpecialKey) {
        *Pressed = VK_UP;
        SpecialKey = false;
    } else if (*Pressed == 80 && SpecialKey) {
        *Pressed = VK_DOWN;
        SpecialKey = false;
    } else if (*Pressed == 13) {
        *Pressed = VK_RETURN;
    }
    return(int(*Pressed));
}

int __cdecl JsonFileFinder::FindJsonFiles(_In_ JsonFileReader* fileReader) {
    struct _finddata_t jsonFilesFinder;
    intptr_t handle;
    std::vector<struct Theme> themes;
    std::vector<Option> options;
    int currentTheme = 0;

    handle = _findfirst("themes\\*.json", &jsonFilesFinder);
    if (handle == -1L) return(1); 

    struct JsonValue JsonConfigs[] = {
        {VT_Boolean, {.booleanValue = true}, nullptr, &fileReader->showMenu, "spectrum.tui.showMenu", "root"},
        {VT_Boolean, {.booleanValue = true}, nullptr, &fileReader->noBgColor, "spectrum.tui.noBackgroundColor", "root"}
    };

    do {
        struct Theme oneTheme{};
        FilePtr jsonFile;
        
        char fullThemesPath[MAX_PATH];
        sprintf_s(fullThemesPath, MAX_PATH, "themes\\%s", jsonFilesFinder.name);

        fopen_s(&jsonFile, fullThemesPath, "r");
        if (!jsonFile) {
            continue;
        } 

        struct JsonValue JsonThemeOptions[] = {
            {VT_Double, {.doubleValue = 1.0}, nullptr, &oneTheme.colorRed, "spectrum.tui.colorR", "theme.properties"},
            {VT_Double, {.doubleValue = 1.0}, nullptr, &oneTheme.colorGreen, "spectrum.tui.colorG", "theme.properties"},
            {VT_Double, {.doubleValue = 1.0}, nullptr, &oneTheme.colorBlue, "spectrum.tui.colorB", "theme.properties"},
            {VT_Boolean, {.booleanValue = false}, nullptr, &oneTheme.useRandomCharacters, "spectrum.tui.useRandomCharacters", "theme.properties"},
            {VT_Character, {.characterValue = '='}, nullptr, &oneTheme.customCharacter, "spectrum.tui.customCharacter", "theme.properties"},
            {VT_String, {.stringValue = "Default Mode"}, nullptr, &oneTheme.themeMode, "spectrum.tui.visualizerMode", "theme.properties"},
            {VT_Int, {.intValue = 0}, nullptr, &oneTheme.key, "spectrum.tui.key", "theme.properties"},
            {VT_String, {.stringValue = "**Untitled-Theme**"}, nullptr, &oneTheme.themeName, "theme.name", "root"},
            {VT_String, {.stringValue = "**No-ID**"}, nullptr, &oneTheme.themeId, "theme.id", "root"}
        };

        int pSuccess = fileReader->ReadSettings(jsonFile, JsonThemeOptions, sizeof(JsonThemeOptions) / sizeof(struct JsonValue));
        if (pSuccess != 0) {
            if (pSuccess != -19) {
                if (jsonFile) fclose(jsonFile);
            }
            _findclose(handle);
            return(-5);
        }

        themes.push_back(oneTheme);
        if (jsonFile) fclose(jsonFile);
    } while (_findnext(handle, &jsonFilesFinder) == 0);

    size_t index = 0;
    while (index < themes.size()) {
        struct Theme oneTheme = themes.at(index);
        fprintf(stdout, "%d\n", oneTheme.key);
        struct Option oneOption;
        oneOption.text = std::string(oneTheme.themeName) +  + " (" + oneTheme.themeId + ")";
        oneOption.prefix = "*";
        oneOption.index = index;
        oneOption.backgroundColor = {0, 0, 25};
        oneOption.foregroundColor = {0, 0, 255};
        options.push_back(oneOption);
        index++;
    }

    FilePtr configs;
    fopen_s(&configs, "settings.json", "r");
    if (!configs) {
        _findclose(handle);
        return(-1);
    } else {
        int pSuccess = fileReader->ReadSettings(configs, JsonConfigs, sizeof(JsonConfigs) / sizeof(struct JsonValue));
        if (pSuccess != 0) {
            if (pSuccess != -19) {
                if (configs) fclose(configs);
            }
            _findclose(handle);
            return(-5);
        }
        fclose(configs);
    }
    _findclose(handle);

    int _Char = 0;
    std::string logo;
    logo = AsciiRgb(48, 0, 25, 23) AsciiRgb(38, 0, 255, 236) " * ╭─╮╭─╮╭─╴╭─╴╶┬╴╭─╮╷ ╷╭┬╮ * Authors <\033[97mMajockbim \"https://majockbim.com/\", Joe.r Dev" AsciiRgb(38, 0, 255, 236) "> \n"
                    AsciiRgb(48, 0, 18, 25) AsciiRgb(38, 0, 180, 255)  " = ╰─╮├─╯├╴ │   │ ├┬╯│ ││││ = [\033[97mSPECTRUM" AsciiRgb(38, 0, 180, 255) "] Terminal Equalizer - Version 1.2.036d9c33 \n"
                    AsciiRgb(48, 0, 7, 25) AsciiRgb(38, 0, 73, 255) " * ╰─╯╵  ╰─╴╰─╴ ╵ ╵╰╴╰─╯╵ ╵ * Releases: https://github.com/majockbim/spectrum/releases/\n";
    logo += AsciiRgb(48, 0, 0, 25) AsciiRgb(38, 0, 0, 255) " * This program was originally created by MajockBim and edited by Joe.r Dev. It is licensed under the MIT License. *\n";
    while (true) {
        GetKeyPressed(&_Char);
        if (_Char == VK_UP) {
            if (currentTheme == 0) {
                currentTheme = options.size() - 1;
            } else {
                currentTheme--;
            }
        } else if (_Char == VK_DOWN) {
            if (currentTheme == (int)options.size() - 1) {
                currentTheme = 0;
            } else {
                currentTheme++;
            }
        } else if (_Char == VK_RETURN) {
            fileReader->currentTheme = themes.at(currentTheme);
            break;
        } 
        ClearConsole(); 
        std::cout << logo;
        std::cout << "Theme Selected: ";
        ShowOption(&options.at(currentTheme)); 
    }
    fileReader->themes = themes;
    return(0);
}
int __cdecl JsonFileReader::ReadSettings(_In_ FilePtr jsonFile, _In_ struct JsonValue* jsonThemeOptions, size_t _Size) {
    if (!jsonThemeOptions) return(-1);
    if (!jsonFile) return(-19);

    char data[MAX_DATA_SIZE];
    std::string text;
    while (fgets(data, MAX_DATA_SIZE, jsonFile)) {
        if (strlen(data) > 0 && data[ToIndex<int>(strlen(data) - 1)] == '\n') {
            data[ToIndex<int>(strlen(data) - 1)] = ' ';
        }
        text += data;
    }
    yyjson_doc *doc = yyjson_read(text.c_str(), text.length(), 0);
    if (!doc) {
        return(-1);
    }
    yyjson_val *root = yyjson_doc_get_root(doc);
    if (!root) {
        yyjson_doc_free(doc);
        return(-2);
    }

    size_t index = 0;
    while (index < _Size) {
        yyjson_val* currentRoot;
        if (strcmp(jsonThemeOptions[index].root, "root") != 0) {
            currentRoot = yyjson_obj_get(root, jsonThemeOptions[index].root);
        } else {
            currentRoot = root;
        }
        if (!currentRoot) {
            yyjson_doc_free(doc);
            return(-6);
        }
        jsonThemeOptions[index].pointer = yyjson_obj_get(currentRoot, jsonThemeOptions[index].name);
        if (jsonThemeOptions[index].pointer == nullptr) {
            yyjson_doc_free(doc);
            return(-3);
        }
        if (jsonThemeOptions[index].pointer) {
            switch (jsonThemeOptions[index].valueType) 
            {
                case VT_Double: {
                    CopyValue<double>(jsonThemeOptions[index].defaultValue.doubleValue, yyjson_get_num(jsonThemeOptions[index].pointer), jsonThemeOptions[index].pointer1);
                } break;

                case VT_Boolean: {
                    CopyValue<bool>(jsonThemeOptions[index].defaultValue.booleanValue, yyjson_get_bool(jsonThemeOptions[index].pointer), jsonThemeOptions[index].pointer1);
                } break;

                case VT_Int: {
                    CopyValue<int>(jsonThemeOptions[index].defaultValue.intValue, yyjson_get_int(jsonThemeOptions[index].pointer), jsonThemeOptions[index].pointer1);
                } break;

                case VT_Character: {
                    const char* tmpString = yyjson_get_str(jsonThemeOptions[index].pointer);
                    if (tmpString) {
                        CopyValue<char>(jsonThemeOptions[index].defaultValue.characterValue, tmpString[0], jsonThemeOptions[index].pointer1);
                    }
                } break;

                case VT_String: {
                    const char* tmpString = yyjson_get_str(jsonThemeOptions[index].pointer);
                    char* _Ptr = reinterpret_cast<char*>(jsonThemeOptions[index].pointer1);
                    if (_Ptr) {
                        if (tmpString) {
                            strcpy_s(_Ptr, MAX_THEME_NAME, tmpString);
                        } else if (jsonThemeOptions[index].defaultValue.stringValue) {
                            strcpy_s(_Ptr, MAX_THEME_NAME, jsonThemeOptions[index].defaultValue.stringValue);
                        }
                    }
                } break;
            }
        }
        index++;
    }

    yyjson_doc_free(doc);
    return(0);
}
