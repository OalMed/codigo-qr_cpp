#include <iostream>
#include <vector>
#include "qrcodegen.hpp" 
#include <clocale>
#include <windows.h>  
#include <fstream>
#include <limits>
//#include <filesystem>  

using namespace qrcodegen;

void mostrarMenu();
void pedirDatos();
void imprimirQRs();
void descargarQRs();


std::string texto;
std::vector<std::string> textos;
std::vector<uint8_t> codigos;
std::vector<std::vector<uint8_t>> codsTextos;
std::vector<QrCode> QRs;
int numQRs, opcion;
bool repetir = true;


int main() {
	setlocale(LC_ALL, "es_ES.UTF-8");
	SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
	system("color f0");
	
    
    while (repetir) {
        system("cls"); 
        mostrarMenu();
        std::cin >> opcion;
        if (std::cin.fail()) {
        	std::cin.clear();
        	std::cin.ignore();
        	opcion = -1;
		}

        // Limpieza del buffer por si quedan saltos de línea
        std::cin.ignore();

        switch (opcion) {
            case 1:
            	pedirDatos();
            	if (numQRs == 0) break;
                imprimirQRs();
                break;

            case 2:
            	pedirDatos();
            	if (numQRs == 0) break;
                descargarQRs();
                break;

            case 3:
                std::cout << "\nSaliendo del programa..." << std::endl;
                repetir = false;
                break;

            default:
                std::cout << "\n[!] Opcion no valida. Intenta de nuevo" << std::endl;
        }

        if (repetir) {
            std::cout << "\nPresiona Enter para volver al menu...";
            std::cin.get(); 
        }
    }
    
    return 0;
}


void mostrarMenu() {
    std::cout << "===========================================" << std::endl;
    std::cout << "          GENERADOR DE CODIGOS QR" << std::endl;
    std::cout << "===========================================\n" << std::endl;
    std::cout << "1. Generar codigo(s) QR en la consola" << std::endl;
    std::cout << "2. Generar codigo(s) QR como imagen (descargar)" << std::endl;
    std::cout << "3. Salir" << std::endl;
    std::cout << "\n===========================================\n" << std::endl;
    std::cout << "Seleccione una opcion: ";
}

void pedirDatos() {
	std::cout << "\nIngrese la cantidad de QRs que desea generar (max 10): " ;
    std::cin >> numQRs;

    while (std::cin.fail() || numQRs < 0 || numQRs > 10) {
    	std::cin.clear();
    	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    	std::cout << "\nIngrese un valor valido: ";
    	std::cin >> numQRs;
	}
    std::cin.ignore();
    
    textos.clear();
    codsTextos.clear();
    QRs.clear();
	for (int i = 0; i < numQRs; i++) {
		codigos.clear();
		std::cout << "\nIngrese el texto #" << i+1 << ": \n"; 
		std::getline(std::cin, texto);
		for (unsigned char cb : texto) codigos.push_back(static_cast<int>(cb));
		textos.push_back(texto);
		codsTextos.push_back(codigos);
	
    	QrCode qr = QrCode::encodeBinary(codigos, QrCode::Ecc::QUARTILE);
    	QRs.push_back(qr);
    }
}

void imprimirQRs() {
    std::cout << "\n\n----- [MODO CONSOLA] Generando QR(s) -----\n" << std::endl;
    
	for (int i = 0; i < numQRs; i++){
		
		QrCode qr = QRs[i];
    	int size = qr.getSize();
    	
    	std::cout << "Texto " << i+1 << ":" << std::endl;
    	std::cout << "    " << textos[i] << std::endl;
    	
	    // Margen (módulos blancos alrededor)
	    int margen = 2;
	
	    for (int y = -margen; y < size + margen; y++) {
	        for (int x = -margen; x < size + margen; x++) {
	            std::cout << (qr.getModule(x,y) ? u8"\u2588\u2588": "  ");
	        }
	        std::cout << std::endl;
	    }
	    
	    std::cout << "\n\n";
	}	
}


void descargarQRs() {
    std::cout << "\n\n----- [MODO IMAGEN] Descargando QR(s) -----\n" << std::endl;

    int margen = 2;    
    int moduloTamano = 10;   
    //std::filesystem::create_directories("qr-generados");

    for (int i = 0; i < numQRs; ++i) {
        const QrCode &qr = QRs[i];
        int size = qr.getSize(); 

        int dimension = (size + 2 * margen) * moduloTamano;  
        std::string nombreArch = textos[i].substr(0,30);

        // limpiar nombre
        for(char& c : nombreArch) {
            if (!isalnum(c) && c != '_' && c != '-') {
                c = '_';
            }
        }

        std::string archivo = "qr_" + nombreArch + ".svg";

        std::ofstream out("qr-generados/" + archivo);
        if (!out) {
            std::cerr << "No se pudo crear " << archivo << "\n";
            continue;  // pasa al siguiente
        }
        // lienzo del svg
        out << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
            << "width=\"" << dimension << "\" height=\"" << dimension << "\" "
            << "viewBox=\"0 0 " << dimension << " " << dimension << "\">\n";

        // fondo blanco
        out << "  <rect x=\"0\" y=\"0\" width=\"" << dimension
            << "\" height=\"" << dimension << "\" fill=\"white\"/>\n";

        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                if (!qr.getModule(x, y)) continue;

                int X = (x + margen) * moduloTamano;
                int Y = (y + margen) * moduloTamano;

                out << "  <rect x=\"" << X
                    << "\" y=\"" << Y
                    << "\" width=\"" << moduloTamano
                    << "\" height=\"" << moduloTamano
                    << "\" fill=\"black\"/>\n";
            }
        }

        out << "</svg>\n";
        out.close();

        std::cout << "QR #" << (i + 1) << " guardado como " << archivo << "\n";
    }

    std::cout << "\n Los QRs han sido generados en formato .svg en la carpeta qr-generados.\n";
    
}

