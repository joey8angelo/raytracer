import os
import sys
import time


def print_text_files_in_order(folder_path):
    files = os.listdir(folder_path)

    text_files = [f for f in files if f.endswith(".txt")]
    text_files.sort(key=lambda x: int(x.split(".")[0]))

    text_contents = []

    for filename in text_files:
        file_path = os.path.join(folder_path, filename)
        with open(file_path, "r") as file:
            text_contents.append(file.read())

    while True:
        for content in text_contents:
            if os.name == "nt":
                os.system("cls")
            else:
                os.system("clear")
            print(content)
            time.sleep(0.05)


if __name__ == "__main__":
    folder = sys.argv[1] if len(sys.argv) > 1 else "."
    print_text_files_in_order(folder)
