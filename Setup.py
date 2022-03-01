# ***
# * Copyright (C) Rodolfo Herrera Hernandez. All rights reserved.
# * Licensed under the MIT license. See LICENSE file in the project root 
# * for full license information.
# *
# * =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
# *
# * For related information - https://github.com/CodeWithRodi/Sydonia/
# *
# ****/

from os import system
import sys, platform, time

k = {
    'OperativeSystem': platform.system(),
    'CallArguments': sys.argv[1:]
}

Configuration = {
    'Source': {
        'Folder': 'Source/',
        'Main': 'Sydonia.hxx'
    },
    'SingleFileOutput': 'Sydonia.hxx'
}

def ClearScreen() -> None:
    system('cls' if k.get('OperativeSystem', 'Linux') == 'Windows' else 'clear')

def FinishScript() -> None:
    ClearScreen()
    print('\n\n: Sydonia => Remember drink water <3.\n\n')
    sys.exit()

try:
    import quom
except ImportError:
    ClearScreen()
    print(f'''\
: Sydonia<Setup> - Installing dependencies

:-: Next, the dependencies that this Setup needs for its correct 
:-: operation will be installed, this should not take much time 
:-: depending on the internet connection you have, after the installation 
:-: the script will be executed correctly automatically.

:-: Executing your request in the next 7 seconds...
''')
    time.sleep(7)
    ClearScreen()
    system('pip install --user quom')
    ClearScreen()

def DoSingleFile() -> None:
    print('''\
: Sydonia<Setup> - Generating A Single Header File

: = A single file file will help you to use libraries in 
: = a simple way, it will only be enough to include the file 
: = in your projects written in C++, this will not take you any 
: = time, then it is likely that you will be asked things, this 
: = to generate the file , this will be fast.
''')
    SingleFileName = Configuration['SingleFileOutput']
    system(f'quom {Configuration["Source"]["Folder"]}{Configuration["Source"]["Main"]} {SingleFileName}')
    SingleFileLines = sum(1 for Line in open(SingleFileName))
    print(f'''\
: Sydonia<Setup> - Generating A Single Header File

: = We believe that everything has gone as expected, a file 
: = with the name "{SingleFileName}" has been generated with a 
: = total of "{SingleFileLines}" lines of code.

: Following his own path...

: = Now that you have a single header file, you just have 
: = to integrate it into your C++ code, an example will be shown below.
: (
    #include <iostream>
    // ! Including the file that has been generated which 
    // ! contains the source code of the library.
    #include "Sydonia.hxx"
)
''')

ArgumentsCallback = {
    'dosinglefile': DoSingleFile
}

try:
    ClearScreen()
    if len(k.get('CallArguments', [])) == 0:
        print('''\
: Sydonia<Setup> - Invalid call
    
:-: You must enter an argument, read the README located 
:-: at the root of the source code.''')
        sys.exit()
    print(f'''\
: Sydonia<Setup> - Welcome water drinker :)

>>> [Website] https://codewithrodi.com/
>>> [Source Code] https://github.com/CodeWithRodi/Sydonia/
>>> [Email] contact@codewithrodi.com

:-: It has been detected that you are using <{k.get('OperativeSystem', 'Unknown')}>, you are executing 
:-: the Sydonia setup, you can read the 'README.md' located at the root 
:-: of the source code antomai to get more information about this 
:-: file, otherwise you can visit the Github repository.

:-: Executing your request in the next 7 seconds...
''')
    time.sleep(7)
    ClearScreen()
    for Argument in k.get('CallArguments', []) :
        if Argument in ArgumentsCallback:
            ArgumentsCallback[Argument.lower()]()
        else:
            print(f':-: Argument "{Argument}" is invalid.')
            sys.exit()
except KeyboardInterrupt:
    ClearScreen(), FinishScript()