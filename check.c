#include <stdio.h>
#include <string.h>
#include <ctype.h>

const char *common_patterns[] = {
    "password", "admin", "qwerty", "letmein", "1234", "abcd"
};
#define NUM_COMMON_PATTERNS (sizeof(common_patterns)/sizeof(common_patterns[0]))

int has_sequence(const char *pwd) {
    int len = strlen(pwd);
    for (int i = 0; i < len - 2; i++) {
        if (pwd[i+1] == pwd[i]+1 && pwd[i+2] == pwd[i]+2) return 1; // ascending
        if (pwd[i+1] == pwd[i]-1 && pwd[i+2] == pwd[i]-2) return 1; // descending
    }
    return 0;
}

int has_repeated_sequence(const char *pwd) {
    int len = strlen(pwd);
    for (int seq_len = 1; seq_len <= len/2; seq_len++) {
        int repeat = 1;
        for (int i = 0; i < seq_len; i++) {
            if (pwd[i] != pwd[i+seq_len]) { repeat = 0; break; }
        }
        if (repeat) return 1;
    }
    return 0;
}

void check_password(const char *pwd) {
    int len = strlen(pwd);
    int has_lower=0, has_upper=0, has_digit=0, has_special=0;
    
    for (int i = 0; i < len; i++) {
        if (islower(pwd[i])) has_lower = 1;
        else if (isupper(pwd[i])) has_upper = 1;
        else if (isdigit(pwd[i])) has_digit = 1;
        else has_special = 1;
    }

    int pattern_found = 0;
    for (int i = 0; i < NUM_COMMON_PATTERNS; i++)
        if (strstr(pwd, common_patterns[i])) pattern_found = 1;

    int seq = has_sequence(pwd);
    int repeat = has_repeated_sequence(pwd);

    int score = 0;
    if (len >= 8) score += 2;
    else if (len >= 6) score += 1;

    score += has_lower + has_upper + has_digit + has_special;
    if (pattern_found) score -= 2;
    if (seq) score -= 2;
    if (repeat) score -= 1;
    if (score < 0) score = 0;

    printf("\nPassword Analysis:\n");
    printf("Length: %d\n", len);
    printf("Contains lowercase: %s\n", has_lower?"Yes":"No");
    printf("Contains uppercase: %s\n", has_upper?"Yes":"No");
    printf("Contains digits:    %s\n", has_digit?"Yes":"No");
    printf("Contains special:   %s\n", has_special?"Yes":"No");
    if (pattern_found) printf("Common weak pattern detected\n");
    if (seq) printf("Sequential characters detected\n");
    if (repeat) printf("Repeated sequence detected\n");

    printf("Strength Score: %d/7\n", score);

    printf("Improvement suggestions:\n");
    if (len < 8) printf(" - Increase length to at least 8 characters\n");
    if (!has_lower) printf(" - Add lowercase letters\n");
    if (!has_upper) printf(" - Add uppercase letters\n");
    if (!has_digit) printf(" - Add digits\n");
    if (!has_special) printf(" - Add special characters (!@#$%%^&*)\n");
    if (pattern_found) printf(" - Avoid common patterns or words\n");
    if (seq) printf(" - Avoid sequential characters like 'abc' or '123'\n");
    if (repeat) printf(" - Avoid repeated sequences like 'abab' or '1212'\n");
}

int main() {
    char password[128];
    printf("Enter password to check: ");
    if (!fgets(password, sizeof(password), stdin)) return 1;
    password[strcspn(password, "\n")] = '\0'; // remove newline

    check_password(password);
    return 0;
}
