void computation(float *farr, int n, float *res) {
    int cnt = n;
    if (cnt == 0) {
        *res = 0;
        return;
    }
    float first = farr[0];
    float second = farr[1];
    float third = farr[2];
    float fourth = farr[3];
    cnt -= 4;
    for (int j = cnt, i = 4; j != 0; j -= 4, i += 4) {
        first *= farr[i];
        second *= farr[i + 1];
        third *= farr[i + 2];
        fourth *= farr[i + 3];
    }
    *res = first + second + third + fourth;
}