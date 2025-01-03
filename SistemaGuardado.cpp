//
// Created by julen on 3/01/25.
//


#include "SistemaGuardado.h"

#include <fstream>
#include <sstream>
#include <string>

#include "controlador.h"
#include "Wrapper/window.h"
#include "puerta.h"
#include "entrada.h"
#include "salida.h"

bool SistemaGuardado::guardar(const Controlador* controlador, const Window* window, const int velocidadSimulacion)
{
    std::ofstream outf{"Guardado.txt"};

    if(!outf)
    {
        printf("No se ha podido crear el file stream de guardado\n");
        return false;
    }

    const Controlador::ListaPuertas* listaPuertas{controlador->listaPuertas} ;
    const Controlador::ListaEntradas* listaEntradas{controlador->listaEntradas};
    const Controlador::ListaSalidas* listaSalidas{controlador->listaSalidas};

    const int esquinaX{window->getEsquinaX()};
    const int esquinaY{window->getEsquinaY()};
    const Window::ListaLineas* listaLineas{window->lineas};

    printf("Guardando...\n");

    printf("-Puerta\n");
    outf << "-Puertas\n";

    while(listaPuertas != nullptr)
    {
        printf("Puerta: id: %u, tipo: %d, arN: %d, abN: %d, sN: %d, x: %d, y: %d\n", listaPuertas->puerta->id, listaPuertas->puerta->tipo, listaPuertas->puerta->arribaNegado, listaPuertas->puerta->abajoNegado, listaPuertas->puerta->salidaNegada, listaPuertas->puerta->x, listaPuertas->puerta->y);
        outf << "Puerta: id: " << listaPuertas->puerta->id << " tipo: " << listaPuertas->puerta->tipo << " arribaNegado: " << listaPuertas->puerta->arribaNegado << " abajoNegado: " << listaPuertas->puerta->abajoNegado << " salidaNegada: " << listaPuertas->puerta->salidaNegada << " x: " << listaPuertas->puerta->x << " y: " << listaPuertas->puerta->y << "\n";
        listaPuertas = listaPuertas->siguiente;
    }

    printf("-Entradas\n");
    outf << "-Entradas\n";

    while(listaEntradas != nullptr)
    {
        printf("Entrada: id: %u, mantener: %d, x: %d, y: %d\n", listaEntradas->entrada->id, listaEntradas->entrada->mantener, listaEntradas->entrada->img.m_rect.x, listaEntradas->entrada->img.m_rect.y);
        outf << "Entrada: id: " << listaEntradas->entrada->id << " mantener: " << listaEntradas->entrada->mantener << " x: " << listaEntradas->entrada->img.m_rect.x << " y: " << listaEntradas->entrada->img.m_rect.y << "\n";
        listaEntradas = listaEntradas->siguiente;
    }

    printf("-Salidas\n");
    outf << "-Salidas\n";

    while(listaSalidas != nullptr)
    {
        printf("Salida: id: %u, x: %d, %d\n", listaSalidas->salida->id, listaSalidas->salida->img.m_rect.x, listaSalidas->salida->img.m_rect.y);
        outf << "Salida: id: " << listaSalidas->salida->id << " x: " << listaSalidas->salida->img.m_rect.x << " y: " << listaSalidas->salida->img.m_rect.y << "\n";
        listaSalidas = listaSalidas->siguiente;
    }

    printf("-Lineas\n");
    outf << "-Lineas\n";

    while(listaLineas != nullptr)
    {
        printf("Linea: ");
        outf << "Linea: ";
        if(listaLineas->origenEntrada != nullptr)
        {
            printf("orE: %u", listaLineas->origenEntrada->id);
            outf << "origenEntrada: " << listaLineas->origenEntrada->id;
        }
        else
        {
            printf("orP: %u", listaLineas->origenPuerta->id);
            outf << "origenPuerta: " << listaLineas->origenPuerta->id;
        }
        if(listaLineas->destinoSalida != nullptr)
        {
            printf("deS: %u", listaLineas->destinoSalida->id);
            outf << " destinoSalida: " << listaLineas->destinoSalida->id;
        }
        else
        {
            printf("deP: %u, puertaArriba: %d", listaLineas->destinoPuerta->id, listaLineas->puertaArriba);
            outf << " destinoPuerta: " << listaLineas->destinoPuerta->id << " puertaArriba: " << listaLineas->puertaArriba;
        }

        printf("\n");
        outf << "\n";
        listaLineas = listaLineas->siguiente;
    }

    printf("-Otros\n");
    outf << "-Otros\n";

    printf("Coordenadas: x: %d, y: %d\n", esquinaX, esquinaY);
    outf << "Coordenadas: x: " << esquinaX << " y: " << esquinaY << "\n";

    printf("Velocidad simulacion: %d\n", velocidadSimulacion);
    outf << "VelocidadSimulacion: " << velocidadSimulacion << "\n";

    return true;
}


