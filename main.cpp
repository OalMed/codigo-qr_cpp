#include <iostream>
#include "qrcodegen.hpp"   

using namespace qrcodegen;
using namespace std;

int main() {
	system("color f0");
	
    std::string texto;
    printf("Ingresa un texto: \n");
	getline(std::cin, texto);
	
    QrCode qr = QrCode::encodeText(texto.c_str(), QrCode::Ecc::LOW);
    int size = qr.getSize();

    // Quiet zone o margen (módulos blancos alrededor)
    int quiet = 1;

    for (int y = -quiet; y < size + quiet; y++) {
        for (int x = -quiet; x < size + quiet; x++) {
            bool dark = false;
            if (x >= 0 && x < size && y >= 0 && y < size) dark = qr.getModule(x, y);
            
            std::cout << (dark ? std::string(2, static_cast<char>(219)) : std::string(2, static_cast<char>(32)));
        }
        std::cout << std::endl;
    }
    return 0;
}

