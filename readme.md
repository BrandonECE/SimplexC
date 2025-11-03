🧮 Solucionador de Programación Lineal - Método Simplex
📋 Descripción
Este es un programa completo implementado en Python que resuelve problemas de programación lineal utilizando el algoritmo simplex. El sistema permite modelar y resolver problemas de optimización con múltiples variables y restricciones, proporcionando una interfaz visual colorida y detallada del proceso.

✨ Características Principales
🔧 Funcionalidades Básicas
✅ Soporte para minimización y maximización

✅ Múltiples variables de decisión (hasta 100)

✅ Múltiples restricciones (hasta 100)

✅ Diferentes tipos de restricciones: ≤, ≥, =

✅ Variables libres (sin restricción de no negatividad)

✅ Validación completa de entradas

🎨 Visualización Avanzada
🎯 Interfaz colorida con códigos ANSI

📊 Tablas simplex detalladas con colores diferenciados

🔄 Proceso iterativo completo mostrando cada paso

📈 Múltiples formatos de visualización: Base, Forma Estándar, Forma Tabular

⚡ Algoritmo Avanzado
🧩 Manejo automático de variables artificiales

🔍 Detección de casos especiales:

Solución no acotada

Problema no factible

Solución óptima única

Soluciones óptimas múltiples

📐 Eliminación gaussiana con pivoteo

🔢 Cálculo de pruebas de cociente (P.C)

🚀 Instalación y Uso
Requisitos del Sistema
bash
Python 3.6 o superior
Sistema operativo: Windows, Linux o macOS
Ejecución
bash
python simplex_solver.py
📖 Guía de Uso
1. Entrada de Datos
🔹 Variables de Decisión
Ingrese el número de variables de decisión (x₁, x₂, ..., xₙ)

Especifique los coeficientes en la función objetivo

🔹 Función Objetivo
Seleccione entre Minimizar o Maximizar

Ingrese los coeficientes de cada variable

🔹 Restricciones Físicas
Defina el número de restricciones

Para cada restricción:

Coeficientes de las variables

Tipo de limitante (≤, ≥, =)

Valor del lado derecho

🔹 Restricciones Lógicas
Especifique si las variables deben ser ≥ 0

Opción de tener variables libres (sin restricción de signo)

2. Proceso de Solución
El programa ejecuta automáticamente:

Construcción de la tabla simplex inicial

Ajuste con variables artificiales (si es necesario)

Iteraciones del método simplex

Análisis de optimalidad

Presentación de resultados finales

3. Interpretación de Resultados
🎯 Tipos de Solución
Estado	Descripción
🟢 ÓPTIMA	Solución única encontrada
🔵 ÓPTIMA MÚLTIPLE	Múltiples soluciones óptimas
🟡 NO FACTIBLE	El problema no tiene solución
🔴 NO ACOTADA	La solución tiende a infinito
📊 Salida de Resultados
Valores de las variables en la base

Valor óptimo de la función objetivo

Tablas intermedias del proceso simplex

🎨 Códigos de Color
Color	Significado	Uso
🟢 Verde	Variables de holgura	Restricciones ≤
🔴 Rojo	Variables de exceso	Restricciones ≥
🔵 Azul	Variables artificiales	Restricciones =
🟣 Magenta	Variables Vi (libres)	Variables sin restricción
🟡 Amarillo	Variables artificiales automáticas	Ajustes automáticos
⚪ Blanco	Valores numéricos	Datos generales
📊 Ejemplo de Flujo
Entrada del Problema
plaintext
VARIABLES: 2
FUNCIÓN OBJETIVO: Max Z = 3x₁ + 2x₂

RESTRICCIONES:
• 2x₁ + x₂ ≤ 18
• 2x₁ + 3x₂ ≤ 42  
• 3x₁ + x₂ ≤ 24
• x₁, x₂ ≥ 0
Proceso de Solución
plaintext
1. 📝 Construcción tabla inicial
2. 🔄 Iteraciones simplex
3. ✅ Verificación optimalidad
Resultados
plaintext
SOLUCIÓN: 🟢 ÓPTIMA
x₁ = 3.00
x₂ = 12.00
Z = 42.00
🏗️ Estructura del Código
Arquitectura Principal
python
class Colors:
    """Manejo de colores ANSI para la interfaz visual"""
    
def leer_entero(mensaje, min_val, max_val):
    """Validación robusta de entrada de enteros"""
    
def leer_double(mensaje):
    """Validación de entrada de números decimales"""
    
def main():
    """Función principal del algoritmo simplex"""
Variables y Estructuras Clave
Variable	Descripción
Arrayfo	Coeficientes de la función objetivo
Arrayrestricciones	Matriz completa de restricciones
Arraysimplex	Tabla simplex inicial
ArraysimplexOfficial	Tabla simplex oficial para iteraciones
base	Tipo de restricciones por fila
🔍 Casos de Uso
🏭 Aplicaciones Prácticas
📈 Optimización de recursos en producción

💰 Análisis financiero y portafolios de inversión

🚚 Problemas de transporte y logística

🎯 Asignación óptima de recursos limitados

🏗️ Planificación de proyectos y producción

🎓 Ejemplos Académicos
Problemas de mezcla de productos

Optimización de dietas y nutrición

Planificación de inversiones financieras

Asignación de tareas y recursos humanos

Problemas de transbordo y distribución

⚠️ Consideraciones y Limitaciones
🔒 Limitaciones Técnicas
❗ Máximo 100 variables y 100 restricciones

❗ No soporta programación lineal entera

❗ Requiere terminal con soporte para colores ANSI

❗ Precisión limitada por aritmética de punto flotante

💡 Recomendaciones de Uso
✅ Verificar consistencia de datos de entrada

✅ Revisar que el problema esté bien formulado

✅ Considerar el significado económico de las variables

✅ Validar unidades y escalas de medición

🛠️ Personalización y Extensión
⚙️ Parámetros Modificables
python
# Valor M para variables artificiales
valor_artificial = 100

# Límites de entrada (en leer_entero)
min_val, max_val = 1, 100

# Precisión decimal en resultados
print(f"{valor:.2f}")
🚀 Extensiones Futuras
🔢 Programación lineal entera

📐 Método de las dos fases

📊 Análisis de sensibilidad

💾 Exportación a Excel/CSV

🌐 Interfaz gráfica web

📱 Aplicación móvil

📄 Licencia
Este proyecto es de uso académico y educativo. Desarrollado específicamente para demostrar la implementación práctica del algoritmo simplex en un entorno educativo.

🤝 Contribuciones
¡Las contribuciones son bienvenidas! Áreas prioritarias de mejora:

🐛 Corrección de Bugs
Errores de redondeo en cálculos

Problemas de visualización en ciertas terminales

Validación de casos extremos

🎨 Mejoras de Interfaz
Mejor formato de tablas

Opciones de personalización visual

Modo sin colores para compatibilidad

⚡ Optimizaciones
Mejora en el rendimiento para problemas grandes

Reducción de uso de memoria

Algoritmos más eficientes

🔧 Nuevas Funcionalidades
Soporte para problemas de transporte

Análisis post-optimal

Exportación de reportes