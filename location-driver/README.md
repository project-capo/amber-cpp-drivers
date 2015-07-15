Create location project in Eclipse IDE 
=================

* Clone repository.
* Open Eclipse.
* Select a workspace.
* New -> Project -> C++ Project 
* Uncheck option `Use default location` and browse directory `amber-cpp-driver\location-driver`
* Project name: for example: `location`
* Choose Project type as `Empty Project` and Toolchains as `Cross GCC`
* Click Next 
* Click Next
* Click Finish
* Go to project properties
* Choose C/C++ Build -> Settings -> GCC C++ Compiler -> Optimization -> Other optimization flag -> Insert:

`-g -static -pedantic -W -Wall -Wextra -Wshadow -Wformat -Winit-self -Wunused -Wfloat-equal -Wcast-qual -Wwrite-strings -Winline -Wstack-protector -Wunsafe-loop-optimizations -Wlogical-op -Wmissing-include-dirs -Wconversion -Wmissing-declarations -Wno-long-long `

* Choose C/C++ Build -> Settings -> GCC C++ Linker -> Libraries -> Libraries (-l) -> Add

`protobuf`
`boost_system`
`rt`
`pthread`
`boost_thread`
`log4cxx`
`boost_program_options`

* `Semantic Errors` uncheck flage
Window -> Preferences -> C/C++ -> Code Analysis -> Syntax and Semantic Errors: OFF
* Ready to compile
