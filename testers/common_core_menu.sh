#!/bin/bash
SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"

source "$ROOT_DIR/includes/colors.sh"
source "$ROOT_DIR/includes/banner.sh"

clear
banner_42
echo -e "${CYAN}📦 𝟜𝟚 Common Core Projects${NC}"
echo

PS3="Elige el proyecto que deseas testear: "
options=("Libft" "get_next_line" "ft_printf" "Volver")

select opt in "${options[@]}"; do
  case $opt in
    "Libft")
      ./testers/libft/test.sh
      break ;;
    "get_next_line")
      echo -e "${YELLOW}🚧 get_next_line aún no está implementado${NC}"
      break ;;
    "ft_printf")
      echo -e "${YELLOW}🚧 ft_printf aún no está implementado${NC}"
      break ;;
    "Volver")
      ../run.sh
      break ;;
    *) echo -e "${RED}❌ Opción inválida${NC}" ;;
  esac
done
