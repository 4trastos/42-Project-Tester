#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Colores
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[1;33m"
#define CYAN    "\033[0;36m"
#define RESET   "\033[0m"

#define MAX_LINE_LENGTH 1024

void clean_newline(char *str) {
    if (!str) return;
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

int assert_line(const char *expected, char *got, int line_num, const char *file_name) {
    clean_newline(got);  // Limpieza también de la línea obtenida

    if (!got || strcmp(expected, got) != 0) {
        printf(RED "[KO]" RESET " %s - Línea %d\n  Esperado: \"%s\"\n  Obtenido: \"%s\"\n",
               file_name, line_num, expected, got ? got : "(null)");
        return 0;
    }
    printf(GREEN "[OK]" RESET " %s - Línea %d\n", file_name, line_num);
    return 1;
}

int run_test_file(const char *input_file, const char *expected_file, int *total_tests, int *total_passed) {
    FILE *exp = fopen(expected_file, "r");
    if (!exp) {
        perror("Error abriendo archivo esperado");
        return 0;
    }

    int fd = open(input_file, O_RDONLY);
    if (fd < 0) {
        perror("Error abriendo archivo de entrada");
        fclose(exp);
        return 0;
    }

    char expected[MAX_LINE_LENGTH];
    char *got;
    int file_passed = 0;
    int file_total = 0;

    while (fgets(expected, MAX_LINE_LENGTH, exp)) {
        clean_newline(expected);
        got = get_next_line(fd);
        file_total++;
        (*total_tests)++;
        file_passed += assert_line(expected, got, file_total, input_file);
        if (got) free(got);
    }

    // Extra lines
    while ((got = get_next_line(fd)) != NULL) {
        file_total++;
        (*total_tests)++;
        printf(RED "[KO]" RESET " %s - Línea %d\n  Esperado: EOF\n  Obtenido: \"%s\"\n", input_file, file_total, got);
        free(got);
    }

    close(fd);
    fclose(exp);

    (*total_passed) += file_passed;

    printf(YELLOW "\nResumen %s — Total Tests: %d — Tests Passed: %d\n\n" RESET,
           input_file, file_total, file_passed);

    return file_passed == file_total;
}

int main(void) {
    const char *test_files[][2] = {
        {"testers/get_next_line/files/normal.txt",      "testers/get_next_line/files/expected/normal.txt"},
        {"testers/get_next_line/files/empty.txt",       "testers/get_next_line/files/expected/empty.txt"},
        {"testers/get_next_line/files/one_line.txt",    "testers/get_next_line/files/expected/one_line.txt"},
        {"testers/get_next_line/files/no_newline.txt",  "testers/get_next_line/files/expected/no_newline.txt"},
        {"testers/get_next_line/files/multiblank.txt",  "testers/get_next_line/files/expected/multiblank.txt"},
    };

    int total_files = sizeof(test_files) / sizeof(test_files[0]);
    int total_tests = 0, total_passed = 0;
    int all_passed = 1;

    for (int i = 0; i < total_files; i++) {
        printf(CYAN "📁 Test %d: %s\n" RESET, i + 1, test_files[i][0]);
        if (!run_test_file(test_files[i][0], test_files[i][1], &total_tests, &total_passed))
            all_passed = 0;
    }

    printf("\n═══════════════════════════════════════════════\n");
    printf("              RESUMEN FINAL - GNL              \n");
    printf("═══════════════════════════════════════════════\n");
    printf("  Total de tests realizados: %d\n", total_tests);
    printf("  Tests OK: %d (%.2f%%)\n", total_passed,
           total_tests > 0 ? 100.0 * total_passed / total_tests : 0.0);
    printf("  Tests KO: %d (%.2f%%)\n", total_tests - total_passed,
           total_tests > 0 ? 100.0 * (total_tests - total_passed) / total_tests : 0.0);
    printf("═══════════════════════════════════════════════\n");

    if (all_passed) {
        printf(GREEN "\n🎉 Todos los tests han pasado correctamente.\n" RESET);
        return 0;
    } else {
        printf(RED "\n❌ Algunos tests han fallado. Revisa tu implementación.\n" RESET);
        return 1;
    }
}