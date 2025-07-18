#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$(dirname "$SCRIPT_DIR")")"
source "$ROOT_DIR/includes/colors.sh"

# Configuración
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
ERROR_LOG="$SCRIPT_DIR/gnl_errors_$TIMESTAMP.log"
EXEC_NAME="gnl_test"

echo -e "${BLUE}🔎 Buscando tu proyecto get_next_line...${NC}"
GNL_DIR=$(find "$HOME" -type f -name "get_next_line.h" -exec dirname {} \; | grep -v 42_tester | head -n 1)

if [[ -z "$GNL_DIR" ]]; then
  echo -e "${RED}❌ No se encontró ningún get_next_line.h fuera de 42_tester${NC}"
  exit 1
fi

echo -e "${GREEN}✅ Proyecto encontrado en: $GNL_DIR${NC}"

# Compilar test
echo -e "${YELLOW}🔧 Compilando get_next_line con test_main.c...${NC}"

gcc -Wall -Wextra -Werror \
    "$SCRIPT_DIR/test_main.c" \
    "$GNL_DIR/get_next_line.c" \
    "$GNL_DIR/get_next_line_utils.c" \
    -I"$GNL_DIR" \
    -o "$SCRIPT_DIR/$EXEC_NAME" || {
  echo -e "${RED}❌ Fallo al compilar el test${NC}"
  exit 1
}

echo -e "${GREEN}✅ Ejecutando tests...${NC}"
TEST_OUTPUT=$("$SCRIPT_DIR/$EXEC_NAME")
echo "$TEST_OUTPUT"

# Extraer resultados del resumen final
TOTAL_TESTS=$(echo "$TEST_OUTPUT" | awk '/Total de tests realizados:/ {print $NF}')
PASSED_TESTS=$(echo "$TEST_OUTPUT" | awk '/Tests OK:/ {print $3}')
FAILED_TESTS=$((TOTAL_TESTS - PASSED_TESTS))

# Calcular porcentajes
if [[ $TOTAL_TESTS -gt 0 ]]; then
  PASS_PERCENT=$(awk "BEGIN {printf \"%.2f\", $PASSED_TESTS * 100 / $TOTAL_TESTS}")
  FAIL_PERCENT=$(awk "BEGIN {printf \"%.2f\", 100 - $PASS_PERCENT}")
else
  PASS_PERCENT=0
  FAIL_PERCENT=0
fi

# Resumen final mejorado
echo -e "\n${MAGENTA}═══════════════════════════════════════════════${NC}"
echo -e "${MAGENTA}              RESUMEN FINAL - GNL              ${NC}"
echo -e "${MAGENTA}═══════════════════════════════════════════════${NC}"
echo -e "  ${CYAN}Total de tests realizados: ${NC}${TOTAL_TESTS}"
echo -e "  ${GREEN}Tests OK: ${NC}${PASSED_TESTS} (${PASS_PERCENT}%)"
echo -e "  ${RED}Tests KO: ${NC}${FAILED_TESTS} (${FAIL_PERCENT}%)"
echo -e "${MAGENTA}═══════════════════════════════════════════════${NC}"

# Cleanup
rm -f gnl_test

# Todos pasaron
if [[ $FAILED_TESTS -eq 0 && $TOTAL_TESTS -gt 0 ]]; then
  echo -e "\n${GREEN}🎉 ¡Todos los tests pasaron correctamente! 💯${NC}"
  exit 0
elif [[ $TOTAL_TESTS -eq 0 ]]; then
  echo -e "\n${YELLOW}⚠️ No se ejecutaron tests válidos${NC}"
  exit 1
else
  echo -e "\n${RED}❌ Algunos tests fallaron${NC}"
  exit 1
fi
