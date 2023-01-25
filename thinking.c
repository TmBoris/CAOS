
void print(int num) {
    while (1) {
        num++;
        if(num > 10) {
            break;
        }
    }
}

int main(void) {
    int num = 5;
    print(num);
}