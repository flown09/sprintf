int define_length(long long int num)
{
    if (num == 0) return 1;
    int res = 0;
    while (num > 0)
    {
        num /= 10;
        res++;
    }
    return res;
}

void number_to_string(long long int num , char *str)
{
    num = num < 0 ? num * (-1) : num;

    int i = 0;
    do {
        str[i++] = (num % 10) + '0';
        num /= 10;
    } while (num > 0);

    str[i] = '\0';

    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}