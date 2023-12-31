// Scaner_A01568158.cpp 
// Table-Driven
// Hecho por Sebastian Salazar Villanueva
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Variables Globales
fstream fin("Sample.txt", fstream::in); // Modo lectura File in
bool aceptchar; // Llave para saber si un caracter es letra, numero o caracter especial, de esta forma no pasar por los dem�s
string identifier; // Buffer donde se guarda cada uno de los caracteres ya sean 
int characterType; // Indica si es letra, numero o caracter especial

// El caracter actual que se lee y los Arreglos de Caracteres
char caracterActual = ' ';
char letraArr[52] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };
int numArr[10] = { '0','1','2','3','4','5','6','7','8','9' };
char caracEspecialesArr[16] = { '/',';',',','=','*','+','-','(',')','<','>','[',']','{','}', '!' };
char espaciosArr[4] = { '\n','\t', ' ', '\0' };

// Lista de Tokens
string lista_token[27] = { "RETURN", "WHILE", "VOID", "ELSE", "IF", "INT", "INPUT" , "OUTPUT" , ">" , "<" , "=", "<=" , "==", ">=" , "!=", "(", ")" , "[" , "]" , "{" , "}" , "/" , "*", "-" , "+", "," , ";" };

// Llave para saber que tipo de caracter es el caracter actual
bool esLetra = false;
bool esNum = false;
bool esEspecial = false;
bool esEspacio = false;

