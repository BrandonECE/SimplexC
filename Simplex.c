#include <stdio.h>
#include <conio.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <string.h>
#include <math.h>

// Función para leer enteros con validación
int leerEntero(const char* mensaje, int min, int max) {
    int valor;
    char buffer[100];
    
    do {
        system("cls");
        printf("%s", mensaje);
        
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Eliminar salto de línea
            buffer[strcspn(buffer, "\n")] = 0;
            
            // Intentar convertir a número
            char* endptr;
            valor = strtol(buffer, &endptr, 10);
            
            // Verificar si la conversión fue exitosa
            if (endptr != buffer && *endptr == '\0') {
                // Verificar rango
                if (valor >= min && valor <= max) {
                    return valor;
                } else {
                    printf("Error: El valor debe estar entre %d y %d.\n", min, max);
                }
            } else {
                printf("Error: '%s' no es un número válido.\n", buffer);
            }
        }
        printf("Presione Enter para continuar...");
        getchar();
    } while (1);
}

// Función para leer doubles con validación
double leerDouble(const char* mensaje) {
    double valor;
    char buffer[100];
    
    do {
        system("cls");
        printf("%s", mensaje);
        
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Eliminar salto de línea
            buffer[strcspn(buffer, "\n")] = 0;
            
            // Intentar convertir a número
            char* endptr;
            valor = strtod(buffer, &endptr);
            
            // Verificar si la conversión fue exitosa
            if (endptr != buffer && *endptr == '\0') {
                return valor;
            } else {
                printf("Error: '%s' no es un número válido.\n", buffer);
            }
        }
        printf("Presione Enter para continuar...");
        getchar();
    } while (1);
}

