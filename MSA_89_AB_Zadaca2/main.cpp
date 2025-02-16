#include <iostream>
#include <chrono>
#include "Symbol.h"
#include "Uniform.h"
#include "Functions.h"
#include "Text.h"
#include "MethodResult.h"
#include "ShannonFano.h"
#include "Huffman.h"
#include "HuffmanShorten.h"
#include "HuffmanMultiplied.h"
#include "SymbolExtended.h"
#include "Menu.h"

int main() {
    std::string inputFilename;
    std::cout << "Unesite naziv ulaznog fajla (bez ekstenzije): ";
    std::getline(std::cin, inputFilename);
    inputFilename += ".txt";

    Data data;
    std::vector<Symbol> symbols;
    std::vector<Symbol> symbolVec;
    std::vector<SymbolExtended> filteredSymbols;
    int blockSize = 2;

    readFileAndCalculateData(inputFilename, symbols, data);

    MethodResult* mr = new MethodResult[5];
    mr[0].name = "ravnomjerni";
    mr[1].name = "shannon_fano";
    mr[2].name = "obicni_huffman";
    mr[3].name = "skraceni_huffman";
    mr[4].name = "multiplicirani_huffman";

    std::vector<Method*> methods { new Uniform, new ShannonFano, new Huffman, new HuffmanShorten, new HuffmanMultiplied };
    
    generateCodesAndEncodeText(inputFilename, methods, mr, symbols, symbolVec, filteredSymbols, blockSize, data.text);
  
    displayResults(mr, symbols, symbolVec);

    std::string outputFilename = "izlaz.csv";
    writeDataToCSV(outputFilename, symbols, filteredSymbols, data.entropy, data.totalSymbols, data.fileSize, mr);

    decodeText(outputFilename, methods, mr);

    verifyDecoding(inputFilename, mr);

    return 0;
}
