enum { Bufsize = 4096 };

struct FileWriteState {
    int fd;  // "файловый дескриптор", для вывода на стандартный поток вывода -
             // 1
    unsigned char *buf;  // указатель на буфер
    int bufsize;         // размер буфера
    // здесь потребуется добавить поля для реализации буферизованной записи
};

unsigned char buf[Bufsize];
struct FileWriteState tmp = {.fd = 1, .buf = buf, .bufsize = 0};
struct FileWriteState *stout = &tmp;

void print_string(int fd, const unsigned char *str, int length) {
    asm volatile("mov $4, %%eax \n"
                 "int $0x80 \n"
                 :
                 : "b"(fd), "c"(str), "d"(length)
                 : "eax"  // indeed necessary clobber
    );
}

void flush(struct FileWriteState *out) {
    print_string(out->fd, out->buf,
                 out->bufsize);  // не очень понял в третий аргумент нужно в
                                 // байтах или просто количество, но поскольку
                                 // мы работаем с чарами нам все равно
}

void writechar(int c, struct FileWriteState *out) {
    out->buf[out->bufsize++] = c;
    if (out->bufsize == sizeof(out->buf)) {  // с нуля потому что
        flush(out);
        out->bufsize = 0;
    }
}

// int main() {
//     for (int i = 0; i < 4100; ++i) {
//         writechar('a', stout);
//     }
//     writechar('b', stout);
//     flush(stout);
//     writechar('c', stout);
//     return 0;
// }