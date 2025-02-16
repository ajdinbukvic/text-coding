#include <string>
#include "Text.h"
#include "Functions.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <conio.h>
#include "Method.h"
#include "Menu.h"

void readFileAndCalculateData(const std::string& inputFilename, std::vector<Symbol>& symbols, Data& data) {
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << "Racunanje frekvencije simbola, relativne frekvencije simbola i entropije izvora..." << std::endl;
    data.text = readAndCleanText(inputFilename);

    auto frequencies = calculateFrequencies(data.text);
    data.totalSymbols = data.text.size();

    auto result = calculateRelativeFrequenciesAndEntropy(frequencies, data.totalSymbols);
    symbols = result.first;
    data.entropy = result.second;

    std::ifstream file(inputFilename, std::ios::binary | std::ios::ate);
    data.fileSize = file.tellg();
    file.close();

    data.totalUniqueSymbols = symbols.size();
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << "Ukupno karaktera: " << data.totalSymbols << std::endl;
    std::cout << "Ukupno razlicitih karaktera: " << data.totalUniqueSymbols << std::endl;
    std::cout << "Velicina u bajtima: " << data.fileSize << std::endl;
    std::cout << "Entropija izvora bez memorije: " << data.entropy << " bita po simbolu" << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << "Pritisnite bilo koju tipku za nastavak..." << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    _getch();
}

void generateCodesAndEncodeText(const std::string& inputFilename, std::vector<Method*> methods, MethodResult* mr, std::vector<Symbol>& symbols, std::vector<Symbol>& symbolVec, std::vector<SymbolExtended>& filteredSymbols,
    int blockSize, std::u32string text) {
    std::cout << "Ucitavanje podataka o relativnoj frekvenciji simbola..." << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    for (int i = 0; i < methods.size(); i++) {
        auto clock_start = std::chrono::system_clock::now();
        std::string codedFilename = "kodirani_" + mr[i].name + ".txt";
        std::cout << "Generisanje kodova metodom: " << mr[i].name << "..." << std::endl;
        if (i == 0 || i == 2 || i == 3)
        {
            methods[i]->generateCodes(symbols);
        }
        if (i == 1)
        {
            methods[i]->generateCodes(symbols, 0, symbols.size() - 1, "");
        }
        std::cout << "Kodiranje ulaznog teksta metodom: " << mr[i].name << "..." << std::endl;
        if (i == 4)
        {
            std::vector<SymbolExtended> symbols2 = methods[i]->generateCodes(symbols, blockSize);
            methods[2]->generateCodes(symbols2);
            filteredSymbols = methods[i]->filterCombinationsByText(symbols2, text, blockSize);
            symbolVec = copyFromSymbolExtendedToSymbol(filteredSymbols, 4);
            methods[i]->encodeText(inputFilename, symbols2, codedFilename, blockSize, i);
        }
        if (i != 4) methods[i]->encodeText(inputFilename, symbols, codedFilename, i);
        auto clock_now = std::chrono::system_clock::now();
        float currentTime = float(std::chrono::duration_cast <std::chrono::milliseconds> (clock_now - clock_start).count());
        mr[i].codingTime = currentTime;
        std::cout << "Vrijeme generisanja kodova i kodiranja: " << currentTime << " ms \n";
        std::cout << "---------------------------------------------------" << std::endl;
    }
    std::cout << "Pritisnite bilo koju tipku za nastavak..." << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    _getch();
}

void decodeText(const std::string& outputFilename, std::vector<Method*> methods, MethodResult* mr) {
    std::cout << "Dekodiranje..." << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    for (int i = 0; i < methods.size(); i++) {
        auto clock_start = std::chrono::system_clock::now();
        std::cout << "Dekodiranje kodiranog teksta metodom: " << mr[i].name << "..." << std::endl;
        std::string codedFilename = "kodirani_" + mr[i].name + ".txt";
        std::string decodedFilename = "dekodirani_" + mr[i].name + ".txt";
        methods[i]->decodeTextFromCSV(outputFilename, codedFilename, decodedFilename, i);
        auto clock_now = std::chrono::system_clock::now();
        float currentTime = float(std::chrono::duration_cast <std::chrono::milliseconds> (clock_now - clock_start).count());
        std::cout << "Vrijeme dekodiranja: " << currentTime << " ms \n";
        mr[i].decodingTime = currentTime;
        std::cout << "---------------------------------------------------" << std::endl;
    }
    std::cout << "Pritisnite bilo koju tipku za nastavak..." << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    _getch();
}