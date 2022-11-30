import os
import regex as re
import shutil
import time

ip_file = "header/gpu_thread.h"
n_iter = 10
mat_sizes = [512,1024,2048,4096,8192,16384]
tile_sizes = [8,16,32]

# n_iter = 3
# mat_sizes = [512,1024]
# tile_sizes = [32]

kernels = ["tiled", "naive"]
if not os.path.isdir(f"results"):
    os.system(f"results")

if not os.path.isdir(f"results_gpu"):
    os.system(f"results_gpu")

def make_command(kernel, mat_size, tile_size):
    if kernel == "tiled":
        slurm_file = f"slurms/mat_mul_{mat_size}_tiled_{tile_size}.sh" 
    else:
        slurm_file = f"slurms/mat_mul_{mat_size}_naive_.sh"
    
    command = f"sbatch {slurm_file}"
    return command


commands = []
if "tiled" in kernels:
    for tile_size in tile_sizes:
        for mat_size in mat_sizes:
            commands.append(make_command("tiled", mat_size, tile_size))
             

if "naive" in kernels:
    for mat_size in mat_sizes:
        commands.append(make_command("naive", mat_size, tile_size))
    
print(len(commands), " commands")

for i in range(n_iter):
    print("Iteration: ", i)
    if not os.path.isdir(f"results/iter_{i}"):
        os.system(f"mkdir results/iter_{i}")
        print("made")

    for cmd in commands:
        print("executing...", cmd)
        with open(cmd.split(" ")[1], "r") as fp:
            content = fp.read()
        
        content = re.sub(r"results/iter_[0-9]*/", f"results/iter_{i}/", content)

        with open(cmd.split(" ")[1], "w") as fp:
            fp.write(content)

        os.system(cmd)
        time.sleep(30)

    # shutil.move(f"results", "results_gpu/iter_{i}")