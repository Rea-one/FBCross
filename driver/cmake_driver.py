import json
import os
import shutil

BUILD_DIR = "build"

try:
    if os.path.exists(BUILD_DIR):
        shutil.rmtree(BUILD_DIR)
        print(f"已删除旧的构建目录: {BUILD_DIR}")
except Exception as e:
    print(f"删除构建目录失败: {e}")


with open("config/config.json", "r") as file:
    config = json.load(file)

# 正确提取path列表
paths = config["path"]
pres = config["cmake"]["pres"]
toolchain = config["cmake"]["toolchain"]

with open("config.cmake", "w") as file:
    file.write(f'{pres}\n\n')
    # for key, target in toolchain.items():
    #     file.write(f'set (CMAKE_TOOLCHAIN_FILE {target})\n\n')
    
    for compiler, targets in paths.items():
        file.write(f"if (${{CMAKE_CXX_COMPILER_ID}} STREQUAL \"{compiler}\") \n")
        file.write(f'\tlist (APPEND CMAKE_PREFIX_PATH\n')
        for name, path in targets.items():
            file.write(f"\t\t\"{path}\"\n")
        file.write('\t)\n')
        file.write("endif()\n")
        
        # file.write(f"if (${{CMAKE_CXX_COMPILER_ID}} STREQUAL \"{compiler}\") \n")
        # for name, path in targets.items():
        #     file.write(f"\tset({name.replace('-', '_')}_DIR \"{path}\")\n")
        # file.write("endif()\n")
        
