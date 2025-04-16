//
// Created by julen on 3/01/25.
//


#include "SistemaGuardado.h"

#include <fstream>
#include <sstream>
#include <string>

#include "controlador.h"
#include "Wrapper/window.h"
#include "Simulables/puerta.h"
#include "Simulables/entrada.h"
#include "Simulables/salida.h"

bool SistemaGuardado::guardar(const Controlador* controlador, const Window* window, const Valores* valores, const std::string& archivo)
{
    std::ofstream outf{archivo.c_str()};

    if(!outf)
    {
        printf("No se ha podido crear el file stream de guardado\n");
        return false;
    }

    const int esquinaX{window->getEsquinaX()};
    const int esquinaY{window->getEsquinaY()};
    const ListaLineas::Lista* listaLineas{controlador->getListaLineas()->getLista()};

    outf << "-Elementos\n";
    const Puerta* puerta;
    const Salida* salida;
    const Entrada* entrada;
    for (const auto& value : controlador->simulables | std::views::values)
    {
        if((puerta = dynamic_cast<Puerta*>(value)) != nullptr)
        {
            outf << "Puerta: id: " <<puerta->getId() << " tipo: " <<puerta->tipo << " arribaNegado: " <<puerta->arribaNegado << " abajoNegado: " <<puerta->abajoNegado << " salidaNegada: " <<puerta->salidaNegada << " x: " <<puerta->x << " y: " <<puerta->y << "\n";
        }
        else if((salida = dynamic_cast<Salida*>(value)) != nullptr)
        {
            outf << "Salida: id: " << salida->getId() << " x: " << salida->img.m_rect.x << " y: " << salida->img.m_rect.y << "\n";
        }
        else if((entrada = dynamic_cast<Entrada*>(value)) != nullptr)
        {
            outf << "Entrada: id: " << entrada->getId() << " mantener: " << entrada->mantener << " x: " << entrada->img.m_rect.x << " y: " << entrada->img.m_rect.y << "\n";
        }
    }

    outf << "-Conexiones\n";

    while(listaLineas != nullptr)
    {
        outf << "Conexion: " << "origen: " << listaLineas->io->getSimulable()->getId() << " destino: " << listaLineas->destino->getId() << " destino_conexion: " << listaLineas->destino->getConexion(listaLineas->io) << "\n";
        listaLineas = listaLineas->siguiente;
    }

    outf << "-Otros\n";
    outf << "Coordenadas: x: " << esquinaX << " y: " << esquinaY << "\n";
    outf << "VelocidadSimulacion: " << valores->velocidadSimulacion << "\n";
    outf << "CuadriculaTamaño: " << valores->cuadriculaTamaño << "\n";

    return true;
}


int SistemaGuardado::cargar(Controlador* controlador, Window* window, Valores* valores, const std::string& archivo)
{
    std::ifstream inf{ archivo.c_str() };

    if(!inf)
    {
        printf("No se ha podido crear el file stream de cargado\n");
        return -1;
    }

    if(controlador->getSimulablesLenght() != 0)
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

    //Conexiones
    unsigned int simulable_origen{0};
    unsigned int simulable_destino{0};
    int simulable_conexion{0};

    //Otros
    int coordenadaX{0};
    int coordenadaY{0};

    while(std::getline(inf, input))
    {
        std::istringstream iss(input);
        iss >> tmp;

        if(tmp == "-Elementos"s || tmp == "-Lineas"s || tmp == "-Otros"s)
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
        else if(tmp == "Conexion:"s)
        {
            iss >> tmp; //origen:
            iss >> simulable_origen;
            iss >> tmp; //destino:
            iss >> simulable_destino;
            iss >> tmp; //destino_conexion:
            iss >> simulable_conexion;

            Simulable* destino = controlador->getSimulable(simulable_destino);
            controlador->marcarOrigen(controlador->getSimulable(simulable_origen)->getIOSalida());
            destino->interactuar(destino->getLinea(simulable_conexion)->first - destino->getImg()->getRect()->x,destino->getLinea(simulable_conexion)->second - destino->getImg()->getRect()->y, INTERACCIONES::ConexionArriba);
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
    Puerta::gastarIds(puertaId ^ get_offset(ID_TIPOS::Puerta)); //Esto sirve para desactivar el bit identificador
    Entrada::gastarIds(entradaId ^ get_offset(ID_TIPOS::Entrada));
    Salida::gastarIds(salidaId ^ get_offset(ID_TIPOS::Salida));

    return 0;
}
