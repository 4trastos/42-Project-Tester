#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define YELLOW "\033[1;33m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

void create_test_file(const char *filename, const char *content) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        perror("Error creando archivo de prueba");
        exit(EXIT_FAILURE);
    }
    fprintf(f, "%s", content);
    fclose(f);
}

int run_command_with_valgrind(const char *cmd) {
    char valgrind_cmd[1024];
    snprintf(valgrind_cmd, sizeof(valgrind_cmd),
             "valgrind --leak-check=full --show-leak-kinds=all "
             "--track-origins=yes --error-exitcode=42 %s",
             cmd);
    
    return system(valgrind_cmd);
}

int run_pipex_test(const char *infile, const char *cmd1, const char *cmd2, const char *outfile) {
    // Ejecución normal
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        return -1;
    }
    
    if (pid == 0) {
        execl("./pipex", "./pipex", infile, cmd1, cmd2, outfile, NULL);
        perror("execl failed");
        exit(EXIT_FAILURE);
    }
    
    int status;
    waitpid(pid, &status, 0);
    
    // Ejecución con valgrind
    char valgrind_cmd[1024];
    snprintf(valgrind_cmd, sizeof(valgrind_cmd),
             "./pipex %s \"%s\" \"%s\" valgrind_out", 
             infile, cmd1, cmd2);
    
    int valgrind_status = run_command_with_valgrind(valgrind_cmd);
    
    return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
}

int compare_files(const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "r");
    FILE *f2 = fopen(file2, "r");
    if (!f1 || !f2) {
        if (!f1) fprintf(stderr, RED "Error abriendo %s\n" RESET, file1);
        if (!f2) fprintf(stderr, RED "Error abriendo %s\n" RESET, file2);
        return 0;
    }
    
    int equal = 1;
    int c1, c2;
    while ((c1 = fgetc(f1)) != EOF && (c2 = fgetc(f2)) != EOF) {
        if (c1 != c2) {
            equal = 0;
            break;
        }
    }
    
    if (c1 != c2) equal = 0;
    
    fclose(f1);
    fclose(f2);
    return equal;
}

void run_test(const char *test_name, const char *infile, 
              const char *cmd1, const char *cmd2, int expected_exit) {
    printf(CYAN "\n🧪 %s\n" RESET, test_name);

    // Verificar comandos primero
    if (system(strjoin("which ", strtok(strdup(cmd1), " > /dev/null")) != 0))
    {
        printf(YELLOW "⚠ Comando 1 no encontrado, omitiendo test\n" RESET);
        return;
    }
    
    if (system(strjoin("which ", strtok(strdup(cmd2), " > /dev/null")) != 0)) 
    {
        printf(YELLOW "⚠ Comando 2 no encontrado, omitiendo test\n" RESET);
        return;
    }

    printf("Comando: pipex %s \"%s\" \"%s\" output\n", infile, cmd1, cmd2);
    
    // Ejecutar versión shell
    char shell_cmd[1024];
    snprintf(shell_cmd, sizeof(shell_cmd), "< %s %s | %s > expected", infile, cmd1, cmd2);
    system(shell_cmd);
    
    // Ejecutar pipex
    int result = run_pipex_test(infile, cmd1, cmd2, "output");
    int files_match = compare_files("expected", "output");
    
    // Verificar resultados
    if (result == expected_exit && files_match) {
        printf(GREEN "✅ %s passed\n" RESET, test_name);
    } else {
        printf(RED "❌ %s failed\n" RESET, test_name);
        if (result != expected_exit) 
            printf(RED "   Exit code: %d (expected %d)\n" RESET, result, expected_exit);
        if (!files_match) 
            printf(RED "   Output differs from expected\n" RESET);
    }
    
    // Limpieza
    unlink("output");
    unlink("expected");
    unlink("valgrind_out");
}

int main() {
    // Configurar entorno de prueba
    create_test_file("infile", "line 1\nline 2\nline 3\n");
    create_test_file("restricted", "secret\n");
    chmod("restricted", 0000);
    
    // Ejecutar pruebas
    run_test("Basic test", "infile", "cat", "wc -l", 0);
    run_test("Invalid command", "infile", "invalidcmd", "wc -l", 127);
    run_test("Missing input", "missing", "cat", "wc -l", 1);
    run_test("No permissions", "restricted", "cat", "wc -l", 1);
    run_test("Command with args", "infile", "ls -la", "grep .c", 0);
    run_test("Complex command", "infile", "echo 'Hello'", "tr 'a-z' 'A-Z'", 0);
    
    // Limpieza final
    unlink("infile");
    unlink("restricted");
    
    printf(CYAN "\n✨ Todas las pruebas completadas\n" RESET);
    return 0;
}