#!/bin/bash
source includes/colors.sh
source includes/banner.sh

clear
banner_42

PS3="Elige una categoría para testear: "
options=("Common Core" "Outer Core" "Exams" "Salir")

select opt in "${options[@]}"; do
  case $opt in
    "Common Core")
      ./testers/common_core_menu.sh
      break ;;
    "Outer Core")
      echo "🚧 Outer Core en construcción"
      break ;;
    "Exams")
      echo "🚧 Exámenes en construcción"
      break ;;
    "Salir")
      echo "👋 Hasta pronto!"
      break ;;
    *) echo "❌ Opción inválida" ;;
  esac
done
