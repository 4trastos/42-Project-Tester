#!/bin/bash

RED='\033[1;31m'; GREEN='\033[1;32m'; YELLOW='\033[1;33m'; CYAN='\033[1;36m'; RESET='\033[0m'

# Configuración
TEST_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LOG_FILE="$TEST_DIR/pipex_test.log"
VALGRIND_LOG="$TEST_DIR/valgrind.log"
rm -f "$LOG_FILE" "$VALGRIND_LOG" 2>/dev/null

# Buscar pipex
find_pipex() {
    echo -e "${CYAN}🔎 Buscando pipex...${RESET}"
    
    local search_dirs=(
        "."
        "$HOME"
        "$HOME/Desktop"
        "$HOME/Documents"
        "$HOME/projects"
    )
    
    for dir in "${search_dirs[@]}"; do
        MAKEFILE=$(find "$dir" -type f -name "Makefile" -exec grep -l "pipex" {} \; 2>/dev/null | head -n 1)
        if [[ -n "$MAKEFILE" ]]; then
            PROJ_DIR=$(dirname "$MAKEFILE")
            echo -e "${GREEN}✅ Proyecto encontrado en: $PROJ_DIR${RESET}"
            return 0
        fi
    done
    
    echo -e "${RED}❌ No se encontró pipex${RESET}"
    echo -e "${YELLOW}ℹ Ejecuta desde tu directorio pipex o usa: $0 /ruta/a/pipex${RESET}"
    exit 1
}

if [[ -n "$1" ]]; then
    PROJ_DIR="$1"
    MAKEFILE="$PROJ_DIR/Makefile"
    if [[ ! -f "$MAKEFILE" ]] || ! grep -q "pipex" "$MAKEFILE"; then
        echo -e "${RED}❌ No es un proyecto pipex válido${RESET}"
        exit 1
    fi
else
    find_pipex
fi

EXE="$PROJ_DIR/pipex"

# Compilación con soporte para valgrind
compile_project() {
    echo -e "${CYAN}🔍 Compilando proyecto...${RESET}"
    
    make -C "$PROJ_DIR" fclean >> "$LOG_FILE" 2>&1 || echo -e "${YELLOW}⚠ make fclean falló (continuando...)"
    
    if ! make -C "$PROJ_DIR" >> "$LOG_FILE" 2>&1; then
        echo -e "${RED}❌ Error de compilación${RESET}"
        echo -e "Revisa $LOG_FILE"
        exit 1
    fi
    
    if [[ ! -x "$EXE" ]]; then
        echo -e "${RED}❌ No se generó el ejecutable $EXE${RESET}"
        exit 1
    fi
    
    echo -e "${GREEN}✅ Compilación exitosa${RESET}"
}

# Ejecutar con valgrind
run_valgrind_test() {
    local cmd="$1"
    echo -e "${CYAN}🐛 Ejecutando Valgrind...${RESET}"
    
    valgrind --leak-check=full \
             --show-leak-kinds=all \
             --track-origins=yes \
             --log-file="$VALGRIND_LOG" \
             --error-exitcode=42 \
             $cmd >> "$LOG_FILE" 2>&1
            
    local valgrind_exit=$?
    
    if [ -f "$VALGRIND_LOG" ]; then
        if grep -q "no leaks are possible" "$VALGRIND_LOG"; then
            echo -e "${GREEN}✅ Valgrind: No memory leaks${RESET}"
        else
            echo -e "${RED}❌ Valgrind: Memory issues detected${RESET}"
            cat "$VALGRIND_LOG"
        fi
    else
        echo -e "${YELLOW}⚠ Valgrind: No se generó log${RESET}"
    fi
}

# Función de test mejorada
run_test_case() {
    local name="$1"
    local infile="$2"
    local cmd1="$3"
    local cmd2="$4"
    
    echo -e "\n${CYAN}🧪 Test: $name${RESET}"
    echo -e "Comando: $EXE $infile \"$cmd1\" \"$cmd2\" outfile"
    
    # Verificar si los comandos existen
    if ! command -v $(echo "$cmd1" | awk '{print $1}') >/dev/null 2>&1; then
        echo -e "${YELLOW}⚠ Comando 1 ($cmd1) no encontrado en PATH${RESET}"
        return
    fi
    
    if ! command -v $(echo "$cmd2" | awk '{print $1}') >/dev/null 2>&1; then
        echo -e "${YELLOW}⚠ Comando 2 ($cmd2) no encontrado en PATH${RESET}"
        return
    fi
    
    # Ejecutar pipex
    "$EXE" "$infile" "$cmd1" "$cmd2" outfile >> "$LOG_FILE" 2>&1
    local exit_code=$?
    
    # Verificar resultado
    if [ $exit_code -eq 0 ]; then
        echo -e "${GREEN}✅ Ejecución exitosa${RESET}"
        
        # Verificar output
        local expected="expected_$RANDOM"
        eval "< $infile $cmd1 | $cmd2 > $expected"
        
        if diff -q outfile $expected >/dev/null; then
            echo -e "${GREEN}✅ Output correcto${RESET}"
        else
            echo -e "${RED}❌ Output incorrecto${RESET}"
        fi
        rm -f "$expected"
    else
        echo -e "${RED}❌ Falló con código $exit_code${RESET}"
    fi

    run_valgrind_test "$EXE $infile \"$cmd1\" \"$cmd2\" valgrind_out"
}

# Preparar entorno de pruebas
prepare_test_env() {
    # Crear archivos de prueba
    echo -e "linea 1\nlinea 2\nlinea 3" > infile
    echo -e "contenido secreto" > noleer
    chmod 000 noleer
    rm -f outfile expected valgrind_out 2>/dev/null
}

# Limpiar después de pruebas
clean_test_env() {
    rm -f infile noleer outfile expected valgrind_out 2>/dev/null
    make -C "$PROJ_DIR" fclean > /dev/null
}

# Ejecutar suite de pruebas
run_test_suite() {
    prepare_test_env
    
    # Tests básicos
    run_test_case "Caso básico" "infile" "cat" "wc -l" 0
    run_test_case "Comando inválido" "infile" "comando_inexistente" "wc -l" 127
    run_test_case "Input faltante" "noexiste" "cat" "wc -l" 1
    run_test_case "Permisos denegados" "noleer" "cat" "wc -l" 1
    
    # Tests adicionales
    run_test_case "Comando con argumentos" "infile" "ls -la" "grep .c" 0
    run_test_case "Comando complejo" "infile" "echo 'Hola'" "tr 'a-z' 'A-Z'" 0
    
    clean_test_env
}

# Ejecución principal
compile_project
run_test_suite

echo -e "\n${CYAN}✨ Pruebas completadas.${RESET}"
echo -e "${CYAN}📝 Log de ejecución: $LOG_FILE${RESET}"
echo -e "${CYAN}🧠 Valgrind log: $VALGRIND_LOG${RESET}"