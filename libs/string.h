bool same_str(const char* a, const char* b)
{
    int ptrA = 0, ptrB = 0;
    while (a[ptrA] != '\0' && b[ptrB] != '\0')
    {
        if (a[ptrA] != b[ptrB])
            return false;
        ptrA++; ptrB++;
    }
    return a[ptrA] == '\0' && b[ptrB] == '\0';
}

const char * strchr ( const char * str, int character )
{
    int ptr = 0;
    while (str[ptr] != '\0')
    {
        if (str[ptr] == character)
        {
            return str + ptr;
        }
        ptr++;
    }
    return 0;
}