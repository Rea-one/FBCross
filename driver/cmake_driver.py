import json

with open("config/config.json", "r") as f:
    config = json.load(f)

# 正确提取path列表
paths = config["path"]

with open("config.cmake", "w") as f:
    for name, path in paths.items():
        f.write(f"set({name.upper()}_ROOT {path})\n")