# IOI 2022 Problem Repository

IOI 2022 uses the modified Task Preparation System (TPS) available in [https://github.com/ioi-2022/tps](https://github.com/ioi-2022/tps). This is slightly modified from the canonical TPS available in [https://github.com/ioi-2017/tps](https://github.com/ioi-2017/tps). TPS documentation is available in [https://github.com/ioi-2022/tps/tree/master/docs](https://github.com/ioi-2022/tps/tree/master/docs).


## Problem Repository Structure

The problem repository contains symlinks. Therefore, a Unix-like operating system (e.g., Linux, macOS) needs to be used to work with this problem repository. Most of the structure follows the [canonical TPS problem structure](https://github.com/ioi-2017/tps/tree/master/docs#task-directory-structure) with the following exceptions.

### TPS Repository

The TPS repository is cloned as a submodule in this repository for easier reference from the problems. Therefore, on the root directory for each problem, there should be a directory `scripts/` symlinked to the `scripts/` in the TPS repository using relative symlink (i.e. to `../tps/scripts/`).

To clone this repository together with the submodule, use `git clone --recursive`. If this repository has been cloned without the submodule, use `git submodule update --init` to clone the submodule.

### Problem Assets

Common problem assets (i.e. `Makefile` and `testlib.h`) are included in `tps/problem-assets/`. Therefore, for the default `Makefile`s and `testlib.h`s, each problem should use relative symlink to these files as well.

### Test Data Generator

`gen/data` must be generated from `gen/data.py` and excluded in the problem repository. This is to avoid duplicating many hardcoded numbers, such as `maxN`. The modified `tps/problem-assets/Makefile` ensures to generate `gen/data` when running `tps gen`.

### Task Statement

The repository contains the `problem-assets/statement` directory containing common files regarding task statement for all tasks. Therefore, the `statement/` directory for each task is sufficient to contain only the following files:
- `assets` symlinked to `../../problem-assets/statement/assets/`
- `index.html` symlinked to `../../problem-assets/statement/index.html`
- `index.md`
- `index.json`


## Convention

### Input Validator

To avoid duplication in parsing the input, only one `validator.cpp` must be written. This validator takes the subtask name as the first argument. The `subtasks.json` file contains the following:
```
{
  "subtask_sensitive_validators": ["validator.cpp {subtask}"],
  "subtasks": {
    "{subtask_name}": {
      "index": {subtask_index},
      "score": {subtask_score},
      "validators": []
    },
    ...
  }
```

### Test Data Generator

If there is only one C++ test data generator code in `gen/`, the filename of the generator must be `gen.cpp`. Otherwise, the filename of the generators must have the prefix `gen-`.

### Public Grader Code

C++ public grader code must be compilable using the public compile script without warnings.

### Other Code

All C++ code must be compilable using C++17 and all Python code must be runnable using Python 3.7.
