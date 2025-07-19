#!/bin/bash

# Configuración de colores
BLUE='\033[1;34m'
GREEN='\033[1;32m'
RED='\033[1;31m'
YELLOW='\033[1;33m'
CYAN='\033[1;36m'
MAGENTA='\033[1;35m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ERROR_LOG="${SCRIPT_DIR}/printf_errors_$(date +"%Y%m%d_%H%M%S").log"
EXEC_NAME="printf_test"

# Función mejorada para encontrar ft_printf
find_printf_project() {
    # Buscar primero el header
    local header_path=$(find ~/ -name "ft_printf.h" -not -path "*42_tester*" -print -quit 2>/dev/null)
    
    if [[ -z "$header_path" ]]; then
        echo -e "${RED}❌ No se encontró ft_printf.h${NC}" >&2
        return 1
    fi

    local project_dir=$(dirname "$header_path")
    echo "$project_dir"
    return 0
}

# Función para compilar
compile_printf() {
    local dir="$1"
    
    echo -e "${CYAN}🔍 Analizando Makefile...${NC}"
    
    # Intento con Makefile primero
    if [[ -f "${dir}/Makefile" ]]; then
        echo -e "${GREEN}✅ Makefile encontrado. Compilando...${NC}"
        # Redirigir la salida de make a ERROR_LOG para depuración
        (cd "$dir" && make clean && make) >> "$ERROR_LOG" 2>&1
        
        if [[ $? -eq 0 ]]; then
            # Buscar la librería generada
            LIBRARY=$(find "$dir" -name "libftprintf.a" -o -name "*.a" -print -quit)
            if [[ -n "$LIBRARY" ]]; then
                echo -e "${GREEN}✅ Librería generada: ${LIBRARY}${NC}"
                COMPILE_ARGS=("$LIBRARY" "-I${dir}")
                return 0
            fi
        fi
    fi
    
    # Compilación manual si Makefile falla o no existe
    echo -e "${YELLOW}⚠ Compilando manualmente...${NC}"
    SRC_FILES=$(find "$dir" -name "ft_printf*.c" ! -name "*bonus*" 2>/dev/null | tr '\n' ' ')
    
    if [[ -z "$SRC_FILES" ]]; then
        echo -e "${RED}❌ No se encontraron archivos fuente ft_printf*.c${NC}"
        return 1
    fi
    
    echo -e "${GREEN}✅ Archivos fuente encontrados:${NC}\n${SRC_FILES}"
    COMPILE_ARGS=($SRC_FILES "-I${dir}")
    return 0
}

# Función principal
main() {
    echo -e "${BLUE}🔎 Buscando ft_printf...${NC}"
    PRINTF_DIR=$(find_printf_project)
    if [[ $? -ne 0 ]]; then
        exit 1
    fi
    
    echo -e "${GREEN}✅ Proyecto encontrado en: ${PRINTF_DIR}${NC}"
    
    # Compilación
    if ! compile_printf "$PRINTF_DIR"; then
        echo -e "${RED}❌ Error de compilación. Ver ${ERROR_LOG}${NC}"
        exit 1
    fi
    
    # Compilar tester
    echo -e "${CYAN}🧪 Compilando tester...${NC}"
    # Usar ${COMPILE_ARGS[@]} para expandir correctamente la lista de argumentos
    gcc -Wall -Wextra -Werror \
        "${SCRIPT_DIR}/test_main.c" \
        "${COMPILE_ARGS[@]}" \
        -o "${EXEC_NAME}" 2>> "$ERROR_LOG" # Redirigir errores de compilación del tester
    
    if [[ $? -ne 0 ]]; then
        echo -e "${RED}❌ Error al compilar el tester. Ver ${ERROR_LOG}${NC}"
        exit 1
    fi
    
    # Ejecutar tests
    echo -e "${GREEN}🚀 Ejecutando tests...${NC}"
    # Capturar la salida completa del ejecutable C
    TEST_OUTPUT=$(./"${EXEC_NAME}")
    echo "$TEST_OUTPUT" # Mostrar la salida completa del tester C

    # Extraer los resultados del resumen del tester C
    # Los patrones de AWK deben coincidir con la salida exacta del test_main.c
    TOTAL_TESTS=$(echo "$TEST_OUTPUT" | awk '/^Resumen:.*tests pasados/ {print $3}' | cut -d'/' -f2)
    PASSED_TESTS=$(echo "$TEST_OUTPUT" | awk '/^Resumen:.*tests pasados/ {print $3}' | cut -d'/' -f1)
    
    # Asegurarse de que las variables no estén vacías
    TOTAL_TESTS=${TOTAL_TESTS:-0}
    PASSED_TESTS=${PASSED_TESTS:-0}
    
    FAILED_TESTS=$((TOTAL_TESTS - PASSED_TESTS)) # Calcular tests fallidos
    
    # Mostrar resumen final (opcional, ya lo imprime test_main.c)
    # Si quieres que el script lo reimprima, descomenta lo siguiente:
    # echo -e "\n${MAGENTA}═══════════════════════════════════════════════${NC}"
    # echo -e "${MAGENTA}          RESULTADOS FT_PRINTF                 ${NC}"
    # echo -e "${MAGENTA}═══════════════════════════════════════════════${NC}"
    
    if [[ "$FAILED_TESTS" -ne 0 ]]; then # Compara el número real de fallidos
        echo -e "${RED}❌ Algunos tests fallaron${NC}"
        echo -e "${YELLOW}ℹ Ver detalles completos en la salida anterior o en: ${ERROR_LOG}${NC}"
        # Puedes añadir un 'exit 1' aquí si quieres que el script termine con error si hay fallos
        exit 1
    else
        echo -e "${GREEN}✅ ¡Todos los tests pasaron correctamente! 🎉${NC}"
    fi
    
    # Limpieza
    rm -f "${EXEC_NAME}"
    [[ -f "${PRINTF_DIR}/Makefile" ]] && (cd "$PRINTF_DIR" && make fclean >/dev/null)
    
    # El script sale con el estado de los tests (0 si todos OK, 1 si hay fallos)
    exit 0
}

main