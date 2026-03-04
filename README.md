*This project has been created as part of the 42 curriculum by mperrine.*

## Description

The mandatory part of `pipex` reproduces the behavior of a single pipe. The program takes an input file, executes two commands and writes the output to the specified output file.

It reproduces the following shell behavior:
```bash
< file1 cmd1 | cmd2 > file2
```

The bonus part add support for `multiple pipes` functionality and the `here_doc` feature to the project.

Multiple pipes allow the user to use chain several commands in a single execution.

It reproduces the following shell behavior:
```bash
< file1 cmd1 | cmd2 | cmd3 > file2
```
The here_doc feature allow the user to user the standard input (`stdin`) instead of an input file and also appends the result to the output file.

It reproduces the following shell behavior:
```bash
<< LIMITER cmd1 | cmd2 | cmd3 >> file2
```

## Instructions

A `Makefile` is provided to compile the project.

**Build**
```bash
make
make bonus
```

**Clean**
```bash
make clean
make fclean
```

**Rebuild**
```bash
make re
```

## Usage

```bash
./pipex file1 cmd1 cmd2 file2
```
or
```bash
./pipex_bonus file1 cmd1 cmd2 cmd3 file2

./pipex_bonus here_doc LIMITER cmd1 cmd2 cmd3 file2
```

Example:
```bash
./pipex infile "ls -l" "wc -l" outfile
# Equivalent to: < infile ls -l | wc -l > outfile
```

## Resources

AI was used to get a better understanding of the bonus part (multiple pipes) and to fix orthography errors in this readme.

This [guide](https://csnotes.medium.com/pipex-tutorial-42-project-4469f5dd5901) was used at first to get a better understanding of the subject.

This [website](https://koor.fr/) was used to read the man of the allowed functions.

The terminal was used as an example to observe pipe behaviors.
