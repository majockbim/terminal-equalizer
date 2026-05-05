#include "../inc/settings/settings.h"
#include <rpcndr.h>

#define MAX_DATA_SIZE 1024

int __cdecl Dev::JoerAndMj::SettingsJsonFileFinder::FindJsonFiles(_In_ Dev::JoerAndMj::SettingsJsonFileReader* fileReader) {
    struct _finddata_t jsonFilesFinder;
    intptr_t handle;

    struct __JsonValue<bool> booleans[] = {
        {false, "spectrum.tui.useRandomCharacters"},
        {true, "spectrum.tui.noColoredBackground"},
        {false, "spectrum.tui.showMenu"}
    };

    struct __JsonValue<double> doubles[] = {
        {1.0, "spectrum.tui.colorR"},
        {1.0, "spectrum.tui.colorG"},
        {1.0, "spectrum.tui.colorB"}
    };

    struct __JsonValue<char> chars[] = {
        {'=', "spectrum.tui.customCharacter"}
    };
    
    struct JsonValue settings;
    settings.booleans = booleans;
    settings.booleanArraySize = sizeof(booleans) / sizeof(struct __JsonValue<bool>);
    settings.doubles = doubles;
    settings.doublesArraySize = sizeof(doubles) / sizeof(struct __JsonValue<double>);
    settings.chars = chars;
    settings.charArraySize = sizeof(chars) / sizeof(struct __JsonValue<char>);


    handle = _findfirst("*.json", &jsonFilesFinder);
    if (handle == -1) return(1); 

    bool readSuccess = true;
    do {
        FilePtr jsonFile;
        fopen_s(&jsonFile, jsonFilesFinder.name, "r");
        if (!jsonFile) {
            readSuccess = false;
            break;
        }

        fileReader->ReadSettings(&jsonFile, &settings);
        if (jsonFile) fclose(jsonFile);
    } while (_findnext(handle, &jsonFilesFinder) == 0);

    _findclose(handle);

    if (!readSuccess) return(1);
    return(0);
}
int __cdecl Dev::JoerAndMj::SettingsJsonFileReader::ReadSettings(_In_ FilePtr* jsonFile, _In_ JsonValue* jsonVal) {
    if (!jsonVal) return(-1);

    char data[MAX_DATA_SIZE]; 
    if (fgets(data, sizeof(data), *jsonFile) != NULL) {
        yyjson_doc *doc = yyjson_read(data, strlen(data), 0);
        if (!doc) return(-1);

        yyjson_val *root = yyjson_doc_get_root(doc);

        size_t index = 0;
        while (index < jsonVal->booleanArraySize) {
            if (jsonVal->booleans) {
                yyjson_val* value = yyjson_obj_get(root, jsonVal->booleans[index].name);
                if (value) {
                    jsonVal->booleans[index].value = yyjson_get_bool(value);
                    if (strcmp(jsonVal->booleans[index].name, "spectrum.tui.useRandomCharacters") == 0) {
                        useRandomCharacters = jsonVal->booleans[index].value;
                    } else if (strcmp(jsonVal->booleans[index].name, "spectrum.tui.noColoredBackground") == 0) {
                        noBgColor = jsonVal->booleans[index].value;
                    } else if (strcmp(jsonVal->booleans[index].name, "spectrum.tui.showMenu") == 0) {
                        showMenu = jsonVal->booleans[index].value;
                    }
                }
            }
            index++;
        }

        index = 0;
        while (index < jsonVal->doublesArraySize) {
            if (jsonVal->doubles) {
                yyjson_val* value = yyjson_obj_get(root, jsonVal->doubles[index].name);
                if (value) {
                    jsonVal->doubles[index].value = yyjson_get_num(value);
                    if (strcmp(jsonVal->doubles[index].name, "spectrum.tui.colorR") == 0) {
                        colorRed = jsonVal->doubles[index].value;
                    } else if (strcmp(jsonVal->doubles[index].name, "spectrum.tui.colorB") == 0) {
                        colorBlue = jsonVal->doubles[index].value;
                    } else if (strcmp(jsonVal->doubles[index].name, "spectrum.tui.colorG") == 0) {
                        colorGreen = jsonVal->doubles[index].value;
                    }
                }
            }
            index++;
        }

        index = 0;
        while (index < jsonVal->charArraySize) {
            if (jsonVal->chars) {
                yyjson_val* value = yyjson_obj_get(root, jsonVal->chars[index].name);
                if (value) {
                    const char* string = yyjson_get_str(value);
                    if (string) {
                        jsonVal->chars[index].value = string[0];
                        if (strcmp(jsonVal->chars[index].name, "spectrum.tui.customCharacter") == 0) {
                            customCharacter = jsonVal->chars[index].value;
                        }
                    }
                }
            }
            index++;
        }

        yyjson_doc_free(doc);
    }
    return(0);
}
