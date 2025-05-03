import argparse
from pathlib import Path
from typing import TextIO

import yaml

from doc_types import Module, Function, Class


def load_all_yaml(directory: Path) -> dict[str, Module]:
    modules: dict[str, Module] = {}
    for path in sorted(directory.glob("*.yaml")):
        print(f"input file: {path}")

        with open(path, encoding="utf-8") as f:
            data = yaml.safe_load(f)

        mod = Module(**data)
        if mod.name in modules:
            modules[mod.name].functions.extend(mod.functions)
            modules[mod.name].classes.extend(mod.classes)
        else:
            modules[mod.name] = mod

    return modules


def write_markdown_header(file: TextIO, text: str, level: int):
    line = ("#" * level) + " " + text + "\n\n"
    file.write(line)


def write_markdown_table(file: TextIO, data: list[dict]):
    keys = data[0].keys()
    header = "| " + " | ".join(keys) + " |\n"
    file.write(header)
    align = "| " + " | ".join(["---" for _ in keys]) + " |\n"
    file.write(align)
    for row in data:
        line = "| " + " | ".join([str(row[k]).replace("\n", "") for k in keys]) + " |\n"
        file.write(line)
    file.write("\n")


def write_markdown_function_doc(file: TextIO, func: Function, head_level: int):
    write_markdown_header(file, func.name, head_level)

    func_params = ", ".join([x.name for x in func.params])
    file.write(f"```lua\n{func.name}({func_params})\n```\n\n")

    file.write(func.description)
    file.write("\n\n")

    if func.params:
        file.write("**引数**\n\n")
        table = [
            {"名前": f"`{p.name}`", "型": f"`{p.type_}`", "説明": p.description}
            for p in func.params
        ]
        write_markdown_table(file, table)

    for p in func.params:
        if p.enum:
            file.write(f"**{p.name}**\n\n")
            if any([bool(x.description) for x in p.enum]):
                table = [{"値": f"`{e.value}`", "説明": e.description} for e in p.enum]
            else:
                table = [{"値": f"`{e.value}`"} for e in p.enum]
            write_markdown_table(file, table)

    file.write("**戻り値**\n\n")
    if func.returns:
        table = [{"型": f"`{r.type_}`", "説明": r.description} for r in func.returns]
        write_markdown_table(file, table)
    else:
        file.write("戻り値はありません。\n\n")


def gen_markdown_doc(directory: Path, data: dict[str, Module]):
    for module_name, module in data.items():
        gen_markdown_functions_doc(directory, module_name, module.functions)
        for cls in module.classes:
            gen_markdown_class_doc(directory, module_name, cls)

    path = directory / "README.md"
    print(f"output file: {path}")
    with open(path, "w", encoding="utf-8") as f:
        for module_name, module in data.items():
            write_markdown_header(f, f"{module_name} リファレンス", 1)

            f.write(f"- [{module_name} モジュール関数]({module_name}.md)\n")
            for cls in module.classes:
                f.write(f"- [{module_name}.{cls.name} クラス]({module_name}.{cls.name}.md)\n")


def gen_markdown_functions_doc(
    directory: Path, module_name: str, functions: list[Function]
):
    path = directory / f"{module_name}.md"
    print(f"output file: {path}")
    with open(path, "w", encoding="utf-8") as f:
        write_markdown_header(f, f"{module_name} モジュール関数", 1)

        for func in functions:
            write_markdown_function_doc(f, func, 2)


def gen_markdown_class_doc(directory: Path, module_name: str, cls: Class):
    path = directory / f"{module_name}.{cls.name}.md"
    print(f"output file: {path}")
    with open(path, "w", encoding="utf-8") as f:
        write_markdown_header(f, f"{module_name}.{cls.name} クラス", 1)
        f.write(cls.description)
        f.write("\n\n")

        write_markdown_header(f, "静的メソッド", 2)
        for func in cls.staticMethods:
            write_markdown_function_doc(f, func, 3)

        write_markdown_header(f, "インスタンスメソッド", 2)
        for func in cls.methods:
            write_markdown_function_doc(f, func, 3)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--in-dir", default="./docs/spec", help="YAML input directory")
    parser.add_argument("--out-dir", default="./docs/api", help="output directory")
    args = parser.parse_args()

    in_dir = Path(args.in_dir)
    out_dir = Path(args.out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)

    print(f"input directory: {in_dir}")
    print(f"output directory: {out_dir}")

    modules = load_all_yaml(in_dir)

    gen_markdown_doc(out_dir, modules)


if __name__ == "__main__":
    main()
