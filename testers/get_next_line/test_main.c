#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// Colores
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[1;33m"
#define CYAN    "\033[0;36m"
#define MAGENTA "\033[0;35m"
#define RESET   "\033[0m"

#define MAX_LINE_LENGTH 1024
#define LARGE_FILE_LINES 19785

void create_large_test_file(const char *path, size_t size_mb) {
    FILE *f = fopen(path, "w");
    if (!f) return;
    
    const char *base_line = "Esta es una línea de prueba para el archivo grande.\n";
    size_t lines = (size_mb * 1024 * 1024) / strlen(base_line);
    
    for (size_t i = 0; i < lines; i++) {
        fprintf(f, "%zu: %s", i, base_line);
    }
    
    fprintf(f, "ÚLTIMA_LÍNEA\n");
    fclose(f);
}

void clean_newline(char *str) {
    if (!str) return;
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

int assert_line(const char *expected, char *got, int line_num, const char *file_name) {
    clean_newline(got);

    if (!got || strcmp(expected, got) != 0) {
        printf(RED "[KO]" RESET " %s - Línea %d\n  Esperado: \"%s\"\n  Obtenido: \"%s\"\n",
               file_name, line_num, expected, got ? got : "(null)");
        return 0;
    }
    printf(GREEN "[OK]" RESET " %s - Línea %d\n", file_name, line_num);
    return 1;
}

int verify_large_file_line(int line_num, char *line) {
    static char expected[100];
    
    if (line_num < LARGE_FILE_LINES) {
        snprintf(expected, sizeof(expected), "%d: Esta es una línea de prueba para el archivo grande.", line_num-1);
        clean_newline(line);
        if (strcmp(expected, line) != 0) {
            printf(RED "[KO]" RESET " large_file.txt - Línea %d\n  Esperado: \"%s\"\n  Obtenido: \"%s\"\n",
                   line_num, expected, line);
            return 0;
        }
        return 1;
    } else if (line_num == LARGE_FILE_LINES) {
        clean_newline(line);
        if (strcmp(line, "ÚLTIMA_LÍNEA") != 0) {
            printf(RED "[KO]" RESET " large_file.txt - Línea %d\n  Esperado: \"ÚLTIMA_LÍNEA\"\n  Obtenido: \"%s\"\n", 
                   line_num, line);
            return 0;
        }
        return 1;
    }
    return 0;
}

int run_large_file_test(const char *input_file, int *total_tests, int *total_passed) {
    int fd = open(input_file, O_RDONLY);
    if (fd < 0) {
        printf(RED "Error: No se pudo abrir %s\n" RESET, input_file);
        return 0;
    }

    char *line;
    int line_num = 0;
    int passed = 0;
    clock_t start = clock();

    while ((line = get_next_line(fd)) != NULL) {
        line_num++;
        (*total_tests)++;
        
        if (line_num <= LARGE_FILE_LINES) {
            if (verify_large_file_line(line_num, line)) {
                passed++;
            }
        } else {
            printf(RED "[KO]" RESET " %s - Línea %d\n  Esperado: EOF\n  Obtenido: \"%s\"\n", 
                   input_file, line_num, line);
        }
        
        free(line);
    }

    if (line_num < LARGE_FILE_LINES) {
        printf(RED "[KO]" RESET " %s - Faltan líneas (leídas %d, esperado %d)\n", 
               input_file, line_num, LARGE_FILE_LINES);
    }

    (*total_passed) += passed;
    double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;

    printf(YELLOW "\nResumen %s — Tiempo: %.3fs — Tests: %d/%d\n\n" RESET,
           input_file, elapsed, passed, line_num);

    close(fd);
    return passed == LARGE_FILE_LINES && line_num == LARGE_FILE_LINES;
}

int run_test_file(const char *input_file, const char *expected_file, int *total_tests, int *total_passed) {
    if (strstr(input_file, "large_file.txt")) {
        return run_large_file_test(input_file, total_tests, total_passed);
    }

    FILE *exp = fopen(expected_file, "r");
    if (!exp) {
        printf(RED "Error: No se pudo abrir el archivo esperado %s\n" RESET, expected_file);
        return 0;
    }

    int fd = open(input_file, O_RDONLY);
    if (fd < 0) {
        printf(RED "Error: No se pudo abrir el archivo de entrada %s\n" RESET, input_file);
        fclose(exp);
        return 0;
    }

    char expected[MAX_LINE_LENGTH];
    char *got;
    int file_passed = 0;
    int file_total = 0;
    clock_t start = clock();

    while (fgets(expected, MAX_LINE_LENGTH, exp)) {
        clean_newline(expected);
        got = get_next_line(fd);
        file_total++;
        (*total_tests)++;
        file_passed += assert_line(expected, got, file_total, input_file);
        free(got);
    }

    while ((got = get_next_line(fd)) != NULL) {
        file_total++;
        (*total_tests)++;
        printf(RED "[KO]" RESET " %s - Línea %d\n  Esperado: EOF\n  Obtenido: \"%s\"\n", 
               input_file, file_total, got);
        free(got);
    }

    close(fd);
    fclose(exp);

    (*total_passed) += file_passed;
    double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;

    printf(YELLOW "\nResumen %s — Tiempo: %.3fs — Tests: %d/%d\n\n" RESET,
           input_file, elapsed, file_passed, file_total);

    return file_passed == file_total;
}

int test_multiple_fd(int *total_tests, int *total_passed) {
    printf(MAGENTA "\n=== TEST AVANZADO: MÚLTIPLES FILE DESCRIPTORS ===\n" RESET);
    
    if (access("testers/get_next_line/files/multi_fd1.txt", F_OK) != 0 ||
        access("testers/get_next_line/files/multi_fd2.txt", F_OK) != 0) {
        printf(RED "Error: Archivos para test multi FD no encontrados\n" RESET);
        return 0;
    }
    
    int fd1 = open("testers/get_next_line/files/multi_fd1.txt", O_RDONLY);
    int fd2 = open("testers/get_next_line/files/multi_fd2.txt", O_RDONLY);
    
    if (fd1 < 0 || fd2 < 0) {
        printf(RED "Error: No se pudieron abrir los archivos multi FD\n" RESET);
        return 0;
    }
    
    int tests = 0, passed = 0;
    char *line1, *line2;
    
    line1 = get_next_line(fd1);
    (*total_tests)++;
    passed += assert_line("Primera línea FD1", line1, ++tests, "MULTI_FD_TEST");
    if (passed == tests) (*total_passed)++;
    free(line1);
    
    line2 = get_next_line(fd2);
    (*total_tests)++;
    passed += assert_line("Primera línea FD2", line2, ++tests, "MULTI_FD_TEST");
    if (passed == tests) (*total_passed)++;
    free(line2);
    
    line1 = get_next_line(fd1);
    (*total_tests)++;
    passed += assert_line("Segunda línea FD1", line1, ++tests, "MULTI_FD_TEST");
    if (passed == tests) (*total_passed)++;
    free(line1);
    
    printf(YELLOW "Resumen MULTI_FD — Tests: %d/%d\n\n" RESET, passed, tests);
    
    close(fd1);
    close(fd2);
    return passed == tests;
}

int stress_test_small_buffer(const char *file) {
    printf(MAGENTA "\n=== TEST DE STRESS CON BUFFER PEQUEÑO ===\n" RESET);
    
    if (access(file, F_OK) != 0) {
        printf(RED "Error: Archivo para test de stress no encontrado\n" RESET);
        return 0;
    }
    
    int fd = open(file, O_RDONLY);
    if (fd < 0) return 0;
    
    char *line;
    int lines = 0;
    clock_t start = clock();
    
    while ((line = get_next_line(fd)) != NULL) {
        free(line);
        lines++;
    }
    
    double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("Leídas %d líneas en %.3fs (BUFFER_SIZE=%d)\n", lines, elapsed, BUFFER_SIZE);
    
    close(fd);
    return 1;
}

void print_final_summary(int total_tests, int total_passed) {
    printf("\n═══════════════════════════════════════════════\n");
    printf("              RESUMEN FINAL - GNL              \n");
    printf("═══════════════════════════════════════════════\n");
    printf("  Total de tests realizados: %d\n", total_tests);
    printf("  Tests OK: %d (%.2f%%)\n", total_passed,
           total_tests > 0 ? 100.0 * total_passed / total_tests : 0.0);
    printf("  Tests KO: %d (%.2f%%)\n", total_tests - total_passed,
           total_tests > 0 ? 100.0 * (total_tests - total_passed) / total_tests : 0.0);
    printf("═══════════════════════════════════════════════\n");
}

int main(void) {
    system("mkdir -p testers/get_next_line/files/expected");
    create_large_test_file("testers/get_next_line/files/large_file.txt", 1);

    const char *test_files[][2] = {
        {"testers/get_next_line/files/normal.txt", "testers/get_next_line/files/expected/normal.txt"},
        {"testers/get_next_line/files/empty.txt", "testers/get_next_line/files/expected/empty.txt"},
        {"testers/get_next_line/files/one_line.txt", "testers/get_next_line/files/expected/one_line.txt"},
        {"testers/get_next_line/files/no_newline.txt", "testers/get_next_line/files/expected/no_newline.txt"},
        {"testers/get_next_line/files/multiblank.txt", "testers/get_next_line/files/expected/multiblank.txt"},
        {"testers/get_next_line/files/large_file.txt", NULL},
        {"testers/get_next_line/files/multi_fd1.txt", "testers/get_next_line/files/expected/multi_fd1.txt"},
        {"testers/get_next_line/files/multi_fd2.txt", "testers/get_next_line/files/expected/multi_fd2.txt"}
    };

    int total_files = sizeof(test_files) / sizeof(test_files[0]);
    int total_tests = 0, total_passed = 0;
    int all_passed = 1;

    for (int i = 0; i < total_files; i++) {
        printf(CYAN "\n📁 Test %d: %s\n" RESET, i + 1, test_files[i][0]);
        if (!run_test_file(test_files[i][0], test_files[i][1], &total_tests, &total_passed))
            all_passed = 0;
    }

    all_passed &= test_multiple_fd(&total_tests, &total_passed);
    all_passed &= stress_test_small_buffer("testers/get_next_line/files/large_file.txt");

    print_final_summary(total_tests, total_passed);

    return all_passed ? 0 : 1;
}



