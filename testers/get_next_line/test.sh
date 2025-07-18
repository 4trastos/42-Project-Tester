#!/bin/bash

# ==============================================
# 42 GNL TESTER - IMPROVED PATH FINDING VERSION
# ==============================================

# Configuración de colores
BLUE='\033[1;34m'
GREEN='\033[1;32m'
RED='\033[1;31m'
YELLOW='\033[1;33m'
MAGENTA='\033[1;35m'
CYAN='\033[1;36m'
NC='\033[0m'

# Configuración de paths
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
EXEC_NAME="gnl_test"
ERROR_LOG="${SCRIPT_DIR}/gnl_errors_$(date +"%Y%m%d_%H%M%S").log"

# Función mejorada para encontrar GNL
find_gnl_project() {
    # Buscar primero cerca del tester
    local nearby_dirs=(
        "${SCRIPT_DIR}/.."      # Directorio padre del tester
        "${SCRIPT_DIR}/../.."   # Directorio abuelo
        "${HOME}"              # Directorio home
    )

    # Buscar el header file excluyendo el propio tester
    for dir in "${nearby_dirs[@]}"; do
        GNL_DIR=$(find "$dir" -type f -name "get_next_line.h" \
            -not -path "*/42_tester/*" \
            -exec dirname {} \; 2>/dev/null | head -n 1)
        
        if [[ -n "$GNL_DIR" ]]; then
            # Verificar que existan los archivos .c necesarios
            if [[ -f "${GNL_DIR}/get_next_line.c" && -f "${GNL_DIR}/get_next_line_utils.c" ]]; then
                echo -e "${GREEN}✅ Proyecto encontrado en: ${GNL_DIR}${NC}"
                return 0
            fi
        fi
    done

    echo -e "${RED}❌ No se encontró un proyecto válido de GNL${NC}"
    echo -e "${YELLOW}ℹ Buscado en:${NC}"
    echo "   - ${SCRIPT_DIR}/.."
    echo "   - ${SCRIPT_DIR}/../.."
    echo "   - ${HOME}"
    echo -e "${YELLOW}ℹ Requisitos:${NC}"
    echo "   - Archivos get_next_line.{c,h}"
    echo "   - get_next_line_utils.c"
    return 1
}

# Buscar proyecto
echo -e "${BLUE}🔎 Buscando tu proyecto get_next_line...${NC}"
if ! find_gnl_project; then
    exit 1
fi

# Compilación mejorada
compile_gnl() {
    echo -e "${CYAN}🔧 Compilando con BUFFER_SIZE=${BUFFER_SIZE:-42}...${NC}"
    
    gcc -Wall -Wextra -Werror \
        -D BUFFER_SIZE=${BUFFER_SIZE:-42} \
        "${SCRIPT_DIR}/test_main.c" \
        "${GNL_DIR}/get_next_line.c" \
        "${GNL_DIR}/get_next_line_utils.c" \
        -I"${GNL_DIR}" \
        -o "${SCRIPT_DIR}/${EXEC_NAME}" 2>> "$ERROR_LOG"

    if [[ $? -ne 0 ]]; then
        echo -e "${RED}❌ Error de compilación${NC}"
        echo -e "${YELLOW}ℹ Ver errores en: ${ERROR_LOG}${NC}"
        return 1
    fi
    return 0
}

if ! compile_gnl; then
    exit 1
fi

# Ejecución de tests
run_tests() {
    echo -e "${BLUE}🚀 Ejecutando tests...${NC}"
    TEST_OUTPUT=$("${SCRIPT_DIR}/${EXEC_NAME}")
    echo "$TEST_OUTPUT"
    
    # Extraer resultados
    TOTAL_TESTS=$(echo "$TEST_OUTPUT" | awk '/Total de tests realizados:/ {print $NF}')
    PASSED_TESTS=$(echo "$TEST_OUTPUT" | awk '/Tests OK:/ {print $3}')
    FAILED_TESTS=$((TOTAL_TESTS - PASSED_TESTS))
    
    # Mostrar resumen
    echo -e "\n${MAGENTA}═══════════════════════════════════════════════${NC}"
    echo -e "${MAGENTA}              RESUMEN FINAL - GNL              ${NC}"
    echo -e "${MAGENTA}═══════════════════════════════════════════════${NC}"
    echo -e "  ${CYAN}Total tests: ${NC}${TOTAL_TESTS}"
    echo -e "  ${GREEN}Aprobados: ${NC}${PASSED_TESTS} ($(awk "BEGIN {printf \"%.1f\", ${PASSED_TESTS}*100/${TOTAL_TESTS}}")%)"
    echo -e "  ${RED}Fallados: ${NC}${FAILED_TESTS} ($(awk "BEGIN {printf \"%.1f\", ${FAILED_TESTS}*100/${TOTAL_TESTS}}")%)"
    echo -e "${MAGENTA}═══════════════════════════════════════════════${NC}"
    
    # Chequear memory leaks solo si valgrind está instalado
    if command -v valgrind &> /dev/null; then
        echo -e "\n${CYAN}🔍 Buscando memory leaks con Valgrind...${NC}"
        valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --quiet "${SCRIPT_DIR}/${EXEC_NAME}" > /dev/null
        if [[ $? -eq 0 ]]; then
            echo -e "${GREEN}✅ Sin memory leaks${NC}"
        else
            echo -e "${RED}❌ Memory leaks detectados!${NC}"
        fi
    fi
}

run_tests

# Limpieza y salida
rm -f "${SCRIPT_DIR}/${EXEC_NAME}"

if [[ $FAILED_TESTS -eq 0 ]]; then
    echo -e "\n${GREEN}🎉 ¡Todos los tests pasaron correctamente!${NC}"
    exit 0
else
    echo -e "\n${RED}❌ Algunos tests fallaron${NC}"
    exit 1
fi