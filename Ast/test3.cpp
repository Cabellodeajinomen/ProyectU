int main()
{
    int x;
    int y;
    bool b1;
    bool b2;
    bool b3;
    int r1;
    int r2;
    bool cond;
    int v[5];

    x = true;
    y = false;

    b1 = 5;
    b2 = 0;

    b3 = 10 && 0;
    b3 = 8 || 0;
    b3 = !5;

    b1 = 10 < true;
    b2 = false >= 1;

    r1 = true + 5;
    r2 = false * 9;

    cond = 3;

    if (cond)
    {
        x = 1;
    }

    while (0)
    {
        x = x + 1;
    }

    v[true] = 8;
    v[false] = 9;

    x = 0;   // reemplaza return 0;

}