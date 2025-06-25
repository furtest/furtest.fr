from ghidra.app.decompiler import DecompInterface
from ghidra.util.task import ConsoleTaskMonitor

program = getCurrentProgram()
ifc = DecompInterface()
ifc.openProgram(program)

f = open("workdir/decompiled_main.txt", "at")
g = open("workdir/decompiled_encode.c", "at")
g.write("#include <stdlib.h>\n#include <stdint.h>")
fm = currentProgram.getFunctionManager()
funcs = fm.getFunctions(True) # True means 'forward'
for func in funcs: 
    if func.getName() == "FUN_00101169":
        f.write(ifc.decompileFunction(func, 0, ConsoleTaskMonitor()).getDecompiledFunction().getC())
    elif func.getName() == "FUN_0010123f":
        g.write(ifc.decompileFunction(func, 0, ConsoleTaskMonitor()).getDecompiledFunction().getC())
f.close()
g.close()
