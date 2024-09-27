//
// Created by julen on 23/08/24.
//

#include "raton.h"
#include "Wrapper/image.h"
#include "controlador.h"
#include "controles.h"

IMG* buscarLista(const Raton::lista_t* lista, const int posX, const int posY)
{
    // ReSharper disable once CppJoinDeclarationAndAssignment
    SDL_Rect* rect;

    while(lista != nullptr)
    {
        if(lista->img->getClickable())
        {
            rect = lista->img->getRect();
            if(rect->x <= posX && rect->x + rect->w >= posX && rect->y <= posY && rect->y + rect->h >= posY)
                return lista->img;
        }

        lista = lista->siguiente;
    }

    return nullptr;
}

void Raton::interactuar(IMG* actual)
{
    const Controles::Accion accion = Controles::getUltimaAccion();
    if(borrando)
    {
        if(accion == Controles::Interactuar)
        {
            if(actual->getPadrePuerta())
                controlador->borrar(actual->getPadrePuerta());
            else if(actual->getPadreEntrada())
                controlador->borrar(actual->getPadreEntrada());
            else if(actual->getPadreSalida())
                controlador->borrar(actual->getPadreSalida());
            borrando = false;
            return;
        }
    }

    if(imgAnterior == nullptr) //El raton a entrado a una imagen desde sin estar antes en otra
    {
        imgAnterior = actual;
        actual->clickar(posX - actual->getRect()->x, posY - actual->getRect()->y, Evento::ENTRAR);
    }
    else
    {
        if(imgAnterior == actual) //El raton sige encima de la misma imagen
        {
            if(accion == Controles::MovimientoRaton)
            {
                const SDL_Event* evento = Controles::getEvent();
                actual->clickar(evento->motion.xrel, evento->motion.yrel, Raton::MOVIMIENTO);
            }
            else
                actual->clickar(posX - actual->getRect()->x, posY - actual->getRect()->y, Raton::NADA);
        }
        else //Se salta de una imagen a otra directamente
        {
            imgAnterior->clickar(-1, -1,  Evento::SALIR);
            imgAnterior = actual;
            actual->clickar(posX - actual->getRect()->x, posY - actual->getRect()->y, Evento::ENTRAR);
        }
    }
}

bool Raton::interactuarConexion(IMG* actual)
{

    if(borrando)
    {
        if(Controles::getUltimaAccion() == Controles::ConexionArriba)
        {
            if(actual->getPadrePuerta())
            {
                const bool borrarArriba{posY < actual->getRect()->y + actual->getRect()->h / 2 && posX <= actual->getRect()->x + actual->getRect()->w / 2};
                const bool borrarAbajo{posY >= actual->getRect()->y + actual->getRect()->h / 2 && posX <= actual->getRect()->x + actual->getRect()->w / 2};
                const bool borrarSalida{posX > actual->getRect()->x + actual->getRect()->w / 2};
                controlador->borrarConexiones(actual->getPadrePuerta(), borrarArriba, borrarAbajo, borrarSalida);
            }
            else if(actual->getPadreEntrada())
                controlador->borrarConexiones(actual->getPadreEntrada());
            else if(actual->getPadreSalida())
                controlador->borrarConexiones(actual->getPadreSalida());
            borrando = false;
            return true;
        }

    }

    if(controlador->getConectando())
    {
        if(Controles::getUltimaAccion() == Controles::ConexionArriba)
        {
            if(actual->getPadrePuerta() != nullptr)
                controlador->destino(actual->getPadrePuerta(), posY < actual->getRect()->y + actual->getRect()->h / 2);
            else if(actual->getPadreSalida() != nullptr)
                controlador->destino(actual->getPadreSalida());
            else
                controlador->desmarcarOrigen();
            return true;
        }
    }
    else if(Controles::getUltimaAccion() == Controles::ConexionAbajo)
    {
        if(actual->getPadrePuerta() != nullptr)
            controlador->marcarOrigen(actual->getPadrePuerta());
        else if(actual->getPadreEntrada() != nullptr)
            controlador->marcarOrigen(actual->getPadreEntrada());
        else
            return false;

        return true;
    }

    return false;
}


void Raton::manejarRaton()
{
    SDL_GetMouseState(&posX, &posY);

    const lista_t* lista{frente};
    IMG* actual = buscarLista(lista, posX, posY);
    if(actual != nullptr)
    {
        interactuar(actual);
        return;
    }

    lista = medio;
    actual = buscarLista(lista, posX, posY);
    if(actual != nullptr)
    {
        if(interactuarConexion(actual))
            return;
        interactuar(actual);
        return;
    }

    //Por si se ha soltado la rueda en algun lugar que no sea clickable y conectable
    if(controlador->getConectando() && Controles::getUltimaAccion() == Controles::ConexionArriba)
    {
        controlador->desmarcarOrigen();
    }

    lista = fondo;
    actual = buscarLista(lista, posX, posY);
    if(actual != nullptr)
    {
        interactuar(actual);
        return;
    }

    if(imgAnterior != nullptr) //El raton no esta en ninguna imagen, y antes si lo estaba
    {
        imgAnterior->clickar(-1, -1, Evento::SALIR);
        imgAnterior = nullptr;
    }
}

