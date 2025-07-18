#define _GNU_SOURCE
#define _BSD_SOURCE
#define _DEFAULT_SOURCE
#define CMP_SIGN(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))
#include "libft.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <limits.h>


size_t strlcpy(char *dst, const char *src, size_t size);
size_t strlcat(char *dst, const char *src, size_t size);
char *strnstr(const char *haystack, const char *needle, size_t len);

// Implementaciones manuales de strlcpy, strlcat, strnstr
size_t strlcpy(char *dst, const char *src, size_t size) {
    size_t i = 0;
    if (size > 0) {
        while (i < size - 1 && src[i]) {
            dst[i] = src[i];
            i++;
        }
        dst[i] = '\0';
    }
    while (src[i]) i++;
    return i;
}

size_t strlcat(char *dst, const char *src, size_t size) {
    size_t dst_len = strlen(dst);
    size_t src_len = strlen(src);
    size_t i = 0;
    
    if (size <= dst_len) return size + src_len;
    
    while (src[i] && (dst_len + i + 1) < size) {
        dst[dst_len + i] = src[i];
        i++;
    }
    dst[dst_len + i] = '\0';
    return dst_len + src_len;
}

char *strnstr(const char *haystack, const char *needle, size_t len) {
    size_t needle_len = strlen(needle);
    if (needle_len == 0) return (char *)haystack;
    
    while (*haystack && len >= needle_len) {
        if (*haystack == *needle && strncmp(haystack, needle, needle_len) == 0) {
            return (char *)haystack;
        }
        haystack++;
        len--;
    }
    return NULL;
}

// --- Colores para la salida en consola ---
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

// --- Variables para el conteo de pruebas ---
int total_tests = 0;
int passed_tests = 0;

