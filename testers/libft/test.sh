#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$(dirname "$SCRIPT_DIR")")"

source "$ROOT_DIR/includes/colors.sh"

# Configuración de archivos
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
ERROR_LOG="$SCRIPT_DIR/libft_errors_$TIMESTAMP.log"

echo -e "${BLUE}🔎 Buscando tu proyecto Libft...${NC}"
LIBFT_DIR=$(find "$HOME" -type f -name "libft.h" -exec dirname {} \; | grep -v 42_tester | head -n 1)

if [[ -z "$LIBFT_DIR" ]]; then
  echo -e "${RED}❌ No se encontró ningún libft.h fuera de 42_tester${NC}"
  exit 1
fi

echo -e "${GREEN}✅ Proyecto encontrado en: $LIBFT_DIR${NC}"

# Check if bonus exists
HAS_BONUS=$(grep -E "bonus:" "$LIBFT_DIR/Makefile" | wc -l)

# Compile libft
if [[ $HAS_BONUS -gt 0 ]]; then
  echo -e "${YELLOW}🔧 Compilando con bonus...${NC}"
  make -C "$LIBFT_DIR" all bonus > /dev/null
else
  echo -e "${YELLOW}🔧 Compilando sin bonus...${NC}"
  make -C "$LIBFT_DIR" re > /dev/null
fi

if [ ! -f "$LIBFT_DIR/libft.a" ]; then
    echo -e "${RED}❌ No se generó libft.a${NC}"
    exit 1
fi

# Compile test
gcc -Wall -Wextra -Werror testers/libft/test_main.c -I"$LIBFT_DIR" "$LIBFT_DIR/libft.a" -o libft_test
if [[ $? -ne 0 ]]; then
    echo -e "${RED}❌ Fallo al compilar los tests${NC}"
    exit 1
fi

# Run test and capture FULL output
echo -e "${GREEN}✅ Ejecutando tests...${NC}"
TEST_OUTPUT=$(./libft_test)
FULL_OUTPUT=$TEST_OUTPUT  # Guardamos la salida completa
echo "$TEST_OUTPUT"

# Process results
TOTAL_TESTS=$(echo "$TEST_OUTPUT" | grep -o "Total Tests: [0-9]*" | awk '{print $3}')
PASSED_TESTS=$(echo "$TEST_OUTPUT" | grep -o "Tests Passed: [0-9]*" | awk '{print $3}')
FAILED_TESTS=$((TOTAL_TESTS - PASSED_TESTS))

# Calculate percentages
if [[ $TOTAL_TESTS -gt 0 ]]; then
    PASS_PERCENT=$((PASSED_TESTS * 100 / TOTAL_TESTS))
    FAIL_PERCENT=$((100 - PASS_PERCENT))
else
    PASS_PERCENT=0
    FAIL_PERCENT=0
fi

# Display summary
echo -e "\n${MAGENTA}═══════════════════════════════════════════════${NC}"
echo -e "${MAGENTA}                RESUMEN FINAL                 ${NC}"
echo -e "${MAGENTA}═══════════════════════════════════════════════${NC}"
echo -e "  ${CYAN}Total de tests realizados: ${NC}${TOTAL_TESTS}"
echo -e "  ${GREEN}Tests OK: ${NC}${PASSED_TESTS} (${PASS_PERCENT}%)"
echo -e "  ${RED}Tests KO: ${NC}${FAILED_TESTS} (${FAIL_PERCENT}%)"
echo -e "${MAGENTA}═══════════════════════════════════════════════${NC}"

# Display success message if all tests passed
if [[ $FAILED_TESTS -eq 0 && $TOTAL_TESTS -gt 0 ]]; then
    echo -e "\n${GREEN}🎉🎉🎉 ¡ENHORABUENA! 🎉🎉🎉${NC}"
    echo -e "${GREEN}¡Todos los tests pasaron correctamente! 💯${NC}"
    echo -e "${GREEN}¡Excelente trabajo! 👏🚀${NC}"
    echo -e "${GREEN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}\n"
fi

# Display failed tests details if any and save to log file
if [[ $FAILED_TESTS -gt 0 ]]; then
    echo -e "\n${RED}🔍 Detalle de tests fallidos:${NC}"
    
    # Create error log with complete information
    echo "--- LIBFT TEST ERROR REPORT ---" > "$ERROR_LOG"
    echo "Fecha: $(date)" >> "$ERROR_LOG"
    echo "Directorio del proyecto: $LIBFT_DIR" >> "$ERROR_LOG"
    echo "Total Tests: $TOTAL_TESTS" >> "$ERROR_LOG"
    echo "Tests Pasados: $PASSED_TESTS" >> "$ERROR_LOG"
    echo "Tests Fallidos: $FAILED_TESTS" >> "$ERROR_LOG"
    echo "--------------------------------" >> "$ERROR_LOG"
    echo "" >> "$ERROR_LOG"
    echo "DETALLE COMPLETO DE ERRORES:" >> "$ERROR_LOG"
    echo "" >> "$ERROR_LOG"
    
    # Extract failed tests with context
    echo "$FULL_OUTPUT" | awk -v error_log="$ERROR_LOG" '
    BEGIN {
        current_section = ""
        print_section = 0
        print_details = 0
    }
    /^--- .* ---$/ {
        current_section = $0
        print_section = 0
        print_details = 0
    }
    /\[KO\]/ || /Test Failed/ {
        if (!print_section) {
            print "\n" current_section >> error_log
            print_section = 1
        }
        print_details = 1
        # Print the failed test line
        sub(/^[ \t]*/, "", $0)
        print "  " $0 >> error_log
        
        # Get next 2 lines for context (expected/got)
        getline; sub(/^[ \t]*/, "", $0); print "  " $0 >> error_log
        getline; sub(/^[ \t]*/, "", $0); print "  " $0 >> error_log
    }
    /\[OK\]/ && print_details {
        print_details = 0
    }'
    
    # Add footer to log file
    echo "" >> "$ERROR_LOG"
    echo "--------------------------------" >> "$ERROR_LOG"
    echo "RECOMENDACIONES:" >> "$ERROR_LOG"
    echo "1. Revisa cada función mencionada comparando con la original" >> "$ERROR_LOG"
    echo "2. Verifica los valores esperados vs obtenidos" >> "$ERROR_LOG"
    echo "3. Comprueba casos límite y manejo de errores" >> "$ERROR_LOG"
    echo "4. Para funciones de strings, revisa terminación nula" >> "$ERROR_LOG"
    echo "5. Para memoria, verifica asignación/liberación correcta" >> "$ERROR_LOG"
    
    # Show log file location with more visibility
    echo -e "\n${YELLOW}📄 Se ha generado un archivo detallado de errores:${NC}"
    echo -e "${YELLOW}👉 $ERROR_LOG 👈${NC}"
    echo -e "${YELLOW}Contiene todos los detalles de los tests fallidos para ayudarte a corregirlos.${NC}"
    echo -e "${YELLOW}Revisa los valores esperados vs obtenidos en cada caso.${NC}"
fi

# Cleanup
rm -f libft_test
make -C "$LIBFT_DIR" fclean > /dev/null

# Exit with appropriate status
if [[ $FAILED_TESTS -gt 0 ]]; then
    exit 1
else
    exit 0
fi