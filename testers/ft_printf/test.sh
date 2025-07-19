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
EXEC_NAME="${EXEC_NAME:-printf_test}"

# Función para encontrar ft_printf
find_printf_project() {
    local base_path="${1:-$HOME}"
    local header_path=$(find "$base_path" -maxdepth 6 -name "ft_printf.h" -not -path "*42_tester*" -print -quit 2>/dev/null)

    if [[ -z "$header_path" ]]; then
        echo -e "${RED}❌ No se encontró ft_printf.h en $base_path${NC}" >&2
        return 1
    fi

    local project_dir
    project_dir=$(dirname "$header_path")
    echo "$project_dir"
    return 0
}

# Compilación
compile_printf() {
    local dir="$1"

    echo -e "${CYAN}🔍 Analizando Makefile...${NC}"

    if [[ -f "${dir}/Makefile" ]]; then
        echo -e "${GREEN}✅ Makefile encontrado. Compilando...${NC}"
        (cd "$dir" && make fclean && make) >> "$ERROR_LOG" 2>&1 || {
            echo -e "${YELLOW}⚠ Make falló. Intentando compilación manual...${NC}"
        }

        LIBRARY=$(find "$dir" -name "libftprintf.a" -o -name "*.a" -print -quit)
        if [[ -n "$LIBRARY" ]]; then
            echo -e "${GREEN}✅ Librería generada: ${LIBRARY}${NC}"
            COMPILE_ARGS=("$LIBRARY" "-I${dir}")
            return 0
        fi
    fi

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

    if ! compile_printf "$PRINTF_DIR"; then
        echo -e "${RED}❌ Error de compilación. Ver ${ERROR_LOG}${NC}"
        exit 1
    fi

    echo -e "${CYAN}🧪 Compilando tester...${NC}"
    gcc -Wall -Wextra -Werror \
        "${SCRIPT_DIR}/test_main.c" \
        "${COMPILE_ARGS[@]}" \
        -o "${EXEC_NAME}" 2>> "$ERROR_LOG"

    if [[ $? -ne 0 ]]; then
        echo -e "${RED}❌ Error al compilar el tester. Ver ${ERROR_LOG}${NC}"
        exit 1
    fi

    echo -e "${GREEN}🚀 Ejecutando tests...${NC}"
    TEST_OUTPUT=$(./"${EXEC_NAME}")
    echo "$TEST_OUTPUT"

    TOTAL_TESTS=$(echo "$TEST_OUTPUT" | awk '/^Resumen:.*tests pasados/ {print $3}' | cut -d'/' -f2)
    PASSED_TESTS=$(echo "$TEST_OUTPUT" | awk '/^Resumen:.*tests pasados/ {print $3}' | cut -d'/' -f1)

    TOTAL_TESTS=${TOTAL_TESTS:-0}
    PASSED_TESTS=${PASSED_TESTS:-0}
    FAILED_TESTS=$((TOTAL_TESTS - PASSED_TESTS))

    if [[ "$FAILED_TESTS" -ne 0 ]]; then
        echo -e "${RED}❌ Algunos tests fallaron${NC}"
        echo -e "${YELLOW}ℹ Ver detalles completos en la salida anterior o en: ${ERROR_LOG}${NC}"
        exit 1
    else
        echo -e "${GREEN}✅ ¡Todos los tests pasaron correctamente! 🎉${NC}"
    fi

    rm -f "${EXEC_NAME}"
    [[ -f "${PRINTF_DIR}/Makefile" ]] && (cd "$PRINTF_DIR" && make fclean >/dev/null)

    exit 0
}

main
