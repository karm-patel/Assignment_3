#!/bin/sh
#SBATCH --job-name=generate_16384
#SBATCH --output=generate_16384
./data/generate 16384
echo "generated"