#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <regex.h>

#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[1;33m"
#define CYAN    "\033[0;36m"
#define RESET   "\033[0m"

int ft_printf(const char *format, ...); // Tu ft_printf

// Función para leer toda la salida del pipe
static char *read_all_from_fd(int fd)
{
    size_t cap = 4096;
    size_t total_read = 0;
    char *buffer = malloc(cap);
    if (!buffer)
        return NULL;

    ssize_t len;
    while ((len = read(fd, buffer + total_read, cap - total_read - 1)) > 0)
    {
        total_read += len;
        if (total_read + 1 >= cap)
        {
            cap *= 2;
            char *tmp = realloc(buffer, cap);
            if (!tmp)
            {
                free(buffer);
                return NULL;
            }
            buffer = tmp;
        }
    }
    if (len < 0)
    {
        free(buffer);
        return NULL;
    }
    buffer[total_read] = '\0';
    return buffer;
}

// Captura salida de ft_printf con callback hijo
char *capture_ft_printf_with_callback(void (*child_func)(void))
{
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return NULL;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return NULL;
    }
    if (pid == 0)
    {
        // hijo
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
            _exit(1);
        close(pipefd[1]);
        child_func();
        fflush(stdout);
        _exit(0);
    }
    // padre
    close(pipefd[1]);
    char *buffer = read_all_from_fd(pipefd[0]);
    close(pipefd[0]);
    wait(NULL);
    return buffer;
}

// Captura salida de printf variádico
char *capture_printf(const char *format, ...)
{
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return NULL;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return NULL;
    }
    if (pid == 0)
    {
        // hijo
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
            _exit(1);
        close(pipefd[1]);

        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        fflush(stdout);
        _exit(0);
    }
    // padre
    close(pipefd[1]);
    char *buffer = read_all_from_fd(pipefd[0]);
    close(pipefd[0]);
    wait(NULL);
    return buffer;
}

// --- Tests con llamadas concretas a ft_printf ---

void child_test_percent(void) { ft_printf("%%"); }
int test_percent(void)
{
    char *expected = capture_printf("%%");
    char *actual   = capture_ft_printf_with_callback(child_test_percent);
    int result = (expected && actual && strcmp(expected, actual) == 0);
    if (result)
        fprintf(stderr, GREEN "Test %% literal [OK]\n" RESET);
    else
    {
        fprintf(stderr, RED "Test %% literal [KO]\n");
        fprintf(stderr, " Esperado: \"%s\"\n Obtenido: \"%s\"\n" RESET, expected ? expected : "(null)", actual ? actual : "(null)");
    }
    free(expected);
    free(actual);
    return result;
}

void child_test_string(void) { ft_printf("Hello %s!", "world"); }
int test_string(void)
{
    char *expected = capture_printf("Hello %s!", "world");
    char *actual   = capture_ft_printf_with_callback(child_test_string);
    int result = (expected && actual && strcmp(expected, actual) == 0);
    if (result)
        fprintf(stderr, GREEN "Test %%s [OK]\n" RESET);
    else
    {
        fprintf(stderr, RED "Test %%s [KO]\n");
        fprintf(stderr, " Esperado: \"%s\"\n Obtenido: \"%s\"\n" RESET, expected ? expected : "(null)", actual ? actual : "(null)");
    }
    free(expected);
    free(actual);
    return result;
}

void child_test_int(void) { ft_printf("Number: %d", 42); }
int test_int(void)
{
    char *expected = capture_printf("Number: %d", 42);
    char *actual   = capture_ft_printf_with_callback(child_test_int);
    int result = (expected && actual && strcmp(expected, actual) == 0);
    if (result)
        fprintf(stderr, GREEN "Test %%d [OK]\n" RESET);
    else
    {
        fprintf(stderr, RED "Test %%d [KO]\n");
        fprintf(stderr, " Esperado: \"%s\"\n Obtenido: \"%s\"\n" RESET, expected ? expected : "(null)", actual ? actual : "(null)");
    }
    free(expected);
    free(actual);
    return result;
}