int SistemaGuardado::cargar(Controlador* controlador, Window* window)
{
    std::ifstream inf{ "Guardado.txt" };

    if(!inf)
    {
        printf("No se ha podido crear el file stream de cargado\n");
        return -1;
    }

    if(controlador->getPuerta(1) != nullptr || controlador->getEntrada(1) != nullptr || controlador->getSalida(1) != nullptr)
        return -2;

    using namespace std::literals::string_literals;

    std::string input{};

    std::string tmp{};

    //Puertas
    int puertaTipo{0};
    bool puertaArribaNegado{false};
    bool puertaAbajoNegado{false};
    bool puertaSalidaNegada{false};
    int puertaX{0};
    int puertaY{0};

    //Entradas
    bool entradaMantener{false};
    int entradaX{0};
    int entradaY{0};

    //Salidas
    int salidaX{false};
    int salidaY{false};

    //Lineas
    unsigned int lineaOrigenEntrada{0};
    unsigned int lineaOrigenPuerta{0};
    unsigned int lineaDestinoSalida{0};
    unsigned int lineaDestinoPuerta{0};
    bool lineaPuertaArriba{false};

    //Otros
    int coordenadaX{0};
    int coordenadaY{0};
    int velocidadSimulacion{50};

    while(std::getline(inf, input))
    {
        std::istringstream iss(input);
        iss >> tmp;

        if(tmp == "-Puertas"s || tmp == "-Entradas"s || tmp == "-Salidas"s || tmp == "-Lineas"s || tmp == "-Otros"s)
            continue;
        if(tmp == "Puerta:"s)
        {
            iss >> tmp; //id:
            iss >> tmp; //x
            iss >> tmp; //Tipo:
            iss >> puertaTipo;
            iss >> tmp; //arribaNegado:
            iss >> puertaArribaNegado;
            iss >> tmp; //abajoNegado:
            iss >> puertaAbajoNegado;
            iss >> tmp; //salidaNegada:
            iss >> puertaSalidaNegada;
            iss >> tmp; //x:
            iss >> puertaX;
            iss >> tmp; //y:
            iss >> puertaY;
            controlador->crear(puertaTipo == 0 ? Puerta::AND : puertaTipo == 1 ? Puerta::OR : Puerta::XOR, puertaX, puertaY, puertaArribaNegado, puertaAbajoNegado, puertaSalidaNegada);
        }
        else if(tmp == "Entrada:"s)
        {
            iss >> tmp; //id:
            iss >> tmp; //x
            iss >> tmp; //mantener:
            iss >> entradaMantener;
            iss >> tmp; //x:
            iss >> entradaX;
            iss >> tmp; //y:
            iss >> entradaY;
            controlador->crear(entradaMantener, entradaX, entradaY);
        }
        else if(tmp == "Salida:"s)
        {
            iss >> tmp; //id:
            iss >> tmp; //x
            iss >> tmp; //x:
            iss >> salidaX;
            iss >> tmp; //y:
            iss >> salidaY;
            controlador->crear(salidaX, salidaY);
        }
        else if(tmp == "Linea:"s)
        {
            lineaOrigenEntrada = 0;
            lineaOrigenPuerta = 0;
            lineaDestinoSalida = 0;
            lineaDestinoPuerta = 0;

            iss >> tmp; //origen
            if(tmp == "origenEntrada:"s)
                iss >> lineaOrigenEntrada;
            else if(tmp == "origenPuerta:"s)
                iss >> lineaOrigenPuerta;
            iss >> tmp; //Destino
            if(tmp == "destinoSalida:"s)
                iss >> lineaDestinoSalida;
            else if(tmp == "destinoPuerta:"s)
            {
                iss >> lineaDestinoPuerta;
                iss >> tmp; //puertaArriba
                iss >> lineaPuertaArriba;
            }

            if(lineaOrigenEntrada != 0)
                controlador->marcarOrigen(controlador->getEntrada(lineaOrigenEntrada));
            else if(lineaOrigenPuerta != 0)
                controlador->marcarOrigen(controlador->getPuerta(lineaOrigenPuerta));

            bool ret{false};
            if(lineaDestinoSalida != 0)
                ret = controlador->destino(controlador->getSalida(lineaDestinoSalida));
            else if(lineaDestinoPuerta != 0)
                ret = controlador->destino(controlador->getPuerta(lineaDestinoPuerta), lineaPuertaArriba);

            if(!ret)
            {
                printf("No se ha podido crear la línea \"%s\": %u %u %d\n", input.c_str(), lineaOrigenPuerta, lineaDestinoPuerta, lineaPuertaArriba);
                controlador->desmarcarOrigen();
            }
        }
        else if(tmp == "Coordenadas:"s)
        {
            iss >> tmp; //x:
            iss >> coordenadaX;
            iss >> tmp; //y:
            iss >> coordenadaY;
            window->mover(coordenadaX, coordenadaY);
        }
        else if(tmp == "VelocidadSimulacion:"s)
        {
            iss >> velocidadSimulacion;
        }
    }

    return velocidadSimulacion;
}
