import os
import regex as re
ip_file = "header/gpu_thread.h"
is_csv = "--csv"
op_dir = f"nvprof_results_{is_csv}"
if not os.path.isdir(op_dir):
    os.system(f"mkdir {op_dir}")

with open(ip_file, 'r') as fp:
    content = fp.read()

# cache statistics
mats_cache = [512, 1024, 2048, 4096]
mats_runtime = [128, 256, 512, 1024, 2048, 4096, 8192, 16384]
tiles = [8, 16, 32]

# # naive
# content = re.sub(r"int kernel = [0-9];", f"int kernel = {0};", content)
# with open(ip_file, 'w') as fp:
#     fp.write(content)

# os.system("nvcc main.cu -o cmm_server -I ./header")
# for mat in mats_cache:
#     print(f"executing naive {mat}")
#     os.system(f"sudo nvprof {is_csv} --log-file {op_dir}/cache_trace_{mat}_naive_p --metrics gld_transactions,l2_read_transactions,dram_read_transactions,sysmem_read_transactions ./cmm_server data/input_{mat}.in")

# for mat in mats_runtime:
#     print(f"executing naive {mat}")
#     os.system(f"sudo nvprof {is_csv} --log-file {op_dir}/gpu_runtime_{mat}_naive_p --print-gpu-trace  ./cmm_server data/input_{mat}.in")

# # tiled 
# content = re.sub(r"int kernel = [0-9];", f"int kernel = {1};", content)
# for tile in tiles:
#     content = re.sub(r"int TILE = [0-9]*;", f"int TILE = {tile};", content)
#     with open(ip_file, 'w') as fp:
#         fp.write(content)

#     os.system("nvcc main.cu -o cmm_server -I ./header")
#     for mat in mats_cache:
#         print(f"executing tiled {mat}_{tile}")
#         os.system(f"sudo nvprof {is_csv} --log-file {op_dir}/cache_trace_{mat}_tiled_{tile} --metrics gld_transactions,l2_read_transactions,dram_read_transactions,sysmem_read_transactions ./cmm_server data/input_{mat}.in")

#     for mat in mats_runtime:
#         print(f"executing tiled {mat}_{tile}")
#         os.system(f"sudo nvprof {is_csv} --log-file {op_dir}/gpu_runtime_{mat}_tiled_{tile} --print-gpu-trace  ./cmm_server data/input_{mat}.in")


# with 4 multiplications
with open(ip_file, 'w') as fp:
    fp.write(content)
content = re.sub(r"int kernel = [0-9];", f"int kernel = {2};", content)
with open(ip_file, 'w') as fp:
    fp.write(content)

os.system("nvcc main.cu -o cmm_server -I ./header")
for mat in mats_cache:
    print(f"executing naive 4p {mat}")
    os.system(f"sudo nvprof {is_csv} --log-file {op_dir}/cache_trace_{mat}_naive_4p --metrics gld_transactions,l2_read_transactions,dram_read_transactions,sysmem_read_transactions ./cmm_server data/input_{mat}.in")

for mat in mats_runtime:
    print(f"executing naive 4p {mat}")
    os.system(f"sudo nvprof {is_csv} --log-file {op_dir}/gpu_runtime_{mat}_naive_4p --print-gpu-trace ./cmm_server data/input_{mat}.in")