void child_test_hex(void) { ft_printf("Hex: %x", 0xABCD); }
int test_hex(void)
{
    char *expected = capture_printf("Hex: %x", 0xABCD);
    char *actual   = capture_ft_printf_with_callback(child_test_hex);
    int result = (expected && actual && strcmp(expected, actual) == 0);
    if (result)
        fprintf(stderr, GREEN "Test %%x [OK]\n" RESET);
    else
    {
        fprintf(stderr, RED "Test %%x [KO]\n");
        fprintf(stderr, " Esperado: \"%s\"\n Obtenido: \"%s\"\n" RESET, expected ? expected : "(null)", actual ? actual : "(null)");
    }
    free(expected);
    free(actual);
    return result;
}

void child_test_char(void) { ft_printf("Char: %c", 'A'); }
int test_char(void)
{
    char *expected = capture_printf("Char: %c", 'A');
    char *actual   = capture_ft_printf_with_callback(child_test_char);
    int result = (expected && actual && strcmp(expected, actual) == 0);
    if (result)
        fprintf(stderr, GREEN "Test %%c [OK]\n" RESET);
    else
    {
        fprintf(stderr, RED "Test %%c [KO]\n");
        fprintf(stderr, " Esperado: \"%s\"\n Obtenido: \"%s\"\n" RESET, expected ? expected : "(null)", actual ? actual : "(null)");
    }
    free(expected);
    free(actual);
    return result;
}

void child_test_pointer(void)
{
    int x = 42;
    ft_printf("Pointer: %p", &x);
}

int starts_with(const char *str, const char *prefix)
{
    if (!str || !prefix)
        return 0;
    while (*prefix)
        if (*str++ != *prefix++)
            return 0;
    return 1;
}

int is_valid_pointer_format(const char *str)
{
    return starts_with(str, "Pointer: 0x");
}

int test_pointer(void)
{
    char *actual = capture_ft_printf_with_callback(child_test_pointer);
    int result = is_valid_pointer_format(actual);

    if (result)
        fprintf(stderr, GREEN "Test %%p [OK]\n" RESET);
    else
    {
        fprintf(stderr, RED "Test %%p [KO]\n");
        fprintf(stderr, " Obtenido: \"%s\"\n" RESET, actual ? actual : "(null)");
    }

    free(actual);
    return result;
}


void child_test_combo(void)
{
    ft_printf("Mix: %d %s %x %c %%", 123, "abc", 0x1A2B, 'Z');
}
int test_combo(void)
{
    char *expected = capture_printf("Mix: %d %s %x %c %%", 123, "abc", 0x1A2B, 'Z');
    char *actual   = capture_ft_printf_with_callback(child_test_combo);
    int result = (expected && actual && strcmp(expected, actual) == 0);
    if (result)
        fprintf(stderr, GREEN "Test combo [OK]\n" RESET);
    else
    {
        fprintf(stderr, RED "Test combo [KO]\n");
        fprintf(stderr, " Esperado: \"%s\"\n Obtenido: \"%s\"\n" RESET, expected ? expected : "(null)", actual ? actual : "(null)");
    }
    free(expected);
    free(actual);
    return result;
}

int main(void)
{
    int total = 0, passed = 0;

    fprintf(stderr, CYAN "\n==== TESTING FT_PRINTF ====\n\n" RESET);

    total++; passed += test_percent();
    total++; passed += test_string();
    total++; passed += test_int();
    total++; passed += test_hex();
    total++; passed += test_char();
    total++; passed += test_pointer();
    total++; passed += test_combo();

    fprintf(stderr, "\n" YELLOW "Resumen: %d/%d tests pasados (%.1f%%)\n\n" RESET,
           passed, total, (float)passed / total * 100);

    return (passed != total);
}
