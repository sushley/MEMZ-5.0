![MEMZ 5 0](https://user-images.githubusercontent.com/107402044/181738372-ed9c240b-db7e-496b-9e18-1b347eb83db1.png)

A trojan made for Danooct1's User Made Malware Series.

I am not responsible for any damage made using this trojan.

The build scripts and converters for the MBR payload are very noobish. You will need some things to run these:

 - Python 2.7 for Windows and [this](https://github.com/vishnubob/python-midi) library.
 - git for Windows or some other linux tools for windows like MinGW
 - NASM in your PATH (I just copyed the executable into C:\Windows\ because I was too lazy to add it to the PATH manually)
 - The [Nyan Cat midi file](https://drive.google.com/file/d/1erNasVvLOJ5PN1d5lrINnZ7hy8LtXHBq/view?usp=sharing)
 - Something to convert binary files into C arrays, to embed the MBR payload into the trojan (I used the export function of the HxD hex editor, you will only need that if you changed and recompiled the MBR payload)
 - qemu for Windows (additional, if you want to test the payload)

Even if you have all of this I don't think you get this to compile because my scripts suck, as I said earlier.
 
If you want to use the exe to batch converter, don't forget to append -zip to it's command line, because this gives way smaller file sizes and also works on Windows XP.

You'll also need Visual Studio 2015 to build the actual Trojan. Make sure you selected the Release configuration, because Debug is not configured properly.

The ASM code may look very noobish, but this is my first big thing written in assembly.


Credits and thanks

Dobby233Liu and Leurak

https://github.com/Leurak

https://github.com/Dobby233Liu

https://github.com/Dobby233Liu/VineMEMZ/blob/master

JLee79

https://chowdera.com/2021/04/20210430135939765k.html


Michael Petrotta

https://stackoverflow.com/questions/2798922/storage-location-of-yellow-blue-shield-icon


0xDEADBEEF

http://www.rohitab.com/discuss/topic/23191-screen-melter/



TheMaynMike
 
 https://github.com/TheMaynMike/ScreenGlitches
 
 https://github.com/TheMaynMike
 
WobbyChip

https://github.com/WobbyChip/Assembly

https://github.com/WobbyChip

Faliture 

https://github.com/Faliture

https://github.com/Faliture/BitBlt/blob/main/BitBlt/BitBlt.cpp


