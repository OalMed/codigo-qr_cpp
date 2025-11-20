#include <iostream>
#include <vector>
#include "qrcodegen.hpp" 
#include <iomanip>
#include <clocale>
#include <windows.h>  

using namespace qrcodegen;

int main() {
	setlocale(LC_ALL, "es_ES.UTF-8");
	SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
	system("color f0");
	
	std::vector<uint8_t> codigos;
    std::string texto;
    printf("Ingresa un texto: \n");
	getline(std::cin, texto);
	
	for (unsigned char cb : texto) codigos.push_back(static_cast<int>(cb));
	
    QrCode qr = QrCode::encodeBinary(codigos, QrCode::Ecc::LOW);
    int size = qr.getSize();

    // Quiet zone o margen (módulos blancos alrededor)
    int quiet = 1;

    for (int y = -quiet; y < size + quiet; y++) {
        for (int x = -quiet; x < size + quiet; x++) {
            bool dark = false;
            if (x >= 0 && x < size && y >= 0 && y < size) dark = qr.getModule(x, y);
            
            std::cout << (dark ? u8"\u2588\u2588": "  ");
        }
        std::cout << std::endl;
    }
    return 0;
}

	/*for (char c : texto) {
		//unsigned char valor = static_cast<unsigned char>(c);
        codigos.push_back(static_cast<int>(c));
        std::cout << c << ", int: " << static_cast<int>(c);
	}*/
