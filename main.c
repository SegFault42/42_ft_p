#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define offset 4096 //buff size
#define total_size 1000000000

static void    print_n_char(char c, int i)
{
	int j = 0;

	while (j < i)
	{
		write(1, &c, 1);
		j++;
	}
}

# define L_C(X, x1, x2, y1, y2) ((int)(((X - x1) * (y2 - y1)) / (x2 - x1)) + y1)


long double    progress_bar(long int end, long int current, struct timeval (*keep_time)[3], long double udelta)
{
	int x;

	gettimeofday(&((*keep_time)[1]), NULL);
	if ((*keep_time)[2].tv_sec == 0 || (*keep_time)[1].tv_sec - (*keep_time)[2].tv_sec > 0)
	{
		udelta = ((current - (*keep_time)[2].tv_usec));
		(*keep_time)[2] = (*keep_time)[1];
		(*keep_time)[2].tv_usec = (__darwin_suseconds_t)current;
	}
	x = L_C(current, 0, end, 0, 99);
	write(1, "[", 1);
	print_n_char('X', x);
	print_n_char(' ', 99 - x);
	write(1, "]", 1);
	printf(" %.3Lf o/s il reste %i second                        \r\033[1A\n", udelta, (int)((end - current) / udelta));
	return udelta;
}

int main()
{
	long double j = 0;

	struct timeval keep_time[3];
	time_t      rstart;
	long double ud = 0;

	rstart = time(NULL);
	while (j < total_size)
	{
		gettimeofday(&(keep_time[0]), NULL);
		j += offset; //(send buffer)
		ud = progress_bar(total_size, (long) j, &keep_time, ud);
	}
	printf("\nproccess took %d sec\n", (int) (time(NULL) - rstart));
	return 0;
}
