/* resample.c - sampling rate conversion subroutines
 *
 * Original version available at the 
 * Digital Audio Resampling Home Page located at
 * http://ccrma.stanford.edu/~jos/resample/.
 *
 * Modified for use on Android by Ethan Chen
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "resample.h"

#define IBUFFSIZE 4096		/* default input buffer size */

static inline short WordToHword(int v, int scl)
{
	short out;
	int llsb = (1 << (scl - 1));
	v += llsb;		/* round */
	v >>= scl;
	if (v > MAX_HWORD) {
		v = MIN_HWORD;
	}
	out = (short)v;
	return out;
}

/* Sampling rate conversion using linear interpolation for maximum speed. */
static int SrcLinear(short X[], short Y[], double factor, unsigned int *Time,
		     unsigned short Nx)
{
	short iconst;
	short *Xp, *Ystart;
	int v, x1, x2;

	unsigned int dt;	/* Step through input signal */
	unsigned int endTime;

	factor = 1.0 / factor;
	dt = factor * FP_FACTOR + 0.5;	/* Output sampling period */

	Ystart = Y;
	endTime = *Time + FP_FACTOR * (int)Nx;
	while (*Time < endTime) {
		iconst = (*Time) & FP_MASK;	/* mask off lower 16 bits of time */
		Xp = &X[(*Time) >> FP_DIGITS];	/* Ptr to current input sample is top 16 bits */
		x1 = *Xp++;
		x2 = *Xp;
		x1 *= FP_FACTOR - iconst;
		x2 *= iconst;
		v = x1 + x2;
		*Y++ = WordToHword(v, FP_DIGITS);	/* Deposit output */
		*Time += dt;	/* Move to next sample by time increment */
	}
	return (Y - Ystart);	/* Return number of output samples */
}

int
resample_simple(double factor, short *in_buf, short *out_buf, int buffer_size)
{
    int num;
    unsigned int time;

    time = (10 << FP_DIGITS);
    num = SrcLinear(in_buf, out_buf, factor, &time, buffer_size);

    return num;
}

struct rs_data *resample_init(int in_rate, int out_rate, int buffer_size)
{
	struct rs_data *rs;

	rs = (struct rs_data *)calloc(sizeof(struct rs_data), 1);
	if (!rs) {
		return NULL;
	}
	if (out_rate <= 0 || in_rate <= 0) {
		return NULL;
	}

	rs->factor = out_rate / (double)in_rate;
	rs->in_buf_offset = 10;
	rs->in_buf_ptr = rs->in_buf_offset;
	rs->in_buf_read = rs->in_buf_offset;
	rs->time = (rs->in_buf_offset << FP_DIGITS);

	rs->in_buf_size = buffer_size;
	rs->out_buf_size =
	    (int)(((double)(rs->in_buf_size)) * rs->factor + 2.0);

	rs->in_buf = (short *)calloc(sizeof(short), rs->in_buf_size + rs->in_buf_offset);
	rs->out_buf = (short *)calloc(sizeof(short), rs->out_buf_size);

	printf("in_buf size = %d\n", rs->in_buf_size + rs->in_buf_offset);
	printf("out_buf size = %d\n", rs->out_buf_size);

	if (!rs->in_buf || !rs->out_buf) {
		resample_close(rs);
		return NULL;
	}
	memset(rs->in_buf, 0, sizeof(short) * rs->in_buf_offset);
	return rs;
}

