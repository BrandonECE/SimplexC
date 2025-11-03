import os
import math
import sys

# Configuración para colores ANSI
class Colors:
    RED = '\033[31m'
    GREEN = '\033[32m'
    YELLOW = '\033[33m'
    BLUE = '\033[34m'
    MAGENTA = '\033[35m'
    CYAN = '\033[36m'
    WHITE = '\033[37m'
    RESET = '\033[0m'
    
    BG_RED = '\033[41m'
    BG_GREEN = '\033[42m'
    BG_YELLOW = '\033[43m'
    BG_BLUE = '\033[44m'
    BG_MAGENTA = '\033[45m'
    BG_CYAN = '\033[46m'

def clear_screen():
    """Limpia la pantalla de la terminal"""
    os.system('cls' if os.name == 'nt' else 'clear')

def leer_entero(mensaje, min_val, max_val):
    """Lee y valida un número entero dentro de un rango específico"""
    while True:
        clear_screen()
        print(mensaje, end='')
        try:
            valor = int(input().strip())
            if min_val <= valor <= max_val:
                return valor
            else:
                print(f"Error: El valor debe estar entre {min_val} y {max_val}.")
        except ValueError:
            print(f"Error: Entrada no válida.")
        
        print("Presione Enter para continuar...")
        input()

def leer_double(mensaje):
    """Lee y valida un número decimal"""
    while True:
        clear_screen()
        print(mensaje, end='')
        try:
            valor = float(input().strip())
            return valor
        except ValueError:
            print(f"Error: Entrada no válida.")
        
        print("Presione Enter para continuar...")
        input()

