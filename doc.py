from pathlib import Path


cwd = Path.cwd()
src = Path(cwd, "src")

doc_files: list[Path] = []

for file in src.rglob("*.h"):
    docs = cwd / "docs" / file.relative_to(src).parent
    docs.mkdir(parents=True, exist_ok=True)
    with open(docs / "README.md", "w") as f:
        f.truncate()

for file in src.rglob("*.h"):
    docs = cwd / "docs" / file.relative_to(src).parent
    with open(docs / "README.md", "a") as f:
        f.write(
            f"# [{file.name}](https://github.com/Code-Nycticebus/clib/blob/main/{file.relative_to(cwd)})\n"
        )

        writing = False
        with open(file, "r") as d:
            for line in d.readlines():
                if line.startswith("*/"):
                    writing = False
                if writing:
                    f.write(line)
                if line.startswith("/* DOCUMENTATION"):
                    writing = True
