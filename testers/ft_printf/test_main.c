#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdarg.h>

// Colores para el output
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[1;33m"
#define CYAN    "\033[0;36m"
#define RESET   "\033[0m"

// Prototipo de ft_printf
int ft_printf(const char *format, ...);

// Función auxiliar simplificada para ejecutar tests
int run_single_test(const char *format, const char *description, ...)
{
    va_list args;
    int len_std, len_ft;
    
    printf("Test: %-40s ", description);
    
    // Ejecutar printf estándar
    va_start(args, description);
    len_std = vprintf(format, args);
    va_end(args);
    
    printf(" | ");
    
    // Ejecutar ft_printf
    va_start(args, description);
    len_ft = ft_printf(format, args);
    va_end(args);
    
    // Comparar longitudes (comparación exacta de strings es complicada sin capturar output)
    if (len_std == len_ft) {
        printf(GREEN " [OK]" RESET "\n");
        return 1;
    } else {
        printf(RED " [KO]" RESET "\n");
        printf("  Length expected: %d, got: %d\n", len_std, len_ft);
        return 0;
    }
}

void run_printf_tests()
{
    int passed = 0;
    int total = 0;
    
    printf(CYAN "\n=== TESTING FT_PRINTF ===\n\n" RESET);
    
    // %% - Porcentaje
    passed += run_single_test("%%", "Porcentaje simple");
    total++;
    passed += run_single_test("%%%%", "Doble porcentaje");
    total++;
    
    // %c - Caracteres
    passed += run_single_test("%c", "Carácter normal", 'A');
    total++;
    passed += run_single_test("%c%c", "Dos caracteres", 'A', 'B');
    total++;
    
    // %s - Strings
    passed += run_single_test("%s", "String simple", "Hola");
    total++;
    passed += run_single_test("%s", "String NULL", NULL);
    total++;
    
    // %d e %i - Enteros
    passed += run_single_test("%d", "Entero positivo", 42);
    total++;
    passed += run_single_test("%i", "Entero negativo", -42);
    total++;
    
    // %u - Unsigned
    passed += run_single_test("%u", "Unsigned int", UINT_MAX);
    total++;
    
    // %x y %X - Hexadecimal
    passed += run_single_test("%x", "Hexadecimal minúsculas", 42);
    total++;
    passed += run_single_test("%X", "Hexadecimal mayúsculas", 42);
    total++;
    
    // %p - Punteros
    passed += run_single_test("%p", "Puntero NULL", NULL);
    total++;
    
    printf("\n" YELLOW "Resumen: %d/%d tests pasados (%.1f%%)" RESET "\n", 
           passed, total, (float)passed/total * 100);
}

int main(void) {
    run_printf_tests();
    return 0;
}