To start a program in GDB (GNU Debugger), follow these steps:

### 1. **Compile Your Program with Debugging Information:**
Before using GDB, compile your program with the `-g` option to include debugging information. For example, if you have a C file called `program.c`, compile it with:

```bash
gcc -g -o program program.c
```

### 2. **Launch GDB:**
Once compiled, you can start GDB by specifying the executable file. For example:

```bash
gdb ./program
```

### 3. **Run the Program in GDB:**
After launching GDB, you can start running your program by using the `run` command:

```bash
(gdb) run
```

You can also pass command-line arguments to the program as you normally would:

```bash
(gdb) run arg1 arg2
```

### 4. **Set Breakpoints (Optional):**
If you want to pause execution at a certain point, set breakpoints. For example, to set a breakpoint at the `main` function:

```bash
(gdb) break main
```

Once a breakpoint is hit, you can step through your code, examine variables, and debug interactively.

### 5. **Other Useful Commands:**
- **`next`**: Step to the next line.
- **`step`**: Step into a function.
- **`continue`**: Resume execution until the next breakpoint.
- **`print <variable>`**: Print the value of a variable.
- **`backtrace`**: Display the call stack.
  
That's it! You can start debugging your program in GDB using these basic commands. 
