#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <codecvt>
#include <vector>
#include <cwctype> 
#include "Symbol.h"
#include "MethodResult.h"
#include "Text.h"
#include "SymbolExtended.h"
#include <set>
#include <xlsxwriter.h>

bool isAlnumUnicode(char32_t c) {
    return (c >= U'0' && c <= U'9') ||
        (c >= U'A' && c <= U'Z') ||
        (c >= U'a' && c <= U'z');
}

bool isAllowedCharacter(char32_t c) {
    std::u32string allowedSpecialChars = U"èæžšðÈÆŽŠÐ#()*+/\\<>=[]{}|~^`%&.,;:!?-\"'";

    return isAlnumUnicode(c) || std::isspace(static_cast<unsigned char>(c)) ||
        allowedSpecialChars.find(c) != std::u32string::npos;
}

std::string symbolToString(char32_t c) {
    if (c == U' ') return "razmak";
    if (c == U',') return "zarez";
    if (c == U'\n') return "novi_red";

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    return converter.to_bytes(c);
}

std::string symbolToString(const std::u32string& str) {
    std::string result;

    for (char32_t c : str) {
        if (c == U' ') {
            result += "razmak";
        }
        else if (c == U',') {
            result += "zarez";
        }
        else if (c == U'\n') {
            result += "novi_red";
        }
        else {
            std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
            result += converter.to_bytes(c);
        }
    }

    return result;
}

std::u32string cleanText(const std::u32string& text) {
    std::u32string cleanedText;
    for (char32_t c : text) {
        if (!std::iswprint(static_cast<wchar_t>(c)) && c != U' ' && c != U'\n') {
            continue;
        }
        if (isAllowedCharacter(c)) {
            cleanedText += c;
        }
    }
    return cleanedText;
}

std::u32string readAndCleanText(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Greska kod otvaranja fajla " << filename << std::endl;
        exit(1);
    }

    std::string utf8Text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string text;
    try {
        text = converter.from_bytes(utf8Text);
    }
    catch (const std::exception& e) {
        std::cerr << "Greska kod konvertovanja UTF-8 fajla: " << e.what() << std::endl;
        exit(1);
    }

    return cleanText(text);
}

void writeDataToCSV(const std::string& filename, const std::vector<Symbol>& symbols, const std::vector<SymbolExtended>& extendedSymbols, double entropy,
    int totalSymbols, size_t fileSize, MethodResult* mr) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Greska kod kreiranja fajla: " << filename << std::endl;
        exit(1);
    }

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    file << "simbol,frekvencija_simbola,relativna_frekvencija_simbola,ravnomjerni,shannon_fano,obicni_huffman,skraceni_huffman" << std::endl;
    for (const auto& symbol : symbols) {
        std::string symbolStr = symbolToString(symbol.character);

        file << symbolStr << "," << symbol.frequency << "," << std::fixed << std::setprecision(6) << symbol.relativeFrequency << ",";
        for (int i = 0; i < 4; i++) {
            file << symbol.code[i];
            if (i < 3) file << ",";
        }
        file << std::endl;
    }

    file << "\ngrupirani_simboli,relativna_frekvencija_grupiranih_simbola,multiplicirani_huffman" << std::endl;
    for (const auto& symbol : extendedSymbols) {
        std::string symbolStr = symbolToString(symbol.combinedCharacters);
        file << symbolStr << "," << std::fixed << std::setprecision(6) << symbol.relativeFrequency << "," << symbol.code[4] << std::endl;
    }

    file << "\nukupno_karaktera," << totalSymbols << std::endl;
    file << "ukupno_razlicitih_karaktera," << symbols.size() << std::endl;
    file << "velicina_u_bajtima," << fileSize << std::endl;
    file << "entropija_izvora_bez_memorije," << entropy << std::endl;

    file << "\nmetoda,kraftova_nejednakost,srednja_duzina_kodne_rijeci,efikasnost_kodiranja,redudancija" << std::endl;
    for (int i = 0; i < 5; i++) {
        file << mr[i].name << "," << mr[i].kraftInequality << "," << mr[i].avgLength << "," << mr[i].efficiency << "," << mr[i].redundancy << std::endl;
    }
    file.close();

    std::cout << "Rezultati spremljeni u fajl: " << filename << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    convertCSVToXLSX(filename, "izlaz.xlsx");
}

void printUniqueUnicodeCharacters(const std::string& fileName) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Greska kod otvaranja fajla " << fileName << std::endl;
        exit(1);
    }

    std::string utf8Text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string utf32Text;
    try {
        utf32Text = converter.from_bytes(utf8Text);
    }
    catch (const std::exception& e) {
        std::cerr << "Greska kod konvertovanja UTF-8 fajla: " << e.what() << std::endl;
        exit(1);
    }

    std::set<char32_t> uniqueChars(utf32Text.begin(), utf32Text.end());

    std::cout << "Razliciti karakteri u fajlu \"" << fileName << "\":\n";
    for (char32_t c : uniqueChars) {
        if (c < 128 && std::isprint(static_cast<char>(c))) {
            std::cout << static_cast<char>(c) << " ";
        }
        else {
            std::cout << "U+" << std::hex << std::uppercase << c << " ";
        }
    }
    std::cout << std::endl;
}

void verifyDecoding(const std::string& originalFile, MethodResult* mr) {
    std::cout << "Provjera ispravnosti dekodiranja..." << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    for (int i = 0; i < 5; i++) {
        std::string decodedFile = "dekodirani_" + mr[i].name + ".txt";

        try {
            std::u32string originalContent = readAndCleanText(originalFile);
            std::u32string decodedContent = readAndCleanText(decodedFile);

            if (originalContent == decodedContent) {
                std::cout << "Dekodirani fajl kodiran metodom \"" << mr[i].name << "\" je isti kao ulazni fajl!" << std::endl;
            }
            else {
                std::cout << "Greska u dekodiranju za fajl \"" << mr[i].name << "\"!" << std::endl;
                std::cout << "Originalna duzina: " << originalContent.size()
                    << ", Dekodirana duzina: " << decodedContent.size() << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Greška: " << e.what() << std::endl;
        }
    }
    std::cout << "---------------------------------------------------" << std::endl;
}

void convertCSVToXLSX(const std::string& csvFilename, const std::string& xlsxFilename) {
    std::ifstream csvFile(csvFilename);
    if (!csvFile.is_open()) {
        std::cerr << "Greska kod otvaranja fajla: " << csvFilename << std::endl;
        return;
    }

    lxw_workbook* workbook = workbook_new(xlsxFilename.c_str());
    lxw_worksheet* worksheet = workbook_add_worksheet(workbook, NULL);

    std::string line;
    int row = 0;

    while (std::getline(csvFile, line)) {
        std::stringstream lineStream(line);
        std::string cell;
        int col = 0;

        while (std::getline(lineStream, cell, ',')) {
            worksheet_write_string(worksheet, row, col, cell.c_str(), NULL);
            col++;
        }
        row++;
    }

    csvFile.close();

    workbook_close(workbook);
}