import os, shutil
import argparse

# -------- consts --------
cmake_min_version = '3.11'
cmake_text = 'CMakeLists.txt'
cmake_find_text1 = f'cmake_minimum_required(VERSION {cmake_min_version})\n'
cmake_find_text2 = 'set(PROJECT_NAME)\n'
# -------- consts --------


def replace_text_in_file(src_path, dst_path, anchor_text, prev_text, text):
    with open(src_path, 'r') as fin, open(dst_path, 'w') as fout:
        for line in fin:
            fout.write(line)
            if line == anchor_text:
                next_line = next(fin)
                if next_line == prev_text:
                    fout.write(text)
                    continue
                fout.write(next_line)

class ProjectGenerator:
    def __init__(self):
        self.template_folder = 'template'

    def __copy_template(self, fullpath):
        src_path = os.curdir
        src_path = os.path.join(src_path, self.template_folder)

        shutil.copytree(src_path, fullpath)

    def __rename_project(self, name, fullpath):
        cmake_path_src = os.path.join(fullpath, f'{cmake_text}.in')
        cmake_path_dst = os.path.join(fullpath, f'{cmake_text}')

        project_text = f'project({name})'

        replace_text_in_file(cmake_path_src,
                             cmake_path_dst,
                             cmake_find_text1,
                             cmake_find_text2,
                             f'{project_text}\n')

        os.remove(cmake_path_src)

    def run(self, name, path):
        fullpath = os.path.join(path, name)

        print("Step 1: Copy Template")
        self.__copy_template(fullpath)

        print("Step 2: Set Project name")
        self.__rename_project(name, fullpath)

        print(f"Success created project: {name}, at {fullpath}")


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Command line tool', epilog='Have fun :)')
    parser.add_argument('-n', '--name', help='Actual game project name', required=True)
    parser.add_argument('-p', '--path', help='Path of project root dir', required=True)
    args = parser.parse_args()

    if args.name and args.path:
        generator = ProjectGenerator()
        generator.run(args.name, args.path)