int main() {
    // DECLARACION DE LAS PRIMERAS VARIABLES
    int maxormin;
    int nrestricciones;
    int nvariablesx;

    //----------------------------------------------------------//
    //-------------------//

    // NUMERO DE VARIABLES DE DECISION (VALIDADO)
    nvariablesx = leerEntero("Ingrese la cantidad de variables de decision que desea: ", 1, 100);

    double Arrayfo[nvariablesx];
    double impresion_original_Arrayfo[nvariablesx];

    //----------------------------------------------------------//
    //-------------------//

    // ENTRADA DE DATOS DE LA FUNCION OBJETIVO (VALIDADO)
    for (int i = 0; i < nvariablesx; i++) {
        char mensaje[100];
        sprintf(mensaje, "Ingresa x%d de Funcion Objetivo (F.O): ", i + 1);
        Arrayfo[i] = leerDouble(mensaje);
        impresion_original_Arrayfo[i] = Arrayfo[i];
    }

    // MAXIMIZAR O MINIMIZAR (VALIDADO)
    maxormin = leerEntero("Ingrese para su Funcion Objetivo(F.O):\n\nPresiona 1 si deseas MINIMIZAR\nPresiona 2 si deseas MAXIMIZAR\n\n: ", 1, 2);

    //----------------------------------------------------------//
    //-------------------//

    // ENTRADA DE DATOS DE LAS RESTRICCIONES FISICAS (VALIDADO)
    nrestricciones = leerEntero("Ingrese cuantas restricciones fisica desea: ", 1, 100);

    double Arrayrestricciones[nrestricciones][nvariablesx + 2];
    int base[nrestricciones];

    for (int i = 0; i < nrestricciones; i++) {
        for (int j = 0; j < nvariablesx; j++) {
            char mensaje[100];
            sprintf(mensaje, "Ingresa x%d de la restriccion N.%d: ", j + 1, i + 1);
            Arrayrestricciones[i][j] = leerDouble(mensaje);
        }

        // TIPO DE LIMITANTE (VALIDADO)
        Arrayrestricciones[i][nvariablesx] = leerEntero(
            "Ingrese que tipo de limitante es:\n\nPresiona 1 si tu limitante es: <=\nPresiona 2 si tu limitante es: >=\nPresiona 3 si tu limitante es: =\n\n: ", 
            1, 3
        );

        base[i] = Arrayrestricciones[i][nvariablesx];

        // VALOR LIMITANTE (VALIDADO)
        char mensaje[100];
        sprintf(mensaje, "Ingresa el valor limitante de la restriccion N.%d: ", i + 1);
        Arrayrestricciones[i][nvariablesx + 1] = leerDouble(mensaje);
    }

    //----------------------------------------------------------//
    //-------------------//

    // ENTRADA DE DATOS DE LAS RESTRICCIONES LOGICAS (VALIDADO)
    int decision_restriccion_logica;
    
    decision_restriccion_logica = leerEntero(
        "Eliga sus restricciones logicas:\n\n"
        "Desea que todas sus variables sean >= 0 ?\n"
        "\nPresiona 1 --> SI\nPresiona 2 --> NO\n\n: ", 
        1, 2
    );

    int cont_nvariablex_sin_restriccion_logica = 0;
    int nvariablex_restriccion_logica[nvariablesx];

    if (decision_restriccion_logica == 1) {
        for (int i = 0; i < nvariablesx; i++) { 
            nvariablex_restriccion_logica[i] = 1;
        }
    } else {
        for (int i = 0; i < nvariablesx; i++) {
            char mensaje[100];
            sprintf(mensaje, 
                "Eliga sus restricciones logicas:\n\n"
                "[ x%d ] >= 0 ?\n"
                "\nPresiona 1 --> CON RESTRICCION\n"
                "Presiona 2 --> SIN RESTRICCION\n\n: ", 
                i + 1
            );
            nvariablex_restriccion_logica[i] = leerEntero(mensaje, 1, 2);
            
            if (nvariablex_restriccion_logica[i] == 2) {
                cont_nvariablex_sin_restriccion_logica++;
            }
        }
    }





    // IMPRESION DE DATOS

    //VARIABLES QUE AYUDARAN A SABER LA POSICION DE LAS COLUMNAS DE LAS VARIABLES X VIPRMIA
    double position_nvariablex_restriccion_logica[nvariablesx+cont_nvariablex_sin_restriccion_logica];
    int cont_vi_indice_position = 0;
    bool cont_vi_indice_position_bool = false;
    //--------------------------------------------------//

    // FUNCION OBJETIVO

    system("cls");
    printf("\x1B[33m"); // #COLOR-VISUAL
    printf(":========DATOS========: \n\n");
    printf("\x1B[37m"); // #COLOR-VISUAL
    printf("Funcion Objetivo(F.O): \n\n");

    if (maxormin == 1) printf("- min z = ");   
    else printf("- max z = ");

    for (int i = 0; i < nvariablesx; i++)
    {
        printf("(%.2f)x%d", Arrayfo[i], i + 1);//IMPRESION DE LAS X NORMALES
        if (i < nvariablesx - 1)printf(" + ");

        //ALGORITMO PARA SABER LAS COLUMNAS DE LAS VARIABLES VIPRIMAS EN CASO DE QUE HAYA
        if (cont_vi_indice_position_bool == true)++cont_vi_indice_position;//PARTE DEL ALGORITMO
        if (cont_vi_indice_position_bool == false && i != 0)++cont_vi_indice_position;//PARTE DEL ALGORITMO
            position_nvariablex_restriccion_logica[cont_vi_indice_position] = 1;//PARTE DEL ALGORITMO
            cont_vi_indice_position_bool = false;//PARTE DEL ALGORITMO

        //ALGORITMO PARA SABER LAS COLUMNAS DE LAS VARIABLES VIPRIMAS EN CASO DE QUE HAYA
        if (nvariablex_restriccion_logica[i] == 2){// EN EL CASO DE QUE EL VALOR SEA 2 ENTONCES SIGNIFICARA QUE HAY VIPRIMAS Y TENDREMOS QUE IMPRIMIRLAS DESPUES DE LA VARIABLE ORIGINAL
            
            if (i == nvariablesx - 1)printf(" + ");
            printf("(%.2f)X%d_vi", Arrayfo[i] * -1, i + 1);// IMPRESION DE LAS X VIPRIMAS
            if (i < nvariablesx - 1)printf(" + ");

            cont_vi_indice_position_bool = true;//PARTE DEL ALGORITMO
            ++cont_vi_indice_position;//PARTE DEL ALGORITMO
            position_nvariablex_restriccion_logica[cont_vi_indice_position] = 2;//PARTE DEL ALGORITMO

         }
        
    }

    printf("\n\n");



    //-------------------//

    // system("PAUSE");

    // RESTRICCIONES FISICAS

    printf("Restricciones Fisicas: \n\n");

    for (int i = 0; i < nrestricciones; i++)
    {

        printf("- Restriccion N.%d: ", i + 1);
        for (int j = 0; j < nvariablesx; j++)
        {
            printf("(%.2f)x%d", Arrayrestricciones[i][j], j + 1);
            if (j < nvariablesx - 1) printf(" + ");

            if (nvariablex_restriccion_logica[j] == 2){
                if (j == nvariablesx - 1)printf(" + ");
                printf("(%.2f)X%d_vi", (Arrayrestricciones[i][j] * -1), j + 1);
                if (j < nvariablesx - 1)printf(" + ");
            }

            
            if (j == nvariablesx - 1 && Arrayrestricciones[i][nvariablesx] == 1) printf(" <= ");
            if (j == nvariablesx - 1 && Arrayrestricciones[i][nvariablesx] == 2)  printf(" >= ");
            if (j == nvariablesx - 1 && Arrayrestricciones[i][nvariablesx] == 3)  printf(" = ");
        
        }
        printf("%.2f", Arrayrestricciones[i][nvariablesx + 1]);
        printf("\n");
    }

    // RESTRICCION LOGICA

    printf("\nRestricciones logicas: \n\n");
    printf("- Restriccion: ");
    
    int cont_comas_rl = nvariablesx - cont_nvariablex_sin_restriccion_logica;//VARIABLE MERAMENTE PARA EL ACOMODO VISUAL (NO AFECTA EL ALGORITMO DE DATOS, SOLO DE IMPRESION)

    for (int i = 0; i < nvariablesx; i++){

        if (nvariablex_restriccion_logica[i] == 1)
        {
             --cont_comas_rl;
            printf("x%d", i+1);
            if (cont_comas_rl != 0) printf(", ");
        }
        
    }
    printf(" >= 0");


    //----------------------------------------------------------//
    //-------------------//

    //----------------------------------------------------------//
    //-------------------//




    //===ARRAY BIDIMENSIONAL SIMPLEX===//

    // FILAS          //COLUMNAS
    double Arraysimplex[1 + nrestricciones][1 + cont_nvariablex_sin_restriccion_logica + nvariablesx + nrestricciones + 1]; // FILAS [1 Contando la F.O + Restricciones]
                                                                                // COLUMNAS [1 Contando Z + N. de variables x (x1,x2,n..) + N. de variables S,-E, que son equivalentes al N. de Restricciones + 1 de valor b]

    // PROCESO / INTRODUCIR DATOS:

    // NUEVAS VARIABLES EN USO

    int ajustadorse = 0;
    int ajustadorx = 0;
    int n = nvariablesx + cont_nvariablex_sin_restriccion_logica;
    int cont = 1;

    //----------------------------------------------------------//
    //-------------------//



    // AUTOAJUSTADOR DE VARIABLES X - PERMITE AJUSTAR EN SU LUGAR CORRESPONDIENTE LOS VALORES EN LA TABLA SIMPLEX DEPENDIENDO DEL N. DE VARIABLES X - LA VARIABLE RESULTANTE "AJUSTADORX" SE USARA EN SIGUIENTES PROCESOS

    if (n > 2)
    {

        do
        {
            --n;
            ++ajustadorx;
        } while (n != 2);
    }
    else if (n == 1)
        ajustadorx = -1;

    //----------------------------------------------------------//
    //-------------------//




    n = nrestricciones; // Redifinir el valor de la variable n

    // AUTOAJUSTADOR DE VARIABLES DE HOLGURA Y EXCESO- PERMITE AJUSTAR EN SU LUGAR CORRESPONDIENTE LOS VALORES EN LA TABLA SIMPLEX DEPENDIENDO DEL N. DE RESTRICCIONES - LA VARIABLE RESULTANTE "AJUSTADORSE" SE USARA EN SIGUIENTES PROCESOS

    if (n > 2)
    {

        do
        {
            --n;
            ++ajustadorse;
        } while (n != 2);
    }
    else if (n == 1)
        ajustadorse = -1;

    //----------------------------------------------------------//
    //-------------------//









    //AJUSTE DEPENDIENDO DE LA FUNCION OBJETIVO Y SI ES MAXIMIZAR O MINIMIZAR

    int cont_ajuste_fo_signo_minimizar = 0;

    if (maxormin == 1)
    {
        for (int i = 0; i < nvariablesx; i++)
        {
            if (Arrayfo[i] == 0);
            else Arrayfo[i] = Arrayfo[i] * -1;
        }
    }






    // DECLARACION DE VARIABLES CON LOS TAMAÑOS DE LA MATRIZ

    int simplex_size_fila = 1 + nrestricciones;
    int simplex_size_columna = 1 + cont_nvariablex_sin_restriccion_logica + nvariablesx + nrestricciones + 1;

    int cont_ajuste_prueba = 0;//PRUEBA

    // GENERADOR DE LA TABLA SIMPLEX CON SUS VALORES POSICIONADOS
    // F.O + Restricciones
    for (int i = 0; i < simplex_size_fila; i++)
    { // FILAS
        cont_ajuste_prueba = 0;
        for (int j = 0; j < simplex_size_columna; j++)
        { // COLUMNAS

            if (i == 0)
            { // PRIMERA FILA [FUNCION OBJETIVO]

                if (i == 0 && j == 0)
                    Arraysimplex[i][j] = 1; // PRIMERA COLUMNA (Z)

                else if (i == 0 && j <= nvariablesx + cont_nvariablex_sin_restriccion_logica){

                    if (Arrayfo[(j - 1) - cont_ajuste_prueba] == 0) Arraysimplex[i][j] = Arrayfo[(j - 1) - cont_ajuste_prueba]; // EN CASO DE QUE SE CONTENGAN 0 EN LA F.O
                    else Arraysimplex[i][j] = (Arrayfo[(j - 1) - cont_ajuste_prueba]) * -1; // COLUMNA VARIABLES X

                    //PRUEBA
                    if (nvariablex_restriccion_logica[(j - 1) - cont_ajuste_prueba] == 2)//EN CASO DE QUE HAYA VARIABLES SIN RESTRICCION LOGICA
                    {
                        ++j;
                        ++cont_ajuste_prueba;

                        if (Arrayfo[(j - 1) - cont_ajuste_prueba] == 0) Arraysimplex[i][j] = Arrayfo[(j - 1) - cont_ajuste_prueba];
                        else Arraysimplex[i][j] = ((Arrayfo[(j - 1) - cont_ajuste_prueba]) * -1) * -1;
                        
    
                    }
                    //

                    
                }
                else
                    Arraysimplex[i][j] = 0; // COLUMNAS SOBRANTES EN 0 YA QUE ES DE LA F.O
            }

            else
            { // SIGUIENTES FILAS [DEPENDERA DEL NUMERO DE RESTRICCIONES]

                if (j == 0)
                    Arraysimplex[i][j] = 0; // PRIMERA COLUMNA (Z)

                else if (j <= (nvariablesx + cont_nvariablex_sin_restriccion_logica)){

                    Arraysimplex[i][j] = Arrayrestricciones[i - 1][(j - 1) - cont_ajuste_prueba]; // COLUMNA VARIABLES X

                    if (nvariablex_restriccion_logica[(j - 1) - cont_ajuste_prueba] == 2)//EN CASO DE QUE HAYA VARIABLES SIN RESTRICCION LOGICA
                    {
                        ++j;
                        ++cont_ajuste_prueba;

                        if (Arrayrestricciones[i - 1][(j - 1) - cont_ajuste_prueba] == 0) Arraysimplex[i][j] = Arrayrestricciones[i - 1][(j - 1) - cont_ajuste_prueba];
                        else Arraysimplex[i][j] = (Arrayrestricciones[i - 1][(j - 1) - cont_ajuste_prueba]) * -1;
                        
    
                    }

                    
                }

                else if (j > nvariablesx + cont_nvariablex_sin_restriccion_logica && j <= (nvariablesx + nrestricciones + cont_nvariablex_sin_restriccion_logica)){ // COLUMNAS VARIANTES DE HOLGURA Y EXCESO

                    if (i == cont && j == (ajustadorx + (nrestricciones - ajustadorse)) + cont){ //

                        if (Arrayrestricciones[i - 1][nvariablesx] == 1) Arraysimplex[i][j] = 1;
                        else if (Arrayrestricciones[i - 1][nvariablesx] == 2) Arraysimplex[i][j] = -1;
                        else if (Arrayrestricciones[i - 1][nvariablesx] == 3) Arraysimplex[i][j] = 1;//CORREGIR!!!

                        ++cont;
                    }

                    else
                        Arraysimplex[i][j] = 0;
                }

                else
                    Arraysimplex[i][j] = Arrayrestricciones[i - 1][nvariablesx + 1]; // ULTIMA COLUMNA
            }
        }
    }

    //----------------------------------------------------------//
    //-------------------//





    //SE AÑADIRAN EN LA F.O LAS ARITIFICALES POR DEFECTO POR INTRODUCIR COMO LIMITANTE "=", CON VALOR DE 100 


    int valor_artificial = 100;

    for (int i = 0; i < simplex_size_fila; i++)
    {
        for (int j = 0; j < simplex_size_columna; j++)
        {
            if (i == 0 && j >= 1 + nvariablesx + cont_nvariablex_sin_restriccion_logica && j < simplex_size_columna)
            {
                if (base[j - (1 + nvariablesx + cont_nvariablex_sin_restriccion_logica)] == 3)
                {
                    Arraysimplex[i][j] = valor_artificial;
                }
                
            }
            
        }   
    }
    




    
    //SE DETERMINARA SI EXISTE VARIABLES CON VALORES NEGATIVOS DESPUES DE LA LIMITANTE




    
    int exist_valores_negativos[nrestricciones];//ESTE ARRAY NOS PERMITIRA SABER LAS FILAS O VARIABLES QUE SON NEGATIVAS

   for (int i = 0; i < nrestricciones; i++)
    {
        for (int j = 0; j < (nvariablesx+2); j++)
        {
            if (j == (nvariablesx+2)-1)
            {
                if (Arrayrestricciones[i][j] < 0)exist_valores_negativos[i] = 1;
                else exist_valores_negativos[i] = 0;
            }
            
        }   
    }








    bool is_artificial_negativa = false;

    for (int i = 0; i < simplex_size_fila; i++)//ESTO NOS PERMITIRA CAMBIAR AL SIGNO CONTRARIO A TODA LA FILA PARA HACER POSITIVO EL VALOR QUE ANTERIORMENTE ERA NEGATIVO
    { // FILAS

        for (int j = 0; j < simplex_size_columna; j++)//COLUMNAS
        {
            if (i != 0)
            {
                if (exist_valores_negativos[i-1] == 1)
                {
                    if (base[i-1] == 3)//ANALIZAREMOS LA RESTRICCION QUE TENGA "=" APARTE
                    {
                        if (j >= (1 + nvariablesx) && j < simplex_size_columna-1)
                        {
                            if (Arraysimplex[i][j] == 1)//SE BUSCA EL 1 PARA CAMBIARLO A -1
                            {
                                Arraysimplex[i][j] = Arraysimplex[i][j]*-1;
                                is_artificial_negativa = true;
                            }
                            
                        }
                    }
                    else{
                        if (Arraysimplex[i][j] != 0) Arraysimplex[i][j] = Arraysimplex[i][j]*-1;
                        else Arraysimplex[i][j] = 0;
                    }
                
                }
            }
            
        }
    }


    if (is_artificial_negativa == true)//SI HAY UN CASO EN QUE UNA RESTRICCION TENGA "=" ENTONCES SE CAMBIARAN LOS SIGNOS AL REVES
    {//DE ESTA MANERA TODO LO DEMAS CAMBIARA Y EL -1 ANTERIOR SE HARA 1 NUEVAMENTE, DEJANDO INTACTA LA ARTIFICIAL
    for (int i = 0; i < simplex_size_fila; i++)
        { // FILAS

            for (int j = 0; j < simplex_size_columna; j++)//COLUMNAS
            {
                if (i != 0)
                {
                    if (exist_valores_negativos[i-1] == 1)
                    {
                        if (base[i-1] == 3)
                        {
                            if (Arraysimplex[i][j] != 0) Arraysimplex[i][j] = Arraysimplex[i][j]*-1;
                            else Arraysimplex[i][j] = 0;
                        }
                    
                    }
                }
                
            }
        }

    }



    

    //----------------------------------------------------------//
    //-------------------//




    //FORMA DE MATRIZ Y LA BASE

    printf("\n\n");
    printf("\x1B[33m"); // #COLOR-VISUAL
    printf(":========BASE========: \n\n");
    printf("\x1B[37m"); // #COLOR-VISUAL
    printf("\x1B[32m");
    printf(" - COLUMNA DE HOLGURA  \n");
    // printf("\x1B[35m");
    // printf(" - COLUMNA DE HOLGURA NEGATIVA  \n");
    printf("\x1B[31m");
    printf(" - COLUMNA DE EXCESO   \n");
    printf("\x1B[36m");
    printf(" - COLUMNA DE ARTIFICIAL\n");
    printf("\x1B[35m");
    printf(" - COLUMNA DE X's ViPrimas\n");
    printf("\x1B[33m");
    printf(" - COLUMNA DE ARITIFICIALES AUTOMATICAS\n\n");
    printf("\x1B[0m");


    printf("\x1b[32m");

    int basearray[nrestricciones][nrestricciones];

    for (int i = 0; i < nrestricciones+1; i++)
    {
        for (int j = 0; j < nrestricciones+1; j++)
        {
            if (i == j) {
                basearray[i][j] = 1;
                printf("  %d",basearray[i][j]);
            }
            else{
                basearray[i][j] = 0;
                printf("  %d",basearray[i][j]);
            }
        }
        printf("\n");
        
    }
    printf("\x1b[0m");

    printf("\n");
    printf("  |||\n");
    printf("  vvv\n");
    printf("\n");

    for (int i = 0; i < simplex_size_fila; i++){
        // int cont_vi_position = 0;
        // int cont_vi = 0;
        // int cont_espace = 1;
        for (int j = 0; j < simplex_size_columna; j++){

            if (j == 0){
                printf("\x1b[32m");
                printf("  %.2f", Arraysimplex[i][j]);  
                printf("\x1b[0m");
            }

            else if(j > nvariablesx + cont_nvariablex_sin_restriccion_logica && j < simplex_size_columna-1){
                if (base[j-(nvariablesx + cont_nvariablex_sin_restriccion_logica + 1 )] ==  1){//EN LA BASE SE MARCARA CON VERDE LAS VARIABLES QUE NO SON DE EXCESO
                    printf("\x1b[32m");
                    if (Arraysimplex[i][j]<0)printf(" %.2f", Arraysimplex[i][j]);
                    else printf("  %.2f", Arraysimplex[i][j]);  
                    printf("\x1b[0m");
                }
                else if(base[j-(nvariablesx + cont_nvariablex_sin_restriccion_logica + 1)] ==  3){
                    printf("\x1B[36m");
                    if (Arraysimplex[i][j]<0)printf(" %.2f", Arraysimplex[i][j]);
                    else printf("  %.2f", Arraysimplex[i][j]);  
                    printf("\x1b[0m");                   
                }
                else{//SE MARCAN LAS VARIABLES QUE SON DE EXCESO
                    printf("\x1b[31m");
                    if (Arraysimplex[i][j]<0)printf(" %.2f", Arraysimplex[i][j]);
                    else printf("  %.2f", Arraysimplex[i][j]);  
                    printf("\x1b[0m");
                }
                
            }
            
            else{

                 if (j != simplex_size_columna-1)if (position_nvariablex_restriccion_logica[j-1] == 2)printf("\x1b[35m");
                if (Arraysimplex[i][j]<0 || Arraysimplex[i][j]>=10)printf(" %.2f", Arraysimplex[i][j]);
                else if(Arraysimplex[i][j]>=100) printf("%.2f", Arraysimplex[i][j]);
                else printf("  %.2f", Arraysimplex[i][j]); 
                printf("\x1b[0m");
            
            }

 
        }
        printf("\n");
    }

    printf("\n");
    printf("  |||\n");
    printf("  vvv\n");
    printf("\n");

    for (int i = 0; i < nrestricciones+1; i++)
    {
        for (int j = 0; j < nrestricciones+1; j++)
        {
            if (j == 0){
                printf("\x1b[32m");
                printf("  %d", basearray[i][j]);  
                printf("\x1b[0m");
            }
            else{
                if (base[j-1] ==  1){
                    printf("\x1b[32m");
                    printf("  %d", basearray[i][j]);  
                    printf("\x1b[0m");
                }
                else if(base[j-1] == 3){
                    printf("\x1B[36m");
                    printf("  %d", basearray[i][j]);  
                    printf("\x1b[0m");                   
                }
                else{
                    printf("\x1b[31m");
                    printf("  %d", basearray[i][j]);  
                    printf("\x1b[0m");
                }
                
            }
            
        }
        printf("\n");
        
    }



    //----------------------------------------------------------//
    //-------------------//



       for (int i = 0; i < nrestricciones; i++)//ESTE ARRAY NOS PERMITIRA SABER LAS FILAS O VARIABLES QUE SON NEGATIVAS
    {
        for (int j = 0; j < (nvariablesx+2); j++)
        {
            if (j == (nvariablesx+2)-1)
            {
                if (Arrayrestricciones[i][j] < 0)exist_valores_negativos[i] = 1;
                else exist_valores_negativos[i] = 0;
            }
            
        }   
    }




    // NUEVO SIMPLEX SI ES QUE EXISTEN VARIABLES ARTIFICIALES O NO



    int cont_var_artifical = 0;//CONTADOR DE VARIABLES ARTIFICIALES
    bool exist_artificial = false; //FALSE = No hay variables artificiales
                                //TRUE = Si hay variables artificiales


    for (int i = 0; i < nrestricciones; i++)//SE ANALIZA LOS CASOS QUE SE PRESENTAN PARA SABER SI HABRA QUE AÑADIR ARTIFICALES
    {   
        // printf("\n%d - BASE ES %d y EXISTE_VAL_NEG: %d\n", i, base[i], exist_valores_negativos[i]);
        if (base[i] == 2 && exist_valores_negativos[i] == 0 || base[i] == 1 && exist_valores_negativos[i] == 1)//SOLO SE EVALUA CUANDO ES DE HOLGURA Y EXCESO YA QUE EN EL CASO DE QUE SEA "=" SE AGREGARA UNA VARIABLE ARTIFICIAL Y AUNQUE SEA NEGATIVO EL VALOR DEL LIMITE SOLO AFECTARA A LAS OTRAS VARIABLES
        {
            ++cont_var_artifical;
            exist_artificial = true;
            // printf("\n%dSI ES\n", i);
        }
        
    }



    int simplex_size_columna_forma__estandar = simplex_size_columna;
                         //z  x1...xn       S1,E1....       A1,A2....           b
    // simplex_size_columna = simplex_size_columna + cont_var_artifical;
    double ArraysimplexOfficial[simplex_size_fila][simplex_size_columna + cont_var_artifical];




    if (cont_var_artifical == 0)// SE EVALUARA SI DE VERDAD HUBO VARIABLES ARTIFICIALES, SI ES IGUAL A 0 ENTONCES NO HAY NINGUNA Y EL ARRAY SE PASA TAL CUAL AL ARRAYSIMPLEXOFFICIAL
    {   
        for (int i = 0; i < simplex_size_fila; i++)//SE PASA ORIGINALMENTE
            {
                for (int j = 0; j < simplex_size_columna; j++)
                {
                    ArraysimplexOfficial[i][j] = Arraysimplex[i][j];
                }
                
            }
        
    }
    else{// DE LO CONTRARIO HARA UN PROCESO PARA EXTENDER EL SIMPLEX Y METER LAS NUEVAS VARIABLES ARTIFICIALES EN EL ARRAYSIMPLEXOFICIAL
           
            int columan_artificial = nvariablesx+nrestricciones + cont_nvariablex_sin_restriccion_logica; //SE DECLARA UNA VARIABLE CON VALOR SUMANDO LA CANTIDAD DE LAS VARIABLES X Y LAS RESTRICCIONES CON EL PROPOSITO DE RECORRER LAS VARIABLES ARITIFICIALES EN SU ESPACIO QUE CORRESPONDE 
        
            for (int i = 0; i < simplex_size_fila; i++)//FILA
            {
                bool find = false;//NOS SERVIRA PARA ENCONTRAR EN QUE FILA EN LOS APARTADOS DE LAS RESTRICCIONES ES -1 PARA ASIGNARLO EN LA MISMA FILA EN EL APARTADO DE LAS VARIABLES ARTIFICIALES
                //FALSE NO EXISTE -1
                //TRUE EXISTE -1
                int change;//VARIABLE PARA GUARDAR EL VALOR DE b
                for (int j = 0; j < (simplex_size_columna + cont_var_artifical); j++)//COLUMNA
                {


                    if (j >= (1+nvariablesx+ cont_nvariablex_sin_restriccion_logica + nrestricciones) && j < (simplex_size_columna + cont_var_artifical-1)){//SE TRABAJA SOLO CON EL APARTADO NUEVO, ES DECIR CON LOS ESPACIOS DE LAS ARTIFICIALES EN DELANTE

                        if (j == 1+nvariablesx+ cont_nvariablex_sin_restriccion_logica + nrestricciones) change = Arraysimplex[i][j];//J LLEGA A LO QUE SERIA LA COLUMNA DE b EN EL ARRAY ORIGINAL, PARA GUARDAR EL VALOR DE b EN CHANGE Y COLOCARLO EN EL NUEVO ARRAYSIMPLESOFFICIAL EN LA ULTIMA COLUMNA

                        if (i == 0) {//SE AÑADE EL VALOR DE LA VARIABLE ARTIFICIAL EN LA FILA DE LA F.O
                            ArraysimplexOfficial[i][j] = valor_artificial;
                        }

                        else {// SE RELLENAN LAS VARIABLES ARTIFICIALES EN LAS DEMAS FILAS COMO CORRESPONDE
 
                            if (find == true && j == columan_artificial){// SE AÑADIRAN LOS 1 NECESARIOS PARA TENER LA BASE
                                ArraysimplexOfficial[i][j] = 1; 
                                find = false;
                            } 
                            else  ArraysimplexOfficial[i][j] = 0;//SE RELLENARAN CON 0 LOS ESPACIOS NO NECESARIOS

                        }

                    }


                    else if(j == simplex_size_columna + cont_var_artifical-1){//ULTIMA COLUMNA
                        ArraysimplexOfficial[i][j] = change;//AGREGAR EL VALOR b
                    }


                    else {//LLENAR LOS APARTADOS YA ESTABLECIDOS  DEL ARRAYSIMPLEX EN EL ARRAYSIMPLEXOFICIAL

                        ArraysimplexOfficial[i][j] = Arraysimplex[i][j];

                        if (i > 0 && j >= 1+nvariablesx + cont_nvariablex_sin_restriccion_logica && j < 1+ cont_nvariablex_sin_restriccion_logica + nvariablesx + nrestricciones)//BUSCAR SI EN EL APARTADO DE LAS RESTRICCIONES EXISTE ALGUN -1 EN LA FILA CON LA QUE ESTE EL PROCESO
                        {
                            if (Arraysimplex[i][j] == -1)
                            {   
                               ++columan_artificial;//SE INCREMENTA PARA RECORRESE
                                find = true;//TRUE EXISTE -1
                            }
    
                        }
                        
                    }
                }
                
            }
            
    }
    
    simplex_size_columna += cont_var_artifical;//SE AÑADE EL NUEVO TAMAÑO AÑADIDO POR LAS VARIABLES ARTIFICIALES EN CASO DE QUE HAYA 

    //----------------------------------------------------------//
    //-------------------//






     //FORMA DE MATRIZ Y LA BASE CON ARTIFICIALES


    if (exist_artificial == true)//SI EXISTEN VARIABLES ARTIFICIALES GRACIAS A LAS VARIABLES DE EXCESO SE IMPRIMIRA LO SIGUIENTE
    {
        
        printf("\n");
        printf("  |||\n");
        printf("  vvv\n");
        printf("\n");


        for (int i = 0; i < simplex_size_fila; i++){
            for (int j = 0; j < simplex_size_columna; j++){

                if (j == 0){
                    printf("\x1b[32m");
                    printf("  %.2f", ArraysimplexOfficial[i][j]);  
                    printf("\x1b[0m");
                }

                else if(j > nvariablesx + cont_nvariablex_sin_restriccion_logica && j < simplex_size_columna-1){


                    if (base[j-(nvariablesx + cont_nvariablex_sin_restriccion_logica +1)] ==  1) printf("\x1b[32m");
                    else if(base[j-(nvariablesx+ cont_nvariablex_sin_restriccion_logica + 1)] ==  3) printf("\x1B[36m");
                    else printf("\x1B[31m");    
                    if (j >= simplex_size_columna-cont_var_artifical-1 && j < simplex_size_columna) printf("\x1B[33m");


                    if (ArraysimplexOfficial[i][j]<0)printf(" %.2f", ArraysimplexOfficial[i][j]);
                    else printf("  %.2f", ArraysimplexOfficial[i][j]);  
                    printf("\x1b[0m");

                    
                }
                
                else{
    
                    if (j != simplex_size_columna-1)if (position_nvariablex_restriccion_logica[j-1] == 2)printf("\x1b[35m");
                 
        
                    if (ArraysimplexOfficial[i][j]<0 || ArraysimplexOfficial[i][j]>=10)printf(" %.2f", ArraysimplexOfficial[i][j]);
                    else if(ArraysimplexOfficial[i][j]>=100) printf("%.2f", ArraysimplexOfficial[i][j]);
                    else printf("  %.2f", ArraysimplexOfficial[i][j]); 
                    printf("\x1b[0m");
                }

    
            }
            printf("\n");
        }




    }
    





    // IMPRESION DE DATOS DE TABLA SIMPLEX A MODO ESTANDAR // ULTIMO USO DEL ARRAYSIMPLEX[]

    printf("\n\n");
    printf("\x1B[33m"); // #COLOR-VISUAL
    printf(":========FORMA ESTANDAR========: \n\n");
    printf("\x1B[37m"); // #COLOR-VISUAL

    cont = 1; // Se reinicia el valor
    int varholguraexceso = 1;
    cont_var_artifical = 0;
    int cont_estandar_x = 1;

    for (int i = 0; i < simplex_size_fila; i++){ // FILAS
        cont_estandar_x = 1;
        for (int j = 0; j < simplex_size_columna_forma__estandar; j++){ // COLUMNAS

            if (i == 0 && j >= 1 && j <= nvariablesx+cont_nvariablex_sin_restriccion_logica){ // FUNCION OBJETIVO

                if (j == 1){
                    if (maxormin == 1) printf("   min z   ");
                    else printf("   max z   ");
                }


                if (position_nvariablex_restriccion_logica[j-1] == 1)
                {
                        printf("(%.2f)x%d", Arraysimplex[i][j], cont_estandar_x);
                        if(j != nvariablesx +cont_nvariablex_sin_restriccion_logica&& Arraysimplex[i][j]<0) printf("  + ");
                        if(j != nvariablesx +cont_nvariablex_sin_restriccion_logica&& Arraysimplex[i][j]>0) printf("  +  ");
                        if(j != nvariablesx +cont_nvariablex_sin_restriccion_logica&& Arraysimplex[i][j] == 0) printf("  +  ");
                        ++cont_estandar_x;
                }
                else{
                        --cont_estandar_x;
                        printf("(%.2f)X%d_vi", Arraysimplex[i][j], cont_estandar_x);
                        if(j != nvariablesx +cont_nvariablex_sin_restriccion_logica&& Arraysimplex[i][j]<0) printf("  + ");
                        if(j != nvariablesx +cont_nvariablex_sin_restriccion_logica&& Arraysimplex[i][j]>0) printf("  +  ");
                        if(j != nvariablesx +cont_nvariablex_sin_restriccion_logica&& Arraysimplex[i][j] == 0) printf("  +  ");
                        ++cont_estandar_x;
                }



            }
            else if( i > 0  && j >= 1 && j <= nvariablesx+cont_nvariablex_sin_restriccion_logica){// VALORES DE VARIABLES X [x1,x2,...xn]
                
                
                if (j == 1)printf("            ");
                if (position_nvariablex_restriccion_logica[j-1] == 1)
                {
                        printf("(%.2f)x%d", Arraysimplex[i][j], cont_estandar_x);
                        if(j != nvariablesx +cont_nvariablex_sin_restriccion_logica&& Arraysimplex[i][j]<0) printf("  + ");
                        if(j != nvariablesx +cont_nvariablex_sin_restriccion_logica&& Arraysimplex[i][j]>0) printf("  +  ");
                        if(j != nvariablesx +cont_nvariablex_sin_restriccion_logica&& Arraysimplex[i][j] == 0) printf("  +  ");
                        ++cont_estandar_x;
                }
                else{
                        --cont_estandar_x;
                        printf("(%.2f)X%d_vi", Arraysimplex[i][j], cont_estandar_x);
                        if(j != nvariablesx +cont_nvariablex_sin_restriccion_logica&& Arraysimplex[i][j]<0) printf("  + ");
                        if(j != nvariablesx +cont_nvariablex_sin_restriccion_logica&& Arraysimplex[i][j]>0) printf("  +  ");
                        if(j != nvariablesx +cont_nvariablex_sin_restriccion_logica&& Arraysimplex[i][j] == 0) printf("  +  ");
                        ++cont_estandar_x;
                }

        


            }
            else if( i > 0  && j > nvariablesx+cont_nvariablex_sin_restriccion_logica){// SELECCION DE VARIABLES DE HOLGURA, EXCESO Y ARTIFICIALES(S Y -E, A) SI ES 1 = S, SI ES -1 = -E, SI ES 0 = "A", DE LO CONTRARIO = "   "

                    if (i == cont && j == (ajustadorx + (nrestricciones - ajustadorse)) + cont){ 

                        if (Arraysimplex[i][j] == 1 && base[i-1] == 1 && exist_valores_negativos[i-1] == 0) {//AÑADIR VARIABLES DE HOLGURA
                            
                            if (base[i-2] == 2  && exist_valores_negativos[i-2] == 0 || base[i-2] == 1 && exist_valores_negativos[i-2] == 1) printf("     + S%d", varholguraexceso); //#VISUAL
                            else printf("       + S%d", varholguraexceso);

                            ++varholguraexceso;
                        }

                        else if (Arraysimplex[i][j] == -1 && base[i-1] == 2 && exist_valores_negativos[i-1] == 0){//AÑADIR VARIABLES DE EXCESO Y ARTIFICIALES

                            ++cont_var_artifical;
                            printf("     -E%d + A%d", varholguraexceso, cont_var_artifical); ++varholguraexceso; 

                        }

                        else if (Arraysimplex[i][j] == 1 && base[i-1] == 3){//AÑADIR VARIABLES ARTIFICIALES



                            ++cont_var_artifical;
                            if (base[i-2] == 2  && exist_valores_negativos[i-2] == 0 || base[i-2] == 1 && exist_valores_negativos[i-2] == 1) printf("       + A%d", cont_var_artifical);//#VISUAL
                            else printf("     + A%d", cont_var_artifical); 
                            

                        }

                        else if (Arraysimplex[i][j] == -1 && base[i-1] == 1 && exist_valores_negativos[i-1] == 1) {//AÑADIR VARIABLES DE HOLGURA NEGATIVAS CON ARTIFICIALES

                            ++cont_var_artifical;
                            if (base[i-2] == 2 && exist_valores_negativos[i-2] == 0 || base[i-2] == 1 && exist_valores_negativos[i-2] == 1) printf("       -S%d + A%d", varholguraexceso, cont_var_artifical); //#VISUAL
                            else printf("     -S%d + A%d", varholguraexceso, cont_var_artifical);
                            ++varholguraexceso;

                        }

                        else if (Arraysimplex[i][j] == 1 && base[i-1] == 2 && exist_valores_negativos[i-1] == 1){//AÑADIR VARIABLES DE EXCESO Y YA NO ES NECESARIO AÑADIR LA ARTIFICIAL ARTIFICIALES

                            
                            if (base[i-2] == 2 && exist_valores_negativos[i-2] == 0 || base[i-2] == 1 && exist_valores_negativos[i-2] == 1) printf("       +E%d", varholguraexceso);
                            else printf("     +E%d", varholguraexceso); 
                            ++varholguraexceso;

                        }

                        else printf("      ");
                        ++cont;
                    }
                    else printf("       ");

                    if (i != 0 && j == simplex_size_columna_forma__estandar - 1)
                    printf("= %.2f", Arraysimplex[i][j]);

            }

        }

        printf("\n");
    }

    //----------------------------------------------------------//
    //-------------------//








 //CREACION DE LAS VARIABLES DEL HEADER DE LA TABLA Y LA COLUMNA DE LAS VARIABLES QUE ESTAN Y LAS QUE NO ESTAN EN LA BASE EN VARIABLES CHAR
    char var_table_head[1000] = "";
    char var_table_base[1000] = "";
    char var_table_nobase[1000] = "";

    printf("\n");       //ALMACENAR Z + VARIABLES x1,x2,....xn
    for (int i = 0; i < 1+nvariablesx; i++)
    {
        char add[2];
        if (i == 0)
        {
            strcat(var_table_head, "Z");
            strcat(var_table_base, "Z");  
        }
        else{
            sprintf(add, "%d", i);
            
            strcat(var_table_head, "x");
            strcat(var_table_head, add);

            if (nvariablex_restriccion_logica[i-1] == 2)
            {
                strcat(var_table_head, "X");
                strcat(var_table_head, add);
                strcat(var_table_nobase, "X");
                strcat(var_table_nobase, add);
            }
            

            strcat(var_table_nobase, "x");
            strcat(var_table_nobase, add);
        }
        

    }


    //|||
    //VVV

    int cantidad_variables_aritifical_ajuste = 0; //PARA TENER DEFINIDO YA LA CANTIDAD DE ARTIFICIALES DESDE UN PRINCIPIO

    for (int i = 0; i < nrestricciones; i++)
    {
        if (base[i] == 3)
        {
            ++cantidad_variables_aritifical_ajuste;
        }
        
    }


    cont_var_artifical = 0;   //CONTADOR SOLO PARA LAS VARIABLES ARITIFICALES INDIVIDUALES
    int cont_var_holgura_exceso = 0;// CONTADOR UNIDO SOLO PARA LAS VARIABLES DE HOLGURA Y EXCESO
    int cont_var_exceso_negative_holgura_artificial = 0;//CONTADOR SOLO PARA LAS VARIABLES ARITIFICALES AUTOMATICAS POR VARIABLES DE EXCESO Y HOLGURA NEGATIVAS
                            //ALMACENAR VARIABLES DE HOLGURA Y EXCESO (S y E) + ARTIFICIALES Y ARTIFICALES AUTOMATICAS
    for (int i = 0; i < nrestricciones; i++)
    {
        char add[2];

        if (base[i] == 1 && exist_valores_negativos[i] == 0){

            ++cont_var_holgura_exceso;

            sprintf(add, "%d", cont_var_holgura_exceso);

            strcat(var_table_head, "S");
            strcat(var_table_head, add);

            strcat(var_table_base, "S");
            strcat(var_table_base, add);
        }

        else if (base[i] == 1 && exist_valores_negativos[i] == 1){

            ++cont_var_holgura_exceso;
            ++cont_var_exceso_negative_holgura_artificial;

            sprintf(add, "%d", cont_var_holgura_exceso);

            strcat(var_table_head, "S") ;
            strcat(var_table_head, add);

            sprintf(add, "%d", cont_var_exceso_negative_holgura_artificial + cantidad_variables_aritifical_ajuste);//SE HACE ESTO PARA QUE LA COLUMNA DE LAS VARIABLES QUE ESTAN EN LA BASE SEAN CONSISTENTES
            strcat(var_table_base, "A");
            strcat(var_table_base, add); 


        }
        else if(base[i] == 2 && exist_valores_negativos[i] == 0){

            ++cont_var_holgura_exceso;
            ++cont_var_exceso_negative_holgura_artificial;

            sprintf(add, "%d", cont_var_holgura_exceso);

            strcat(var_table_head, "E");
            strcat(var_table_head, add);    

            sprintf(add, "%d", cont_var_exceso_negative_holgura_artificial + cantidad_variables_aritifical_ajuste);//SE HACE ESTO PARA QUE LA COLUMNA DE LAS VARIABLES QUE ESTAN EN LA BASE SEAN CONSISTENTES
            strcat(var_table_base, "A");
            strcat(var_table_base, add); 

        }
            else if(base[i] == 2 && exist_valores_negativos[i] == 1){

            ++cont_var_holgura_exceso;

            sprintf(add, "%d", cont_var_holgura_exceso);

            strcat(var_table_head, "E");
            strcat(var_table_head, add);    

            strcat(var_table_base, "E");
            strcat(var_table_base, add);

        }
        else{
            ++cont_var_artifical;
            sprintf(add, "%d", cont_var_artifical);

            strcat(var_table_head, "A");
            strcat(var_table_head, add);

            strcat(var_table_base, "A");
            strcat(var_table_base, add); 
        }

    }   
    

                    //Variables artificiales
    if (cont_var_exceso_negative_holgura_artificial != 0)
    {
        for (int i = cont_var_artifical; i < (cont_var_exceso_negative_holgura_artificial + cont_var_artifical); i++)
        {
            char add[2];
            sprintf(add, "%d", i+1);
            strcat(var_table_head, "A");
            strcat(var_table_head, add);
            strcat(var_table_base, "A");
            strcat(var_table_base, add);      
        }
        
    }
        strcat(var_table_head, "b");
        strcat(var_table_head, "B");

    //----------------------------------------------------------//
    //-------------------//
    
        // printf("Variables_HEAD: %s\n\n", var_table_head);
        // printf("Variables_BASE: %s\n\n", var_table_base);
        // printf("Variables_NOBASE: %s\n\n", var_table_nobase);

    //----------------------------------------------------------//
    //-------------------//






    // IMPRESION DE LA TABLA SIMPLEX // EMPEZARA EL USO DEL ARRAYSIMPLEXOFFICIAL[] Y TERMINARA EL USO DEL ARRAYSIMPLEX[]

    printf("\n\n");
    printf("\x1B[33m"); // #COLOR-VISUAL
    printf(":========FORMA TABULAR========: \n\n");
    printf("\x1B[37m"); // #COLOR-VISUAL

    printf("\x1B[31m"); // #COLOR-VISUAL

    // #VISUAL - IMPRESION DE LAS VARIABLES HORIZONTALES EN EL HEADER

                        //z + (x1..xn    +   S1,E1,A1    +   A2,A3 AGREGADAS AUTOMATICO) * 2(Porque se toman de dos en dos posiciones con las variables) + b + B
    for (int i = 0; i < 1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 2; i++)
    {
        if (i == 0) printf("            %c", var_table_head[i]);
        else if(i == (1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 1)) printf("        %c", var_table_head[i]);
        else if (i % 2 == 0) printf("%c", var_table_head[i]);
        else printf("        %c", var_table_head[i]);
    }
    

    printf("\n");

    printf("\x1B[37m"); // #COLOR-VISUAL

    //----------------------------------------------------------//
    //-------------------//
    
    int cont__position__base = 0;
    for (int i = 0; i < simplex_size_fila; i++){ // FILAS

        for (int j = 0; j < simplex_size_columna; j++){ // COLUMNAS

            if (j == 0){
                
                printf("\x1B[31m"); // #COLOR-VISUAL
                if (i == 0 && j == 0)
                    printf("Ro(C)     "); // #VISUAL - VARIABLES VERTICALES
                else
                    printf("R%d        ", i); // #VISUAL - VARIABLES VERTICALES
                printf("\x1B[37m");           // #COLOR-VISUAL

                printf("%.2f", ArraysimplexOfficial[i][j]);
            }

            else{
                if (ArraysimplexOfficial[i][j] < 0)       printf("     %.2f", ArraysimplexOfficial[i][j]);// #VISUAL
                else if(ArraysimplexOfficial[i][j] >= 1000) printf("   %.2f", ArraysimplexOfficial[i][j]);
                else if(ArraysimplexOfficial[i][j] >= 100) printf("    %.2f", ArraysimplexOfficial[i][j]);
                else if(ArraysimplexOfficial[i][j] >= 10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                else                              printf("      %.2f", ArraysimplexOfficial[i][j]);
            }
        }

        //IMPRESION DE LAS VARIABLES EN LA BASE
        printf("\x1B[33m");// #COLOR-VISUAL
        if (i == 0) printf("      %c", var_table_base[i]);
        else if(i > 0){
            printf("      %c%c", var_table_base[i + cont__position__base], var_table_base[i + cont__position__base + 1]);
            ++cont__position__base;
        }
        printf("\x1B[37m"); // #COLOR-VISUAL

        printf("\n");
    }

    //----------------------------------------------------------//
    //-------------------//
    //----------------------------------------------------------//
    //-------------------//



    printf("\n\n");
    printf("\x1B[33m"); // #COLOR-VISUAL
    printf(":========PROCESO========: \n");
    printf("\x1B[37m"); // #COLOR-VISUAL

    
    //DETERMINARA SI EXISTEN VARIABLES ARTIFICIALES EN GENERAL, NO SOLO LAS DE EXCESO.

    bool exist_artificial_general = false;
    for (int i = 0; i < nrestricciones; i++)
    {
        if (base[i] == 2 && exist_valores_negativos[i] == 0 || base[i] == 1 && exist_valores_negativos[i] == 1 || base[i] == 3 && exist_valores_negativos[i] == 0)
        {
            exist_artificial_general  = true;
        }
        
    }
    
    if (exist_artificial_general == true)//SE PASARA AL PROCESO DE MULTIPLICAR Y SUMAR LAS VARIABLES ARTIFICIALES PARA TENER LA BASE LISTA
    {
            printf("\n\n\n");
            printf("\x1B[34m"); // #COLOR-VISUAL
            printf(":========AJUSTE========: \n\n");
            printf("\x1B[37m"); // #COLOR-VISUAL

        //SE SE SEÑALAN LAS FILAS A LAS QUE SE LES APLICARA EL PROCESO Y CON QUE VALOR

            //ALGORITMO RESUMIDO PARA IMPRIMIR TABLAS


                //IMPRESION DE DATOS//
                printf("\x1B[31m");
        for (int i = 0; i < 1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 2; i++)
            {
                if (i == 0) printf("            %c", var_table_head[i]);
                else if(i == (1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 1)) printf("        %c", var_table_head[i]);
                else if (i % 2 == 0) printf("%c", var_table_head[i]);
                else printf("        %c", var_table_head[i]);
            }
    
                printf("\n");
                printf("\x1B[37m"); // #COLOR-VISUAL
                cont__position__base = 0;
                for (int i = 0; i < simplex_size_fila; i++){ // FILAS

                for (int j = 0; j < simplex_size_columna; j++){ // COLUMNAS

                    if (j == 0){
                        
                        printf("\x1B[31m"); // #COLOR-VISUAL
                        if (i == 0 && j == 0)
                            printf("Ro(C)     "); // #VISUAL - VARIABLES VERTICALES
                        else
                            printf("R%d        ", i); // #VISUAL - VARIABLES VERTICALES
                        printf("\x1B[37m");           // #COLOR-VISUAL

                        if (i != 0)
                        {
                            if (base[i-1] == 2 && exist_valores_negativos[i-1] == 0 || base[i-1] == 1 && exist_valores_negativos[i-1] == 1 || base[i-1] == 3) printf("\x1B[34m");// #COLOR-VISUAL
                            printf("%.2f", ArraysimplexOfficial[i][j]);
                            printf("\x1B[37m");// #COLOR-VISUAL
                            
                        }
                        else printf("%.2f", ArraysimplexOfficial[i][j]);
                        
                    }

                    else{

                        if (i != 0){   
                            //IMPRIMIR EN AZUL CUANDO SE ENCUENTRAN FILAS CON ARTIFICIALES O EXCESO
                            if (base[i-1] == 2 && exist_valores_negativos[i-1] == 0 || base[i-1] == 1 && exist_valores_negativos[i-1] == 1 || base[i-1] == 3)printf("\x1B[34m");// #COLOR-VISUAL
                                if(ArraysimplexOfficial[i][j] <= -1000000) printf("%.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] <= -100000) printf("%.2f", ArraysimplexOfficial[i][j]);// #VISUAL
                                    else if(ArraysimplexOfficial[i][j] <= -10000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -1000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -100) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if (ArraysimplexOfficial[i][j] < 0)  printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000000) printf("  %.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] >= 100000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 100) printf("    %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else                              printf("      %.2f", ArraysimplexOfficial[i][j]);
                            printf("\x1B[37m");// #COLOR-VISUAL
                            
                        }
                        else{//IMPRIMIR CUANDO i == 0
                            if(ArraysimplexOfficial[i][j] <= -1000000) printf("%.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] <= -100000) printf("%.2f", ArraysimplexOfficial[i][j]);// #VISUAL
                                    else if(ArraysimplexOfficial[i][j] <= -10000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -1000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -100) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if (ArraysimplexOfficial[i][j] < 0)  printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000000) printf("  %.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] >= 100000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 100) printf("    %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else                              printf("      %.2f", ArraysimplexOfficial[i][j]);
                        }
                    }
                }

                //IMPRESION DE LAS VARIABLES EN LA BASE
                printf("\x1B[33m");// #COLOR-VISUAL
                if (i == 0) printf("      %c", var_table_base[i]);
                else if(i > 0){
                    printf("      %c%c", var_table_base[i + cont__position__base], var_table_base[i + cont__position__base + 1]);
                    ++cont__position__base;
                }
                printf("\x1B[37m"); // #COLOR-VISUAL


                //COMO DETALLE IMPRIMIRA AL LADO DERECHO DE LA FILA POR CUAL VALOR SE HARA EL PROCESO DE LA MISMA(EN EL CASO DE LAS ARTIFICIALES COMO DEFECTO ES -100 ORIGINALMENTE)
                if (i != 0) 
                {   printf("\x1B[31m");// #COLOR-VISUAL
                    if (base[i-1] == 2 && exist_valores_negativos[i-1] == 0 || base[i-1] == 1 && exist_valores_negativos[i-1] == 1 || base[i-1] == 3)
                    {
                        printf("    (%d)", valor_artificial*-1);
                    }
                    printf("\x1B[37m"); // #COLOR-VISUAL    
                }
                

                printf("\n");
            }


            

        //PROCESO PARA HACER EL AJUSTE CON EL VALOR DE LAS VARIABLES ARTIFICIALES

        for (int i = 0; i < simplex_size_fila; i++)
        {   
            for (int j = 0; j < simplex_size_columna; j++)
            {
                 if (i != 0)
                 {
                    if (base[i-1] == 2 && exist_valores_negativos[i-1] == 0 || base[i-1] == 1 && exist_valores_negativos[i-1] == 1 || base[i-1] == 3)
                    {
                        ArraysimplexOfficial[0][j] = (ArraysimplexOfficial[i][j] * (valor_artificial*-1)) + ArraysimplexOfficial[0][j];
                    }
                    
                 }
                 
            }

            
        }



        //SE IMPRIME LA TABLA SIMPLEX RESULTANTE

            printf("\n\n");
            printf("\x1B[34m"); // #COLOR-VISUAL
            printf(":========AJUSTE / RESULTADO========: \n\n");
            printf("\x1B[37m"); // #COLOR-VISUAL
            printf("\x1B[31m");


            //IMPRESION DE DATOS//
            for (int i = 0; i < 1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 2; i++)
            {
                if (i == 0) printf("            %c", var_table_head[i]);
                else if(i == (1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 1)) printf("        %c", var_table_head[i]);
                else if (i % 2 == 0) printf("%c", var_table_head[i]);
                else printf("        %c", var_table_head[i]);
            }
            printf("\n");
            printf("\x1B[37m"); // #COLOR-VISUAL
            cont__position__base = 0;
            for (int i = 0; i < simplex_size_fila; i++){ // FILAS

                for (int j = 0; j < simplex_size_columna; j++){ // COLUMNAS

                    if (j == 0){
                        
                        printf("\x1B[31m"); // #COLOR-VISUAL
                        if (i == 0 && j == 0)
                            printf("Ro(C)     "); // #VISUAL - VARIABLES VERTICALES
                        else
                            printf("R%d        ", i); // #VISUAL - VARIABLES VERTICALES
                        printf("\x1B[37m");           // #COLOR-VISUAL

                        if (i == 0) printf("\x1B[32m");
                        printf("%.2f", ArraysimplexOfficial[i][j]);
                        printf("\x1B[37m");
                    }

                    else{
                        if (i == 0) printf("\x1B[32m");
                        if(ArraysimplexOfficial[i][j] <= -1000000) printf("%.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] <= -100000) printf("%.2f", ArraysimplexOfficial[i][j]);// #VISUAL
                                    else if(ArraysimplexOfficial[i][j] <= -10000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -1000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -100) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if (ArraysimplexOfficial[i][j] < 0)  printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000000) printf("  %.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] >= 100000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 100) printf("    %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else                              printf("      %.2f", ArraysimplexOfficial[i][j]);
                        printf("\x1B[37m");
                    }
                }

                //IMPRESION DE LAS VARIABLES EN LA BASE
                printf("\x1B[33m");// #COLOR-VISUAL
                if (i == 0) printf("      %c", var_table_base[i]);
                else if(i > 0){
                    printf("      %c%c", var_table_base[i + cont__position__base], var_table_base[i + cont__position__base + 1]);
                    ++cont__position__base;
                }
                printf("\x1B[37m"); // #COLOR-VISUAL

                printf("\n");
            }
            
        //----------------------------------------------------------//
        //-------------------//
    }
    
    printf("\n");

    //----------------------------------------------------------//
    //-------------------//









    //PROCESO PARA EMPEZAR CON EL SIMPLEX OFICIAL (ITERACIONES)

    int cont_iteracion = 1;
    bool exist_negativos_of = false;
    bool is_no_acotada = false;

    do
    {


            printf("\n\n");
            printf("\x1B[36m"); // #COLOR-VISUAL
            printf(":========ITERACION %d========: \n\n", cont_iteracion);
            printf("\x1B[37m"); // #COLOR-VISUAL



            for (int i = 0; i < simplex_size_fila; i++)//DETERMINARA SI EXISTEN NEGATIVOS EN LA F.O PARA VER SI SE PUEDE MEJORAR
            {
                for (int j = 0; j < simplex_size_columna; j++)
                {
                    if (i == 0 && j > 0 && j <= simplex_size_columna-2)
                    {
                        if (ArraysimplexOfficial[i][j] < 0)
                        {
                            exist_negativos_of = true;
                        }
                    }
                    
                }
            }




            if (exist_negativos_of == true)// EN EL CASO DE QUE HAYA NEGATIVOS EN LA F.O SEGUIRA EL SIGUIENTE PROCESO
            {

                
                double pc_valor[nrestricciones + 1];
                double more_negative = 0;
                int j_columna_more_negative;

                for (int i = 0; i < simplex_size_fila; i++)//DETERMINARA CUAL ES LA COLUMNA MAS NEGATIVA 
                {
                    for (int j = 0; j < simplex_size_columna; j++)
                    {
                        if (i == 0 && j > 0 && j <= simplex_size_columna-2)
                        {
                            if (more_negative > ArraysimplexOfficial[i][j])
                            {
                                more_negative = ArraysimplexOfficial[i][j];
                                j_columna_more_negative = j;
                            }
                            
                        }
                    }
                }

                



                for (int i = 0; i < simplex_size_fila; i++)//SACARA LOS VALORES DEL P.C DE CADA FILA EN BASE A LA COLUMNA CON LA VARIABLE DE LA F.O MAS NEGATIVA
                {
                    for (int j = 0; j < simplex_size_columna; j++)
                    {
                        if (j == j_columna_more_negative){
                            if (i == 0) pc_valor[i] = NAN;
                            else if(ArraysimplexOfficial[i][j] <= 0) pc_valor[i] = NAN;
                            else pc_valor[i] =  ArraysimplexOfficial[i][simplex_size_columna-1] / ArraysimplexOfficial[i][j_columna_more_negative];
                        } 
                        
                    }
                }



                
                double pc_more_small;
                int i_fila_more_small;
                int cont_no_acotada = 0;
                bool first_valor = false;

                for (int i = 0; i < nrestricciones+1; i++)//DETERMINAR LA FILA CON EL P.C MAS BAJO
                {
                    if (i != 0)
                    {
                        if (isnan(pc_valor[i])) ++cont_no_acotada;//SI EN DADO CASO TODOS LOS VALORES DE P.C SON NAN ENTONCES LA SOLUCION SERA NO ACOTADA
                        else{

                            if (first_valor == false && isnan(pc_valor[i]) == false)
                            {
                                pc_more_small = pc_valor[i];
                                i_fila_more_small = i;
                                first_valor = true;
                            }
                            else if (pc_more_small >= pc_valor[i] && isnan(pc_valor[i]) == false)
                            {
                                i_fila_more_small = i;
                                pc_more_small = pc_valor[i];
                            }
 
                        }
                        
                    }
                    
                }




                if (cont_no_acotada == nrestricciones)//EN CASO DE QUE TODOS LOS VALORES HAYAN SIDO --- EN P.C ENTONCES SERA NO ACOTADA
                {
                        
                    //IMPRESION DE DATOS//
                    printf("\x1B[31m");
                for (int i = 0; i < 1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 2; i++)
                        {
                            if (i == 0) printf("            %c", var_table_head[i]);
                            else if(i == (1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 1)) printf("        %c", var_table_head[i]);
                            else if (i % 2 == 0) printf("%c", var_table_head[i]);
                            else printf("        %c", var_table_head[i]);
                        }
                    printf("\n");
                    printf("\x1B[37m"); // #COLOR-VISUAL
                    cont__position__base = 0;
                    for (int i = 0; i < simplex_size_fila; i++){ // FILAS

                    for (int j = 0; j < simplex_size_columna; j++){ // COLUMNAS

                        if (j == 0){
                            
                            printf("\x1B[31m"); // #COLOR-VISUAL
                            if (i == 0 && j == 0)
                                printf("Ro(C)     "); // #VISUAL - VARIABLES VERTICALES
                            else
                                printf("R%d        ", i); // #VISUAL - VARIABLES VERTICALES
                            printf("\x1B[37m");           // #COLOR-VISUAL

                            if (i == i_fila_more_small) printf("\x1B[31m");  // #COLOR-VISUAL
                            printf("%.2f", ArraysimplexOfficial[i][j]);
                            printf("\x1B[37m");           // #COLOR-VISUAL
                        }

                        else{
                            if (j == j_columna_more_negative || i == i_fila_more_small) printf("\x1B[31m");  // #COLOR-VISUAL
                            if (j == j_columna_more_negative && i == i_fila_more_small) printf("\x1B[33m");  // #COLOR-VISUAL
                             if(ArraysimplexOfficial[i][j] <= -1000000) printf("%.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] <= -100000) printf("%.2f", ArraysimplexOfficial[i][j]);// #VISUAL
                                    else if(ArraysimplexOfficial[i][j] <= -10000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -1000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -100) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if (ArraysimplexOfficial[i][j] < 0)  printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000000) printf("  %.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] >= 100000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 100) printf("    %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else                              printf("      %.2f", ArraysimplexOfficial[i][j]);
                            printf("\x1B[37m");           // #COLOR-VISUAL
                        }
                    }

                    //IMPRESION DE LAS VARIABLES EN LA BASE
                    printf("\x1B[33m");// #COLOR-VISUAL
                    if (i == 0) printf("      %c", var_table_base[i]);
                    else if(i > 0){
                        printf("      %c%c", var_table_base[i + cont__position__base], var_table_base[i + cont__position__base + 1]);
                        ++cont__position__base;
                    }
                
                    printf("\x1B[31m"); // #COLOR-VISUAL
                    printf("      P.C =");
                    printf("\x1B[34m"); // #COLOR-VISUAL
                    if (isnan(pc_valor[i])) printf(" -----");
                    else printf(" %.2f", pc_valor[i]);
                    printf("\x1B[37m"); // #COLOR-VISUAL
                    printf("\n");
                }

                    
                    printf("\n\n");
                    printf("\x1B[36m"); // #COLOR-VISUAL
                    printf(":========ITERACION %d / RESULTADO========: \n\n", cont_iteracion);
                    printf("\x1B[37m"); // #COLOR-VISUAL

                    //IMPRESION DE DATOS//
                    printf("\x1B[31m");
                    for (int i = 0; i < 1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 2; i++)
                        {
                            if (i == 0) printf("            %c", var_table_head[i]);
                            else if(i == (1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 1)) printf("        %c", var_table_head[i]);
                            else if (i % 2 == 0) printf("%c", var_table_head[i]);
                            else printf("        %c", var_table_head[i]);
                        }
                    printf("\n");
                    printf("\x1B[37m"); // #COLOR-VISUAL

                    cont__position__base = 0;
                    for (int i = 0; i < simplex_size_fila; i++){ // FILAS

                            for (int j = 0; j < simplex_size_columna; j++){ // COLUMNAS

                                if (j == 0){
                                    
                                    printf("\x1B[31m"); // #COLOR-VISUAL
                                    if (i == 0 && j == 0)
                                        printf("Ro(C)     "); // #VISUAL - VARIABLES VERTICALES
                                    else
                                        printf("R%d        ", i); // #VISUAL - VARIABLES VERTICALES
                                    printf("\x1B[37m");           // #COLOR-VISUAL

                                    printf("%.2f", ArraysimplexOfficial[i][j]);
                                    
                                }

                                else{
                                    
                                    if(ArraysimplexOfficial[i][j] <= -1000000) printf("%.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] <= -100000) printf("%.2f", ArraysimplexOfficial[i][j]);// #VISUAL
                                    else if(ArraysimplexOfficial[i][j] <= -10000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -1000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -100) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if (ArraysimplexOfficial[i][j] < 0)  printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000000) printf("  %.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] >= 100000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 100) printf("    %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else                              printf("      %.2f", ArraysimplexOfficial[i][j]);
                                }
                            }

                            //IMPRESION DE LAS VARIABLES EN LA BASE
                            printf("\x1B[33m");// #COLOR-VISUAL
                            if (i == 0) printf("      %c", var_table_base[i]);
                            else if(i > 0){
                                printf("      %c%c", var_table_base[i + cont__position__base], var_table_base[i + cont__position__base + 1]);
                                ++cont__position__base;
                            }
                            
                            printf("\x1B[37m"); // #COLOR-VISUAL
                            printf("\n");
                        }


                    exist_negativos_of = true;
                    is_no_acotada = true;


                }
                else
                {//DE LO CONTRARIO SE SEGUIRA NORMALMENTE HASTA LLEGAR A UNA SOLUCION

                    //IMPRESION DE DATOS//
                    printf("\x1B[31m");
            for (int i = 0; i < 1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 2; i++)
            {
                if (i == 0) printf("            %c", var_table_head[i]);
                else if(i == (1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 1)) printf("        %c", var_table_head[i]);
                else if (i % 2 == 0) printf("%c", var_table_head[i]);
                else printf("        %c", var_table_head[i]);
            }
                    printf("\n");
                    printf("\x1B[37m"); // #COLOR-VISUAL
                    cont__position__base = 0;
                    for (int i = 0; i < simplex_size_fila; i++){ // FILAS

                    for (int j = 0; j < simplex_size_columna; j++){ // COLUMNAS

                        if (j == 0){
                            
                            printf("\x1B[31m"); // #COLOR-VISUAL
                            if (i == 0 && j == 0)
                                printf("Ro(C)     "); // #VISUAL - VARIABLES VERTICALES
                            else
                                printf("R%d        ", i); // #VISUAL - VARIABLES VERTICALES
                            printf("\x1B[37m");           // #COLOR-VISUAL

                            if (i == i_fila_more_small) printf("\x1B[31m");  // #COLOR-VISUAL
                            printf("%.2f", ArraysimplexOfficial[i][j]);
                            printf("\x1B[37m");           // #COLOR-VISUAL
                        }

                        else{
                            if (j == j_columna_more_negative || i == i_fila_more_small) printf("\x1B[31m");  // #COLOR-VISUAL
                            if (j == j_columna_more_negative && i == i_fila_more_small) printf("\x1B[33m");  // #COLOR-VISUAL
                                if(ArraysimplexOfficial[i][j] <= -1000000) printf("%.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] <= -100000) printf("%.2f", ArraysimplexOfficial[i][j]);// #VISUAL
                                    else if(ArraysimplexOfficial[i][j] <= -10000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -1000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -100) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if (ArraysimplexOfficial[i][j] < 0)  printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000000) printf("  %.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] >= 100000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 100) printf("    %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else                              printf("      %.2f", ArraysimplexOfficial[i][j]);
                            printf("\x1B[37m");           // #COLOR-VISUAL
                        }
                    }

                    //IMPRESION DE LAS VARIABLES EN LA BASE
                    printf("\x1B[33m");// #COLOR-VISUAL
                    if (i == 0) printf("      %c", var_table_base[i]);
                    else if(i > 0){
                        printf("      %c%c", var_table_base[i + cont__position__base], var_table_base[i + cont__position__base + 1]);
                        ++cont__position__base;
                    }
                
                    printf("\x1B[31m"); // #COLOR-VISUAL
                    printf("      P.C =");
                    printf("\x1B[34m"); // #COLOR-VISUAL
                    if (isnan(pc_valor[i])) printf(" -----");
                    else printf(" %.2f", pc_valor[i]);
                    printf("\x1B[37m"); // #COLOR-VISUAL
                    printf("\n");
                    }


                    double pivote_original = ArraysimplexOfficial[i_fila_more_small][j_columna_more_negative];

                    if (ArraysimplexOfficial[i_fila_more_small][j_columna_more_negative] != 1)//SI EL PIVTOE ES MAYOR A 1 ENTONCES SE AJUSTARA PARA QUE SEA 1 Y CON ESO MODIFICAR TODA LA FILA
                    {

                        for (int i = 0; i < simplex_size_fila; i++)
                        {
                            for (int j = 0; j < simplex_size_columna; j++)
                            {
                                if (i == i_fila_more_small)
                                {
                                    ArraysimplexOfficial[i][j] = (1 / pivote_original) * ArraysimplexOfficial[i][j];
                                }
                            }
                        }
                        
                            //IMPRESION DE DATOS//

                            printf("\n\n");
                                            printf("\x1B[31m");
                        for (int i = 0; i < 1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 2; i++)
                            {
                                if (i == 0) printf("            %c", var_table_head[i]);
                                else if(i == (1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 1)) printf("        %c", var_table_head[i]);
                                else if (i % 2 == 0) printf("%c", var_table_head[i]);
                                else printf("        %c", var_table_head[i]);
                            }
                            printf("\n");
                            printf("\x1B[37m"); // #COLOR-VISUAL
                            cont__position__base = 0;
                            for (int i = 0; i < simplex_size_fila; i++){ // FILAS

                            for (int j = 0; j < simplex_size_columna; j++){ // COLUMNAS

                                if (j == 0){
                                    
                                    printf("\x1B[31m"); // #COLOR-VISUAL
                                    if (i == 0 && j == 0)
                                        printf("Ro(C)     "); // #VISUAL - VARIABLES VERTICALES
                                    else
                                        printf("R%d        ", i); // #VISUAL - VARIABLES VERTICALES
                                    printf("\x1B[37m");           // #COLOR-VISUAL

                                    if (i == i_fila_more_small)printf("\x1B[32m"); 
                                    printf("%.2f", ArraysimplexOfficial[i][j]);
                                    printf("\x1B[37m");           // #COLOR-VISUAL
                                }

                                else{
                                    if (i == i_fila_more_small)printf("\x1B[32m");           // #COLOR-VISUAL
                                    if (i == i_fila_more_small && j == j_columna_more_negative)printf("\x1B[33m");           // #COLOR-VISUAL
                                    if(ArraysimplexOfficial[i][j] <= -1000000) printf("%.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] <= -100000) printf("%.2f", ArraysimplexOfficial[i][j]);// #VISUAL
                                    else if(ArraysimplexOfficial[i][j] <= -10000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -1000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -100) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if (ArraysimplexOfficial[i][j] < 0)  printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000000) printf("  %.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] >= 100000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 100) printf("    %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else                              printf("      %.2f", ArraysimplexOfficial[i][j]);
                                    printf("\x1B[37m");           // #COLOR-VISUAL
                                }
                            }

                            //IMPRESION DE LAS VARIABLES EN LA BASE
                            printf("\x1B[33m");// #COLOR-VISUAL
                            if (i == 0) printf("      %c", var_table_base[i]);
                            else if(i > 0){
                                printf("      %c%c", var_table_base[i + cont__position__base], var_table_base[i + cont__position__base + 1]);
                                ++cont__position__base;
                            }
                        
                            if (i == i_fila_more_small){
                                printf("\x1B[32m");           // #COLOR-VISUAL
                                printf("    (1 / %.2f)", pivote_original);
                            }
                            
                            printf("\x1B[37m"); // #COLOR-VISUAL
                            printf("\n");
                        }


                    }


                    

                    double valores_columna_pivote[nrestricciones + 1];//VARIABLE QUE AYUDARA PARA SACAR LOS VALORES DE LA COLUMNA PIVOTE PARA HACERLOS 0

                    
                    for (int i = 0; i < simplex_size_fila; i++)//PROCESO PARA SACAR LOS VALORES DE LO MENCIONADO
                    {
                        for (int j = 0; j < simplex_size_columna; j++)
                        {
                            if (j == j_columna_more_negative)
                            {
                                if (i == i_fila_more_small)
                                {
                                   valores_columna_pivote[i] = NAN;
                                }
                                else{
                                    
                                    if (ArraysimplexOfficial[i][j] == 0) valores_columna_pivote[i] = ArraysimplexOfficial[i][j];
                                    else valores_columna_pivote[i] = ArraysimplexOfficial[i][j] * -1;
                                    
                                }
                            }
                            
                        }
                    }



                    //IMPRESION DE DATOS//

                    printf("\n\n");
                    printf("\x1B[31m");
       for (int i = 0; i < 1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 2; i++)
            {
                if (i == 0) printf("            %c", var_table_head[i]);
                else if(i == (1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 1)) printf("        %c", var_table_head[i]);
                else if (i % 2 == 0) printf("%c", var_table_head[i]);
                else printf("        %c", var_table_head[i]);
            }
                    printf("\n");
                    printf("\x1B[37m"); // #COLOR-VISUAL
                    cont__position__base = 0;
                    for (int i = 0; i < simplex_size_fila; i++){ // FILAS

                            for (int j = 0; j < simplex_size_columna; j++){ // COLUMNAS

                                if (j == 0){
                                    
                                    printf("\x1B[31m"); // #COLOR-VISUAL
                                    if (i == 0 && j == 0)
                                        printf("Ro(C)     "); // #VISUAL - VARIABLES VERTICALES
                                    else
                                        printf("R%d        ", i); // #VISUAL - VARIABLES VERTICALES
                                    printf("\x1B[37m");           // #COLOR-VISUAL

                                    if (j != j_columna_more_negative && i != i_fila_more_small)printf("\x1B[34m");           // #COLOR-VISUAL
                                    printf("%.2f", ArraysimplexOfficial[i][j]);
                                    printf("\x1B[37m");           // #COLOR-VISUAL
                                }

                                else{
                                    
                                    if (j == j_columna_more_negative)printf("\x1B[35m");           // #COLOR-VISUAL
                                    if (j != j_columna_more_negative && i != i_fila_more_small)printf("\x1B[34m");           // #COLOR-VISUAL
                                    if (i == i_fila_more_small && j == j_columna_more_negative)printf("\x1B[33m");           // #COLOR-VISUAL
                                    if(ArraysimplexOfficial[i][j] <= -1000000) printf("%.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] <= -100000) printf("%.2f", ArraysimplexOfficial[i][j]);// #VISUAL
                                    else if(ArraysimplexOfficial[i][j] <= -10000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -1000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -100) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if (ArraysimplexOfficial[i][j] < 0)  printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000000) printf("  %.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] >= 100000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 100) printf("    %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else                              printf("      %.2f", ArraysimplexOfficial[i][j]);
                                    printf("\x1B[37m");           // #COLOR-VISUAL
                                }
                            }

                            //IMPRESION DE LAS VARIABLES EN LA BASE
                            printf("\x1B[33m");// #COLOR-VISUAL
                            if (i == 0) printf("      %c", var_table_base[i]);
                            else if(i > 0){
                                printf("      %c%c", var_table_base[i + cont__position__base], var_table_base[i + cont__position__base + 1]);
                                ++cont__position__base;
                            }
                        
                            if (i != i_fila_more_small){
                                printf("\x1B[31m");           // #COLOR-VISUAL
                                if (valores_columna_pivote[i] == 0) printf("    (Se Deja)");
                                else printf("    (%.2f)", valores_columna_pivote[i]);
                            }
                            
                            printf("\x1B[37m"); // #COLOR-VISUAL
                            printf("\n");
                        }




                    for (int i = 0; i < simplex_size_fila; i++)//PROCESO PARA MULTIPLICAR Y SUMAR COLUMNA PIVOTE POR LAS DEMAS
                    {
                        for (int j = 0; j < simplex_size_columna; j++)
                        {
                           
                           if (i != i_fila_more_small)
                           {
                                ArraysimplexOfficial[i][j] = (ArraysimplexOfficial[i_fila_more_small][j] * valores_columna_pivote[i]) + ArraysimplexOfficial[i][j];
                                if (j == j_columna_more_negative)
                                {
                                    ArraysimplexOfficial[i][j] = round(ArraysimplexOfficial[i][j]);
                                }

                           }
                            
                        }
                    }


                    
                    ArraysimplexOfficial[i_fila_more_small][j_columna_more_negative] = round(ArraysimplexOfficial[i_fila_more_small][j_columna_more_negative]);


                    //IMPRESION DE DATOS//

                    printf("\n\n");
                    printf("\x1B[31m");
                           for (int i = 0; i < 1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 2; i++)
            {
                if (i == 0) printf("            %c", var_table_head[i]);
                else if(i == (1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 1)) printf("        %c", var_table_head[i]);
                else if (i % 2 == 0) printf("%c", var_table_head[i]);
                else printf("        %c", var_table_head[i]);
            }
                    printf("\n");
                    printf("\x1B[37m"); // #COLOR-VISUAL

                    cont__position__base = 0;
                    for (int i = 0; i < simplex_size_fila; i++){ // FILAS

                            for (int j = 0; j < simplex_size_columna; j++){ // COLUMNAS

                                if (j == 0){
                                    
                                    printf("\x1B[31m"); // #COLOR-VISUAL
                                    if (i == 0 && j == 0)
                                        printf("Ro(C)     "); // #VISUAL - VARIABLES VERTICALES
                                    else
                                        printf("R%d        ", i); // #VISUAL - VARIABLES VERTICALES
                                    printf("\x1B[37m");           // #COLOR-VISUAL

                                    if (j != j_columna_more_negative && i != i_fila_more_small) printf("\x1B[32m");           // #COLOR-VISUAL
                                    printf("%.2f", ArraysimplexOfficial[i][j]);
                                    printf("\x1B[37m");           // #COLOR-VISUAL
                                }

                                else{
                                    
                                    if (j != j_columna_more_negative && i != i_fila_more_small)printf("\x1B[32m");           // #COLOR-VISUAL
                                    if (j == j_columna_more_negative)printf("\x1B[33m");           // #COLOR-VISUAL
                                    if(ArraysimplexOfficial[i][j] <= -1000000) printf("%.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] <= -100000) printf("%.2f", ArraysimplexOfficial[i][j]);// #VISUAL
                                    else if(ArraysimplexOfficial[i][j] <= -10000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -1000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -100) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if (ArraysimplexOfficial[i][j] < 0)  printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000000) printf("  %.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] >= 100000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 100) printf("    %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else                              printf("      %.2f", ArraysimplexOfficial[i][j]);
                                    printf("\x1B[37m");           // #COLOR-VISUAL
                                }
                            }

                            //IMPRESION DE LAS VARIABLES EN LA BASE
                            printf("\x1B[33m");// #COLOR-VISUAL
                            if (i == 0) printf("      %c", var_table_base[i]);
                            else if(i > 0){
                                printf("      %c%c", var_table_base[i + cont__position__base], var_table_base[i + cont__position__base + 1]);
                                ++cont__position__base;
                            }
                            
                            printf("\x1B[37m"); // #COLOR-VISUAL
                            printf("\n");
                        }




                    
                    //PARA FINALIZAR CADA RESULTADO SE HARA EL CAMBIO DE VARIABLES EN LA BASE 

                    var_table_base[i_fila_more_small*2-1] = var_table_head[j_columna_more_negative*2-1];
                    var_table_base[i_fila_more_small*2] = var_table_head[j_columna_more_negative*2];

                    // printf("Variables_HEAD: %s\n\n", var_table_head);
                    // printf("Variables_BASE: %s\n\n", var_table_base);
                    // printf("Variables_NOBASE: %s\n\n", var_table_nobase);





                    printf("\n\n");
                    printf("\x1B[36m"); // #COLOR-VISUAL
                    printf(":========ITERACION %d / RESULTADO========: \n\n", cont_iteracion);
                    printf("\x1B[37m"); // #COLOR-VISUAL

                    //IMPRESION DE DATOS//
                    printf("\x1B[31m");
       for (int i = 0; i < 1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 2; i++)
            {
                if (i == 0) printf("            %c", var_table_head[i]);
                else if(i == (1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 1)) printf("        %c", var_table_head[i]);
                else if (i % 2 == 0) printf("%c", var_table_head[i]);
                else printf("        %c", var_table_head[i]);
            }
                    printf("\n");
                    printf("\x1B[37m"); // #COLOR-VISUAL

                    cont__position__base = 0;
                    for (int i = 0; i < simplex_size_fila; i++){ // FILAS

                            for (int j = 0; j < simplex_size_columna; j++){ // COLUMNAS

                                if (j == 0){
                                    
                                    printf("\x1B[31m"); // #COLOR-VISUAL
                                    if (i == 0 && j == 0)
                                        printf("Ro(C)     "); // #VISUAL - VARIABLES VERTICALES
                                    else
                                        printf("R%d        ", i); // #VISUAL - VARIABLES VERTICALES
                                    printf("\x1B[37m");           // #COLOR-VISUAL

                                    printf("%.2f", ArraysimplexOfficial[i][j]);
                                    
                                }

                                else{
    
                                    if(ArraysimplexOfficial[i][j] <= -1000000) printf("%.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] <= -100000) printf("%.2f", ArraysimplexOfficial[i][j]);// #VISUAL
                                    else if(ArraysimplexOfficial[i][j] <= -10000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -1000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -100) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if (ArraysimplexOfficial[i][j] < 0)  printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000000) printf("  %.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] >= 100000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 100) printf("    %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else                              printf("      %.2f", ArraysimplexOfficial[i][j]);
                                }
                            }

                            //IMPRESION DE LAS VARIABLES EN LA BASE
                            printf("\x1B[33m");// #COLOR-VISUAL
                            if (i == 0) printf("      %c", var_table_base[i]);
                            else if(i > 0){
                                printf("      %c%c", var_table_base[i + cont__position__base], var_table_base[i + cont__position__base + 1]);
                                ++cont__position__base;
                            }
                            
                            printf("\x1B[37m"); // #COLOR-VISUAL
                            printf("\n");
                        }



                    exist_negativos_of = true;//EL VALOR SE VUELVE A REINICIAR

                    for (int i = 0; i < simplex_size_fila; i++)//DETERMINARA DE NUEVO SI EXISTEN NEGATIVOS EN LA F.O PARA VER SI SE PUEDE MEJORAR
                    {
                        for (int j = 0; j < simplex_size_columna; j++)
                        {
                            if (i == 0 && j > 0 && j <= simplex_size_columna-2)
                            {
                                if (ArraysimplexOfficial[i][j] < 0)
                                {
                                    exist_negativos_of = false;
                                }
                            }
                            
                        }
                    }

                    ++cont_iteracion;//SE INCREMENTA EL NUMERO DE LA ITERACION

                }                   
                    
            }

            else{
                


                    //IMPRESION DE DATOS//
                    printf("\x1B[31m");
       for (int i = 0; i < 1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 2; i++)
            {
                if (i == 0) printf("            %c", var_table_head[i]);
                else if(i == (1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 1)) printf("        %c", var_table_head[i]);
                else if (i % 2 == 0) printf("%c", var_table_head[i]);
                else printf("        %c", var_table_head[i]);
            }
                    printf("\n");
                    printf("\x1B[37m"); // #COLOR-VISUAL

                    cont__position__base = 0;
                    for (int i = 0; i < simplex_size_fila; i++){ // FILAS

                            for (int j = 0; j < simplex_size_columna; j++){ // COLUMNAS

                                if (j == 0){
                                    
                                    printf("\x1B[31m"); // #COLOR-VISUAL
                                    if (i == 0 && j == 0)
                                        printf("Ro(C)     "); // #VISUAL - VARIABLES VERTICALES
                                    else
                                        printf("R%d        ", i); // #VISUAL - VARIABLES VERTICALES
                                    printf("\x1B[37m");           // #COLOR-VISUAL

                                    printf("%.2f", ArraysimplexOfficial[i][j]);
                                    
                                }

                                else{
                                    
                                    if(ArraysimplexOfficial[i][j] <= -1000000) printf("%.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] <= -100000) printf("%.2f", ArraysimplexOfficial[i][j]);// #VISUAL
                                    else if(ArraysimplexOfficial[i][j] <= -10000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -1000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -100) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if (ArraysimplexOfficial[i][j] < 0)  printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000000) printf("  %.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] >= 100000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 100) printf("    %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else                              printf("      %.2f", ArraysimplexOfficial[i][j]);
                                }
                            }

                            //IMPRESION DE LAS VARIABLES EN LA BASE
                            printf("\x1B[33m");// #COLOR-VISUAL
                            if (i == 0) printf("      %c", var_table_base[i]);
                            else if(i > 0){
                                printf("      %c%c", var_table_base[i + cont__position__base], var_table_base[i + cont__position__base + 1]);
                                ++cont__position__base;
                            }
                            
                            printf("\x1B[37m"); // #COLOR-VISUAL
                            printf("\n");
                        }


                    printf("\n\n");
                    printf("\x1B[36m"); // #COLOR-VISUAL
                    printf(":========ITERACION %d / RESULTADO========: \n\n", cont_iteracion);
                    printf("\x1B[37m"); // #COLOR-VISUAL

                    //IMPRESION DE DATOS//
                    printf("\x1B[31m");
       for (int i = 0; i < 1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 2; i++)
            {
                if (i == 0) printf("            %c", var_table_head[i]);
                else if(i == (1 + (nvariablesx + nrestricciones + cont_var_exceso_negative_holgura_artificial + cont_nvariablex_sin_restriccion_logica)*2 + 1)) printf("        %c", var_table_head[i]);
                else if (i % 2 == 0) printf("%c", var_table_head[i]);
                else printf("        %c", var_table_head[i]);
            }
                    printf("\n");
                    printf("\x1B[37m"); // #COLOR-VISUAL

                    cont__position__base = 0;
                    for (int i = 0; i < simplex_size_fila; i++){ // FILAS

                            for (int j = 0; j < simplex_size_columna; j++){ // COLUMNAS

                                if (j == 0){
                                    
                                    printf("\x1B[31m"); // #COLOR-VISUAL
                                    if (i == 0 && j == 0)
                                        printf("Ro(C)     "); // #VISUAL - VARIABLES VERTICALES
                                    else
                                        printf("R%d        ", i); // #VISUAL - VARIABLES VERTICALES
                                    printf("\x1B[37m");           // #COLOR-VISUAL

                                    printf("%.2f", ArraysimplexOfficial[i][j]);
                                    
                                }

                                else{
                                    
                                    if(ArraysimplexOfficial[i][j] <= -1000000) printf("%.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] <= -100000) printf("%.2f", ArraysimplexOfficial[i][j]);// #VISUAL
                                    else if(ArraysimplexOfficial[i][j] <= -10000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -1000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -100) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] <= -10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if (ArraysimplexOfficial[i][j] < 0)  printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000000) printf("  %.2e", ArraysimplexOfficial[i][j]);// #VISUA
                                    else if(ArraysimplexOfficial[i][j] >= 100000) printf(" %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10000) printf("  %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 1000) printf("   %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 100) printf("    %.2f", ArraysimplexOfficial[i][j]);
                                    else if(ArraysimplexOfficial[i][j] >= 10) printf("     %.2f", ArraysimplexOfficial[i][j]);
                                    else                              printf("      %.2f", ArraysimplexOfficial[i][j]);
                                }
                            }

                            //IMPRESION DE LAS VARIABLES EN LA BASE
                            printf("\x1B[33m");// #COLOR-VISUAL
                            if (i == 0) printf("      %c", var_table_base[i]);
                            else if(i > 0){
                                printf("      %c%c", var_table_base[i + cont__position__base], var_table_base[i + cont__position__base + 1]);
                                ++cont__position__base;
                            }
                            
                            printf("\x1B[37m"); // #COLOR-VISUAL
                            printf("\n");
                        }


                exist_negativos_of = true;
            }


    } while (exist_negativos_of == false);


    //----------------------------------------------------------//
    //-------------------//




    printf("\n\n");
    printf("\x1B[32m"); // #COLOR-VISUAL
    printf(":========RESULTADOS FINALES========:\n\n\n");
    printf("\x1B[37m"); // #COLOR-VISUAL




    bool is_no_factible = false;

    if (is_no_acotada == true) {//EN CASO DE QUE SEA ACOTADA SE IMPRIMIRA EL SIGUIENTE MENSAJE
        printf("\x1B[41m"); // #COLOR-VISUAL
        printf("TIPO DE SOLUCION / [ NO ACOTADA ]");
        printf("\x1B[0m"); // #COLOR-VISUAL
    }
    else{//DE LO CONTRARIO TODAVIA ESTA LA POSIBILIDAD DE QUE SEA NO FACTIBLE, OPTIMA U OPTIMA MULTIPLE


        for (int i = 0; i < simplex_size_fila; i++)
            {
                for (int j = 0; j < simplex_size_columna; j++)
                {
                    if (i != 0 && j == simplex_size_columna - 1)
                    {

                        if (var_table_base[(i*2)-1] == 'A')
                        {
                            if (ArraysimplexOfficial[i][j] > 0)
                            {
                                is_no_factible = true;
                            }
                            
                        }
                        
                    }
                }
            }




            if (is_no_factible == true) {//EN CASO DE QUE SEA ACOTADA SE IMPRIMIRA EL SIGUIENTE MENSAJE
                printf("\x1B[43m"); // #COLOR-VISUAL
                printf("TIPO DE SOLUCION / [ NO FACTIBLE ]");
                printf("\x1B[0m"); // #COLOR-VISUAL
            }
            else{//AHORA SOLO CABE LA POSIBILIDAD SI ES OPTIMA U OPTIMA MULTIPLE

                bool is_optima = false;

                for (int i = 0; i < simplex_size_fila; i++)
                {
                    for (int j = 0; j < simplex_size_columna; j++)
                    {
                        if (i == 0)//PRIMERA FILA
                        {
                            if (j > 0 && j < simplex_size_columna-1)//DETERMINAR DE A QUE COLUMNA A QUE COLUMNA DE ANALIZARA, MOTIENDO LA PRIMERA Y ULTIMA
                            {

                                if (round(ArraysimplexOfficial[i][j]) == 0 && ArraysimplexOfficial[i][j]<0.01)
                                {   
                                    // ArraysimplexOfficial[i][j] = round(ArraysimplexOfficial[i][j]);
                                    // printf("   %.20f  ", ArraysimplexOfficial[i][j]);
                                     for (int z = 1; z < simplex_size_fila; z++)
                                    {   

                                        if (ArraysimplexOfficial[z][j] != 1 && ArraysimplexOfficial[z][j] != 0)//SI SON DIFERENTES DE 0 O 1 ENTONCES ES OPTIMA MULTIPLE
                                        {
                                            // printf("\nColumna: %d | Fila: %d | Resultado: %.20f\n", j, z, ArraysimplexOfficial[z][j]);
                                            
                                            is_optima = true;
                                            z = simplex_size_fila;//ESTO ES PARA PARAR TODOS LOS CICLOS
                                            j = simplex_size_columna;//ESTO ES PARA PARAR TODOS LOS CICLOS
                                            i = simplex_size_columna;//ESTO ES PARA PARAR TODOS LOS CICLOS
                                        }




                                        
                                    }
                                }
                      
                            }
                            
                        }
                        
                    }
                    
                }

                printf("\n");
                
                if (is_optima == true)
                {
                    printf("\x1B[44m"); // #COLOR-VISUAL
                    printf("TIPO DE SOLUCION / [ OPTIMA MULTIPLE ]");
                    printf("\x1B[0m"); // #COLOR-VISUAL
                }
                else{
                    printf("\x1B[42m"); // #COLOR-VISUAL
                    printf("TIPO DE SOLUCION / [ OPTIMA ]");
                    printf("\x1B[0m"); // #COLOR-VISUAL
                }


            //ALGORITMO PARA IMPRIMIR LOS RESULTADOS DE LAS VARIABLES EN LA BASE
                printf("\x1B[33m"); // #COLOR-VISUAL
                printf("\n\n- VARIABLES EN LA BASE:");
                printf("\x1B[37m"); // #COLOR-VISUAL
                for (int i = 0; i < simplex_size_fila; i++)
                {
                    for (int j = 0; j < simplex_size_columna; j++)
                    {
                        if (i != 0 && j == simplex_size_columna - 1)
                        {
                            printf("\x1B[33m"); // #COLOR-VISUA
                            printf("- %c%c = ", var_table_base[(i*2)-1],var_table_base[(i*2)]);
                            printf("\x1B[37m"); // #COLOR-VISUAL
                            printf("%.2f", ArraysimplexOfficial[i][j]);
                            if (var_table_base[(i*2)-1] == 'X')
                            {
                                printf("\x1B[33m"); // #COLOR-VISUA
                                printf(" = ");
                                printf("\x1B[37m"); // #COLOR-VISUAL
                                if (ArraysimplexOfficial[i][j] == 0)  printf("%.2f", ArraysimplexOfficial[i][j]);
                                else  printf("%.2f", ArraysimplexOfficial[i][j] * -1);
                                
                                
                            }
                            
                            
                        }
                    }
                    printf("\n");
                }

                //IMPRIMIR LA FUNCION OBJETIVO RESULTANTE (F.O)

                printf("\x1B[33m"); // #COLOR-VISUAL
                printf("\n- FUNCION OBJETIVO:");

                
                printf("\n\n"); 
                if (maxormin == 1) printf("- min z = ");   
                else printf("- max z = ");
                printf("\x1B[37m"); // #COLOR-VISUAL

                for (int i = 0; i < nvariablesx; i++)
                {

                    printf("(%.2f)x%d", impresion_original_Arrayfo[i], i + 1);
                    if (i < nvariablesx - 1)printf(" + ");

                    if (nvariablex_restriccion_logica[i] == 2){
                        
                        if (i == nvariablesx - 1)printf(" + ");
                        printf("(%.2f)X%d_vi", impresion_original_Arrayfo[i] * -1, i + 1);
                        if (i < nvariablesx - 1)printf(" + ");

                    }

                }
                
                printf(" = ");


                if (maxormin == 1) {
                    if (ArraysimplexOfficial[0][simplex_size_columna-1] == 0)printf("%.2f", ArraysimplexOfficial[0][simplex_size_columna-1]);
                    else printf("%.2f", ArraysimplexOfficial[0][simplex_size_columna-1] * -1);
                }
                else printf("%.2f", ArraysimplexOfficial[0][simplex_size_columna-1]);  

                //----------------------------------------------------------//
                //-------------------//

            }
    }
    printf("\n\n\n");
    printf("\x1B[33m"); // #COLOR-VISUAL
    printf(":========:¡GRACIAS!:========:");
    printf("\x1B[0m"); // #COLOR-VISUAL
    getch();
    return 0;
}