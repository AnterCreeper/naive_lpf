#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define FCOUNT 1

struct filter{
    long cm, cn;
};

void lpf(struct filter f[FCOUNT], int16_t* m, int16_t* n){
    long mm, nn;
    mm = *m;
    nn = *n;
	for(int i = 0; i < FCOUNT; i++) {
		float up_cm = (mm - f[i].cm) / 16.0f + f[i].cm;
		float up_cn = (nn - f[i].cn) / 16.0f + f[i].cn;
		mm = f[i].cm;
		nn = f[i].cn;
		f[i].cm = up_cm;
		f[i].cn = up_cn;
	}
    *m = mm;
    *n = nn;
    return;
}

void cut(int16_t* m, int16_t* n){
    int16_t mm, nn;
    mm = *m;
    nn = *n;
    double ffm, ffn;
    ffm = (double)mm / pow(2.0, 12.0);
    ffn = (double)nn / pow(2.0, 12.0);
    const double A = 2.51;
    const double B = 0.03;
    const double C = 2.43;
    const double D = 0.59;
    const double E = 0.14;
    double sm, sn;
    sm = mm < 0 ? -1.0 : 1.0;
    sn = nn < 0 ? -1.0 : 1.0;
    double fm, fn;
    fm = mm < 0 ? -ffm : ffm;
    fn = nn < 0 ? -ffn : ffn;
    fm = (fm * (A * fm + B)) / (fm * (C * fm + D) + E) * sm * pow(2.0, 15.0);
    fn = (fn * (A * fn + B)) / (fn * (C * fn + D) + E) * sn * pow(2.0, 15.0);
    *m = fm;
    *n = fn;
    return;
}

int main(){
    struct filter f[FCOUNT];
    struct filter f1[FCOUNT];
    memset(f, 0, sizeof(struct filter)*FCOUNT);
    memset(f1, 0, sizeof(struct filter)*FCOUNT);
    FILE* fsrc = fopen("input.bin", "rb");
    FILE* fdst = fopen("result.bin", "wb");
    while(!feof(fsrc)) {
        int16_t m, n;
        fread(&m, 2, 1, fsrc);
        fread(&n, 2, 1, fsrc);
        lpf(f, &m, &n);
        cut(&m, &n);
        fwrite(&m, 2, 1, fdst);
        fwrite(&n, 2, 1, fdst);
    }
    return 0;
}