int resample(struct rs_data *rs, short *in_buf, int in_buf_size, short *out_buf,
	 int out_buf_size, int last)
{
	int i, len;
	int num_in;		/* number of samples from previous buffer */
	int num_out;		/* number of samples resampled by SrcLinear */
	int num_reuse;		/* number of samples to re-use in next buffer */
	int num_creep;		/* number of samples of time accumulation */
	int out_total_samples;

	if (!rs) {
		return -1;
	}

	rs->in_buf_used = 0;
	out_total_samples = 0;

	if (rs->out_buf_ptr) {
		len = MIN(out_buf_size, rs->out_buf_ptr);
		/* copy leftover samples to the output */
		for (i = 0; i < len; i++) {
			out_buf[out_total_samples + i] = rs->out_buf[i];
		}
		out_total_samples += len;
		/* shift remaining samples in output buffer to beginning */
		for (i = 0; i < rs->out_buf_ptr - len; i++) {
			rs->out_buf[i] = rs->out_buf[len + i];
		}
		rs->out_buf_ptr -= len;

		return out_total_samples;
	}

	for (;;) {
		/* grab input samples from buffer */
		len = rs->in_buf_size - rs->in_buf_read;
		if (len >= in_buf_size - rs->in_buf_used) {
			len = in_buf_size - rs->in_buf_used;
		}
		for (i = 0; i < len; i++) {
			rs->in_buf[rs->in_buf_read + i] =
			    in_buf[rs->in_buf_used + i];
		}
		rs->in_buf_used += len;
		rs->in_buf_read += len;

		if (last && (rs->in_buf_used == in_buf_size)) {
			/* pad buffer with zero if no more data */
			num_in = rs->in_buf_read - rs->in_buf_offset;
			for (i = 0; i < rs->in_buf_offset; i++) {
				rs->in_buf[rs->in_buf_read + i] = 0;
			}
		} else {
			num_in = rs->in_buf_read - 2 * rs->in_buf_offset;
		}

		if (num_in <= 0) {
			break;
		}

		/* do linear interpolation */
		num_out =
		    SrcLinear(rs->in_buf, rs->out_buf, rs->factor,
			      &rs->time, num_in);

		/* move time back num_in samples back */
		rs->time -= (num_in << FP_DIGITS);
		rs->in_buf_ptr += num_in;

		/* remove time accumulation */
		num_creep = (rs->time >> FP_DIGITS) - rs->in_buf_offset;
		if (num_creep) {
			rs->time -= (num_creep << FP_DIGITS);
			rs->in_buf_ptr += num_creep;
		}

		/* copy input signal that needs to be reused */
		num_reuse =
		    rs->in_buf_read - (rs->in_buf_ptr - rs->in_buf_offset);
		for (i = 0; i < num_reuse; i++) {
			rs->in_buf[i] =
			    rs->in_buf[(rs->in_buf_ptr - rs->in_buf_offset) +
				       i];
		}
		rs->in_buf_read = num_reuse;
		rs->in_buf_ptr = rs->in_buf_offset;

		/* copy samples to output buffer */
		rs->out_buf_ptr = num_out;
		if (rs->out_buf_ptr && (out_buf_size - out_total_samples > 0)) {
			len =
			    MIN(out_buf_size - out_total_samples,
				rs->out_buf_ptr);
			for (i = 0; i < len; i++) {
				out_buf[out_total_samples + i] = rs->out_buf[i];
			}
			out_total_samples += len;
			/* store uncopied output buffer */
			for (i = 0; i < rs->out_buf_ptr - len; i++) {
				rs->out_buf[i] = rs->out_buf[len + i];
			}
			rs->out_buf_ptr -= len;
		}
		if (rs->out_buf_ptr) {
			break;
		}
	}

	return out_total_samples;
}

void resample_close(struct rs_data *rs)
{
	if (rs) {
		free(rs->in_buf);
		free(rs->out_buf);
		free(rs);
		rs = NULL;
	}
}

short input_buf[7680/2];
short output_buf[5760+10]; //5760

int main(int argc, char const *argv[])
{

    const char *in_file_name = argv[1];
    const char *out_file_name = argv[2];

    FILE *in_file, *out_file;

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

#if 0
    while((read_cnt = fread(input_buf, sizeof(short), 4096, in_file)) != 0)
    {
    	write_cnt = resample_simple(1.5, input_buf, output_buf, read_cnt);	
    	if(write_cnt)
    	{
    		fwrite(output_buf, sizeof(short), write_cnt, out_file);
    	}
    }
#else
    struct rs_data *rsData = NULL;
    rsData = resample_init(32000, 48000, 7680/2);

    if(!rsData)
    {
    	printf("init fail\n");
    	return 0;
    }

    int isLast = 0;
    while((read_cnt = fread(input_buf, sizeof(short), 7680/2, in_file)) != 0)
    {
    	if(read_cnt != 7680/2)
    		isLast = 1;	
    	write_cnt = resample(rsData, input_buf, sizeof(input_buf)/2, output_buf, sizeof(output_buf)/2, isLast);

    	if(write_cnt)
    	{
    		fwrite(output_buf, sizeof(short), write_cnt, out_file);
    	}
    }
#endif

    resample_close(rsData);
    fclose(in_file);
    fclose(out_file);

	return 0;
}