bool Raton::añadir(IMG* img, const int altura_, const bool final)
{
    lista_t* lista;
    Altura altura{INVALIDO};

    if(altura_ == FONDO)
        altura = FONDO;
    else if (altura_ == MEDIO)
        altura = MEDIO;
    else if(altura_ == FRENTE)
        altura = FRENTE;

    switch(altura)
    {
    case FONDO:
        lista = fondo;
        break;
    case MEDIO:
        lista = medio;
        break;
    case FRENTE:
        lista = frente;
        break;
    case INVALIDO:
    default:
        return false;
    }

    if(lista == nullptr)
    {
        switch(altura)
        {
        case FONDO:
            fondo = new lista_t;
            lista = fondo;
            break;
        case MEDIO:
            medio = new lista_t;
            lista = medio;
            break;
        case FRENTE:
            frente = new lista_t;
            lista = frente;
        case INVALIDO:
            break;
        }
        lista->img = img;
        lista->siguiente = nullptr;
    }
    else if (final)
    {
        while(lista->siguiente != nullptr)
        {
            lista = lista->siguiente;
        }

        lista->siguiente = new lista_t;
        lista = lista->siguiente;
        lista->img = img;
        lista->siguiente = nullptr;
    }
    else
    {
        const auto tmp = new lista_t;
        tmp->img = img;
        tmp->siguiente = lista;
        switch (altura)
        {
        case FONDO:
            fondo = tmp;
            break;
        case MEDIO:
            medio = tmp;
            break;
        case FRENTE:
            frente = tmp;
        case INVALIDO:
            break;
        }
    }

    return true;
}


//No borra la imagen. Eso es trabajo de la clase IMG
bool Raton::borrar(const IMG* img, const int altura_)
{
    lista_t* lista;
    Altura altura{INVALIDO};

    if(altura_ == FONDO)
        altura = FONDO;
    else if (altura_ == MEDIO)
        altura = MEDIO;
    else if(altura_ == FRENTE)
        altura = FRENTE;

    switch (altura)
    {
    case FRENTE:
        lista = frente;
        break;
    case MEDIO:
        lista = medio;
        break;
    case FONDO:
        lista = fondo;
        break;
    case INVALIDO:
    default:
        return false;
    }

    if(lista == nullptr) //La lista está vacia
        return false;

    if(lista->siguiente == nullptr) //Solo hay un elemento
    {
        if(lista->img == img) //y coincide
        {
            delete lista;
            switch (altura)
            {
            case FRENTE:
                frente = nullptr;
                break;
            case MEDIO:
                medio = nullptr;
                break;
            case FONDO:
                fondo = nullptr;
            case INVALIDO:
                break;
            }
            return true;
        }

        return false; // y no coincide
    }

    if(lista->img == img) //Es el primer elemento de la lista
    {
        const lista_t* tmp = lista;

        switch (altura)
        {
        case FRENTE:
            frente = frente->siguiente;
            break;
        case MEDIO:
            medio = medio->siguiente;
            break;
        case FONDO:
            fondo = fondo->siguiente;
        case INVALIDO:
            break;
        }

        delete tmp;
        return true;
    }

    lista_t* atras{lista};
    lista = lista->siguiente;
    while(lista != nullptr && lista->img != img)
    {
        atras = lista;
        lista = lista->siguiente;
    }

    if(lista == nullptr || lista->img != img)
        return false;

    atras->siguiente = lista->siguiente;

    delete lista;

    return true;
}


//No borra las imagenes. Eso es trabajo de la clase IMG
bool Raton::borrarTodo(const int altura_)
{
    lista_t* lista;
    Altura altura{INVALIDO};

    if(altura_ == FONDO)
        altura = FONDO;
    else if (altura_ == MEDIO)
        altura = MEDIO;
    else if(altura_ == FRENTE)
        altura = FRENTE;

    // ReSharper disable once CppDFAUnusedValue
    const lista_t* tmp{nullptr};

    switch (altura)
    {
    case FONDO:
        lista = fondo;
        break;
    case MEDIO:
        lista = medio;
        break;
    case FRENTE:
        lista = frente;
        break;
    case INVALIDO:
    default:
        return false;
    }

    while(lista != nullptr)
    {
        tmp = lista;
        lista = lista->siguiente;
        delete tmp;
    }

    switch (altura)
    {
    case FONDO:
        fondo = nullptr;
        break;
    case MEDIO:
        medio = nullptr;
        break;
    case FRENTE:
        frente = nullptr;
    case INVALIDO:
        break;
    }

    return true;
}

void Raton::setBorrando(const bool borrando_)
{
    borrando = borrando_;
    controlador->desmarcarOrigen();
}
