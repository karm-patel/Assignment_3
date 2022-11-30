#!/bin/sh
#SBATCH --job-name=generate 16384
#SBATCH --output=generate 16384
nvcc vector_add.cu
./a.out
nvidia-smi
./data/generate 16384