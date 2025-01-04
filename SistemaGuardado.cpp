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

bool SistemaGuardado::guardar(const Controlador* controlador, const Window* window, Valores* valores)
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

    outf << "-Puertas\n";

    while(listaPuertas != nullptr)
    {
        outf << "Puerta: id: " << listaPuertas->puerta->id << " tipo: " << listaPuertas->puerta->tipo << " arribaNegado: " << listaPuertas->puerta->arribaNegado << " abajoNegado: " << listaPuertas->puerta->abajoNegado << " salidaNegada: " << listaPuertas->puerta->salidaNegada << " x: " << listaPuertas->puerta->x << " y: " << listaPuertas->puerta->y << "\n";
        listaPuertas = listaPuertas->siguiente;
    }

    outf << "-Entradas\n";

    while(listaEntradas != nullptr)
    {
        outf << "Entrada: id: " << listaEntradas->entrada->id << " mantener: " << listaEntradas->entrada->mantener << " x: " << listaEntradas->entrada->img.m_rect.x << " y: " << listaEntradas->entrada->img.m_rect.y << "\n";
        listaEntradas = listaEntradas->siguiente;
    }

    outf << "-Salidas\n";

    while(listaSalidas != nullptr)
    {
        outf << "Salida: id: " << listaSalidas->salida->id << " x: " << listaSalidas->salida->img.m_rect.x << " y: " << listaSalidas->salida->img.m_rect.y << "\n";
        listaSalidas = listaSalidas->siguiente;
    }

    outf << "-Lineas\n";

    while(listaLineas != nullptr)
    {
        outf << "Linea: ";
        if(listaLineas->origenEntrada != nullptr)
            outf << "origenEntrada: " << listaLineas->origenEntrada->id;
        else
            outf << "origenPuerta: " << listaLineas->origenPuerta->id;
        if(listaLineas->destinoSalida != nullptr)
            outf << " destinoSalida: " << listaLineas->destinoSalida->id;
        else
            outf << " destinoPuerta: " << listaLineas->destinoPuerta->id << " puertaArriba: " << listaLineas->puertaArriba;

        outf << "\n";
        listaLineas = listaLineas->siguiente;
    }

    outf << "-Otros\n";
    outf << "Coordenadas: x: " << esquinaX << " y: " << esquinaY << "\n";
    outf << "VelocidadSimulacion: " << valores->velocidadSimulacion << "\n";
    outf << "CuadriculaTamaño: " << valores->cuadriculaTamaño << "\n";

    return true;
}


int SistemaGuardado::cargar(Controlador* controlador, Window* window, Valores* valores)
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
    unsigned int puertaId{0};
    int puertaTipo{0};
    bool puertaArribaNegado{false};
    bool puertaAbajoNegado{false};
    bool puertaSalidaNegada{false};
    int puertaX{0};
    int puertaY{0};

    //Entradas
    unsigned int entradaId{0};
    bool entradaMantener{false};
    int entradaX{0};
    int entradaY{0};

    //Salidas
    unsigned int salidaId{0};
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

    while(std::getline(inf, input))
    {
        std::istringstream iss(input);
        iss >> tmp;

        if(tmp == "-Puertas"s || tmp == "-Entradas"s || tmp == "-Salidas"s || tmp == "-Lineas"s || tmp == "-Otros"s)
            continue;
        if(tmp == "Puerta:"s)
        {
            iss >> tmp; //id:
            iss >> puertaId; //x
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
            controlador->crear(puertaTipo == 0 ? Puerta::AND : puertaTipo == 1 ? Puerta::OR : Puerta::XOR, puertaX, puertaY, puertaArribaNegado, puertaAbajoNegado, puertaSalidaNegada, puertaId);
        }
        else if(tmp == "Entrada:"s)
        {
            iss >> tmp; //id:
            iss >> entradaId; //x
            iss >> tmp; //mantener:
            iss >> entradaMantener;
            iss >> tmp; //x:
            iss >> entradaX;
            iss >> tmp; //y:
            iss >> entradaY;
            controlador->crear(entradaMantener, entradaX, entradaY, entradaId);
        }
        else if(tmp == "Salida:"s)
        {
            iss >> tmp; //id:
            iss >> salidaId; //x
            iss >> tmp; //x:
            iss >> salidaX;
            iss >> tmp; //y:
            iss >> salidaY;
            controlador->crear(salidaX, salidaY, salidaId);
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
                controlador->desmarcarOrigen();
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
            iss >> valores->velocidadSimulacion;
        else if(tmp == "CuadriculaTamaño:"s)
            iss >> valores->cuadriculaTamaño;
    }

    Puerta::gastarIds(puertaId);
    Entrada::gastarIds(entradaId);
    Salida::gastarIds(salidaId);

    return 0;
}
