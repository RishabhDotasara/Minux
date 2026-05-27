#include "string.h"

/**
 * K&R implementation
 */
void int_to_ascii(int n, char str[])
{
    int i, sign;
    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do
    {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0)
        str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}

/* K&R */
void reverse(char s[])
{
    int c, i, j;
    for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* K&R */
int strlen(char s[])
{
    int i = 0;
    while (s[i] != '\0')
        ++i;
    return i;
}

void append(char s[], char n)
{
    int len = strlen(s);
    s[len] = n;
    s[len + 1] = '\0';
}

int kstoi(char *str)
{
    int result = 0; 
    int n = strlen(str);
    for (int i = 0; i < n; i++){
        result *= 10;
        result += (str[i] - '0');
    }
    return result; 
}

void backspace(char s[])
{
    int len = strlen(s);
    s[len - 1] = '\0';
}

void strsplit(char* str, char del){
    int i = 0;
    int j = 0;
    char buffer[256];
    
    while (str[i] != '\0') {
        if (str[i] == del) {
            buffer[j] = '\0';
            i++;
            j = 0;
        } else {
            buffer[j++] = str[i++];
        }
    }
    buffer[j] = '\0';
}

/* K&R
 * Returns <0 if s1<s2, 0 if s1==s2, >0 if s1>s2 */
int strcmp(char s1[], char s2[])
{
    int i;
    for (i = 0; s1[i] == s2[i]; i++)
    {
        if (s1[i] == '\0')
            return 0;
    }
    return s1[i] - s2[i];
}