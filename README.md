#Usage
###Tool offset for X
1. Load tool 1
2. touch off in X-direction
3. press X (only for tool 1). This currently hardcodes a diameter (PART_PROBE_DIAMETER in Configuration.h)
4. Store 1 (optional)
5. Load tool 2
6. touch off in X-direction
7. Store 2   

###Tool offset for Z (not implemented)
1. Load tool 1
2. Touch off in Z-direction
3. Press Z (for tool 1 only). This zeros axis
4. Store 1 (optional)
5. Load tool 2
6. touch off in Z-direction
7. Store 2

#Instructions
1. Delete menwiz directory in C:\Program Files\ArduinoXXX\libraries if exists (where XXX is the arduino version number).
2. Download zip from this github repository
3. Unzipping will produce a folder called menwiz-master
4. Move the folder to C:\Program Files\ArduinoXXX\libraries
5. Rename menwiz-master to menwiz
6. Close all Arduino windows.
7. Open Arduino
8. File->Examples->menwiz->Basic
9. If you want to rename to Basic5, File->Save As