def main():
    # DECLARACION DE LAS PRIMERAS VARIABLES
    maxormin = 0  # 1 para minimizar, 2 para maximizar
    nrestricciones = 0  # Número de restricciones físicas
    nvariablesx = 0  # Número de variables de decisión

    # NUMERO DE VARIABLES DE DECISION (VALIDADO)
    nvariablesx = leer_entero("Ingrese la cantidad de variables de decision que desea: ", 1, 100)

    # Arrays para la función objetivo
    Arrayfo = [0.0] * nvariablesx  # Coeficientes de la función objetivo
    impresion_original_Arrayfo = [0.0] * nvariablesx  # Copia para impresión final

    # ENTRADA DE DATOS DE LA FUNCION OBJETIVO (VALIDADO)
    for i in range(nvariablesx):
        mensaje = f"Ingresa x{i+1} de Funcion Objetivo (F.O): "
        Arrayfo[i] = leer_double(mensaje)
        impresion_original_Arrayfo[i] = Arrayfo[i]

    # MAXIMIZAR O MINIMIZAR (VALIDADO)
    mensaje_maxmin = (
        "Ingrese para su Funcion Objetivo(F.O):\n\n"
        "Presiona 1 si deseas MINIMIZAR\n"
        "Presiona 2 si deseas MAXIMIZAR\n\n: "
    )
    maxormin = leer_entero(mensaje_maxmin, 1, 2)

    # ENTRADA DE DATOS DE LAS RESTRICCIONES FISICAS (VALIDADO)
    nrestricciones = leer_entero("Ingrese cuantas restricciones fisica desea: ", 1, 100)

    # Matriz de restricciones: [coeficientes, tipo_restriccion, valor_limitante]
    Arrayrestricciones = [[0.0] * (nvariablesx + 2) for _ in range(nrestricciones)]
    base = [0] * nrestricciones  # Tipo de restricción para cada fila

    for i in range(nrestricciones):
        # Coeficientes de las variables en la restricción
        for j in range(nvariablesx):
            mensaje = f"Ingresa x{j+1} de la restriccion N.{i+1}: "
            Arrayrestricciones[i][j] = leer_double(mensaje)

        # Tipo de limitante (<=, >=, =)
        mensaje_tipo = (
            "Ingrese que tipo de limitante es:\n\n"
            "Presiona 1 si tu limitante es: <=\n"
            "Presiona 2 si tu limitante es: >=\n"
            "Presiona 3 si tu limitante es: =\n\n: "
        )
        Arrayrestricciones[i][nvariablesx] = leer_entero(mensaje_tipo, 1, 3)
        base[i] = Arrayrestricciones[i][nvariablesx]

        # Valor limitante de la restricción
        mensaje = f"Ingresa el valor limitante de la restriccion N.{i+1}: "
        Arrayrestricciones[i][nvariablesx + 1] = leer_double(mensaje)

    # ENTRADA DE DATOS DE LAS RESTRICCIONES LOGICAS (VALIDADO)
    mensaje_logica = (
        "Eliga sus restricciones logicas:\n\n"
        "Desea que todas sus variables sean >= 0 ?\n"
        "\nPresiona 1 --> SI\nPresiona 2 --> NO\n\n: "
    )
    decision_restriccion_logica = leer_entero(mensaje_logica, 1, 2)

    cont_nvariablex_sin_restriccion_logica = 0  # Contador de variables sin restricción
    nvariablex_restriccion_logica = [0] * nvariablesx  # Restricciones lógicas por variable

    if decision_restriccion_logica == 1:
        # Todas las variables con restricción >= 0
        for i in range(nvariablesx):
            nvariablex_restriccion_logica[i] = 1
    else:
        # Preguntar restricción para cada variable individualmente
        for i in range(nvariablesx):
            mensaje = (
                f"Eliga sus restricciones logicas:\n\n"
                f"[ x{i+1} ] >= 0 ?\n"
                f"\nPresiona 1 --> CON RESTRICCION\n"
                f"Presiona 2 --> SIN RESTRICCION\n\n: "
            )
            nvariablex_restriccion_logica[i] = leer_entero(mensaje, 1, 2)
            
            if nvariablex_restriccion_logica[i] == 2:
                cont_nvariablex_sin_restriccion_logica += 1

    # VARIABLES PARA POSICIONES
    position_nvariablex_restriccion_logica = [0] * (nvariablesx + cont_nvariablex_sin_restriccion_logica)
    cont_vi_indice_position = 0
    cont_vi_indice_position_bool = False

    # FUNCION OBJETIVO - IMPRESIÓN
    clear_screen()
    print(f"{Colors.YELLOW}:========DATOS========: {Colors.RESET}\n\n")
    print(f"{Colors.WHITE}Funcion Objetivo(F.O): {Colors.RESET}\n\n")

    # Mostrar si es minimizar o maximizar
    if maxormin == 1: 
        print("- min z = ", end='')  
    else: 
        print("- max z = ", end='')

    # Mostrar coeficientes de la función objetivo
    for i in range(nvariablesx):
        print(f"({Arrayfo[i]:.2f})x{i+1}", end='')
        if i < nvariablesx - 1:
            print(" + ", end='')

        # Algoritmo para determinar posiciones de variables Vi
        if cont_vi_indice_position_bool == True:
            cont_vi_indice_position += 1
        if cont_vi_indice_position_bool == False and i != 0:
            cont_vi_indice_position += 1
        position_nvariablex_restriccion_logica[cont_vi_indice_position] = 1
        cont_vi_indice_position_bool = False

        # Si la variable no tiene restricción, agregar variable Vi
        if nvariablex_restriccion_logica[i] == 2:
            if i == nvariablesx - 1:
                print(" + ", end='')
            print(f"({Arrayfo[i] * -1:.2f})X{i+1}_vi", end='')
            if i < nvariablesx - 1:
                print(" + ", end='')

            cont_vi_indice_position_bool = True
            cont_vi_indice_position += 1
            position_nvariablex_restriccion_logica[cont_vi_indice_position] = 2

    print("\n\n")

    # RESTRICCIONES FISICAS - IMPRESIÓN
    print("Restricciones Fisicas: \n\n")

    for i in range(nrestricciones):
        print(f"- Restriccion N.{i+1}: ", end='')
        for j in range(nvariablesx):
            print(f"({Arrayrestricciones[i][j]:.2f})x{j+1}", end='')
            if j < nvariablesx - 1: 
                print(" + ", end='')

            # Agregar variables Vi si no tienen restricción
            if nvariablex_restriccion_logica[j] == 2:
                if j == nvariablesx - 1:
                    print(" + ", end='')
                print(f"({Arrayrestricciones[i][j] * -1:.2f})X{j+1}_vi", end='')
                if j < nvariablesx - 1:
                    print(" + ", end='')
            
            # Mostrar tipo de restricción al final
            if j == nvariablesx - 1:
                if Arrayrestricciones[i][nvariablesx] == 1: 
                    print(" <= ", end='')
                if Arrayrestricciones[i][nvariablesx] == 2:  
                    print(" >= ", end='')
                if Arrayrestricciones[i][nvariablesx] == 3:  
                    print(" = ", end='')
        
        print(f"{Arrayrestricciones[i][nvariablesx + 1]:.2f}", end='')
        print()

    # RESTRICCION LOGICA - IMPRESIÓN
    print("\nRestricciones logicas: \n\n")
    print("- Restriccion: ", end='')
    
    cont_comas_rl = nvariablesx - cont_nvariablex_sin_restriccion_logica

    # Mostrar variables con restricción >= 0
    first = True
    for i in range(nvariablesx):
        if nvariablex_restriccion_logica[i] == 1:
            if not first:
                print(", ", end='')
            print(f"x{i+1}", end='')
            first = False
            cont_comas_rl -= 1
    
    print(" >= 0")

    # ARRAY BIDIMENSIONAL SIMPLEX
    simplex_cols = 1 + cont_nvariablex_sin_restriccion_logica + nvariablesx + nrestricciones + 1
    simplex_rows = 1 + nrestricciones
    
    Arraysimplex = [[0.0] * simplex_cols for _ in range(simplex_rows)]

    # PROCESO / INTRODUCIR DATOS
    ajustadorse = 0  # Ajustador para variables de holgura/exceso
    ajustadorx = 0   # Ajustador para variables X
    n = nvariablesx + cont_nvariablex_sin_restriccion_logica
    cont = 1

    # AUTOAJUSTADOR DE VARIABLES X
    if n > 2:
        while n != 2:
            n -= 1
            ajustadorx += 1
    elif n == 1:
        ajustadorx = -1

    n = nrestricciones

    # AUTOAJUSTADOR DE VARIABLES DE HOLGURA Y EXCESO
    if n > 2:
        while n != 2:
            n -= 1
            ajustadorse += 1
    elif n == 1:
        ajustadorse = -1

    # AJUSTE DEPENDIENDO DE LA FUNCION OBJETIVO
    # Para minimizar, se cambia el signo de los coeficientes
    if maxormin == 1:
        for i in range(nvariablesx):
            if Arrayfo[i] != 0:
                Arrayfo[i] = Arrayfo[i] * -1

    # DECLARACION DE VARIABLES CON LOS TAMAÑOS DE LA MATRIZ
    simplex_size_fila = 1 + nrestricciones
    simplex_size_columna = 1 + cont_nvariablex_sin_restriccion_logica + nvariablesx + nrestricciones + 1

    # GENERADOR DE LA TABLA SIMPLEX CON SUS VALORES POSICIONADOS
    for i in range(simplex_size_fila):
        cont_ajuste_prueba = 0
        j = 0
        while j < simplex_size_columna:
            if i == 0:  # Fila de la función objetivo
                if i == 0 and j == 0:
                    Arraysimplex[i][j] = 1  # Columna Z
                elif i == 0 and j <= nvariablesx + cont_nvariablex_sin_restriccion_logica:
                    if Arrayfo[(j - 1) - cont_ajuste_prueba] == 0: 
                        Arraysimplex[i][j] = Arrayfo[(j - 1) - cont_ajuste_prueba]
                    else: 
                        Arraysimplex[i][j] = (Arrayfo[(j - 1) - cont_ajuste_prueba]) * -1

                    # Manejar variables sin restricción (Vi)
                    if nvariablex_restriccion_logica[(j - 1) - cont_ajuste_prueba] == 2:
                        j += 1
                        cont_ajuste_prueba += 1

                        if Arrayfo[(j - 1) - cont_ajuste_prueba] == 0: 
                            Arraysimplex[i][j] = Arrayfo[(j - 1) - cont_ajuste_prueba]
                        else: 
                            Arraysimplex[i][j] = ((Arrayfo[(j - 1) - cont_ajuste_prueba]) * -1) * -1
                else:
                    Arraysimplex[i][j] = 0
            else:  # Filas de restricciones
                if j == 0:
                    Arraysimplex[i][j] = 0  # Columna Z
                elif j <= (nvariablesx + cont_nvariablex_sin_restriccion_logica):
                    Arraysimplex[i][j] = Arrayrestricciones[i - 1][(j - 1) - cont_ajuste_prueba]

                    # Manejar variables sin restricción (Vi)
                    if nvariablex_restriccion_logica[(j - 1) - cont_ajuste_prueba] == 2:
                        j += 1
                        cont_ajuste_prueba += 1

                        if Arrayrestricciones[i - 1][(j - 1) - cont_ajuste_prueba] == 0: 
                            Arraysimplex[i][j] = Arrayrestricciones[i - 1][(j - 1) - cont_ajuste_prueba]
                        else: 
                            Arraysimplex[i][j] = (Arrayrestricciones[i - 1][(j - 1) - cont_ajuste_prueba]) * -1
                elif j > nvariablesx + cont_nvariablex_sin_restriccion_logica and j <= (nvariablesx + nrestricciones + cont_nvariablex_sin_restriccion_logica):
                    # Variables de holgura, exceso y artificiales
                    if i == cont and j == (ajustadorx + (nrestricciones - ajustadorse)) + cont:
                        if Arrayrestricciones[i - 1][nvariablesx] == 1: 
                            Arraysimplex[i][j] = 1  # Holgura
                        elif Arrayrestricciones[i - 1][nvariablesx] == 2: 
                            Arraysimplex[i][j] = -1  # Exceso
                        elif Arrayrestricciones[i - 1][nvariablesx] == 3: 
                            Arraysimplex[i][j] = 1  # Artificial
                        cont += 1
                    else:
                        Arraysimplex[i][j] = 0
                else:
                    # Columna de valores b (lado derecho)
                    Arraysimplex[i][j] = Arrayrestricciones[i - 1][nvariablesx + 1]
            j += 1

    # SE AÑADIRAN EN LA F.O LAS ARTIFICIALES POR DEFECTO
    valor_artificial = 100  # Valor M grande para variables artificiales

    for i in range(simplex_size_fila):
        for j in range(simplex_size_columna):
            if i == 0 and j >= 1 + nvariablesx + cont_nvariablex_sin_restriccion_logica and j < simplex_size_columna:
                idx_base = j - (1 + nvariablesx + cont_nvariablex_sin_restriccion_logica)
                if idx_base >= 0 and idx_base < nrestricciones and base[idx_base] == 3:
                    Arraysimplex[i][j] = valor_artificial

    # SE DETERMINARA SI EXISTE VARIABLES CON VALORES NEGATIVOS
    exist_valores_negativos = [0] * nrestricciones
    for i in range(nrestricciones):
        for j in range(nvariablesx + 2):
            if j == (nvariablesx + 2) - 1:
                if Arrayrestricciones[i][j] < 0:
                    exist_valores_negativos[i] = 1
                else:
                    exist_valores_negativos[i] = 0

    is_artificial_negativa = False

    # Ajustar signos para valores negativos en restricciones
    for i in range(simplex_size_fila):
        for j in range(simplex_size_columna):
            if i != 0:
                if i-1 < len(exist_valores_negativos) and exist_valores_negativos[i-1] == 1:
                    if i-1 < len(base) and base[i-1] == 3:
                        if j >= (1 + nvariablesx) and j < simplex_size_columna - 1:
                            if Arraysimplex[i][j] == 1:
                                Arraysimplex[i][j] = Arraysimplex[i][j] * -1
                                is_artificial_negativa = True
                    else:
                        if Arraysimplex[i][j] != 0: 
                            Arraysimplex[i][j] = Arraysimplex[i][j] * -1
                        else: 
                            Arraysimplex[i][j] = 0

    # Ajuste adicional para artificiales negativas
    if is_artificial_negativa == True:
        for i in range(simplex_size_fila):
            for j in range(simplex_size_columna):
                if i != 0:
                    if i-1 < len(exist_valores_negativos) and exist_valores_negativos[i-1] == 1:
                        if i-1 < len(base) and base[i-1] == 3:
                            if Arraysimplex[i][j] != 0: 
                                Arraysimplex[i][j] = Arraysimplex[i][j] * -1
                            else: 
                                Arraysimplex[i][j] = 0

    # FORMA DE MATRIZ Y LA BASE
    print("\n\n")
    print(f"{Colors.YELLOW}:========BASE========: {Colors.RESET}\n\n")
    print(f"{Colors.GREEN}- COLUMNA DE HOLGURA  {Colors.RESET}")
    print(f"{Colors.RED}- COLUMNA DE EXCESO   {Colors.RESET}")
    print(f"{Colors.CYAN}- COLUMNA DE ARTIFICIAL{Colors.RESET}")
    print(f"{Colors.MAGENTA}- COLUMNA DE X's ViPrimas{Colors.RESET}")
    print(f"{Colors.YELLOW}- COLUMNA DE ARITIFICIALES AUTOMATICAS{Colors.RESET}\n\n")

    print(f"{Colors.GREEN}", end='')

    # Matriz identidad para la base
    basearray = [[0] * (nrestricciones + 1) for _ in range(nrestricciones + 1)]

    for i in range(nrestricciones + 1):
        for j in range(nrestricciones + 1):
            if i == j:
                basearray[i][j] = 1
                print(f"  {basearray[i][j]}", end='')
            else:
                basearray[i][j] = 0
                print(f"  {basearray[i][j]}", end='')
        print()
    
    print(f"{Colors.RESET}")

    print("\n  |||")
    print("  vvv")
    print()

    # Mostrar tabla simplex con colores
    for i in range(simplex_size_fila):
        for j in range(simplex_size_columna):
            if j == 0:
                print(f"{Colors.GREEN}  {Arraysimplex[i][j]:.2f}{Colors.RESET}", end='')
            elif j > nvariablesx + cont_nvariablex_sin_restriccion_logica and j < simplex_size_columna - 1:
                idx_base = j - (nvariablesx + cont_nvariablex_sin_restriccion_logica + 1)
                if idx_base >= 0 and idx_base < len(base) and base[idx_base] == 1:
                    print(f"{Colors.GREEN}", end='')
                    if Arraysimplex[i][j] < 0:
                        print(f" {Arraysimplex[i][j]:.2f}", end='')
                    else:
                        print(f"  {Arraysimplex[i][j]:.2f}", end='')
                    print(f"{Colors.RESET}", end='')
                elif idx_base >= 0 and idx_base < len(base) and base[idx_base] == 3:
                    print(f"{Colors.CYAN}", end='')
                    if Arraysimplex[i][j] < 0:
                        print(f" {Arraysimplex[i][j]:.2f}", end='')
                    else:
                        print(f"  {Arraysimplex[i][j]:.2f}", end='')
                    print(f"{Colors.RESET}", end='')
                else:
                    print(f"{Colors.RED}", end='')
                    if Arraysimplex[i][j] < 0:
                        print(f" {Arraysimplex[i][j]:.2f}", end='')
                    else:
                        print(f"  {Arraysimplex[i][j]:.2f}", end='')
                    print(f"{Colors.RESET}", end='')
            else:
                if j != simplex_size_columna - 1:
                    if j-1 < len(position_nvariablex_restriccion_logica) and position_nvariablex_restriccion_logica[j-1] == 2:
                        print(f"{Colors.MAGENTA}", end='')
                if Arraysimplex[i][j] < 0 or Arraysimplex[i][j] >= 10:
                    print(f" {Arraysimplex[i][j]:.2f}", end='')
                elif Arraysimplex[i][j] >= 100:
                    print(f"{Arraysimplex[i][j]:.2f}", end='')
                else:
                    print(f"  {Arraysimplex[i][j]:.2f}", end='')
                print(f"{Colors.RESET}", end='')
        print()

    print("\n  |||")
    print("  vvv")
    print()

    # Mostrar base con colores
    for i in range(nrestricciones + 1):
        for j in range(nrestricciones + 1):
            if j == 0:
                print(f"{Colors.GREEN}  {basearray[i][j]}{Colors.RESET}", end='')
            else:
                if j-1 < len(base):
                    if base[j-1] == 1:
                        print(f"{Colors.GREEN}", end='')
                        print(f"  {basearray[i][j]}", end='')
                        print(f"{Colors.RESET}", end='')
                    elif base[j-1] == 3:
                        print(f"{Colors.CYAN}", end='')
                        print(f"  {basearray[i][j]}", end='')
                        print(f"{Colors.RESET}", end='')
                    else:
                        print(f"{Colors.RED}", end='')
                        print(f"  {basearray[i][j]}", end='')
                        print(f"{Colors.RESET}", end='')
                else:
                    print(f"  {basearray[i][j]}", end='')
        print()

    # Recalcular existencia de valores negativos
    for i in range(nrestricciones):
        for j in range(nvariablesx + 2):
            if j == (nvariablesx + 2) - 1:
                if Arrayrestricciones[i][j] < 0:
                    exist_valores_negativos[i] = 1
                else:
                    exist_valores_negativos[i] = 0

    # NUEVO SIMPLEX SI ES QUE EXISTEN VARIABLES ARTIFICIALES O NO
    cont_var_artifical = 0
    exist_artificial = False

    # Contar variables artificiales necesarias
    for i in range(nrestricciones):
        if (base[i] == 2 and exist_valores_negativos[i] == 0) or (base[i] == 1 and exist_valores_negativos[i] == 1):
            cont_var_artifical += 1
            exist_artificial = True

    simplex_size_columna_forma_estandar = simplex_size_columna
    ArraysimplexOfficial = [[0.0] * (simplex_size_columna + cont_var_artifical) for _ in range(simplex_size_fila)]

    if cont_var_artifical == 0:
        # Sin variables artificiales, copiar directamente
        for i in range(simplex_size_fila):
            for j in range(simplex_size_columna):
                ArraysimplexOfficial[i][j] = Arraysimplex[i][j]
    else:
        # Con variables artificiales, expandir la matriz
        columan_artificial = nvariablesx + nrestricciones + cont_nvariablex_sin_restriccion_logica
        
        for i in range(simplex_size_fila):
            find = False
            change = 0.0
            for j in range(simplex_size_columna + cont_var_artifical):
                if j >= (1 + nvariablesx + cont_nvariablex_sin_restriccion_logica + nrestricciones) and j < (simplex_size_columna + cont_var_artifical - 1):
                    if j == 1 + nvariablesx + cont_nvariablex_sin_restriccion_logica + nrestricciones: 
                        change = Arraysimplex[i][j]

                    if i == 0:
                        ArraysimplexOfficial[i][j] = valor_artificial
                    else:
                        if find == True and j == columan_artificial:
                            ArraysimplexOfficial[i][j] = 1
                            find = False
                        else: 
                            ArraysimplexOfficial[i][j] = 0
                elif j == simplex_size_columna + cont_var_artifical - 1:
                    ArraysimplexOfficial[i][j] = change
                else:
                    ArraysimplexOfficial[i][j] = Arraysimplex[i][j]

                    if i > 0 and j >= 1 + nvariablesx + cont_nvariablex_sin_restriccion_logica and j < 1 + cont_nvariablex_sin_restriccion_logica + nvariablesx + nrestricciones:
                        if Arraysimplex[i][j] == -1:
                            columan_artificial += 1
                            find = True
            
    simplex_size_columna += cont_var_artifical

    # FORMA DE MATRIZ Y LA BASE CON ARTIFICIALES
    if exist_artificial == True:
        print("\n")
        print("  |||")
        print("  vvv")
        print("\n")

        for i in range(simplex_size_fila):
            for j in range(simplex_size_columna):
                if j == 0:
                    print(f"{Colors.GREEN}  {ArraysimplexOfficial[i][j]:.2f}{Colors.RESET}", end='')
                elif j > nvariablesx + cont_nvariablex_sin_restriccion_logica and j < simplex_size_columna - 1:
                    idx_base = j - (nvariablesx + cont_nvariablex_sin_restriccion_logica + 1)
                    if idx_base >= 0 and idx_base < len(base):
                        if base[idx_base] == 1: 
                            print(f"{Colors.GREEN}", end='')
                        elif base[idx_base] == 3: 
                            print(f"{Colors.CYAN}", end='')
                        else: 
                            print(f"{Colors.RED}", end='')    
                    if j >= simplex_size_columna - cont_var_artifical - 1 and j < simplex_size_columna: 
                        print(f"{Colors.YELLOW}", end='')

                    if ArraysimplexOfficial[i][j] < 0:
                        print(f" {ArraysimplexOfficial[i][j]:.2f}", end='')
                    else:
                        print(f"  {ArraysimplexOfficial[i][j]:.2f}", end='')  
                    print(f"{Colors.RESET}", end='')
                else:
                    if j != simplex_size_columna - 1:
                        if j-1 < len(position_nvariablex_restriccion_logica) and position_nvariablex_restriccion_logica[j-1] == 2:
                            print(f"{Colors.MAGENTA}", end='')
                 
                    if ArraysimplexOfficial[i][j] < 0 or ArraysimplexOfficial[i][j] >= 10:
                        print(f" {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] >= 100:
                        print(f"{ArraysimplexOfficial[i][j]:.2f}", end='')
                    else:
                        print(f"  {ArraysimplexOfficial[i][j]:.2f}", end='') 
                    print(f"{Colors.RESET}", end='')
            print()

    # IMPRESION DE DATOS DE TABLA SIMPLEX A MODO ESTANDAR
    print("\n\n")
    print(f"{Colors.YELLOW}:========FORMA ESTANDAR========: {Colors.RESET}\n\n")
    print(f"{Colors.WHITE}", end='')

    cont = 1
    varholguraexceso = 1
    cont_var_artifical = 0
    cont_estandar_x = 1

    for i in range(simplex_size_fila):
        cont_estandar_x = 1
        for j in range(simplex_size_columna_forma_estandar):
            if i == 0 and j >= 1 and j <= nvariablesx + cont_nvariablex_sin_restriccion_logica:
                if j == 1:
                    if maxormin == 1: 
                        print("   min z   ", end='')
                    else: 
                        print("   max z   ", end='')

                if j-1 < len(position_nvariablex_restriccion_logica):
                    if position_nvariablex_restriccion_logica[j-1] == 1:
                        print(f"({Arraysimplex[i][j]:.2f})x{cont_estandar_x}", end='')
                        if j != nvariablesx + cont_nvariablex_sin_restriccion_logica and Arraysimplex[i][j] < 0: 
                            print("  + ", end='')
                        if j != nvariablesx + cont_nvariablex_sin_restriccion_logica and Arraysimplex[i][j] > 0: 
                            print("  +  ", end='')
                        if j != nvariablesx + cont_nvariablex_sin_restriccion_logica and Arraysimplex[i][j] == 0: 
                            print("  +  ", end='')
                        cont_estandar_x += 1
                    else:
                        cont_estandar_x -= 1
                        print(f"({Arraysimplex[i][j]:.2f})X{cont_estandar_x}_vi", end='')
                        if j != nvariablesx + cont_nvariablex_sin_restriccion_logica and Arraysimplex[i][j] < 0: 
                            print("  + ", end='')
                        if j != nvariablesx + cont_nvariablex_sin_restriccion_logica and Arraysimplex[i][j] > 0: 
                            print("  +  ", end='')
                        if j != nvariablesx + cont_nvariablex_sin_restriccion_logica and Arraysimplex[i][j] == 0: 
                            print("  +  ", end='')
                        cont_estandar_x += 1

            elif i > 0 and j >= 1 and j <= nvariablesx + cont_nvariablex_sin_restriccion_logica:
                if j == 1:
                    print("            ", end='')
                if j-1 < len(position_nvariablex_restriccion_logica):
                    if position_nvariablex_restriccion_logica[j-1] == 1:
                        print(f"({Arraysimplex[i][j]:.2f})x{cont_estandar_x}", end='')
                        if j != nvariablesx + cont_nvariablex_sin_restriccion_logica and Arraysimplex[i][j] < 0: 
                            print("  + ", end='')
                        if j != nvariablesx + cont_nvariablex_sin_restriccion_logica and Arraysimplex[i][j] > 0: 
                            print("  +  ", end='')
                        if j != nvariablesx + cont_nvariablex_sin_restriccion_logica and Arraysimplex[i][j] == 0: 
                            print("  +  ", end='')
                        cont_estandar_x += 1
                    else:
                        cont_estandar_x -= 1
                        print(f"({Arraysimplex[i][j]:.2f})X{cont_estandar_x}_vi", end='')
                        if j != nvariablesx + cont_nvariablex_sin_restriccion_logica and Arraysimplex[i][j] < 0: 
                            print("  + ", end='')
                        if j != nvariablesx + cont_nvariablex_sin_restriccion_logica and Arraysimplex[i][j] > 0: 
                            print("  +  ", end='')
                        if j != nvariablesx + cont_nvariablex_sin_restriccion_logica and Arraysimplex[i][j] == 0: 
                            print("  +  ", end='')
                        cont_estandar_x += 1

            elif i > 0 and j > nvariablesx + cont_nvariablex_sin_restriccion_logica:
                if i == cont and j == (ajustadorx + (nrestricciones - ajustadorse)) + cont:
                    if i-1 < len(base) and i-1 < len(exist_valores_negativos):
                        if Arraysimplex[i][j] == 1 and base[i-1] == 1 and exist_valores_negativos[i-1] == 0:
                            if i > 1 and i-2 < len(base) and i-2 < len(exist_valores_negativos):
                                if (base[i-2] == 2 and exist_valores_negativos[i-2] == 0) or (base[i-2] == 1 and exist_valores_negativos[i-2] == 1): 
                                    print(f"     + S{varholguraexceso}", end='')
                                else: 
                                    print(f"       + S{varholguraexceso}", end='')
                            else:
                                print(f"       + S{varholguraexceso}", end='')
                            varholguraexceso += 1

                        elif Arraysimplex[i][j] == -1 and base[i-1] == 2 and exist_valores_negativos[i-1] == 0:
                            cont_var_artifical += 1
                            print(f"     -E{varholguraexceso} + A{cont_var_artifical}", end='') 
                            varholguraexceso += 1

                        elif Arraysimplex[i][j] == 1 and base[i-1] == 3:
                            cont_var_artifical += 1
                            if i > 1 and i-2 < len(base) and i-2 < len(exist_valores_negativos):
                                if (base[i-2] == 2 and exist_valores_negativos[i-2] == 0) or (base[i-2] == 1 and exist_valores_negativos[i-2] == 1): 
                                    print(f"       + A{cont_var_artifical}", end='')
                                else: 
                                    print(f"     + A{cont_var_artifical}", end='') 
                            else:
                                print(f"     + A{cont_var_artifical}", end='')

                        elif Arraysimplex[i][j] == -1 and base[i-1] == 1 and exist_valores_negativos[i-1] == 1:
                            cont_var_artifical += 1
                            if i > 1 and i-2 < len(base) and i-2 < len(exist_valores_negativos):
                                if (base[i-2] == 2 and exist_valores_negativos[i-2] == 0) or (base[i-2] == 1 and exist_valores_negativos[i-2] == 1): 
                                    print(f"       -S{varholguraexceso} + A{cont_var_artifical}", end='')
                                else: 
                                    print(f"     -S{varholguraexceso} + A{cont_var_artifical}", end='')
                            else:
                                print(f"     -S{varholguraexceso} + A{cont_var_artifical}", end='')
                            varholguraexceso += 1

                        elif Arraysimplex[i][j] == 1 and base[i-1] == 2 and exist_valores_negativos[i-1] == 1:
                            if i > 1 and i-2 < len(base) and i-2 < len(exist_valores_negativos):
                                if (base[i-2] == 2 and exist_valores_negativos[i-2] == 0) or (base[i-2] == 1 and exist_valores_negativos[i-2] == 1): 
                                    print(f"       +E{varholguraexceso}", end='')
                                else: 
                                    print(f"     +E{varholguraexceso}", end='') 
                            else:
                                print(f"     +E{varholguraexceso}", end='')
                            varholguraexceso += 1

                        else: 
                            print("      ", end='')
                    else:
                        print("      ", end='')
                    cont += 1
                else: 
                    print("       ", end='')

                if i != 0 and j == simplex_size_columna_forma_estandar - 1:
                    print(f"= {Arraysimplex[i][j]:.2f}", end='')

        print()

    # CREACION DE LAS VARIABLES DEL HEADER - CAMBIO CRÍTICO: Usar listas en lugar de strings
    var_table_head = []
    var_table_base = []
    var_table_nobase = []

    # ALMACENAR Z + VARIABLES x1,x2,....xn
    for i in range(1 + nvariablesx):
        if i == 0:
            var_table_head.append('Z')
            var_table_base.append('Z')
        else:
            var_table_head.append(f'x')
            var_table_head.append(f'{i}')
            
            if i-1 < len(nvariablex_restriccion_logica) and nvariablex_restriccion_logica[i-1] == 2:
                var_table_head.append(f'X')
                var_table_head.append(f'{i}')
                var_table_nobase.append(f'X')
                var_table_nobase.append(f'{i}')
            
            var_table_nobase.append(f'x')
            var_table_nobase.append(f'{i}')

    cantidad_variables_aritifical_ajuste = 0

    for i in range(nrestricciones):
        if base[i] == 3:
            cantidad_variables_aritifical_ajuste += 1

    cont_var_artifical = 0
    cont_var_holgura_exceso = 0
    cont_var_exceso_negative_holgura_artificial = 0

    # Generar nombres para variables de holgura, exceso y artificiales
    for i in range(nrestricciones):
        if base[i] == 1 and exist_valores_negativos[i] == 0:
            cont_var_holgura_exceso += 1
            var_table_head.append(f'S')
            var_table_head.append(f'{cont_var_holgura_exceso}')
            var_table_base.append(f'S')
            var_table_base.append(f'{cont_var_holgura_exceso}')

        elif base[i] == 1 and exist_valores_negativos[i] == 1:
            cont_var_holgura_exceso += 1
            cont_var_exceso_negative_holgura_artificial += 1
            var_table_head.append(f'S')
            var_table_head.append(f'{cont_var_holgura_exceso}')
            var_table_base.append(f'A')
            var_table_base.append(f'{cont_var_exceso_negative_holgura_artificial + cantidad_variables_aritifical_ajuste}')

        elif base[i] == 2 and exist_valores_negativos[i] == 0:
            cont_var_holgura_exceso += 1
            cont_var_exceso_negative_holgura_artificial += 1
            var_table_head.append(f'E')
            var_table_head.append(f'{cont_var_holgura_exceso}')
            var_table_base.append(f'A')
            var_table_base.append(f'{cont_var_exceso_negative_holgura_artificial + cantidad_variables_aritifical_ajuste}')

        elif base[i] == 2 and exist_valores_negativos[i] == 1:
            cont_var_holgura_exceso += 1
            var_table_head.append(f'E')
            var_table_head.append(f'{cont_var_holgura_exceso}')
            var_table_base.append(f'E')
            var_table_base.append(f'{cont_var_holgura_exceso}')

        else:
            cont_var_artifical += 1
            var_table_head.append(f'A')
            var_table_head.append(f'{cont_var_artifical}')
            var_table_base.append(f'A')
            var_table_base.append(f'{cont_var_artifical}')

    # Variables artificiales adicionales
    if cont_var_exceso_negative_holgura_artificial != 0:
        for i in range(cont_var_artifical, cont_var_exceso_negative_holgura_artificial + cont_var_artifical):
            var_table_head.append(f'A')
            var_table_head.append(f'{i+1}')
            var_table_base.append(f'A')
            var_table_base.append(f'{i+1}')
        
    var_table_head.append('b')
    var_table_head.append('B')

    # IMPRESION DE LA TABLA SIMPLEX
    print("\n\n")
    print(f"{Colors.YELLOW}:========FORMA TABULAR========: {Colors.RESET}\n\n")
    print(f"{Colors.WHITE}", end='')

    print(f"{Colors.RED}", end='')

    total_chars = len(var_table_head)
    
    # Imprimir encabezado de la tabla
    for i in range(total_chars):
        if i == 0: 
            print(f"            {var_table_head[i]}", end='')
        elif i == total_chars - 1: 
            print(f"        {var_table_head[i]}", end='')
        elif i % 2 == 0: 
            print(f"{var_table_head[i]}", end='')
        else: 
            print(f"        {var_table_head[i]}", end='')
    
    print()

    print(f"{Colors.WHITE}", end='')
    
    cont_position_base = 0
    for i in range(simplex_size_fila):
        for j in range(simplex_size_columna):
            if j == 0:
                print(f"{Colors.RED}", end='')
                if i == 0 and j == 0:
                    print("Ro(C)     ", end='')
                else:
                    print(f"R{i}        ", end='')
                print(f"{Colors.WHITE}", end='')

                print(f"{ArraysimplexOfficial[i][j]:.2f}", end='')
            else:
                if ArraysimplexOfficial[i][j] < 0:       
                    print(f"     {ArraysimplexOfficial[i][j]:.2f}", end='')
                elif ArraysimplexOfficial[i][j] >= 1000: 
                    print(f"   {ArraysimplexOfficial[i][j]:.2f}", end='')
                elif ArraysimplexOfficial[i][j] >= 100: 
                    print(f"    {ArraysimplexOfficial[i][j]:.2f}", end='')
                elif ArraysimplexOfficial[i][j] >= 10: 
                    print(f"     {ArraysimplexOfficial[i][j]:.2f}", end='')
                else:                              
                    print(f"      {ArraysimplexOfficial[i][j]:.2f}", end='')

        # IMPRESION DE LAS VARIABLES EN LA BASE
        print(f"{Colors.YELLOW}", end='')
        if i == 0: 
            print(f"      {var_table_base[i]}", end='')
        elif i > 0:
            if i + cont_position_base + 1 < len(var_table_base):
                print(f"      {var_table_base[i + cont_position_base]}{var_table_base[i + cont_position_base + 1]}", end='')
            cont_position_base += 1
        print(f"{Colors.WHITE}", end='')

        print()

    print("\n\n")
    print(f"{Colors.YELLOW}:========PROCESO========: {Colors.RESET}")
    print(f"{Colors.WHITE}", end='')

    # DETERMINARA SI EXISTEN VARIABLES ARTIFICIALES EN GENERAL
    exist_artificial_general = False
    for i in range(nrestricciones):
        if (base[i] == 2 and exist_valores_negativos[i] == 0) or (base[i] == 1 and exist_valores_negativos[i] == 1) or (base[i] == 3 and exist_valores_negativos[i] == 0):
            exist_artificial_general = True
    
    if exist_artificial_general == True:
        print("\n\n\n")
        print(f"{Colors.BLUE}:========AJUSTE========: {Colors.RESET}\n\n")
        print(f"{Colors.WHITE}", end='')

        # IMPRESION DE DATOS
        print(f"{Colors.RED}", end='')
        for i in range(total_chars):
            if i == 0: 
                print(f"            {var_table_head[i]}", end='')
            elif i == total_chars - 1: 
                print(f"        {var_table_head[i]}", end='')
            elif i % 2 == 0: 
                print(f"{var_table_head[i]}", end='')
            else: 
                print(f"        {var_table_head[i]}", end='')
        
        print()
        print(f"{Colors.WHITE}", end='')
        
        cont_position_base = 0
        for i in range(simplex_size_fila):
            for j in range(simplex_size_columna):
                if j == 0:
                    print(f"{Colors.RED}", end='')
                    if i == 0 and j == 0:
                        print("Ro(C)     ", end='')
                    else:
                        print(f"R{i}        ", end='')
                    print(f"{Colors.WHITE}", end='')

                    if i != 0:
                        if i-1 < len(base) and i-1 < len(exist_valores_negativos):
                            if (base[i-1] == 2 and exist_valores_negativos[i-1] == 0) or (base[i-1] == 1 and exist_valores_negativos[i-1] == 1) or (base[i-1] == 3):
                                print(f"{Colors.BLUE}", end='')
                        print(f"{ArraysimplexOfficial[i][j]:.2f}", end='')
                        print(f"{Colors.WHITE}", end='')
                    else: 
                        print(f"{ArraysimplexOfficial[i][j]:.2f}", end='')
                else:
                    if i != 0:   
                        if i-1 < len(base) and i-1 < len(exist_valores_negativos):
                            if (base[i-1] == 2 and exist_valores_negativos[i-1] == 0) or (base[i-1] == 1 and exist_valores_negativos[i-1] == 1) or (base[i-1] == 3):
                                print(f"{Colors.BLUE}", end='')
                        
                        if ArraysimplexOfficial[i][j] <= -1000000: 
                            print(f"{ArraysimplexOfficial[i][j]:.2e}", end='')
                        elif ArraysimplexOfficial[i][j] <= -100000: 
                            print(f"{ArraysimplexOfficial[i][j]:.2f}", end='')
                        elif ArraysimplexOfficial[i][j] <= -10000: 
                            print(f" {ArraysimplexOfficial[i][j]:.2f}", end='')
                        elif ArraysimplexOfficial[i][j] <= -1000: 
                            print(f"  {ArraysimplexOfficial[i][j]:.2f}", end='')
                        elif ArraysimplexOfficial[i][j] <= -100: 
                            print(f"   {ArraysimplexOfficial[i][j]:.2f}", end='')
                        elif ArraysimplexOfficial[i][j] <= -10: 
                            print(f"     {ArraysimplexOfficial[i][j]:.2f}", end='')
                        elif ArraysimplexOfficial[i][j] < 0:  
                            print(f"     {ArraysimplexOfficial[i][j]:.2f}", end='')
                        elif ArraysimplexOfficial[i][j] >= 1000000: 
                            print(f"  {ArraysimplexOfficial[i][j]:.2e}", end='')
                        elif ArraysimplexOfficial[i][j] >= 100000: 
                            print(f" {ArraysimplexOfficial[i][j]:.2f}", end='')
                        elif ArraysimplexOfficial[i][j] >= 10000: 
                            print(f"  {ArraysimplexOfficial[i][j]:.2f}", end='')
                        elif ArraysimplexOfficial[i][j] >= 1000: 
                            print(f"   {ArraysimplexOfficial[i][j]:.2f}", end='')
                        elif ArraysimplexOfficial[i][j] >= 100: 
                            print(f"    {ArraysimplexOfficial[i][j]:.2f}", end='')
                        elif ArraysimplexOfficial[i][j] >= 10: 
                            print(f"     {ArraysimplexOfficial[i][j]:.2f}", end='')
                        else:                              
                            print(f"      {ArraysimplexOfficial[i][j]:.2f}", end='')
                        print(f"{Colors.WHITE}", end='')
                    else:
                        if ArraysimplexOfficial[i][j] <= -1000000: 
                            print(f"{ArraysimplexOfficial[i][j]:.2e}", end='')
                        elif ArraysimplexOfficial[i][j] <= -100000: 
                            print(f"{ArraysimplexOfficial[i][j]:.2f}", end='')
                        elif ArraysimplexOfficial[i][j] <= -10000: 
                            print(f" {ArraysimplexOfficial[i][j]:.2f}", end='')
                        elif ArraysimplexOfficial[i][j] <= -1000: 
                            print(f"  {ArraysimplexOfficial[i][j]:.2f}", end='')
                        elif ArraysimplexOfficial[i][j] <= -100: 
                            print(f"   {ArraysimplexOfficial[i][j]:.2f}", end='')
                        elif ArraysimplexOfficial[i][j] <= -10: 
                            print(f"     {ArraysimplexOfficial[i][j]:.2f}", end='')
                        elif ArraysimplexOfficial[i][j] < 0:  
                            print(f"     {ArraysimplexOfficial[i][j]:.2f}", end='')
                        elif ArraysimplexOfficial[i][j] >= 1000000: 
                            print(f"  {ArraysimplexOfficial[i][j]:.2e}", end='')
                        elif ArraysimplexOfficial[i][j] >= 100000: 
                            print(f" {ArraysimplexOfficial[i][j]:.2f}", end='')
                        elif ArraysimplexOfficial[i][j] >= 10000: 
                            print(f"  {ArraysimplexOfficial[i][j]:.2f}", end='')
                        elif ArraysimplexOfficial[i][j] >= 1000: 
                            print(f"   {ArraysimplexOfficial[i][j]:.2f}", end='')
                        elif ArraysimplexOfficial[i][j] >= 100: 
                            print(f"    {ArraysimplexOfficial[i][j]:.2f}", end='')
                        elif ArraysimplexOfficial[i][j] >= 10: 
                            print(f"     {ArraysimplexOfficial[i][j]:.2f}", end='')
                        else:                              
                            print(f"      {ArraysimplexOfficial[i][j]:.2f}", end='')

            print(f"{Colors.YELLOW}", end='')
            if i == 0: 
                print(f"      {var_table_base[i]}", end='')
            elif i > 0:
                if i + cont_position_base + 1 < len(var_table_base):
                    print(f"      {var_table_base[i + cont_position_base]}{var_table_base[i + cont_position_base + 1]}", end='')
                cont_position_base += 1
            
            print(f"{Colors.RED}", end='')
            if i != 0: 
                if i-1 < len(base) and i-1 < len(exist_valores_negativos):
                    if (base[i-1] == 2 and exist_valores_negativos[i-1] == 0) or (base[i-1] == 1 and exist_valores_negativos[i-1] == 1) or (base[i-1] == 3):
                        print(f"    ({valor_artificial * -1})", end='')
            print(f"{Colors.WHITE}", end='')    
            
            print()

        # PROCESO PARA HACER EL AJUSTE
        # Eliminar variables artificiales de la función objetivo
        for i in range(simplex_size_fila):   
            for j in range(simplex_size_columna):
                if i != 0:
                    if i-1 < len(base) and i-1 < len(exist_valores_negativos):
                        if (base[i-1] == 2 and exist_valores_negativos[i-1] == 0) or (base[i-1] == 1 and exist_valores_negativos[i-1] == 1) or (base[i-1] == 3):
                            ArraysimplexOfficial[0][j] = (ArraysimplexOfficial[i][j] * (valor_artificial * -1)) + ArraysimplexOfficial[0][j]

        # SE IMPRIME LA TABLA SIMPLEX RESULTANTE
        print("\n\n")
        print(f"{Colors.BLUE}:========AJUSTE / RESULTADO========: {Colors.RESET}\n\n")
        print(f"{Colors.WHITE}", end='')
        print(f"{Colors.RED}", end='')

        for i in range(total_chars):
            if i == 0: 
                print(f"            {var_table_head[i]}", end='')
            elif i == total_chars - 1: 
                print(f"        {var_table_head[i]}", end='')
            elif i % 2 == 0: 
                print(f"{var_table_head[i]}", end='')
            else: 
                print(f"        {var_table_head[i]}", end='')
        
        print()
        print(f"{Colors.WHITE}", end='')
        
        cont_position_base = 0
        for i in range(simplex_size_fila):
            for j in range(simplex_size_columna):
                if j == 0:
                    print(f"{Colors.RED}", end='')
                    if i == 0 and j == 0:
                        print("Ro(C)     ", end='')
                    else:
                        print(f"R{i}        ", end='')
                    print(f"{Colors.WHITE}", end='')

                    if i == 0: 
                        print(f"{Colors.GREEN}", end='')
                    print(f"{ArraysimplexOfficial[i][j]:.2f}", end='')
                    print(f"{Colors.WHITE}", end='')
                else:
                    if i == 0: 
                        print(f"{Colors.GREEN}", end='')
                    if ArraysimplexOfficial[i][j] <= -1000000: 
                        print(f"{ArraysimplexOfficial[i][j]:.2e}", end='')
                    elif ArraysimplexOfficial[i][j] <= -100000: 
                        print(f"{ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] <= -10000: 
                        print(f" {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] <= -1000: 
                        print(f"  {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] <= -100: 
                        print(f"   {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] <= -10: 
                        print(f"     {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] < 0:  
                        print(f"     {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] >= 1000000: 
                        print(f"  {ArraysimplexOfficial[i][j]:.2e}", end='')
                    elif ArraysimplexOfficial[i][j] >= 100000: 
                        print(f" {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] >= 10000: 
                        print(f"  {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] >= 1000: 
                        print(f"   {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] >= 100: 
                        print(f"    {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] >= 10: 
                        print(f"     {ArraysimplexOfficial[i][j]:.2f}", end='')
                    else:                              
                        print(f"      {ArraysimplexOfficial[i][j]:.2f}", end='')
                    print(f"{Colors.WHITE}", end='')
            
            print(f"{Colors.YELLOW}", end='')
            if i == 0: 
                print(f"      {var_table_base[i]}", end='')
            elif i > 0:
                if i + cont_position_base + 1 < len(var_table_base):
                    print(f"      {var_table_base[i + cont_position_base]}{var_table_base[i + cont_position_base + 1]}", end='')
                cont_position_base += 1
            print(f"{Colors.WHITE}", end='')

            print()
    
    print("\n")

    # PROCESO PARA EMPEZAR CON EL SIMPLEX OFICIAL (ITERACIONES)
    cont_iteracion = 1
    exist_negativos_of = False
    is_no_acotada = False

    while True:
        print(f"\n\n{Colors.CYAN}:========ITERACION {cont_iteracion}========: {Colors.RESET}\n\n")

        # DETERMINAR SI EXISTEN NEGATIVOS EN LA F.O
        exist_negativos_of = False
        for i in range(simplex_size_fila):
            for j in range(simplex_size_columna):
                if i == 0 and j > 0 and j <= simplex_size_columna - 2:
                    if ArraysimplexOfficial[i][j] < 0:
                        exist_negativos_of = True

        if not exist_negativos_of:
            print("No hay valores negativos en F.O - Solución óptima alcanzada")
            break

        # ENCONTRAR LA COLUMNA MÁS NEGATIVA
        more_negative = 0
        j_columna_more_negative = -1

        for j in range(1, simplex_size_columna - 1):
            if ArraysimplexOfficial[0][j] < more_negative:
                more_negative = ArraysimplexOfficial[0][j]
                j_columna_more_negative = j

        if j_columna_more_negative == -1:
            print("No se encontró columna pivote")
            break

        # CALCULAR LOS VALORES DE P.C (Prueba de Cociente)
        pc_valor = [float('nan')] * (nrestricciones + 1)

        for i in range(simplex_size_fila):
            if i == 0:
                continue
            if ArraysimplexOfficial[i][j_columna_more_negative] > 0:
                pc_valor[i] = ArraysimplexOfficial[i][simplex_size_columna - 1] / ArraysimplexOfficial[i][j_columna_more_negative]
            else:
                pc_valor[i] = float('nan')

        # ENCONTRAR LA FILA PIVOTE
        pc_more_small = float('inf')
        i_fila_more_small = -1
        cont_no_acotada = 0

        for i in range(1, simplex_size_fila):
            if math.isnan(pc_valor[i]):
                cont_no_acotada += 1
            elif pc_valor[i] < pc_more_small:
                pc_more_small = pc_valor[i]
                i_fila_more_small = i

        if cont_no_acotada == nrestricciones:
            print(f"{Colors.RED}SOLUCIÓN NO ACOTADA{Colors.RESET}")
            is_no_acotada = True
            break

        if i_fila_more_small == -1:
            print("No se encontró fila pivote válida")
            break

        # MOSTRAR TABLA CON P.C
        print(f"{Colors.RED}", end='')
        for i in range(total_chars):
            if i == 0: 
                print(f"            {var_table_head[i]}", end='')
            elif i == total_chars - 1: 
                print(f"        {var_table_head[i]}", end='')
            elif i % 2 == 0: 
                print(f"{var_table_head[i]}", end='')
            else: 
                print(f"        {var_table_head[i]}", end='')
        print(f"{Colors.RESET}")

        cont_position_base = 0
        for i in range(simplex_size_fila):
            for j in range(simplex_size_columna):
                if j == 0:
                    print(f"{Colors.RED}", end='')
                    if i == 0 and j == 0:
                        print("Ro(C)     ", end='')
                    else:
                        print(f"R{i}        ", end='')
                    print(f"{Colors.RESET}", end='')

                    if i == i_fila_more_small:
                        print(f"{Colors.RED}", end='')
                    print(f"{ArraysimplexOfficial[i][j]:.2f}", end='')
                    print(f"{Colors.RESET}", end='')
                else:
                    if j == j_columna_more_negative or i == i_fila_more_small:
                        print(f"{Colors.RED}", end='')
                    if j == j_columna_more_negative and i == i_fila_more_small:
                        print(f"{Colors.YELLOW}", end='')
                    
                    if ArraysimplexOfficial[i][j] <= -1000000: 
                        print(f"{ArraysimplexOfficial[i][j]:.2e}", end='')
                    elif ArraysimplexOfficial[i][j] <= -100000: 
                        print(f"{ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] <= -10000: 
                        print(f" {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] <= -1000: 
                        print(f"  {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] <= -100: 
                        print(f"   {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] <= -10: 
                        print(f"     {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] < 0:  
                        print(f"     {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] >= 1000000: 
                        print(f"  {ArraysimplexOfficial[i][j]:.2e}", end='')
                    elif ArraysimplexOfficial[i][j] >= 100000: 
                        print(f" {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] >= 10000: 
                        print(f"  {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] >= 1000: 
                        print(f"   {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] >= 100: 
                        print(f"    {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] >= 10: 
                        print(f"     {ArraysimplexOfficial[i][j]:.2f}", end='')
                    else:                              
                        print(f"      {ArraysimplexOfficial[i][j]:.2f}", end='')
                    print(f"{Colors.RESET}", end='')

            # IMPRESIÓN P.C
            print(f"{Colors.BLUE}", end='')
            if i == 0:
                print("      P.C = -----", end='')
            else:
                if math.isnan(pc_valor[i]):
                    print("      P.C = -----", end='')
                else:
                    print(f"      P.C = {pc_valor[i]:.2f}", end='')
            print(f"{Colors.RESET}")

        # REALIZAR OPERACIONES DE FILA
        pivote = ArraysimplexOfficial[i_fila_more_small][j_columna_more_negative]

        # NORMALIZAR FILA PIVOTE
        if pivote != 1:
            for j in range(simplex_size_columna):
                ArraysimplexOfficial[i_fila_more_small][j] /= pivote

        # ELIMINACIÓN GAUSSIANA
        for i in range(simplex_size_fila):
            if i != i_fila_more_small:
                factor = ArraysimplexOfficial[i][j_columna_more_negative]
                for j in range(simplex_size_columna):
                    ArraysimplexOfficial[i][j] -= factor * ArraysimplexOfficial[i_fila_more_small][j]

        # ACTUALIZAR BASE - AQUÍ ESTÁ LA CORRECCIÓN CRÍTICA
        base_index = i_fila_more_small * 2 - 1
        if base_index < len(var_table_base) and base_index + 1 < len(var_table_base):
            head_index = j_columna_more_negative * 2 - 1
            if head_index < len(var_table_head) and head_index + 1 < len(var_table_head):
                var_table_base[base_index] = var_table_head[head_index]
                var_table_base[base_index + 1] = var_table_head[head_index + 1]

        cont_iteracion += 1

        # MOSTRAR RESULTADO DE LA ITERACIÓN
        print(f"\n\n{Colors.CYAN}:========ITERACION {cont_iteracion-1} / RESULTADO========: {Colors.RESET}\n\n")
        
        print(f"{Colors.RED}", end='')
        for i in range(total_chars):
            if i == 0: 
                print(f"            {var_table_head[i]}", end='')
            elif i == total_chars - 1: 
                print(f"        {var_table_head[i]}", end='')
            elif i % 2 == 0: 
                print(f"{var_table_head[i]}", end='')
            else: 
                print(f"        {var_table_head[i]}", end='')
        print(f"{Colors.RESET}")

        cont_position_base = 0
        for i in range(simplex_size_fila):
            for j in range(simplex_size_columna):
                if j == 0:
                    print(f"{Colors.RED}", end='')
                    if i == 0 and j == 0:
                        print("Ro(C)     ", end='')
                    else:
                        print(f"R{i}        ", end='')
                    print(f"{Colors.RESET}", end='')
                    print(f"{ArraysimplexOfficial[i][j]:.2f}", end='')
                else:
                    if ArraysimplexOfficial[i][j] <= -1000000: 
                        print(f"{ArraysimplexOfficial[i][j]:.2e}", end='')
                    elif ArraysimplexOfficial[i][j] <= -100000: 
                        print(f"{ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] <= -10000: 
                        print(f" {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] <= -1000: 
                        print(f"  {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] <= -100: 
                        print(f"   {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] <= -10: 
                        print(f"     {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] < 0:  
                        print(f"     {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] >= 1000000: 
                        print(f"  {ArraysimplexOfficial[i][j]:.2e}", end='')
                    elif ArraysimplexOfficial[i][j] >= 100000: 
                        print(f" {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] >= 10000: 
                        print(f"  {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] >= 1000: 
                        print(f"   {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] >= 100: 
                        print(f"    {ArraysimplexOfficial[i][j]:.2f}", end='')
                    elif ArraysimplexOfficial[i][j] >= 10: 
                        print(f"     {ArraysimplexOfficial[i][j]:.2f}", end='')
                    else:                              
                        print(f"      {ArraysimplexOfficial[i][j]:.2f}", end='')
            
            # IMPRESIÓN BASE
            print(f"{Colors.YELLOW}", end='')
            if i == 0: 
                print(f"      {var_table_base[i]}", end='')
            elif i > 0:
                if i + cont_position_base + 1 < len(var_table_base):
                    print(f"      {var_table_base[i + cont_position_base]}{var_table_base[i + cont_position_base + 1]}", end='')
                cont_position_base += 1
            print(f"{Colors.RESET}")

        # VERIFICAR SI HEMOS ALCANZADO LA OPTIMALIDAD
        exist_negativos_of = False
        for j in range(1, simplex_size_columna - 1):
            if ArraysimplexOfficial[0][j] < 0:
                exist_negativos_of = True
                break

        if not exist_negativos_of:
            print(f"\n{Colors.GREEN}¡SOLUCIÓN ÓPTIMA ALCANZADA!{Colors.RESET}")
            break

        if cont_iteracion > 20:
            print(f"\n{Colors.YELLOW}Límite de iteraciones alcanzado{Colors.RESET}")
            break

    # ANÁLISIS DE RESULTADOS FINALES
    print(f"\n\n{Colors.GREEN}:========RESULTADOS FINALES========:{Colors.RESET}\n\n\n")

    is_no_factible = False

    # VERIFICAR FACTIBILIDAD
    for i in range(1, simplex_size_fila):
        base_idx = i * 2 - 1
        if base_idx < len(var_table_base) and var_table_base[base_idx] == 'A':
            if ArraysimplexOfficial[i][simplex_size_columna-1] > 0:
                is_no_factible = True

    if is_no_acotada:
        print(f"{Colors.BG_RED}TIPO DE SOLUCION / [ NO ACOTADA ]{Colors.RESET}")
    elif is_no_factible:
        print(f"{Colors.BG_YELLOW}TIPO DE SOLUCION / [ NO FACTIBLE ]{Colors.RESET}")
    else:
        # VERIFICAR OPTIMALIDAD MÚLTIPLE
        is_optima_multiple = False
        for j in range(1, simplex_size_columna - 1):
            if abs(ArraysimplexOfficial[0][j]) < 0.0001:
                for i in range(1, simplex_size_fila):
                    if (abs(ArraysimplexOfficial[i][j] - 1) > 0.0001 and 
                        abs(ArraysimplexOfficial[i][j]) > 0.0001):
                        is_optima_multiple = True
                        break
                if is_optima_multiple:
                    break

        if is_optima_multiple:
            print(f"{Colors.BG_BLUE}TIPO DE SOLUCION / [ OPTIMA MULTIPLE ]{Colors.RESET}")
        else:
            print(f"{Colors.BG_GREEN}TIPO DE SOLUCION / [ OPTIMA ]{Colors.RESET}")

        # IMPRIMIR VARIABLES EN LA BASE
        print(f"{Colors.YELLOW}\n\n- VARIABLES EN LA BASE:{Colors.RESET}")
        for i in range(1, simplex_size_fila):
            base_idx = i * 2 - 1
            if base_idx < len(var_table_base) and base_idx + 1 < len(var_table_base):
                print(f"{Colors.YELLOW}- {var_table_base[base_idx]}{var_table_base[base_idx + 1]} = {Colors.RESET}", end='')
                valor = ArraysimplexOfficial[i][simplex_size_columna-1]
                
                if var_table_base[base_idx] == 'X':
                    if valor == 0:
                        print(f"{valor:.2f}")
                    else:
                        print(f"{valor:.2f} = {Colors.RESET}{-valor:.2f}")
                else:
                    print(f"{valor:.2f}")

        # IMPRIMIR FUNCIÓN OBJETIVO
        print(f"{Colors.YELLOW}\n- FUNCION OBJETIVO:{Colors.RESET}")
        print(f"\n")
        if maxormin == 1: 
            print("- min z = ")
        else: 
            print("- max z = ")

        for i in range(nvariablesx):
            print(f"({impresion_original_Arrayfo[i]:.2f})x{i+1}", end='')
            if i < nvariablesx - 1:
                print(" + ", end='')

            if (i < len(nvariablex_restriccion_logica) and 
                nvariablex_restriccion_logica[i] == 2):
                if i == nvariablesx - 1:
                    print(" + ", end='')
                print(f"({impresion_original_Arrayfo[i] * -1:.2f})X{i+1}_vi", end='')
                if i < nvariablesx - 1:
                    print(" + ", end='')

        print(" = ", end='')

        valor_fo = ArraysimplexOfficial[0][simplex_size_columna-1]
        if maxormin == 1:
            if valor_fo == 0:
                print(f"{valor_fo:.2f}")
            else:
                print(f"{-valor_fo:.2f}")
        else:
            print(f"{valor_fo:.2f}")

    print(f"\n\n\n{Colors.YELLOW}:========:¡GRACIAS!:========:{Colors.RESET}")
    input()

if __name__ == "__main__":
    main()