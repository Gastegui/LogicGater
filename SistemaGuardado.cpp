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
#include "Simulables/temporizador.h"

auto SistemaGuardado::guardar(Controlador* controlador, const Window* window, const Valores* valores, const std::string& archivo) -> bool
{
    std::ofstream outf{archivo.c_str()};

    if(!outf)
    {
        std::println(std::cout, "No se ha podido crear el file stream de guardado");
        return false;
    }

    const int esquinaX{window->getEsquinaX()};
    const int esquinaY{window->getEsquinaY()};
    const std::vector<ListaLineas::Linea>*  listaLineas{controlador->getListaLineas()->getLista()};

    outf << "-Elementos\n";
    const Puerta* puerta{};
    const Salida* salida{};
    const Entrada* entrada{};
    const Temporizador* temp{};
    for(const auto& value : controlador->simulables | std::views::values)
    {
        if((puerta = dynamic_cast<Puerta*>(value)) != nullptr) // NOLINT(*-assignment-in-if-condition)
            outf << "Puerta: id: " << puerta->getId() << " tipo: " << puerta->tipo << " arribaNegado: " << puerta->arribaNegado << " abajoNegado: " << puerta->abajoNegado << " salidaNegada: " << puerta->salidaNegada << " x: " << puerta->x << " y: " << puerta->y << "\n";
        else if((salida = dynamic_cast<Salida*>(value)) != nullptr) // NOLINT(*-assignment-in-if-condition)
            outf << "Salida: id: " << salida->getId() << " x: " << salida->img.rect.x << " y: " << salida->img.rect.y << "\n";
        else if((entrada = dynamic_cast<Entrada*>(value)) != nullptr) // NOLINT(*-assignment-in-if-condition)
            outf << "Entrada: id: " << entrada->getId() << " mantener: " << entrada->mantener << " x: " << entrada->img.rect.x << " y: " << entrada->img.rect.y << "\n";
        else if((temp = dynamic_cast<Temporizador*>(value)) != nullptr) // NOLINT(*-assignment-in-if-condition)
            outf << "Temporizador: id: " << temp->getId() << " x: " << temp->imagen.rect.x << " y: " << temp->imagen.rect.y << " entradaNegada: " << temp->entradaNegada << " salidaNegada: " << temp->salidaNegada << " ciclosTotales: " << temp->ciclosTotales << "\n";
    }

    outf << "-Conexiones\n";

    for(const ListaLineas::Linea linea : *listaLineas)
        outf << "Conexion: " << "origen: " << linea.io->getSimulable()->getId() << " destino: " << linea.destino->getId() << " destino_conexion: " << linea.destino->getConexion(linea.io) << "\n";

    outf << "-Otros\n";
    outf << "Coordenadas: x: " << esquinaX << " y: " << esquinaY << "\n";
    outf << "VelocidadSimulacion: " << valores->velocidadSimulacion << "\n";
    outf << "CuadriculaTamaño: " << valores->cuadriculaTamaño << "\n";

    return true;
}


auto SistemaGuardado::cargar(Controlador* controlador, Window* window, Valores* valores, const std::string& archivo) -> int
{
    std::ifstream inf{archivo.c_str()};

    if(!inf)
    {
        std::println(std::cout, "No se ha podido crear el file stream de cargado");
        return -1;
    }

    if(controlador->getSimulablesLenght() != 0)
        return -2;

    using namespace std::literals::string_literals;

    std::string input{};

    std::string tmp{};

    //Puertas
    unsigned int puertaId{getOffset(ID_TIPOS::Puerta)};
    int puertaTipo{0};
    bool puertaArribaNegado{false};
    bool puertaAbajoNegado{false};
    bool puertaSalidaNegada{false};
    int puertaX{0};
    int puertaY{0};

    //Entradas
    unsigned int entradaId{getOffset(ID_TIPOS::Entrada)};
    bool entradaMantener{false};
    int entradaX{0};
    int entradaY{0};

    //Salidas
    unsigned int salidaId{getOffset(ID_TIPOS::Salida)};
    int salidaX{0};
    int salidaY{0};

    //Temporizadores
    unsigned int tempId{getOffset(ID_TIPOS::Temporizador)};
    int tempX{0};
    int tempY{0};
    bool tempEntradaNegada{false};
    bool tempSalidaNegada{false};
    int tempCiclos{0};

    //Conexiones
    unsigned int simulableOrigen{0};
    unsigned int simulableDestino{0};
    int simulableConexion{0};

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
        else if(tmp == "Temporizador:"s)
        {
            iss >> tmp; //id:
            iss >> tempId;
            iss >> tmp; //x:
            iss >> tempX;
            iss >> tmp; //y:
            iss >> tempY;
            iss >> tmp; //entradaNegada:
            iss >> tempEntradaNegada;
            iss >> tmp; //salidaNegada:
            iss >> tempSalidaNegada;
            iss >> tmp; //ciclosTotales:
            iss >> tempCiclos;
            controlador->crear(tempX, tempY, tempCiclos, tempEntradaNegada, tempSalidaNegada, tempId);
        }
        else if(tmp == "Conexion:"s)
        {
            iss >> tmp; //origen:
            iss >> simulableOrigen;
            iss >> tmp; //destino:
            iss >> simulableDestino;
            iss >> tmp; //destino_conexion:
            iss >> simulableConexion;

            Simulable* destino = controlador->getSimulable(simulableDestino);
            controlador->marcarOrigen(controlador->getSimulable(simulableOrigen)->getIOSalida());
            destino->interactuar(destino->getLinea(simulableConexion)->first - destino->getImg()->getRect()->x, destino->getLinea(simulableConexion)->second - destino->getImg()->getRect()->y, INTERACCIONES::ConexionArriba);
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
    Puerta::gastarIds(puertaId ^ getOffset(ID_TIPOS::Puerta)); //Esto sirve para desactivar el bit identificador
    Entrada::gastarIds(entradaId ^ getOffset(ID_TIPOS::Entrada));
    Salida::gastarIds(salidaId ^ getOffset(ID_TIPOS::Salida));
    Temporizador::gastarIds(tempId ^ getOffset(ID_TIPOS::Temporizador));

    return 0;
}
