from pathlib import Path, PurePosixPath
from collections import defaultdict
import subprocess
from typing import TextIO

REMOTE = (
    subprocess.check_output(["git", "config", "--get", "remote.origin.url"])
    .decode()
    .strip()[:-4]
)
CWD = Path.cwd()
SRC = Path(CWD, "src")
DOC = CWD / "docs"
README = DOC / "README.md"


def write_src_to_doc(doc: TextIO, src: Path):
    first = True
    with open(src, "r") as d:
        write_mode: bool = False
        for line in d.readlines():
            if "*/" in line:
                write_mode = False
            if write_mode:
                doc.write(line)
            if "/* DOCUMENTATION" in line:
                if first:
                    first = False
                    doc.write(
                        f"#### [{src.name}]({REMOTE}/blob/main/{PurePosixPath(src.relative_to(CWD))})\n"
                    )
                write_mode = True


def main() -> None:
    docs_dict: dict[Path, list[Path]] = defaultdict(list)

    # Collects the files
    for file in SRC.rglob("*.h"):
        doc = DOC / file.relative_to(SRC).parent
        docs_dict[doc].append(file)

    with open(README, "r") as f:
        # Removes any previous concatinated documentation
        lines_to_keep = []
        for line in f.readlines():
            lines_to_keep.append(line)
            if "<!-- DOCUMENTATION -->" in line:
                break

    sorted_entries = tuple(
        (docs, tuple(sorted(files))) for docs, files in sorted(docs_dict.items())
    )

    # Creates table of content
    with open(README, "w") as f:
        for line in lines_to_keep:
            f.write(line)
        for docs, files in sorted_entries:
            f.write(f"- {docs.name.title()}\n")
            for file in files:
                f.write(f"   - [{file.name}](#{file.name.replace('.', '')})\n")

        # Iterates over all files and writes their content to readme
        for docs, files in sorted_entries:
            f.write(f"### {docs.name.title()}\n")

            for file in files:
                write_src_to_doc(f, file)


if __name__ == "__main__":
    main()