// --- Macros de Asistencia para Pruebas ---
#define TEST_CASE(func_name, condition) \
    do { \
        total_tests++; \
        if (condition) { \
            printf(GREEN "    [OK] " RESET #func_name ": Test Passed\n"); \
            passed_tests++; \
        } else { \
            printf(RED "    [KO] " RESET #func_name ": Test Failed\n"); \
        } \
    } while(0)

#define TEST_CASE_STR(func_name, actual, expected) \
    do { \
        total_tests++; \
        if (actual == NULL && expected == NULL) { \
            printf(GREEN "    [OK] " RESET #func_name ": Test Passed (both NULL)\n"); \
            passed_tests++; \
        } else if (actual == NULL || expected == NULL) { \
            printf(RED "    [KO] " RESET #func_name ": Test Failed (one NULL, one not)\n"); \
            printf(YELLOW "        Expected: \"%s\", Got: \"%s\"\n" RESET, expected ? expected : "(null)", actual ? actual : "(null)"); \
        } else if (strcmp(actual, expected) == 0) { \
            printf(GREEN "    [OK] " RESET #func_name ": Test Passed\n"); \
            passed_tests++; \
        } else { \
            printf(RED "    [KO] " RESET #func_name ": Test Failed\n"); \
            printf(YELLOW "        Expected: \"%s\", Got: \"%s\"\n" RESET, expected, actual); \
        } \
    } while(0)

// --- Funciones para imprimir headers de sección ---
void print_section_header(const char *title) {
    printf(BLUE "\n--- %s ---\n" RESET, title);
}

void print_subsection_header(const char *title) {
    printf(CYAN "\n--- %s ---\n" RESET, title);
}

void print_test_summary() {
    printf(MAGENTA "\n--- Test Summary ---\n" RESET);
    printf("Total Tests: %d\n", total_tests);
    printf("Tests Passed: %d\n", passed_tests);
    printf("%sResult: %s\n" RESET, (passed_tests == total_tests) ? GREEN : RED, (passed_tests == total_tests) ? "ALL TESTS PASSED!" : "SOME TESTS FAILED!");
    printf(MAGENTA "--------------------\n" RESET);
}

// --- PARTE 1: Funciones Libc ---

void test_isalpha() {
    print_subsection_header("ft_isalpha");
    TEST_CASE(ft_isalpha, (ft_isalpha('A') != 0) == (isalpha('A') != 0));
    TEST_CASE(ft_isalpha, (ft_isalpha('z') != 0) == (isalpha('z') != 0));
    TEST_CASE(ft_isalpha, (ft_isalpha('5') != 0) == (isalpha('5') != 0));
    TEST_CASE(ft_isalpha, (ft_isalpha('@') != 0) == (isalpha('@') != 0));
    TEST_CASE(ft_isalpha, (ft_isalpha(0) != 0) == (isalpha(0) != 0));     // ASCII 0
    TEST_CASE(ft_isalpha, (ft_isalpha(127) != 0) == (isalpha(127) != 0)); // ASCII 127
}

void test_isdigit() {
    print_subsection_header("ft_isdigit");
    TEST_CASE(ft_isdigit, (ft_isdigit('5') != 0) == (isdigit('5') != 0));
    TEST_CASE(ft_isdigit, (ft_isdigit('a') != 0) == (isdigit('a') != 0));
    TEST_CASE(ft_isdigit, (ft_isdigit('0') != 0) == (isdigit('0') != 0));
    TEST_CASE(ft_isdigit, (ft_isdigit('9') != 0) == (isdigit('9') != 0));
    TEST_CASE(ft_isdigit, (ft_isdigit(-1) != 0) == (isdigit(-1) != 0)); // Valor fuera de rango
}

void test_isalnum() {
    print_subsection_header("ft_isalnum");
    TEST_CASE(ft_isalnum, (ft_isalnum('a') != 0) == (isalnum('a') != 0));
    TEST_CASE(ft_isalnum, (ft_isalnum('Z') != 0) == (isalnum('Z') != 0));
    TEST_CASE(ft_isalnum, (ft_isalnum('7') != 0) == (isalnum('7') != 0));
    TEST_CASE(ft_isalnum, (ft_isalnum('$') != 0) == (isalnum('$') != 0));
    TEST_CASE(ft_isalnum, (ft_isalnum(' ') != 0) == (isalnum(' ') != 0));
}

void test_isascii() {
    print_subsection_header("ft_isascii");
    TEST_CASE(ft_isascii, ft_isascii('A') == isascii('A'));
    TEST_CASE(ft_isascii, ft_isascii(0) == isascii(0));
    TEST_CASE(ft_isascii, ft_isascii(127) == isascii(127));
    TEST_CASE(ft_isascii, ft_isascii(128) == isascii(128)); // Fuera de ASCII
    TEST_CASE(ft_isascii, ft_isascii(-1) == isascii(-1)); // Valor negativo
}

void test_isprint() {
    print_subsection_header("ft_isprint");
    TEST_CASE(ft_isprint, (ft_isprint('A') != 0) == (isprint('A') != 0));
    TEST_CASE(ft_isprint, (ft_isprint(' ') != 0) == (isprint(' ') != 0));
    TEST_CASE(ft_isprint, (ft_isprint('~') != 0) == (isprint('~') != 0)); // Último imprimible
    TEST_CASE(ft_isprint, (ft_isprint('\n') != 0) == (isprint('\n') != 0)); // No imprimible
    TEST_CASE(ft_isprint, (ft_isprint(126) != 0) == (isprint(126) != 0));
    TEST_CASE(ft_isprint, (ft_isprint(31) != 0) == (isprint(31) != 0)); // No imprimible
}

void test_strlen() {
    print_subsection_header("ft_strlen");
    TEST_CASE(ft_strlen, ft_strlen("hola") == strlen("hola"));
    TEST_CASE(ft_strlen, ft_strlen("") == strlen(""));
    TEST_CASE(ft_strlen, ft_strlen("Lorem ipsum dolor sit amet, consectetur adipiscing elit.") == strlen("Lorem ipsum dolor sit amet, consectetur adipiscing elit."));
    TEST_CASE(ft_strlen, ft_strlen("\t\n\r") == strlen("\t\n\r"));
    // TEST_CASE(ft_strlen, ft_strlen(NULL) == strlen(NULL)); // Esto debería SEGFAULT o manejarse con un try-catch si fuese C++, en C se espera SEGFAULT. Comentar para evitar crash.
}

void test_memset() {
    print_subsection_header("ft_memset");
    
    // Test 1: Llenar 5 bytes con 'A'
    char s1[10] = {0};  // Inicializado a 0
    char s2[10] = {0};  // Inicializado a 0
    ft_memset(s1, 'A', 5);
    memset(s2, 'A', 5);
    TEST_CASE(ft_memset, memcmp(s1, s2, 10) == 0);  // Compara ambos buffers

    // Test 2: Llenar todo con 0
    char s3[10] = {1};  // Inicializado a valores no cero
    char s4[10] = {1};  // Inicializado a valores no cero
    ft_memset(s3, 0, 10);
    memset(s4, 0, 10);
    TEST_CASE(ft_memset, memcmp(s3, s4, 10) == 0);

    // Test 3: Buffer grande
    char large_s1[1000], large_s2[1000];
    memset(large_s1, 'Y', 1000);  // Llenamos con valor conocido
    memset(large_s2, 'Y', 1000);
    ft_memset(large_s1, 'X', 999);
    memset(large_s2, 'X', 999);
    TEST_CASE(ft_memset, memcmp(large_s1, large_s2, 1000) == 0);
}

void test_bzero() {
    print_subsection_header("ft_bzero");
    char s1[10] = "abcdefghi";
    char s2[10] = "abcdefghi";
    ft_bzero(s1, 5);
    bzero(s2, 5);
    TEST_CASE(ft_bzero, memcmp(s1, s2, 10) == 0);
    ft_bzero(s1, 0);
    bzero(s2, 0);
    TEST_CASE(ft_bzero, memcmp(s1, s2, 10) == 0);
    ft_bzero(s1, 10);
    bzero(s2, 10);
    TEST_CASE(ft_bzero, memcmp(s1, s2, 10) == 0);
}

void test_memcpy() {
    print_subsection_header("ft_memcpy");

    // Test 1: Copia normal
    char dst1[20] = {0}, src1[20] = "Hello World";
    char dst2[20] = {0}, src2[20] = "Hello World";
    ft_memcpy(dst1, src1, 12); // Incluye el '\0'
    memcpy(dst2, src2, 12);
    TEST_CASE(ft_memcpy, memcmp(dst1, dst2, 20) == 0);

    // Test 2: Longitud 0 (no debería modificar nada)
    char dst3[20] = "Original";
    char dst4[20] = "Original";
    ft_memcpy(dst3, src1, 0);
    memcpy(dst4, src2, 0);
    TEST_CASE(ft_memcpy, memcmp(dst3, dst4, 20) == 0);

    // Test 3: Copia parcial sin '\0'
    char dst5[20] = {0}, dst6[20] = {0};
    ft_memcpy(dst5, "123456789", 5);
    memcpy(dst6, "123456789", 5);
    TEST_CASE(ft_memcpy, memcmp(dst5, dst6, 20) == 0);
}
void test_memmove() {
    print_subsection_header("ft_memmove");

    // Test 1: Copia normal sin solapamiento
    char dst1[20] = {0}, src1[20] = "Hello World";
    char dst2[20] = {0}, src2[20] = "Hello World";
    ft_memmove(dst1, src1, 12);
    memmove(dst2, src2, 12);
    TEST_CASE(ft_memmove, memcmp(dst1, dst2, 20) == 0);

    // Test 2: Longitud 0 (no modificación)
    char dst3[20] = "Original";
    char dst4[20] = "Original";
    ft_memmove(dst3, src1, 0);
    memmove(dst4, src2, 0);
    TEST_CASE(ft_memmove, memcmp(dst3, dst4, 20) == 0);

    // Test 3: Solapamiento (origen < destino)
    char overlap1[20] = "abcdefghijklm";
    char overlap2[20] = "abcdefghijklm";
    ft_memmove(overlap1 + 3, overlap1, 5);  // "abc" + "abcde" + "ijklm" -> "abcabcdeijklm"
    memmove(overlap2 + 3, overlap2, 5);
    TEST_CASE(ft_memmove, memcmp(overlap1, overlap2, 20) == 0);

    // Test 4: Solapamiento (origen > destino)
    char overlap3[20] = "abcdefghijklm";
    char overlap4[20] = "abcdefghijklm";
    ft_memmove(overlap3, overlap3 + 3, 5);  // "defgh" sobrescribe "abcde" -> "defghfghijklm"
    memmove(overlap4, overlap4 + 3, 5);
    TEST_CASE(ft_memmove, memcmp(overlap3, overlap4, 20) == 0);

    // Test 5: Copia exactamente igual al tamaño del buffer
    char big1[1000], big2[1000];
    memset(big1, 'X', 1000);
    memset(big2, 'X', 1000);
    ft_memmove(big1 + 100, big1, 900);
    memmove(big2 + 100, big2, 900);
    TEST_CASE(ft_memmove, memcmp(big1, big2, 1000) == 0);
}

void test_strlcpy() {
    print_subsection_header("ft_strlcpy");
    char dst1[10];
    char dst2[10];
    size_t ret1, ret2;

    // Test 1: Full copy
    ret1 = ft_strlcpy(dst1, "Hello", sizeof(dst1));
    ret2 = strlcpy(dst2, "Hello", sizeof(dst2));
    TEST_CASE(ft_strlcpy, ret1 == ret2 && strcmp(dst1, dst2) == 0);

    // Test 2: Truncation
    ret1 = ft_strlcpy(dst1, "Long string", 5); // Should copy "Long\0"
    ret2 = strlcpy(dst2, "Long string", 5);
    TEST_CASE(ft_strlcpy, ret1 == ret2 && strcmp(dst1, "Long") == 0 && strcmp(dst2, "Long") == 0);

    // Test 3: dst_size = 0
    ret1 = ft_strlcpy(dst1, "Test", 0);
    ret2 = strlcpy(dst2, "Test", 0);
    TEST_CASE(ft_strlcpy, ret1 == ret2); // dst1/dst2 should be untouched
    
    // Test 4: src is empty
    ret1 = ft_strlcpy(dst1, "", sizeof(dst1));
    ret2 = strlcpy(dst2, "", sizeof(dst2));
    TEST_CASE(ft_strlcpy, ret1 == ret2 && strcmp(dst1, "") == 0 && strcmp(dst2, "") == 0);

    // Test 5: dst_size = 1 (only null terminator)
    ret1 = ft_strlcpy(dst1, "abc", 1);
    ret2 = strlcpy(dst2, "abc", 1);
    TEST_CASE(ft_strlcpy, ret1 == ret2 && dst1[0] == '\0' && dst2[0] == '\0');
}

void test_strlcat() {
    print_subsection_header("ft_strlcat");
    char dst1[20] = "Hello ";
    char dst2[20] = "Hello ";
    size_t ret1, ret2;

    // Test 1: Normal concat
    ret1 = ft_strlcat(dst1, "World", sizeof(dst1)); // "Hello World\0"
    ret2 = strlcat(dst2, "World", sizeof(dst2));
    TEST_CASE(ft_strlcat, ret1 == ret2 && strcmp(dst1, dst2) == 0); // Expected 11

    // Test 2: No space for src (truncation)
    char dst3[10] = "12345"; // Length 5, 4 space left for src + null
    char dst4[10] = "12345";
    ret1 = ft_strlcat(dst3, "67890", sizeof(dst3)); // Should result in "123456789\0"
    ret2 = strlcat(dst4, "67890", sizeof(dst4));
    TEST_CASE(ft_strlcat, ret1 == ret2 && strcmp(dst3, "123456789") == 0 && strcmp(dst4, "123456789") == 0); // Expected len("12345") + len("67890") = 5 + 5 = 10

    // Test 3: dst_size = current_len + 1 (only null terminator)
    char dst5[10] = "abc"; // len 3
    char dst6[10] = "abc";
    ret1 = ft_strlcat(dst5, "defg", 4); // should be "abc\0"
    ret2 = strlcat(dst6, "defg", 4);
    TEST_CASE(ft_strlcat, ret1 == ret2 && strcmp(dst5, "abc") == 0 && strcmp(dst6, "abc") == 0); // Expected len("abc") + len("defg") = 3 + 4 = 7

    // Test 4: dst is full
    char dst7[6] = "hello"; // Already full with null terminator
    char dst8[6] = "hello";
    ret1 = ft_strlcat(dst7, " world", sizeof(dst7)); // Should return len("hello") + len(" world") = 5 + 6 = 11
    ret2 = strlcat(dst8, " world", sizeof(dst8));
    TEST_CASE(ft_strlcat, ret1 == ret2 && strcmp(dst7, "hello") == 0 && strcmp(dst8, "hello") == 0); // Expected 11
}

void test_toupper() {
    print_subsection_header("ft_toupper");
    TEST_CASE(ft_toupper, ft_toupper('a') == toupper('a'));
    TEST_CASE(ft_toupper, ft_toupper('Z') == toupper('Z'));
    TEST_CASE(ft_toupper, ft_toupper('5') == toupper('5'));
    TEST_CASE(ft_toupper, ft_toupper('@') == toupper('@'));
    TEST_CASE(ft_toupper, ft_toupper(' ') == toupper(' '));
    TEST_CASE(ft_toupper, ft_toupper(0) == toupper(0));
}

void test_tolower() {
    print_subsection_header("ft_tolower");
    TEST_CASE(ft_tolower, ft_tolower('A') == tolower('A'));
    TEST_CASE(ft_tolower, ft_tolower('z') == tolower('z'));
    TEST_CASE(ft_tolower, ft_tolower('5') == tolower('5'));
    TEST_CASE(ft_tolower, ft_tolower('@') == tolower('@'));
    TEST_CASE(ft_tolower, ft_tolower(' ') == tolower(' '));
    TEST_CASE(ft_tolower, ft_tolower(0) == tolower(0));
}

void test_strchr() {
    print_subsection_header("ft_strchr");
    TEST_CASE(ft_strchr, ft_strchr("hello", 'o') == strchr("hello", 'o'));
    TEST_CASE(ft_strchr, ft_strchr("hello", 'H') == strchr("hello", 'H'));
    TEST_CASE(ft_strchr, ft_strchr("hello", '\0') == strchr("hello", '\0'));
    TEST_CASE(ft_strchr, ft_strchr("hello", 'z') == strchr("hello", 'z')); // Not found
    TEST_CASE(ft_strchr, ft_strchr("", 'a') == strchr("", 'a')); // Empty string
}

void test_strrchr() {
    print_subsection_header("ft_strrchr");
    TEST_CASE(ft_strrchr, ft_strrchr("hello", 'l') == strrchr("hello", 'l'));
    TEST_CASE(ft_strrchr, ft_strrchr("hello", 'o') == strrchr("hello", 'o'));
    TEST_CASE(ft_strrchr, ft_strrchr("hello", 'H') == strrchr("hello", 'H'));
    TEST_CASE(ft_strrchr, ft_strrchr("hello", '\0') == strrchr("hello", '\0'));
    TEST_CASE(ft_strrchr, ft_strrchr("hello", 'z') == strrchr("hello", 'z')); // Not found
    TEST_CASE(ft_strrchr, ft_strrchr("apple", 'p') == strrchr("apple", 'p'));
}

void test_strncmp() {
    print_subsection_header("ft_strncmp");
    TEST_CASE(ft_strncmp, ft_strncmp("hello", "hello", 5) == strncmp("hello", "hello", 5));
    TEST_CASE(ft_strncmp, ft_strncmp("hello", "hell", 5) == strncmp("hello", "hell", 5)); // Differ by null terminator
    TEST_CASE(ft_strncmp, ft_strncmp("hello", "hallo", 5) == strncmp("hello", "hallo", 5));
    TEST_CASE(ft_strncmp, ft_strncmp("hello", "hello", 0) == strncmp("hello", "hello", 0)); // n = 0
    TEST_CASE(ft_strncmp, ft_strncmp("test", "testing", 4) == strncmp("test", "testing", 4)); // Equal up to n
    TEST_CASE(ft_strncmp, ft_strncmp("testing", "test", 4) == strncmp("testing", "test", 4)); // Equal up to n
    TEST_CASE(ft_strncmp, ft_strncmp("abc", "abd", 2) == strncmp("abc", "abd", 2));
    TEST_CASE(ft_strncmp, ft_strncmp("abc", "Abc", 3) == strncmp("abc", "Abc", 3)); // Case sensitive
}

void test_memchr() {
    print_subsection_header("ft_memchr");
    char s[] = "hello world";
    TEST_CASE(ft_memchr, ft_memchr(s, 'o', strlen(s)) == memchr(s, 'o', strlen(s)));
    TEST_CASE(ft_memchr, ft_memchr(s, 'z', strlen(s)) == memchr(s, 'z', strlen(s)));
    TEST_CASE(ft_memchr, ft_memchr(s, 'h', 1) == memchr(s, 'h', 1));
    TEST_CASE(ft_memchr, ft_memchr(s, 'o', 4) == memchr(s, 'o', 4)); // 'o' not in first 4 bytes
    TEST_CASE(ft_memchr, ft_memchr(s, '\0', strlen(s) + 1) == memchr(s, '\0', strlen(s) + 1));
    TEST_CASE(ft_memchr, ft_memchr("", 'a', 0) == memchr("", 'a', 0));
}

void test_memcmp() {
    print_subsection_header("ft_memcmp");
    TEST_CASE(ft_memcmp, CMP_SIGN(ft_memcmp("hello", "hello", 5)) == CMP_SIGN(memcmp("hello", "hello", 5)));
    TEST_CASE(ft_memcmp, CMP_SIGN(ft_memcmp("hello", "hell", 4)) == CMP_SIGN(memcmp("hello", "hell", 4)));
    TEST_CASE(ft_memcmp, CMP_SIGN(ft_memcmp("hello", "world", 5)) == CMP_SIGN(memcmp("hello", "world", 5)));
    TEST_CASE(ft_memcmp, CMP_SIGN(ft_memcmp("hello", "hell", 5)) == CMP_SIGN(memcmp("hello", "hell", 5)));
    TEST_CASE(ft_memcmp, CMP_SIGN(ft_memcmp("abc", "ABC", 3)) == CMP_SIGN(memcmp("abc", "ABC", 3)));
    TEST_CASE(ft_memcmp, CMP_SIGN(ft_memcmp("abc", "abc", 0)) == CMP_SIGN(memcmp("abc", "abc", 0)));
}

void test_strnstr() {
    print_subsection_header("ft_strnstr");
    const char *haystack = "Foo Bar Baz";
    TEST_CASE(ft_strnstr, ft_strnstr(haystack, "Bar", strlen(haystack)) == strnstr(haystack, "Bar", strlen(haystack)));
    TEST_CASE(ft_strnstr, ft_strnstr(haystack, "Baz", strlen(haystack)) == strnstr(haystack, "Baz", strlen(haystack)));
    TEST_CASE(ft_strnstr, ft_strnstr(haystack, "Foo", 2) == strnstr(haystack, "Foo", 2)); // Should return NULL
    TEST_CASE(ft_strnstr, ft_strnstr(haystack, "QuX", strlen(haystack)) == strnstr(haystack, "QuX", strlen(haystack))); // Not found
    TEST_CASE(ft_strnstr, ft_strnstr("", "Bar", 0) == strnstr("", "Bar", 0)); // Empty haystack
    TEST_CASE(ft_strnstr, ft_strnstr(haystack, "", strlen(haystack)) == strnstr(haystack, "", strlen(haystack))); // Empty needle
    TEST_CASE(ft_strnstr, ft_strnstr("abc", "c", 2) == strnstr("abc", "c", 2)); // 'c' out of bounds for n
    TEST_CASE(ft_strnstr, ft_strnstr("abc", "c", 3) == strnstr("abc", "c", 3)); // 'c' in bounds for n
}

void test_atoi() {
    print_subsection_header("ft_atoi");
    TEST_CASE(ft_atoi, ft_atoi("123") == atoi("123"));
    TEST_CASE(ft_atoi, ft_atoi("-123") == atoi("-123"));
    TEST_CASE(ft_atoi, ft_atoi("0") == atoi("0"));
    TEST_CASE(ft_atoi, ft_atoi("   123") == atoi("   123"));
    TEST_CASE(ft_atoi, ft_atoi("  -42abc") == atoi("  -42abc"));
    TEST_CASE(ft_atoi, ft_atoi("++123") == atoi("++123")); // Should be 0
    TEST_CASE(ft_atoi, ft_atoi("--123") == atoi("--123")); // Should be 0
    TEST_CASE(ft_atoi, ft_atoi("+-123") == atoi("+-123")); // Should be 0
    TEST_CASE(ft_atoi, ft_atoi("2147483647") == atoi("2147483647")); // INT_MAX
    TEST_CASE(ft_atoi, ft_atoi("-2147483648") == atoi("-2147483648")); // INT_MIN
    TEST_CASE(ft_atoi, ft_atoi("99999999999") == atoi("99999999999")); // Overflow
    TEST_CASE(ft_atoi, ft_atoi("-99999999999") == atoi("-99999999999")); // Underflow
    TEST_CASE(ft_atoi, ft_atoi("    +1") == atoi("    +1"));
    TEST_CASE(ft_atoi, ft_atoi("    -1") == atoi("    -1"));
    TEST_CASE(ft_atoi, ft_atoi("   \t\n\r\v\f-123") == atoi("   \t\n\r\v\f-123"));
}

// --- PARTE 2: Funciones Adicionales ---

void test_calloc() {
    print_subsection_header("ft_calloc");
    char *s1, *s2;
    // Test 1: Normal allocation
    s1 = (char *)ft_calloc(10, sizeof(char));
    s2 = (char *)calloc(10, sizeof(char));
    TEST_CASE(ft_calloc, s1 != NULL && s2 != NULL && memcmp(s1, s2, 10) == 0);
    free(s1);
    free(s2);

    // Test 2: Zero count
    s1 = (char *)ft_calloc(0, sizeof(char));
    s2 = (char *)calloc(0, sizeof(char));
    TEST_CASE(ft_calloc, s1 != NULL && s2 != NULL && memcmp(s1, s2, 0) == 0);
    free(s1);
    free(s2);

    // Test 3: Large allocation (pero no SIZE_MAX para evitar warnings)
    size_t large_size = 1 << 20; // 1MB en lugar de SIZE_MAX
    s1 = (char *)ft_calloc(large_size, 1);
    s2 = (char *)calloc(large_size, 1);
    TEST_CASE(ft_calloc, (s1 == NULL && s2 == NULL) || (s1 != NULL && s2 != NULL));
    free(s1);
    free(s2);

    // Test 4: Zero size
    s1 = (char *)ft_calloc(10, 0);
    s2 = (char *)calloc(10, 0);
    TEST_CASE(ft_calloc, s1 != NULL && s2 != NULL);
    free(s1);
    free(s2);
}

void test_strdup() {
    print_subsection_header("ft_strdup");
    char *s1, *s2;
    // Test 1: Normal string
    s1 = ft_strdup("Hello World");
    s2 = strdup("Hello World");
    TEST_CASE_STR(ft_strdup, s1, s2);
    free(s1);
    free(s2);

    // Test 2: Empty string
    s1 = ft_strdup("");
    s2 = strdup("");
    TEST_CASE_STR(ft_strdup, s1, s2);
    free(s1);
    free(s2);

    // Test 3: Long string
    const char *long_str = "This is a very long string that should be duplicated correctly by ft_strdup and strdup functions.";
    s1 = ft_strdup(long_str);
    s2 = strdup(long_str);
    TEST_CASE_STR(ft_strdup, s1, s2);
    free(s1);
    free(s2);
}

void test_substr() {
    print_subsection_header("ft_substr");
    char *sub;

    // Test 1: Normal case
    sub = ft_substr("Hello World", 0, 5);
    TEST_CASE_STR(ft_substr, sub, "Hello");
    free(sub);

    // Test 2: Start in middle
    sub = ft_substr("Hello World", 6, 5);
    TEST_CASE_STR(ft_substr, sub, "World");
    free(sub);

    // Test 3: Start beyond string length
    sub = ft_substr("Hello World", 20, 5);
    TEST_CASE_STR(ft_substr, sub, "");
    free(sub);

    // Test 4: Length longer than remaining string
    sub = ft_substr("Hello", 2, 10);
    TEST_CASE_STR(ft_substr, sub, "llo");
    free(sub);

    // Test 5: Empty string as input
    sub = ft_substr("", 0, 5);
    TEST_CASE_STR(ft_substr, sub, "");
    free(sub);

    // Test 6: Start 0, len 0
    sub = ft_substr("Hello", 0, 0);
    TEST_CASE_STR(ft_substr, sub, "");
    free(sub);
}

void test_strjoin() {
    print_subsection_header("ft_strjoin");
    char *join;

    // Test 1: Normal join
    join = ft_strjoin("Hello", " World");
    TEST_CASE_STR(ft_strjoin, join, "Hello World");
    free(join);

    // Test 2: First string empty
    join = ft_strjoin("", "World");
    TEST_CASE_STR(ft_strjoin, join, "World");
    free(join);

    // Test 3: Second string empty
    join = ft_strjoin("Hello", "");
    TEST_CASE_STR(ft_strjoin, join, "Hello");
    free(join);

    // Test 4: Both empty
    join = ft_strjoin("", "");
    TEST_CASE_STR(ft_strjoin, join, "");
    free(join);

    // Test 5: Long strings
    join = ft_strjoin("Lorem ipsum dolor sit amet", ", consectetur adipiscing elit.");
    TEST_CASE_STR(ft_strjoin, join, "Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    free(join);
}

void test_strtrim() {
    print_subsection_header("ft_strtrim");
    char *trim;

    // Test 1: Basic trim
    trim = ft_strtrim("   Hello World   ", " ");
    TEST_CASE_STR(ft_strtrim, trim, "Hello World");
    free(trim);

    // Test 2: No trim needed
    trim = ft_strtrim("Hello World", " ");
    TEST_CASE_STR(ft_strtrim, trim, "Hello World");
    free(trim);

    // Test 3: Trim all characters
    trim = ft_strtrim("xxxxxxxxx", "x");
    TEST_CASE_STR(ft_strtrim, trim, "");
    free(trim);

    // Test 4: Empty string
    trim = ft_strtrim("", " ");
    TEST_CASE_STR(ft_strtrim, trim, "");
    free(trim);

    // Test 5: Trim with multiple characters in set
    trim = ft_strtrim("abacaabaHello Worldababaac", "abc");
    TEST_CASE_STR(ft_strtrim, trim, "Hello World");
    free(trim);

    // Test 6: Only set characters
    trim = ft_strtrim("abcde", "abcde");
    TEST_CASE_STR(ft_strtrim, trim, "");
    free(trim);
}

void test_split() {
    print_subsection_header("ft_split");
    char **arr;
    
    // Test 1: Basic split
    arr = ft_split("hello world foo bar", ' ');
    TEST_CASE(ft_split, arr[0] && strcmp(arr[0], "hello") == 0);
    TEST_CASE(ft_split, arr[1] && strcmp(arr[1], "world") == 0);
    TEST_CASE(ft_split, arr[2] && strcmp(arr[2], "foo") == 0);
    TEST_CASE(ft_split, arr[3] && strcmp(arr[3], "bar") == 0);
    TEST_CASE(ft_split, arr[4] == NULL);
    if (arr) { for (int i = 0; arr[i]; i++) { free(arr[i]); } free(arr); } // Fix for misleading-indentation

    // Test 2: Multiple delimiters
    arr = ft_split("   hello   world   ", ' ');
    TEST_CASE(ft_split, arr[0] && strcmp(arr[0], "hello") == 0);
    TEST_CASE(ft_split, arr[1] && strcmp(arr[1], "world") == 0);
    TEST_CASE(ft_split, arr[2] == NULL);
    if (arr) { for (int i = 0; arr[i]; i++) { free(arr[i]); } free(arr); } // Fix for misleading-indentation

    // Test 3: String starts/ends with delimiter
    arr = ft_split(" hello world ", ' ');
    TEST_CASE(ft_split, arr[0] && strcmp(arr[0], "hello") == 0);
    TEST_CASE(ft_split, arr[1] && strcmp(arr[1], "world") == 0);
    TEST_CASE(ft_split, arr[2] == NULL);
    if (arr) { for (int i = 0; arr[i]; i++) { free(arr[i]); } free(arr); } // Fix for misleading-indentation

    // Test 4: Empty string
    arr = ft_split("", ' ');
    TEST_CASE(ft_split, arr[0] == NULL);
    free(arr);

    // Test 5: No delimiters
    arr = ft_split("helloworld", ' ');
    TEST_CASE(ft_split, arr[0] && strcmp(arr[0], "helloworld") == 0);
    TEST_CASE(ft_split, arr[1] == NULL);
    if (arr) { for (int i = 0; arr[i]; i++) { free(arr[i]); } free(arr); } // Fix for misleading-indentation

    // Test 6: String with only delimiters
    arr = ft_split("   ", ' ');
    TEST_CASE(ft_split, arr[0] == NULL);
    free(arr);

    // Test 7: Different delimiter
    arr = ft_split("one,two,three", ',');
    TEST_CASE(ft_split, arr[0] && strcmp(arr[0], "one") == 0);
    TEST_CASE(ft_split, arr[1] && strcmp(arr[1], "two") == 0);
    TEST_CASE(ft_split, arr[2] && strcmp(arr[2], "three") == 0);
    TEST_CASE(ft_split, arr[3] == NULL);
    if (arr) { for (int i = 0; arr[i]; i++) { free(arr[i]); } free(arr); } // Fix for misleading-indentation
}

void test_itoa() {
    print_subsection_header("ft_itoa");
    char *s;
    s = ft_itoa(123);
    TEST_CASE_STR(ft_itoa, s, "123");
    free(s);

    s = ft_itoa(-123);
    TEST_CASE_STR(ft_itoa, s, "-123");
    free(s);

    s = ft_itoa(0);
    TEST_CASE_STR(ft_itoa, s, "0");
    free(s);

    s = ft_itoa(2147483647); // INT_MAX
    TEST_CASE_STR(ft_itoa, s, "2147483647");
    free(s);

    s = ft_itoa(-2147483648); // INT_MIN
    TEST_CASE_STR(ft_itoa, s, "-2147483648");
    free(s);

    s = ft_itoa(100000);
    TEST_CASE_STR(ft_itoa, s, "100000");
    free(s);
}

void test_strmapi() {
    print_subsection_header("ft_strmapi");
    char *result;
    
    // Example map function: to_uppercase
    char to_uppercase(unsigned int i, char c) {
        (void)i; // Unused parameter
        return (char)toupper(c);
    }
    result = ft_strmapi("hello world", &to_uppercase);
    TEST_CASE_STR(ft_strmapi, result, "HELLO WORLD");
    free(result);

    // Example map function: add_index
    char add_index(unsigned int i, char c) {
        return c + (i % 10); // Add index to char, mod 10 to keep it somewhat readable
    }
    result = ft_strmapi("abc", &add_index);
    TEST_CASE_STR(ft_strmapi, result, "ace"); // 'a'+0='a', 'b'+1='c', 'c'+2='e'
    free(result);
    
    // Empty string
    result = ft_strmapi("", &to_uppercase);
    TEST_CASE_STR(ft_strmapi, result, "");
    free(result);
}

// Helper para ft_putchar_fd y ft_putstr_fd
// Redirige stdout a un buffer para capturar la salida
char    fd_buffer[1024];
int     original_stdout_fd;
int     pipe_fds[2];

void redirect_stdout_to_buffer() {
    fflush(stdout);
    original_stdout_fd = dup(STDOUT_FILENO);  // Guardar el stdout original
    if (pipe(pipe_fds) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    dup2(pipe_fds[1], STDOUT_FILENO);         // Redirigir stdout al pipe
    close(pipe_fds[1]);                       // Cerrar el extremo de escritura del pipe (IMPORTANTE)
}

void restore_stdout_from_buffer() {
    fflush(stdout);
    close(STDOUT_FILENO);                     // Cerrar el FD redirigido (pipe_fds[1])
    fd_buffer[read(pipe_fds[0], fd_buffer, sizeof(fd_buffer) - 1)] = '\0';  // Leer salida
    close(pipe_fds[0]);                       // Cerrar el extremo de lectura
    dup2(original_stdout_fd, STDOUT_FILENO);  // Restaurar stdout original
    close(original_stdout_fd);                // Cerrar la copia del stdout original
}


void test_putchar_fd() {
    print_subsection_header("ft_putchar_fd");
    redirect_stdout_to_buffer();
    ft_putchar_fd('A', STDOUT_FILENO);
    restore_stdout_from_buffer();
    TEST_CASE(ft_putchar_fd, strcmp(fd_buffer, "A") == 0);

    redirect_stdout_to_buffer();
    ft_putchar_fd('\n', STDOUT_FILENO);
    restore_stdout_from_buffer();
    TEST_CASE(ft_putchar_fd, strcmp(fd_buffer, "\n") == 0);

    redirect_stdout_to_buffer();
    ft_putchar_fd('Z', STDOUT_FILENO);
    restore_stdout_from_buffer();
    TEST_CASE(ft_putchar_fd, strcmp(fd_buffer, "Z") == 0);
}

void test_putstr_fd() {
    print_subsection_header("ft_putstr_fd");
    redirect_stdout_to_buffer();
    ft_putstr_fd("Hello", STDOUT_FILENO);
    restore_stdout_from_buffer();
    TEST_CASE(ft_putstr_fd, strcmp(fd_buffer, "Hello") == 0);

    redirect_stdout_to_buffer();
    ft_putstr_fd("", STDOUT_FILENO);
    restore_stdout_from_buffer();
    TEST_CASE(ft_putstr_fd, strcmp(fd_buffer, "") == 0);
    
    redirect_stdout_to_buffer();
    ft_putstr_fd("Hello\nWorld", STDOUT_FILENO);
    restore_stdout_from_buffer();
    TEST_CASE(ft_putstr_fd, strcmp(fd_buffer, "Hello\nWorld") == 0);
}

void test_putendl_fd() {
    print_subsection_header("ft_putendl_fd");
    redirect_stdout_to_buffer();
    ft_putendl_fd("Hello", STDOUT_FILENO);
    restore_stdout_from_buffer();
    TEST_CASE(ft_putendl_fd, strcmp(fd_buffer, "Hello\n") == 0);

    redirect_stdout_to_buffer();
    ft_putendl_fd("", STDOUT_FILENO);
    restore_stdout_from_buffer();
    TEST_CASE(ft_putendl_fd, strcmp(fd_buffer, "\n") == 0);
}

void test_putnbr_fd() {
    print_subsection_header("ft_putnbr_fd");
    
    // Test 1: Número positivo
    redirect_stdout_to_buffer();
    ft_putnbr_fd(123, STDOUT_FILENO);
    restore_stdout_from_buffer();
    TEST_CASE(ft_putnbr_fd, strcmp(fd_buffer, "123") == 0);

    // Test 2: Número negativo
    redirect_stdout_to_buffer();
    ft_putnbr_fd(-123, STDOUT_FILENO);
    restore_stdout_from_buffer();
    TEST_CASE(ft_putnbr_fd, strcmp(fd_buffer, "-123") == 0);

    // Test 3: Cero
    redirect_stdout_to_buffer();
    ft_putnbr_fd(0, STDOUT_FILENO);
    restore_stdout_from_buffer();
    TEST_CASE(ft_putnbr_fd, strcmp(fd_buffer, "0") == 0);

    // Test 4: INT_MAX
    redirect_stdout_to_buffer();
    ft_putnbr_fd(2147483647, STDOUT_FILENO);
    restore_stdout_from_buffer();
    TEST_CASE(ft_putnbr_fd, strcmp(fd_buffer, "2147483647") == 0);

    // Test 5: INT_MIN (caso especial)
    redirect_stdout_to_buffer();
    ft_putnbr_fd(-2147483648, STDOUT_FILENO);
    restore_stdout_from_buffer();
    TEST_CASE(ft_putnbr_fd, strcmp(fd_buffer, "-2147483648") == 0);
}


// --- BONUS PART ---

// Define t_list if not already in libft.h for testing purposes
// (This is usually provided by 42 for the bonus part)
/*
typedef struct s_list
{
    void            *content;
    struct s_list   *next;
}                   t_list;
*/

// Helper to free list (assuming content is dynamically allocated)
// Forward declaration for free_list
void free_list(t_list *lst);

// Helper for list creation (for testing)
t_list *create_list(char **strs) {
    t_list *head = NULL;
    t_list *current = NULL;
    int i = 0;
    while (strs[i]) {
        t_list *new_node = ft_lstnew(strdup(strs[i])); // Assume ft_lstnew is correct, and content is duped
        if (!new_node) {
            free_list(head); // Clean up if allocation fails
            return NULL;
        }
        if (!head) {
            head = new_node;
            current = head;
        } else {
            current->next = new_node;
            current = new_node;
        }
        i++;
    }
    return head;
}

// Helper to free list (assuming content is dynamically allocated)
void free_list(t_list *lst) {
    t_list *tmp;
    while (lst) {
        tmp = lst->next;
        free(lst->content); // Free content if it was malloc'd
        free(lst);
        lst = tmp;
    }
}

void free_content_null(void *content) {
    (void)content; // Do nothing, for testing ft_lstclear with dummy content
}

void free_content_string(void *content) {
    free(content);
}


void test_lstnew() {
    print_subsection_header("ft_lstnew");
    char *content = strdup("test_content"); // Use strdup to simulate dynamically allocated content
    t_list *node = ft_lstnew(content);
    TEST_CASE(ft_lstnew, node != NULL && node->content == content && node->next == NULL);
    // Be sure to free the content and the node itself
    if (node) { // Check for NULL to prevent crash if ft_lstnew returns NULL
        free(node->content);
        free(node);
    }
}

void test_lstadd_front() {
    print_subsection_header("ft_lstadd_front");
    t_list *head = NULL;
    char *content1 = strdup("first");
    char *content2 = strdup("second");

    ft_lstadd_front(&head, ft_lstnew(content1)); // Head: first
    TEST_CASE(ft_lstadd_front, head != NULL && strcmp(head->content, "first") == 0);

    ft_lstadd_front(&head, ft_lstnew(content2)); // Head: second -> first
    TEST_CASE(ft_lstadd_front, head != NULL && strcmp(head->content, "second") == 0 && strcmp(head->next->content, "first") == 0);
    
    free_list(head);
}

void test_lstsize() {
    print_subsection_header("ft_lstsize");
    t_list *head = NULL;
    TEST_CASE(ft_lstsize, ft_lstsize(head) == 0);

    char *strs1[] = {"1", "2", "3", NULL};
    head = create_list(strs1);
    TEST_CASE(ft_lstsize, ft_lstsize(head) == 3);
    free_list(head);

    char *strs2[] = {"single", NULL};
    head = create_list(strs2);
    TEST_CASE(ft_lstsize, ft_lstsize(head) == 1);
    free_list(head);
}

void test_lstlast() {
    print_subsection_header("ft_lstlast");
    t_list *head = NULL;
    TEST_CASE(ft_lstlast, ft_lstlast(head) == NULL);

    char *strs1[] = {"1", "2", "3", NULL};
    head = create_list(strs1);
    TEST_CASE(ft_lstlast, ft_lstlast(head) != NULL && strcmp(ft_lstlast(head)->content, "3") == 0);
    free_list(head);

    char *strs2[] = {"single", NULL};
    head = create_list(strs2);
    TEST_CASE(ft_lstlast, ft_lstlast(head) != NULL && strcmp(ft_lstlast(head)->content, "single") == 0);
    free_list(head);
}

void test_lstadd_back() {
    print_subsection_header("ft_lstadd_back");
    t_list *head = NULL;

    ft_lstadd_back(&head, ft_lstnew(strdup("first"))); // Head: first
    TEST_CASE(ft_lstadd_back, head != NULL && strcmp(head->content, "first") == 0);

    ft_lstadd_back(&head, ft_lstnew(strdup("second"))); // Head: first -> second
    TEST_CASE(ft_lstadd_back, strcmp(ft_lstlast(head)->content, "second") == 0);
    TEST_CASE(ft_lstadd_back, ft_lstsize(head) == 2);
    free_list(head);

    head = NULL;
    ft_lstadd_back(&head, ft_lstnew(strdup("only")));
    TEST_CASE(ft_lstadd_back, head != NULL && strcmp(head->content, "only") == 0);
    TEST_CASE(ft_lstadd_back, head->next == NULL);
    free_list(head);
}

void test_lstdelone() {
    print_subsection_header("ft_lstdelone");
    char *content = strdup("to_delete");
    t_list *node = ft_lstnew(content);
    int *flag = malloc(sizeof(int));
    *flag = 0; // Use a flag to check if custom free function is called

    void custom_free_func(void *content_to_free) {
        free(content_to_free);
        *flag = 1; // Set flag when content is freed
    }
    
    ft_lstdelone(node, custom_free_func);
    TEST_CASE(ft_lstdelone, *flag == 1); // Check if custom free was called
    // Note: It's hard to test if node itself is truly freed without causing a segfault.
    // The main point is that its content is freed and its internal pointers are nulled.
    free(flag);
}

void test_lstclear() {
    print_subsection_header("ft_lstclear");
    
    // Create test flags
    int flag_a = 0, flag_b = 0, flag_c = 0;
    
    // Create list with custom nodes
    t_list *node1 = ft_lstnew(&flag_a);
    t_list *node2 = ft_lstnew(&flag_b);
    t_list *node3 = ft_lstnew(&flag_c);
    node1->next = node2;
    node2->next = node3;
    
    void custom_free_flag(void *content) {
        int *flag = (int *)content;
        *flag = 1;  // Set flag to indicate content was freed
        // Note: We don't actually free the flag as it's stack-allocated
    }
    
    t_list *head = node1;
    ft_lstclear(&head, custom_free_flag);
    
    // Verify list is cleared
    TEST_CASE(ft_lstclear, head == NULL);
    
    // Verify all contents were processed
    TEST_CASE(ft_lstclear, flag_a == 1);
    TEST_CASE(ft_lstclear, flag_b == 1);
    TEST_CASE(ft_lstclear, flag_c == 1);
}

void test_lstiter() {
    print_subsection_header("ft_lstiter");
    
    char *s1_iter = strdup("hello");
    char *s2_iter = strdup("world");
    t_list *head = ft_lstnew(s1_iter);
    ft_lstadd_back(&head, ft_lstnew(s2_iter));

    void modify_content(void *content) {
        char *str = (char *)content;
        for (int i = 0; str[i]; i++) {
            str[i] = toupper(str[i]); // Change in-place
        }
    }

    ft_lstiter(head, modify_content);
    TEST_CASE(ft_lstiter, strcmp(head->content, "HELLO") == 0);
    TEST_CASE(ft_lstiter, strcmp(head->next->content, "WORLD") == 0);
    free_list(head);

    // Test with empty list
    head = NULL;
    ft_lstiter(head, modify_content); // Should do nothing
    TEST_CASE(ft_lstiter, head == NULL); // Still NULL
}

void test_lstmap() {
    print_subsection_header("ft_lstmap");
    char *strs[] = {"one", "two", "three", NULL};
    t_list *original_list = create_list(strs);
    
    void *map_func(void *content) {
        char *s = (char *)content;
        char *new_s = malloc(strlen(s) + 6); // + "_MAP" + '\0'
        if (!new_s) return NULL; // Simulate malloc failure
        strcpy(new_s, s);
        strcat(new_s, "_MAP");
        return new_s;
    }

    t_list *new_list = ft_lstmap(original_list, map_func, free_content_string); // free_content_string will free original node content

    TEST_CASE(ft_lstmap, new_list != NULL);
    TEST_CASE(ft_lstmap, new_list && strcmp(new_list->content, "one_MAP") == 0);
    TEST_CASE(ft_lstmap, new_list && new_list->next && strcmp(new_list->next->content, "two_MAP") == 0);
    TEST_CASE(ft_lstmap, new_list && new_list->next && new_list->next->next && strcmp(new_list->next->next->content, "three_MAP") == 0);
    TEST_CASE(ft_lstmap, new_list && new_list->next && new_list->next->next && new_list->next->next->next == NULL);
    
    // Check if original list content was freed (by passing free_content_string)
    // This is harder to test directly without valgrind. Assume if it runs without crash, it's ok.
    free_list(new_list); // Free the newly created list

    // Test with empty list
    original_list = NULL; // Already freed by ft_lstmap if it was properly implemented or already null
    new_list = ft_lstmap(original_list, map_func, free_content_string);
    TEST_CASE(ft_lstmap, new_list == NULL);

    // Test with map_func returning NULL (allocation failure simulation)
    t_list *fail_list = create_list(strs); // Create a fresh list for this test
    void *map_fail(void *content) {
        (void)content;
        return NULL; // Simulate malloc failure
    }
    new_list = ft_lstmap(fail_list, map_fail, free_content_string);
    TEST_CASE(ft_lstmap, new_list == NULL); // Should return NULL
    free_list(fail_list); // Make sure original list is freed here.
}


int main(void) {
    setbuf(stdout, NULL);
    print_section_header("LIBFT TESTER");

    // Parte 1
    print_section_header("PART 1: Libc Functions");
    test_isalpha();
    test_isdigit();
    test_isalnum();
    test_isascii();
    test_isprint();
    test_strlen();
    test_memset();
    test_bzero();
    test_memcpy();
    test_memmove();
    test_strlcpy();
    test_strlcat();
    test_toupper();
    test_tolower();
    test_strchr();
    test_strrchr();
    test_strncmp();
    test_memchr();
    test_memcmp();
    test_strnstr();
    test_atoi();

    // Parte 2
    print_section_header("PART 2: Additional Functions");
    test_calloc();
    test_strdup();
    test_substr();
    test_strjoin();
    test_strtrim();
    test_split();
    test_itoa();
    test_strmapi();
    test_putchar_fd();
    test_putstr_fd();
    test_putendl_fd();
    test_putnbr_fd();

    // Bonus
    print_section_header("BONUS PART: Linked List Functions");
    test_lstnew();
    test_lstadd_front();
    test_lstsize();
    test_lstlast();
    test_lstadd_back();
    test_lstdelone();
    test_lstclear();
    test_lstiter();
    test_lstmap();

    print_test_summary();

    return (passed_tests == total_tests) ? 0 : 1;
}