#!/bin/bash
SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"

source "$ROOT_DIR/includes/colors.sh"
source "$ROOT_DIR/includes/banner.sh"

clear
banner_42
echo -e "${CYAN}📦 𝟜𝟚 Common Core Projects${NC}"
echo

options=("Libft" "get_next_line" "ft_printf" "pipex" "so_long" "push_swap" "minishell" "philosophers" "Cube3D" "CPP" "Volver")

# Mostrar opciones en vertical (sin usar select)
for i in "${!options[@]}"; do
  printf "%2d) %s\n" $((i+1)) "${options[i]}"
done
echo

# Leer entrada del usuario manualmente
while true; do
  read -p "Elige el proyecto que deseas testear (1-${#options[@]}): " choice
  
  # Verificar si la entrada es un número válido
  if [[ "$choice" =~ ^[0-9]+$ ]] && [ "$choice" -ge 1 ] && [ "$choice" -le ${#options[@]} ]; then
    opt="${options[choice-1]}"
    break
  else
    echo -e "${RED}❌ Opción inválida, por favor elige un número entre 1 y ${#options[@]}${NC}"
  fi
done

# Ejecutar la opción seleccionada
case $opt in
  "Libft")
    ./testers/libft/test.sh
    ;;
  "get_next_line")
    ./testers/get_next_line/test.sh
    ;;
  "ft_printf")
    ./testers/ft_printf/test.sh
    ;;
  "pipex")
    ./testers/pipex/test.sh
    ;;
  "so_long")
    echo -e "${YELLOW}🚧 so_long aún no está implementado${NC}"
    ;;
  "push_swap")
    echo -e "${YELLOW}🚧 push_swap aún no está implementado${NC}"
    ;;
  "minishell")
    echo -e "${YELLOW}🚧 minishell aún no está implementado${NC}"
    ;;
  "philosophers")
    echo -e "${YELLOW}🚧 philosophers aún no está implementado${NC}"
    ;;
  "Cube3D")
    echo -e "${YELLOW}🚧 Cube3D aún no está implementado${NC}"
    ;;
  "CPP")
    echo -e "${YELLOW}🚧 CPP aún no está implementado${NC}"
    ;;
  "Volver")
    ./run.sh
    ;;
esac