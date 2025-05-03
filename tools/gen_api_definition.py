import argparse
from pathlib import Path
from typing import TextIO

import yaml

from doc_types import Module, Function, Class, ParamEnum


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


def write_comment(file: TextIO, comment: str):
    for line in comment.split("\n"):
        file.write(f"---{line}\n")


def write_enum_alias(file: TextIO, name: str, enum: list[ParamEnum]):
    file.write(f"---@alias {name}\n")
    for e in enum:
        if e.description:
            file.write(f"---| '\"{e.value}\"' # {e.description}\n")
        else:
            file.write(f"---| '\"{e.value}\"'\n")
    file.write("\n")


def write_function_def(file: TextIO, func: Function, prefix: str):
    write_comment(file, func.description)

    for p in func.params:
        param_name = p.name + ("?" if p.optional else "")
        desc = p.description.replace("\n", "")
        file.write(f"---@param {param_name} {p.type_} {desc}\n")
        if p.enum:
            for e in p.enum:
                if e.description:
                    file.write(f"---| '\"{e.value}\"' # {e.description}\n")
                else:
                    file.write(f"---| '\"{e.value}\"'\n")

    for r in func.returns:
        desc = r.description.replace("\n", "")
        file.write(f"---@return {r.type_} # {desc}\n")

    args = ", ".join([p.name for p in func.params])
    file.write(f"function {prefix}{func.name}({args}) end\n\n")


def write_ctor_def(file: TextIO, func: Function, cls: Class, prefix: str):
    write_comment(file, func.description)

    for p in func.params:
        param_name = p.name + ("?" if p.optional else "")
        desc = p.description.replace("\n", "")
        file.write(f"---@param {param_name} {p.type_} {desc}\n")
        if p.enum:
            for e in p.enum:
                if e.description:
                    file.write(f"---| '\"{e.value}\"' # {e.description}\n")
                else:
                    file.write(f"---| '\"{e.value}\"'\n")

    args = ", ".join([p.name for p in func.params])
    file.write(f"function {prefix}{func.name}({args})\n")
    file.write(f"    ---@type {cls.name}\n")
    file.write("    local o = {\n")

    methods = {m.name: m for m in cls.methods}
    for method in methods.values():
        file.write(f"        {method.name} = {cls.name}.{method.name},\n")

    file.write("    }\n")
    file.write("    return o\n")
    file.write("end\n\n")


def write_class_def(file: TextIO, cls: Class, prefix: str):
    file.write(f"{prefix}{cls.name} = {{}}\n\n")

    file.write(f"---@class {cls.name}\n")
    file.write(f"local {cls.name} = {{}}\n\n")

    for func in cls.staticMethods:
        is_ctor = bool(func.returns) and func.returns[0].type_ == cls.name
        if is_ctor:
            write_ctor_def(file, func, cls, f"{prefix}{cls.name}:")
        else:
            write_function_def(file, func, f"{prefix}{cls.name}:")

    for func in cls.methods:
        write_function_def(file, func, f"{cls.name}:")


def write_module_def(file: TextIO, module: Module):
    file.write(f"---@meta {module.name}\n\n")

    file.write(f"local {module.name} = {{}}\n\n")

    for func in module.functions:
        write_function_def(file, func, f"{module.name}.")

    for cls in module.classes:
        write_class_def(file, cls, f"{module.name}.")

    file.write(f"return {module.name}\n")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--in-dir", default="./docs/spec", help="YAML input directory")
    parser.add_argument("--out-dir", default="./docs/def", help="output directory")
    args = parser.parse_args()

    in_dir = Path(args.in_dir)
    out_dir = Path(args.out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)

    print(f"input directory: {in_dir}")
    print(f"output directory: {out_dir}")

    modules = load_all_yaml(in_dir)

    for module in modules.values():
        path = out_dir / f"{module.name}_def.lua"
        print(f"output file: {path}")
        with open(path, "w", encoding="utf-8") as f:
            write_module_def(f, module)


if __name__ == "__main__":
    main()
