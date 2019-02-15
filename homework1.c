#include "homework1.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int num_threads;
int resolution;

image *draw = NULL;

void initialize(image *im) {
	im->pixels = malloc(resolution * sizeof(unsigned char*));
	for(int i = 0; i < resolution; i++) {
		im->pixels[i] = malloc(resolution * sizeof(unsigned char));
	}
}

unsigned char dist(float x, float y) {
	if(abs(-((x + 0.5f)* 100 / resolution) + 2*((y + 0.5f)* 100 / resolution)) / sqrt(5) <= 3) {
		return 0;
	}
	return 255;
}

void* threadFunction(void *var) {
	int thread_id = *(int*)var;

	int start_i = thread_id * (resolution/num_threads);
	int stop_i = (thread_id + 1) * (resolution/num_threads);

	if(thread_id == num_threads - 1) {
		stop_i = resolution;
	}

	for(int i = stop_i - 1; i >= start_i; i--) {
		for(int j = 0; j < resolution; j++) {
			unsigned char aux = dist((float)j,(float)i);
			draw->pixels[i][j] = aux;
		}
	}

	return 0;
}



void render(image *im) {
	int i;
	im->pixels = malloc(resolution * sizeof(unsigned char*));
	for(int i = 0; i < resolution; i++) {
		im->pixels[i] = malloc(resolution * sizeof(unsigned char));
	}

	draw = malloc(sizeof (image));
	draw->pixels = malloc(resolution * sizeof(unsigned char*));
	for(int i = 0; i < resolution; i++) {
		draw->pixels[i] = malloc(resolution * sizeof(unsigned char));
	}

	pthread_t tid[num_threads];
	int thread_id[num_threads];
	for(i = 0;i < num_threads; i++)
		thread_id[i] = i;

	for(i = 0; i < num_threads; i++) {
		pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
		
	}

	for(i = 0; i < num_threads; i++) {
		pthread_join(tid[i], NULL);
	}
	

	*im = *draw;
}

void writeData(const char * fileName, image *img) {
	FILE *file;
	file = fopen(fileName, "w");
	int maxval = 255;
	fprintf(file, "P5\n");
	fprintf(file, "%d %d\n%d\n", resolution, resolution, maxval);

	for(int i = resolution - 1; i >= 0; i--) {
		for(int j = 0; j < resolution; j++) {
			fprintf(file, "%c", img->pixels[i][j]);
		}
	}

	fclose(file);
}

