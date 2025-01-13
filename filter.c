#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define FCOUNT 4
//FCOUNT * 20db/10 times freq

struct filter {
    int cm, cn;
};

int main(){
    struct filter f[FCOUNT];
    memset(f, 0, sizeof(struct filter)*FCOUNT);
    FILE* fsrc = fopen("input.bin", "rb");
    FILE* fdst = fopen("result.bin", "wb");
    while(!feof(fsrc)) {
        int16_t m, n;
        fread(&m, 2, 1, fsrc);
        fread(&n, 2, 1, fsrc);
        int mm, nn;
        mm = m;
	mm = mm << 6;
        nn = n;
	nn = nn << 6;
        for(int i = 0; i < FCOUNT; i++) {
//            float up_cm = (mm - f[i].cm) / 64.0f + f[i].cm;
//            float up_cn = (nn - f[i].cn) / 64.0f + f[i].cn;
            int up_cm = ((mm - f[i].cm) >> 6) + f[i].cm;
            int up_cn = ((nn - f[i].cn) >> 6) + f[i].cn;
//2*pi*freq*dt=1/64.0
//dt=sample interval
//freq=cutoff freq
            mm = f[i].cm;
            nn = f[i].cn;
            f[i].cm = up_cm;
            f[i].cn = up_cn;
        }
        m = mm >> 6;
        n = nn >> 6;
        fwrite(&m, 2, 1, fdst);
        fwrite(&n, 2, 1, fdst);
    }
    return 0;
}
