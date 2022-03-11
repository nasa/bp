# Bundle Protocol (BP) Application Updates

This is a living document that reflects development changes and other noteworthy items since the last release.
It should be integrated into final release documentation before the next.

## Integration with BPLIB

The embedded "lib" directory has been removed, as it was mostly duplicate code from the separately-released "bplib" repository.  With updates being made to bplib to support other use cases and also BPv7, it is important that the BP CFS app works with all the latest developments in bplib.

The current "bplib" has added a CMakeLists.txt build file such that it can be dropped into a CFS build and built like any other library.  Just clone the bplib repository under libs (or apps) and add "bplib" to the targets.cmake of CFS.

Add the following line to cfe_es_startup.scr, _before_ loading BP APP:

    CFE_LIB, bplib, NULL, BP_LIB, 0, 0, 0x0, 0;

## Removal of embedded CFDP "file" code

This was a local, barebone CFDP implementation to support polling of directories.  Going forward, this functionality should be provided by the separate CF application.  The existing implementation did not work with the CFS "Caelum" and corresponding OSAL, so this local implementation was removed.  PACE had some special requirements regarding the polling operation and file retention, which is why the local implementation existed in the first place.  Users without such special requirements should be able to use CF to provide file transfer and directory poll capabilities.
