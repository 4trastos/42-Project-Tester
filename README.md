# 🧪 42 Project Tester

[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

Un tester modular y automatizado para los proyectos del currículo de [42 Network](https://42.fr):

- ✅ **Common Core**
- 🚧 **Outer Core** (en desarrollo)
- 🚧 **Exams** (en desarrollo)

> ⚙️ Diseñado para testear con rapidez, claridad y estilo profesional.

---

## 📦 Proyectos soportados

### ✅ Common Core
- Libft
- ft_printf
- get_next_line
- Born2beroot
- so_long
- push_swap
- minishell
- Philosophers
- ...

### 🚧 Outer Core (próximamente)
- NetPractice
- Inception
- cub3d
- ft_transcendence

### 🚧 Exams
- C Piscine Rush
- Exam Rank 02-05

---

## 🚀 Instalación y uso

```bash
git clone https://github.com/tu-usuario/42-project-tester.git
cd 42-project-tester
chmod +x run.sh
./run.sh
````

---

## 🧭 Menú interactivo

```text
Elige una categoría para testear:
1) Common Core
2) Outer Core
3) Exams
4) Salir
```

---

## 📁 Estructura del repositorio

```
42-project-tester/
├── run.sh
├── includes/
│   ├── colors.sh
│   └── banner.sh
├── testers/
│   ├── common_core_menu.sh
│   ├── outer_core_menu.sh  (en desarrollo)
│   └── exams_menu.sh       (en desarrollo)
├── common_core/
│   ├── libft/
│   ├── printf/
│   └── ...
├── README.md
```

Cada categoría tendrá su propio subdirectorio y su propio `README.md`.

---

## 🎯 Objetivos

* Automatizar tests funcionales.
* Verificar cumplimiento con norminette.
* Facilitar la autoevaluación y revisión entre pares.
* Imitar la moulinette sin perder flexibilidad.

---

## 🧪 Integración de Tests

* ✅ Norminette: se lanza automáticamente sobre el código.
* ✅ Makefile: validación de reglas obligatorias.
* ✅ Funcionalidad: tests definidos para cada ejercicio.
* 🧪 Integración continua con GitHub Actions (`.github/workflows/ci.yml`).
* 🔜 Cobertura de código (`lcov + genhtml` o integración con Codecov).

---

## 📋 Subcategorías

Cada subtester incluirá su propio `README.md` con instrucciones, criterios y ejemplos:

* [`common_core/README.md`](common_core/README.md)
* [`outer_core/README.md`](outer_core/README.md) *(próximamente)*
* [`exams/README.md`](exams/README.md) *(próximamente)*

---

## 🛠️ Requisitos

* Bash 4.0+
* `norminette` instalado (si estás en 42, ya lo tienes)
* Linux o macOS

---

## 🤝 Contribuir

¡Pull requests bienvenidos!

1. Crea una rama:

   ```bash
   git checkout -b feature/nueva-funcion
   ```
2. Haz cambios y commitea:

   ```bash
   git commit -m "Añade test para push_swap"
   ```
3. Haz push y abre un PR.

---

## 📜 Licencia

MIT License – libre para usar, modificar y compartir.
*Este proyecto no está afiliado oficialmente con 42 Network.*

---

## 👨‍💻 Autor

Desarrollado por [davgalle](https://github.com/4trastos)
Hecho con ❤️ para los cadetes de 42.

````

---

### 🗂 Archivos `README.md` por categoría

#### 📁 `common_core/README.md`
```markdown
# 🧪 Common Core Tester

Este subtester incluye scripts para testear automáticamente los proyectos del Common Core de 42:

## Proyectos soportados:

- [x] Libft
- [x] ft_printf
- [x] get_next_line
- [x] Born2beroot
- [x] so_long
- [x] push_swap
- [x] minishell
- [x] Philosophers

Cada carpeta contiene:

- Tests funcionales
- Validación de Norminette
- Revisión del Makefile
- Errores comunes y edge cases

Ejemplo de uso:

```bash
cd common_core/libft
./test.sh
````

```

---

¿Quieres que te prepare también la configuración del workflow `ci.yml` de GitHub Actions, incluyendo validación de Norminette y tests automáticos?
```
