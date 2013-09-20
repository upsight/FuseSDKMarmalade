# fixes return values for FuseAPI_Interface.cpp after auto-generation
from tempfile import mkstemp
from shutil import move
from os import remove, close

def fixReturns(file_path):
    #Create temp file
    fh, abs_path = mkstemp()
    new_file = open(abs_path,'w')
    old_file = open(file_path)
    retType = 0
    for line in old_file:
		if retType == -1:
			retType = 0
		else:
			new_file.write(line)
		if "const char* FuseAPI" in line:
			retType = 1
		elif "int FuseAPI" in line:
			retType = 2
		elif "bool FuseAPI" in line:
			retType = 3
		if "if (!_extLoad())" in line:
			if retType == 1:
				new_file.write("		return NULL;\n")
			elif retType == 2:
				new_file.write("		return 0;\n")
			elif retType == 3:
				new_file.write("		return false;\n")
			if retType > 0:
				retType = -1
    #close temp file
    new_file.close()
    close(fh)
    old_file.close()
    #Remove original file
    remove(file_path)
    #Move new file
    move(abs_path, file_path)

fixReturns("./interface/FuseAPI_interface.cpp")