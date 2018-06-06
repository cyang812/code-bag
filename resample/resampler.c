#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

int resampleData(const int16_t *sourceData, int32_t sampleRate, uint32_t srcSize, int16_t *destinationData,
                  int32_t newSampleRate) {
    if (sampleRate == newSampleRate) {
        memcpy(destinationData, sourceData, srcSize * sizeof(int16_t));
        return srcSize;
    }
    uint32_t last_pos = srcSize - 1;
    uint32_t dstSize = (uint32_t) (srcSize * ((float) newSampleRate / sampleRate));
    for (uint32_t idx = 0; idx < dstSize; idx++) {
        float index = ((float) idx * sampleRate) / (newSampleRate);
        uint32_t p1 = (uint32_t) index;
        float coef = index - p1;
        uint32_t p2 = (p1 == last_pos) ? last_pos : p1 + 1;
        destinationData[idx] = (int16_t) ((1.0f - coef) * sourceData[p1] + coef * sourceData[p2]);
    }

    return dstSize;
}

short input_buf[4096];
short output_buf[4096];

int main(int argc, char *argv[]) {
    printf("Audio Processing\n");
    if (argc < 3)
        return -1;
    const char *in_file_name = argv[1];
    const char *out_file_name = argv[2];

    FILE *in_file, *out_file;

    if(in_file_name == NULL || out_file_name == NULL)
    {
        printf("please input in_file_name and out_file_name\n");
        return 0;
    }

    in_file = fopen(in_file_name, "rb");
    out_file = fopen(out_file_name, "wb");

    if(in_file == NULL || out_file == NULL)
    {
        if(in_file)
            fclose(in_file);    
        if(out_file)
            fclose(out_file);

        printf("open file fail!\n");
        return 0;
    }

    int read_cnt = 0;
    int write_cnt = 0;

    clock_t start, finish;
    start = clock();
    // resampler(in_file, out_file);
    while((read_cnt = fread(input_buf, sizeof(short), 4096, in_file)) != 0)
    {
        // write_cnt = resample_simple(1.5, input_buf, output_buf, read_cnt);  
        write_cnt = resampleData(input_buf, 32000, 4096, output_buf, 48000);

        if(write_cnt)
        {
            fwrite(output_buf, sizeof(short), write_cnt, out_file);
        }
    }
    finish = clock();

    fclose(in_file);
    fclose(out_file);
    double Total_time = (double)(finish-start) / CLOCKS_PER_SEC; 
    printf( "%f seconds", Total_time); 

    return 0;
}

#ifdef __cplusplus
}
#endif