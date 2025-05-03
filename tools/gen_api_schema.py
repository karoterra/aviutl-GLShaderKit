import argparse
import json
from pathlib import Path

from doc_types import Module


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-o", "--output", default="./docs/spec/api.schema.json")

    args = parser.parse_args()

    path = Path(args.output).resolve()
    print(f"output: {path}")

    schema = Module.model_json_schema()
    with open(path, "w", encoding="utf-8") as f:
        json.dump(schema, f, ensure_ascii=False, indent=4)


if __name__ == "__main__":
    main()
