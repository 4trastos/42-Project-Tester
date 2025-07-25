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

# Variable para almacenar el manual de printf y evitar llamarlo múltiples veces
PRINTF_MAN_PAGE=""
MAN_PRINTF_FETCHED=false

# Función para obtener el manual de printf
get_printf_man_page() {
    if [ "$MAN_PRINTF_FETCHED" = false ]; then
        echo -e "${CYAN}\n--- Manual de la función printf() ---\n${NC}" >> "$ERROR_LOG"
        # Usamos col -b para eliminar caracteres de formato y obtener texto plano
        man 3 printf | col -b >> "$ERROR_LOG" 2>&1
        if [ $? -ne 0 ]; then
            echo -e "${RED}❌ No se pudo obtener el manual de printf. Asegúrate de tener 'man' instalado y las páginas de manual de C.${NC}" >> "$ERROR_LOG"
        fi
        PRINTF_MAN_PAGE=$(cat "$ERROR_LOG" | awk '/^--- Manual de la función printf\(\) ---/{flag=1;next}/^--- Fin del Manual ---/{flag=0}flag')
        MAN_PRINTF_FETCHED=true
    fi
}

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
        # Capturamos la salida de make en el log de errores
        (cd "$dir" && make fclean && make) >> "$ERROR_LOG" 2>&1 || {
            echo -e "${YELLOW}⚠ Make falló. Intentando compilación manual...${NC}" >> "$ERROR_LOG"
        }

        LIBRARY=$(find "$dir" -name "libftprintf.a" -o -name "*.a" -print -quit)
        if [[ -n "$LIBRARY" ]]; then
            echo -e "${GREEN}✅ Librería generada: ${LIBRARY}${NC}"
            COMPILE_ARGS=("$LIBRARY" "-I${dir}")
            return 0
        fi
    fi

    echo -e "${YELLOW}⚠ No se encontró librería o Makefile falló. Compilando manualmente...${NC}"
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
        echo -e "${RED}❌ Error de compilación. Consulta ${ERROR_LOG} para más detalles.${NC}"
        exit 1
    fi

    echo -e "${CYAN}🧪 Compilando tester...${NC}"
    # Redirigimos la salida de compilación del tester al log de errores
    gcc -Wall -Wextra -Werror \
        "${SCRIPT_DIR}/test_main.c" \
        "${COMPILE_ARGS[@]}" \
        -o "${EXEC_NAME}" 2>> "$ERROR_LOG"

    if [[ $? -ne 0 ]]; then
        echo -e "${RED}❌ Error al compilar el tester. Consulta ${ERROR_LOG} para más detalles.${NC}"
        exit 1
    fi

    echo -e "${GREEN}🚀 Ejecutando tests...${NC}"
    # Ejecutamos el tester y procesamos su salida línea por línea
    # La salida del tester (OK/KO) va a stderr, así que la capturamos
    TEST_RAW_OUTPUT=$(./"${EXEC_NAME}" 2>&1)
    
    # Imprimimos la salida del tester y, si hay KOs, el manual
    echo "$TEST_RAW_OUTPUT" | while IFS= read -r line; do
        echo "$line"
        if [[ "$line" == *"[KO]"* ]]; then
            if [ "$MAN_PRINTF_FETCHED" = false ]; then
                get_printf_man_page
                echo -e "${MAGENTA}\n--- Extracto del Manual de printf() (para referencia) ---\n${NC}"
                echo "$PRINTF_MAN_PAGE"
                echo -e "${MAGENTA}\n--- Fin del Extracto del Manual ---\n${NC}"
            fi
        fi
    done

    # Extraemos el resumen final para el estado
    TOTAL_TESTS=$(echo "$TEST_RAW_OUTPUT" | awk '/^Resumen:.*tests pasados/ {print $3}' | cut -d'/' -f2)
    PASSED_TESTS=$(echo "$TEST_RAW_OUTPUT" | awk '/^Resumen:.*tests pasados/ {print $3}' | cut -d'/' -f1)

    TOTAL_TESTS=${TOTAL_TESTS:-0}
    PASSED_TESTS=${PASSED_TESTS:-0}
    FAILED_TESTS=$((TOTAL_TESTS - PASSED_TESTS))

    if [[ "$FAILED_TESTS" -ne 0 ]]; then
        echo -e "${RED}❌ Algunos tests fallaron. Consulta los detalles arriba y el log: ${ERROR_LOG}${NC}"
        exit 1
    else
        echo -e "${GREEN}✅ ¡Todos los tests pasaron correctamente! 🎉${NC}"
    fi

    rm -f "${EXEC_NAME}"
    # Limpiamos el proyecto ft_printf si tiene Makefile
    [[ -f "${PRINTF_DIR}/Makefile" ]] && (cd "$PRINTF_DIR" && make fclean >/dev/null 2>&1)

    exit 0
}

main