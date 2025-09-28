# REL, DOL, & Apploader Loader plugins for IDA Pro

The REL, DOL, & Apploader files are found in Nintendo Gamecube/Wii games.

## Environment
* Visual Studio 2022
* IDA Pro 9.1 SDK

## DOL Loader
A modified fork of the DOL loader by Stefan Esser, source from [here](http://hitmen.c02.at/html/gc_tools.html).

### Changes
* Currently none (will consider bug fixes)

## REL Loader
A rewrite/fork of the RSO loader by Stephen Simpson, source from [here](https://github.com/Megazig/rso_ida_loader).

### Features
* Creates segments/sections (.text, .data, .bss).
* Strips loader data from the binary.
* Identifies exported functions (prolog, epilog, unresolved).
* Treats relocations to external modules as imports.
* Reads other modules in the same folder as the target module to map ids to names and obtain correct import offsets.
* Allows selecting of a `symbol map` file on analysis to load meaningful function & variable names.

## Apploader Loader
Loads Apploader.img files into IDA.

### Features
* Creates boot & trailer .text sections.
* Sets the entrypoint function name.

### Limitations
Since the apploader is a raw image (.text, .rodata, .data, .bss, etc), everything is set as text (code). This means that IDA will likely find false code positives during analysis. You'll have to fix those manually.

## Planned (TODOs)
* Support symbol loading for externals & DOLs.
* Make imports appear in the imports tab.
* Allow some settings such as relocating to any base (?).