// Estados, se especifica cuales estados son aceptores, de error, un arreglo bidimensional para revisar si puede avanzar 
bool aceptarEstados[36] = { false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true,true, true, true, true, true, true, true, true, true, true, true, true, false, false, false ,false };
bool errorEstados[36] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,true, true,true, true };
bool avanEstados[10][20] = {
                       { true, true, true, false, false, false, true, true, true, true, false, false, false, false, false, false, false, false, true, false},
                       { true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,false},
                       { false, true,false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,false},
                       { false, false,false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,false},
                       { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
                       { true, true, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
                       { false, false,false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,false},
                       { false, false,false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,false},
                       { false, false,false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,false},
                       { false, false,false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,false},
};

//Tabla de transiciones
int tablaTrans[10][20] = {
                       {1, 2 , 3, 14, 15, 16, 6, 7, 8, 9, 17, 18, 19, 20, 21, 22, 23, 24, 0, 34},
                       {1, 32, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 34},
                       {33, 2, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 34},
                       {12, 12, 12, 4, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 34},
                       {4, 4, 4, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
                       {4, 4, 13, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
                       {25, 25, 25, 25, 25, 25, 25, 26, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 34},
                       {27, 27, 27, 27, 27, 27, 27, 28, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 34},
                       {29, 29, 29, 29, 29, 29, 29, 30, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 34},
                       {35, 35, 35, 35, 35, 35, 35, 31, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 34}
};

namespace Vec {
    template<typename Id>
    class vector {
    private:
        // La capacidad inicial del arreglo con los 29 tokens
        //size_t tipo de entero sin ser asignado
        static const size_t capacidad_inicio = 1;
        // La capacidad actual del arreglo
        size_t capacidad_actual;
        // El num_obj actual del arreglo
        size_t num_obj;
        //El array que contiene la data
        Id* arr_datos;
    public:
        //Constructor
        vector<Id>() : capacidad_actual(capacidad_inicio), arr_datos(new Id[capacidad_inicio]), num_obj(0) { }
        // Regresa el numero de objetos que estan insertados, los cuales sirven para recorrer todo el vector de output
        size_t getActualPos() {
            return num_obj;
        }
        // Obtiene lo que se encuentra en la posici�n especificada en el par�metro
        Id& getIdent(int x) {
            return  arr_datos[x];
        }
        // Expande el vector para que sea posible insertar 
        void reservar(size_t capacidad_nueva) {
            if (capacidad_nueva > capacidad_actual) {
                if (capacidad_nueva > 2 * capacidad_actual) {
                    capacidad_actual = capacidad_nueva;

                }
                else {
                    // Aumenta por 2 la capacidad
                    capacidad_actual *= 2;
                }
                Id* nuevos_datos = new Id[capacidad_actual];

                // Copiar la data en el arreglo
                for (size_t i = 0; i < num_obj; i++) {
                    nuevos_datos[i] = arr_datos[i];
                }
                // Reiniciar y liberar el arreglo
                delete[] arr_datos;
                // Asignar el nuevo arreglo con nuevos datos
                arr_datos = nuevos_datos;
            }
        }
        // Insertar objeto en el vector especificado dependiendo de su tipo de dato
        void insertar(size_t index, const Id& valor) {
            if (index > num_obj) {
                cout << "ERROR: Fuera de rango";
            }
            // Asegurarnos que exista espacio para el nuevo obj
            if (num_obj == capacidad_actual) {
                reservar(2 * capacidad_actual);
            }
            // Recorre hasta encontrar el index
            for (size_t i = num_obj; i > index; i--) {
                arr_datos[i] = arr_datos[i - 1];
            }
            // Insertar nuevo objecto
            arr_datos[index] = valor;
            num_obj++;
        }
    };
}
// *Vectores con sus tipos de datos*
Vec::vector<string> vecOutput; // Vector para la salida
Vec::vector<int> vecNum; // Vector para los n�meros
Vec::vector<string> vecLetra; // Vector para los identificadores

// Transforma todas las letras de un identificador para revisar si es una Keyword
string upperCase(string ident) {
    string upper = "";
    for (int i = 0; i < ident.length(); i++) {
        upper += toupper(ident[i]);
    }

    return upper;
}

// Compara en que estado se encuentra y regresa Token
int compararToken(int estado) {

    string upper = upperCase(identifier);
    switch (estado) {
    case 10:
        for (int d = 0; d <= 7;) {
            if (upper == lista_token[d]) {
                return d + 1;
            }
            d++;
        }
        return 28;
        break;
    case 11:
        return 29;
        break;
    case 12:
        return 22;
        break;
    case 13:
        return 0;
        break;
    case 14:
        return 23;
        break;
    case 15:
        return 25;
        break;
    case 16:
        return 24;
        break;
    case 17:
        return 16;
        break;
    case 18:
        return 17;
        break;
    case 19:
        return 18;
        break;
    case 20:
        return 19;
        break;
    case 21:
        return 20;
        break;
    case 22:
        return 21;
        break;
    case 23:
        return 27;
        break;
    case 24:
        return 26;
        break;
    case 25:
        return 10;
        break;
    case 26:
        return 12;
        break;
    case 27:
        return 11;
        break;
    case 28:
        return 13;
        break;
    case 29:
        return 9;
        break;
    case 30:
        return 14;
        break;
    case 31:
        return 15;
        break;
    case 32:
        cout << " ------ERROR, d�gito despu�s de letra----- ";
        abort();
        break;
    case 33:
        cout << " ------ERROR, letra despu�s de d�gito------ ";
        abort();
        break;
    case 34:
        cout << " ----ERROR, caracter no valido encontrado---- ";
        abort();
        break;
    case 35:
        cout << " -----ERROR, no se encontr� '=' despu�s de '!'-----";
        abort();
        break;

    }
}

// Avanza a el proximo caracter a leer
void siguienteChar() {
    fin >> noskipws >> caracterActual;
}

// Regresa el indice en la tabla de transiciones dependiendo de si es numero, letra, caracter especial o espacio
void indicarChar() {

    if (esLetra == true) {
        characterType = 0;
    }
    else if (esNum == true) {
        characterType = 1;
    }
    else if (esEspecial == true) {

        if (caracterActual == '/') {
            characterType = 2;
        }

        else if (caracterActual == '*') {
            characterType = 3;
        }

        else if (caracterActual == '+') {
            characterType = 4;
        }

        else if (caracterActual == '-') {
            characterType = 5;
        }

        else if (caracterActual == '<') {
            characterType = 6;
        }

        else if (caracterActual == '=') {
            characterType = 7;
        }

        else if (caracterActual == '>') {
            characterType = 8;
        }

        else if (caracterActual == '!') {
            characterType = 9;
        }

        else if (caracterActual == '(') {
            characterType = 10;
        }

        else if (caracterActual == ')') {
            characterType = 11;
        }

        else if (caracterActual == '[') {
            characterType = 12;
        }

        else if (caracterActual == ']') {
            characterType = 13;
        }

        else if (caracterActual == '{') {
            characterType = 14;
        }

        else if (caracterActual == '}') {
            characterType = 15;
        }

        else if (caracterActual == ';') {
            characterType = 16;
        }

        else if (caracterActual == ',') {
            characterType = 17;
        }
    }
    else if (esEspacio == true) {
        characterType = 18;
    }
    else {
        characterType = 19;
    }

}

// Checa si es una letra
void checkLetra(char characters) {
    for (int i = 0; i <= 51;) {
        if (characters == letraArr[i]) {
            esLetra = true;
            aceptchar = true;
            i = 51;
        }
        i++;
    }
}

// Checa si es un Numero
void checkNum(char characters) {
    if (aceptchar == false) {
        for (int x = 0; x <= 9;) {
            if (characters == numArr[x]) {
                aceptchar = true;
                esNum = true;
                x = 9;
            }
            x++;
        }
    }
}

// Checa si es un caracter especial
void checkSpecial(char characters) {
    if (aceptchar == false) {
        for (int y = 0; y <= 15;) {
            if (characters == caracEspecialesArr[y]) {
                esEspecial = true;
                aceptchar = true;
                y = 17;
            }
            y++;
        }
    }
}

// Checa si es un espacio
void checkEspacios(char characters) {
    if (aceptchar == false) {
        for (int y = 0; y <= 4;) {
            if (characters == espaciosArr[y]) {
                esEspacio = true;
                aceptchar = true;
                y = 3;
            }
            y++;
        }
    }
}

// Se encarga de checar que tipo de caracter es 
void checkCharacter(char characters) {
    if (aceptchar == false) {
        checkLetra(characters);
    }
    if (aceptchar == false) {
        checkNum(characters);
    }

    if (aceptchar == false) {
        checkSpecial(characters);
    }

    if (aceptchar == false) {
        checkEspacios(characters);
    }
}

// Regresa que tipo de caracter es para luego ser usado como index en peekCharType
int getCharType(char characters) {

    if (characters == EOF) {
        return 3;
    }
    else {
        for (int i = 0; i <= 51;) {
            if (characters == letraArr[i]) {
                return 0;
            }
            i++;
        }
        for (int x = 0; x <= 9;) {
            if (characters == numArr[x]) {
                return 1;
            }
            x++;
        }

        for (int y = 0; y <= 15;) {
            if (characters == caracEspecialesArr[y]) {
                return 2;
            }
            y++;
        }

        for (int y = 0; y <= 5;) {
            if (characters == espaciosArr[y]) {
                return 3;
            }
            y++;
        }
    }
}

// Checa el tipo con argumento del siguiente caracter 
int peekCharType(char c) {
    if (getCharType(c) == 0) {
        return 0;
    }
    else if (getCharType(c) == 1) {
        return 1;
    }

    else if (getCharType(c) == 2) {

        if (c == '/') {
            return 2;
        }

        else if (c == '*') {
            return 3;
        }

        else if (c == '+') {
            return 4;
        }

        else if (c == '-') {
            return 5;
        }

        else if (c == '<') {
            return 6;
        }

        else if (c == '=') {
            return 7;
        }

        else if (c == '>') {
            return 8;
        }

        else if (c == '!') {
            return 9;
        }

        else if (c == '(') {
            return 10;
        }

        else if (c == ')') {
            return 11;
        }

        else if (c == '[') {
            return 12;
        }

        else if (c == ']') {
            return 13;
        }

        else if (c == '{') {
            return 14;
        }

        else if (c == '}') {
            return 15;
        }

        else if (c == ';') {
            return 16;
        }

        else if (c == ',') {
            return 17;
        }
    }
    else if (getCharType(c) == 3) {
        return 18;
    }
    else {
        return 19;
    }

}

// Resetea las llaves de el caracter 
void restartChar() {
    esLetra = false;
    esNum = false;
    esEspecial = false;
    esEspacio = false;
    aceptchar = false;
}

// Inserta el token, con su id(De ser necesario) en la el vector de output
void mostrarToken(int estado) {
    if (compararToken(estado) == 0) {
        //Ignorar comentario
    }
    else if(compararToken(estado) > 0 && compararToken(estado) < 28) {

        vecOutput.insertar(vecOutput.getActualPos(), to_string(compararToken(estado)));
    }
    else if (compararToken(estado) == 28) {
        bool existe = false;
        int x;
        for (x = 0; x < vecLetra.getActualPos();) {
            if (identifier == vecLetra.getIdent(x)) {
                existe = true;
                break;
            }
            x++;
        }
        if (existe) {
            vecOutput.insertar(vecOutput.getActualPos(), to_string(compararToken(estado)) + ',' + to_string(x));
        }
        else {
            int index = vecLetra.getActualPos();
            vecLetra.insertar(vecLetra.getActualPos(), identifier);
            vecOutput.insertar(vecOutput.getActualPos(), to_string(compararToken(estado)) + ',' + to_string(index));
        }
    }
    else if (compararToken(estado) == 29) {
        bool existe = false;
        int x;
        for (x = 0; x < vecNum.getActualPos();) {
            if (identifier == to_string(vecNum.getIdent(x))) {
                existe = true;
                break;
            }
            x++;
        }
        if (existe) {
           vecOutput.insertar(vecOutput.getActualPos(), to_string(compararToken(estado)) +  ',' + to_string(x));
        }
        else {
           int index = vecNum.getActualPos();
           vecNum.insertar(vecNum.getActualPos(), stoi(identifier));
           vecOutput.insertar(vecOutput.getActualPos(), to_string(compararToken(estado)) + ',' + to_string(index));
        }
    }
}

// Algoritmo del DFA, el cual facilita el manejo del caracter actual
void estadoRecorrido() {
    int estado = 0;
    siguienteChar();

    // Si el valor es un espacio en blanco re regres, de no ser as� empuja el caracter actual en el buffer
    if ((caracterActual == ' ' || caracterActual == '\n' || caracterActual == '\t')) {
        return;
    }
    else {
        identifier.push_back(caracterActual);
    }

    while (!aceptarEstados[estado] && !errorEstados[estado]) {
        // Empieza con nuevo estado
        restartChar(); // Reinicia las vars booleanas del tipo de caracter
        checkCharacter(caracterActual);  //Checa si e caracter es letra,num,especial o espacio
        indicarChar(); // Obtiene characterType (Index de tabla de transiciones)
        int nuevoEstado = tablaTrans[estado][characterType];

        if (avanEstados[estado][characterType]) {

            // getCharType regresa : 0 letra, 1 num, 2 especial, 3 espacio
            // Si es letra Y el caracter de enfrente no es un num Y el siguiente estado es aceptor
            if (getCharType(caracterActual) == 0 && !(getCharType(fin.peek()) != 1 && aceptarEstados[tablaTrans[nuevoEstado][peekCharType(fin.peek())]])) {
                    siguienteChar();
                    identifier.push_back(caracterActual); 
            }
            // Si es un num Y el de enfrente no es letra Y el siguiente estado es aceptor
            else if (getCharType(caracterActual) == 1 && !(getCharType(fin.peek()) != 0 && aceptarEstados[tablaTrans[nuevoEstado][peekCharType(fin.peek())]])) {
                siguienteChar();
                identifier.push_back(caracterActual);
            }
            // Si los estados son comentarios sigue empujandolos al buffer
            else if (estado == 4 || estado == 5) {
                siguienteChar();
                identifier.push_back(caracterActual);
            }
            // peekCharType especifica el caracter especial
            // Si el caracter actual es Espacio en blanco o el caracter de enfrente es aceptor
            else if ( aceptarEstados[tablaTrans[nuevoEstado][peekCharType(fin.peek())]]) {
                if (caracterActual == EOF) {
                    return;
                }
                else {
                    //identifier.push_back(characters);
                    nuevoEstado = tablaTrans[nuevoEstado][peekCharType(fin.peek())];
                }
            }
            // Para espacios en blanco
            else if (getCharType(caracterActual) == 3) {
                identifier = "";
                siguienteChar();
                identifier.push_back(caracterActual);
            }

            else {
                if (fin.peek() == EOF) {
                    mostrarToken(tablaTrans[nuevoEstado][peekCharType(fin.peek())]);
                    return;
                }
                else {
                    caracterActual = fin.peek();
                }
            }

        }
        //cout << "ESTADO: " << nuevoEstado << "\n";
        estado = nuevoEstado;
    }


    // Aqui recolecta e imprime el token
    if (aceptarEstados[estado]) {

        if ((caracterActual == '<' && fin.peek() == '=') || (caracterActual == '=' && fin.peek() == '=') || (caracterActual == '>' && fin.peek() == '=') || (caracterActual == '!' && fin.peek() == '=')) {
            caracterActual = fin.peek();
            identifier.push_back(caracterActual);
            mostrarToken(estado);
            identifier = "";
            siguienteChar();
        }
        else {
            mostrarToken(estado);
            identifier = "";
        }
    }
    else {
        mostrarToken(estado);
    }

}

// Recorre estados hasta que se termina el archivo
void recorrerEstados() {
    while (fin.peek() != EOF) {
        estadoRecorrido();
    }
}

// Recorre estados y luego imprime el vectorOutput para obtener los resultados
int main()
{
    recorrerEstados();
    for (int x = 0; x < vecOutput.getActualPos();) {
        cout <<"<" + vecOutput.getIdent(x) + ">"<< endl;
        x++;
    }
}