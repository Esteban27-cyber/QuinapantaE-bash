#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>   // para rename()

void write_str(int fd, const char *s)
{
    while (*s)
        write(fd, s++, 1);
}

int sqr_eq(const char *a, const char *b)
{
    while (*a && *b && *a == *b)
    {
        a++;
        b++;
    }
    return (*a == 0 && *b == 0);
}

int str_len(const char *s)
{
    if (!s)
        return 0;
    int len = 0;
    while (s[len])
        len++;
    return len;
}

// Separar los comandos en tokens simples
int split(char *buf, char *args[])
{
    int i = 0, j = 0;

    while (buf[i] != 0)
    {
        // Saltar espacios y saltos de línea
        while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
        {
            buf[i] = 0;   // convertimos en fin de cadena
            i++;
        }

        if (buf[i] == 0)
            break;        // no hay más argumentos

        // Inicio de un argumento
        args[j++] = &buf[i];

        // Avanzar hasta el siguiente espacio / fin / salto de línea
        while (buf[i] != 0 && buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\n')
            i++;
    }

    args[j] = 0;
    return j;
}

// cp: copia de archivos (read y write)
void cpn_file(char *src, char *dest)
{
    int f1 = open(src, O_RDONLY);
    if (f1 < 0)
    {
        write_str(2, "Error al abrir el archivo fuente\n");
        return;
    }

    int f2 = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (f2 < 0)
    {
        write_str(2, "Error al crear el archivo destino\n");
        close(f1);
        return;
    }

    char buf[512];
    int r;

    while ((r = read(f1, buf, sizeof(buf))) > 0)
        write(f2, buf, r);

    close(f1);
    close(f2);
}

// cat: muestra archivo usando (read/write)
void cat_file(char *file)
{
    int f = open(file, O_RDONLY);
    if (f < 0)
    {
        write_str(2, "No se puede abrir el archivo\n");
        return;
    }

    char buf[512];
    int r;
    while ((r = read(f, buf, sizeof(buf))) > 0)
        write(1, buf, r);

    close(f);
}

// pwd: usando getcwd
void pwd_cmd()
{
    char b[512];
    if (getcwd(b, sizeof(b)))
    {
        write_str(1, b);
        write(1, "\n", 1);
    }
}

// main del programa
int main()
{
    char buf[256];
    char *args[10];

    while (1)
    {
        write_str(1, "mini - bash > ");

        int r = read(0, buf, sizeof(buf) - 1);
        if (r <= 0)
            break;

        buf[r] = 0;   // aseguramos fin de cadena

        int nargs = split(buf, args);
        if (nargs == 0)
            continue;

        // cd : comando interno
        if (sqr_eq(args[0], "cd"))
        {
            if (nargs < 2)
                write_str(2, "Falta el argumento del directorio\n");
            else if (chdir(args[1]) < 0)
                write_str(2, "Error cd\n");
            continue;
        }

        // pwd : comando interno
        if (sqr_eq(args[0], "pwd"))
        {
            pwd_cmd();
            continue;
        }

        // mkdir
        if (sqr_eq(args[0], "mkdir"))
        {
            if (nargs < 2)
                write_str(2, "Falta el argumento del directorio\n");
            else if (mkdir(args[1], 0755) < 0)
                write_str(2, "Error mkdir\n");
            continue;
        }

        // rm archivo o directorio vacío
        if (sqr_eq(args[0], "rm"))
        {
            if (nargs < 2)
            {
                write_str(2, "Falta el argumento del archivo\n");
            }
            else
            {
                // primero intentamos borrar como archivo
                if (unlink(args[1]) < 0)
                {
                    // si falla, intentamos borrar como directorio vacío
                    if (rmdir(args[1]) < 0)
                        write_str(2, "Error rm\n");
                }
            }
            continue;
        }

        // mv archivo
        if (sqr_eq(args[0], "mv"))
        {
            if (nargs < 3)
                write_str(2, "Faltan argumentos para mover\n");
            else if (rename(args[1], args[2]) < 0)
                write_str(2, "Error mv\n");
            continue;
        }

        // cp archivo
        if (sqr_eq(args[0], "cp"))
        {
            if (nargs < 3)
                write_str(2, "Faltan argumentos para copiar\n");
            else
                cpn_file(args[1], args[2]);
            continue;
        }

        // cat archivo
        if (sqr_eq(args[0], "cat"))
        {
            if (nargs < 2)
                write_str(2, "Falta el argumento del archivo\n");
            else
                cat_file(args[1]);
            continue;
        }

        // Ejecutar otros comandos usando fork y execvp
        int background = 0;
        if (nargs > 0 && sqr_eq(args[nargs - 1], "&"))
        {
            background = 1;
            args[nargs - 1] = 0;
        }

        int pid = fork();
        if (pid == 0)
        {
            // Proceso hijo
            execvp(args[0], args);   // busca en PATH
            write_str(2, "Error al ejecutar el comando\n");
            _exit(1);
        }
        else if (pid > 0)
        {
            // Proceso padre
            if (!background)
                wait(0);
        }
        else
        {
            write_str(2, "Error al hacer fork\n");
        }
    }

    return 0;
}