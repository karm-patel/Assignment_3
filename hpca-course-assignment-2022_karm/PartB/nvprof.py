import os
import regex as re
ip_file = "header/gpu_thread.h"

if not os.path.isdir("nvprof_results"):
    os.system("mkdir nvprof_results")

with open(ip_file, 'r') as fp:
    content = fp.read()

# cache statistics
mats_cache = [512, 1024, 2048, 4096]
mats_runtime = [128, 256, 512, 1024, 2048, 4096, 8192, 16384]
tiles = [8, 16, 32]

# naive
# content = re.sub(r"int kernel = [0-9];", f"int kernel = {0};", content)
# with open(ip_file, 'w') as fp:
#     fp.write(content)

# os.system("nvcc main.cu -o cmm_server -I ./header")
# for mat in mats_cache:
#     print(f"executing naive {mat}")
#     os.system(f"sudo nvprof --log-file nvprof_results/cache_trace_{mat}_naive.txt --metrics gld_transactions,l2_read_transactions,dram_read_transactions,sysmem_read_transactions ./cmm_server data/input_{mat}.in")

# for mat in mats_runtime:
#     print(f"executing naive {mat}")
#     os.system(f"sudo nvprof --print-gpu-trace --log-file nvprof_results/gpu_runtime_{mat}_naive.txt  ./cmm_server data/input_{mat}.in")

# tiled 
content = re.sub(r"int kernel = [0-9];", f"int kernel = {1};", content)
for tile in tiles:
    content = re.sub(r"int TILE = [0-9]*;", f"int TILE = {tile};", content)
    with open(ip_file, 'w') as fp:
        fp.write(content)

    os.system("nvcc main.cu -o cmm_server -I ./header")
    # for mat in mats_cache:
    #     print(f"executing tiled {mat}_{tile}")
    #     os.system(f"sudo nvprof --log-file nvprof_results/cache_trace_{mat}_tiled_{tile}.txt --metrics gld_transactions,l2_read_transactions,dram_read_transactions,sysmem_read_transactions ./cmm_server data/input_{mat}.in")

    for mat in mats_runtime:
        print(f"executing tiled {mat}_{tile}")
        os.system(f"sudo nvprof --print-gpu-trace --log-file nvprof_results/gpu_runtime_{mat}_tiled_{tile}.txt  ./cmm_server data/input_{mat}.in")