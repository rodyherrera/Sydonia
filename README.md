### Sydonia Template Engine
Sydonia is a template engine which is inspired by [Jinja](https://github.com/pallets/jinja/), Sydonia is a recreation of the [Inja](https://github.com/pantor/inja) engine, it is possible that Sydonia contains other implementations, originally the creation of this software was learn how this engine or template language is made, change the style of the code and be able to use it in other projects, Sydonia to work in your projects the [nlohmann json library](https://github.com/nlohmann/json), this library is used for data entry, this template language is quite intuitive and easy to use, it will not be a problem to learn how this works, this software has a file called Setup.py which will allow you to execute various operations that affect the source code with only one line of code, for example generate a header-only file from the library, it is already ready, but it is likely that you will want to update it after a modification in the source code of Sydonia, then you will be explained more about said file.

#### Installing
To install this library so that you can start using it in your projects, you just have to clone the repository, the respective commands will be shown below.
```bash
# Cloning the repository...
git clone https://github.com/rodyherrera/Sydonia/
# Accessing the repository
cd Sydonia
```
After executing the previous commands, you will already be inside the Sydonia source code, I invite you now to take a good look at the README so that you can understand more about this library.

#### The setup file
Before we start, let's take a look at the Setup file, as in other of my installments, most of the time you will come across a file called Setup, which has features that you can execute with a command that will save you a couple of time.
```bash
# <In the root directory>
# Generate or update the library's header-only file.
python3 Setup.py DoSingleFile
```

#### Integration
The way in which you should integrate Sydonia to your projects is quite easy, this is because you only have to move a file which contains all the source code, this file can also be known as 'header-only', this file is called Sydonia .hxx and is located at the root of the library source code, you just have to move it to your project and do the respective include to start using Sydonia.
```c++
// Including the Header-Only file
#include "Sydonia.hxx"

// For convenience you can use the Sydonia namespace, so if you don't want to call a library method every time you don't have to declare the namespace in each statement.
using namespace Sydonia;
```
#### Basic example
```c++
#include <iostream>
// Importing the header-only library located at the root of the source code.
#include "Sydonia.hxx"

int main(){
    // Creating a JSON instance of the nlohmann library, it will be used for data entry to later use in our mini template.
    Sydonia::JSON Context;
    // ! Setting a key defined as 'Message' whose value will be 'Hello World!'.
    Context["Message"] = "Hello world!";
    // ! Rendering a template using a string, where with {{Message}} we refer to the fact that we want to obtain the previously defined message.
    std::cout << Sydonia::Render("-> {{Message}}!\n", Context);
    // Another way to render using a stream, which is indicated in the first parameter.
    Sydonia::RenderTo(std::cout, "-> {{Message}}\n", Context);
    return 0;
}
```

### Using environments
For a more advanced use, it is highly recommended that you use the environments, you can have all the environments you want in your project.
```c++
Sydonia::JSON Context;
Sydonia::Environment Environment;

Context["Name"] = "Hello world";
 
// Render a string with json data
std::string Result = Environment.Render("Hello {{Name}}!", Context); // "Hello world!"
 
// Or directly read a template file
Sydonia::Template Template = Environment.ParseTemplate("./Templates/Greeting.txt");
std::string Result = Environment.Render(Template, Context); // "Hello world!"
 
Context["Name"] = "Sydonia";
std::string Result = Environment.Render(Template, Context); // "Hello Inja!"
 
// Or read the template file (and/or the json file) directly from the environment
Result = Environment.RenderFile("./Templates/Greeting.txt", Context);
Result = Environment.RenderFileWithJsonFile("./Templates/Greeting.txt", "./Data.json");
 
// Or write a rendered template file
Environment.Write(Template, Context, "./Result.txt");
Environment.WriteWithJsonFile("./Templates/Greeting.txt", "./Data.json", "./Result.txt");
```

The environments can change their default settings, you can adjust according to your needs

```c++
// With default settings
Sydonia::Environment Environment;
 
// With global path to template files and where files will be saved
Sydonia::Environment PrimaryEnvironment {"../Path/Templates/"};
 
// With separate input and output path
Sydonia::Environment SecondaryEnvironment {"../Path/Templates/", "../Path/Results/"};
 
// With other opening and closing strings (here the defaults)
PrimaryEnvironment.SetExpression("{{", "}}"); // Expressions
PrimaryEnvironment.SetComment("{#", "#}"); // Comments
PrimaryEnvironment.SetStatement("{%", "%}"); // Statements {% %} for many things, see below
PrimaryEnvironment.SetLineStatement("##"); // Line statements ## (just an opener)

```
A look at variables and objects
```c++
Sydonia::JSON Context;
Context["Neighbour"] = "Peter";
Context["Guests"] = {"Rodolfo", "John", "Elon"};
Context["Time"]["Start"] = 16;
Context["Time"]["End"] = 22;
 
// Indexing in array <Array.Index>
Sydonia::Render("{{ Guest.1 }}", Context); // "Tom"
 
// Objects
Sydonia::Render("{{Time.Start}} to {{Time.End + 1}} pm", Context); // "16 to 23 pm"

```
Declarations can be written with the {% ... %} syntax or the ## syntax for full lines. Note that ## must start the line without indentation. The most important declarations are loops, conditions, and included files. All statements can be nested.
```c++
// Combining loops and line statements
Sydonia::Render(R"(
Guest List:
## For Guest In Guests
    {{ Loop.Index1 }}: {{ Guest }}
## endfor
)", Context)
 
/* Guest List:
    1: Jeff
    2: Tom
    3: Patrick */
```

Conditions support the typical 'If', 'Else If' and 'Else' statements. Following conditions are for example possible: 

```c++
// Standard comparisons with a variable
Sydonia::Render("{% If Time.Hour >= 20 %}Serve{% Else If Time.Hour >= 18 %}Make{% EndIf %} dinner.", Context); // Serve dinner.
 
// Variable in list
Sydonia::Render("{% If Neighbour In Guests %}Turn up the music!{% EndIf %}", Context); // Turn up the music!
 
// Logical operations
Sydonia::Render("{% If GuestCount < (3+2) And AllTired %}Sleepy...{% Else %}Keep going...{% EndIf %}", Context); // Sleepy...
 
// Negations
Sydonia::Render("{% If Not GuestCount %}The End{% EndIf %}", Context); // The End

```
You can either include other in-memory templates or from the file system, this is quite useful no matter what you're developing, because it's also a good way to reuse code, especially when developing web pages.
```c++
// To include in-memory templates, add them to the environment first
Sydonia::Template ContentTemplate = DefaultEnvironment.Parse("Hello {{ Neighbour }}!");
DefaultEnvironment.IncludeTemplate("Content", ContentTemplate);
DefaultEnvironment.Render("Content: {% Include \"Content\" %}", Context); // "Content: Hello Peter!"
 
// Other template files are included relative from the current file location
Sydonia::Render("{% Include \"Footer.html\" %}", Context);

```
#### Assignments

Variables can also be defined within the template using the set statment, that is, you can define variables from the template and not just from C++ using the Context.

```c++
Sydonia::Render("{% Set NewHour=23 %}{{ NewHour }} pm", Context); // "23 pm"
render("{% Set Time.Start=18 %}{{ Time.Start }} pm", Context); // using json pointers

```
#### Functions
There are functions that you can use within the templates, you can also create your own functions, then it will be explained to you, then the default functions that Sydonia offers you will be shown.

```c++
// Upper and lower function, for string cases
Sydonia::Render("Hello {{ Upper(Neighbour) }}!", Context); // "Hello PETER!"
Sydonia::Render("Hello {{ Lower(Neighbour) }}!", Context); // "Hello peter!"
 
// Range function, useful for loops
Sydonia::Render("{% For Iterator In Range(4) %}{{ Loop.Index1 }}{% EndFor %}", Context); // "1234"
Sydonia::Render("{% For Iterator In Range(3) %}{{ At(Guests, Iterator) }} {% EndFor %}", Context); // "Jeff Tom Patrick "
 
// Length function (please don't combine with range, use list directly...)
Sydonia::Render("I count {{ Length(Guests) }} guests.", Context); // "I count 3 guests."
 
// Get first and last element in a list
Sydonia::Render("{{ First(Guests) }} was first.", Context); // "Jeff was first."
Sydonia::Render("{{ Last(Guests) }} was last.", Context); // "Patir was last."
 
// Sort a list
Sydonia::Render("{{ Sort([3,2,1]) }}", Context); // "[1,2,3]"
Sydonia::Render("{{ Sort(Guests) }}", Context); // "[\"Jeff\", \"Patrick\", \"Tom\"]"
 
// Join a list with a separator
Sydonia::Render("{{ Join([1,2,3], \" + \") }}", Context); // "1 + 2 + 3"
Sydonia::Render("{{ Join(Guests, \", \") }}", Context); // "Jeff, Patrick, Tom"
 
// Round numbers to a given precision
Sydonia::Render("{{ Round(3.1415, 0) }}", Context); // 3
Sydonia::Render("{{ Round(3.1415, 3) }}", Context); // 3.142
 
// Check if a value is odd, even or divisible by a number
Sydonia::Render("{{ Odd(42) }}", Context); // false
Sydonia::Render("{{ Even(42) }}", Context); // true
Sydonia::Render("{{ DivisibleBy(42, 7) }}", Context); // true
 
// Maximum and minimum values from a list
Sydonia::Render("{{ Max([1, 2, 3]) }}", Context); // 3
Sydonia::Render("{{ Min([-2.4, -1.2, 4.5]) }}", Context); // -2.4
 
// Convert strings to numbers
Sydonia::Render("{{ Int(\"2\") == 2 }}", Context); // true
Sydonia::Render("{{ Float(\"1.8\") > 2 }}", Context); // false
 
// Set default values if variables are not defined
Sydonia::Render("Hello {{ Default(Neighbour, \"my friend\") }}!", Context); // "Hello Peter!"
Sydonia::Render("Hello {{ Default(Colleague, \"my friend\") }}!", Context); // "Hello my friend!"
 
// Access an objects value dynamically
Sydonia::Render("{{ At(Time, \"Start\") }} to {{ Time.End }}", Context); // "16 to 22"
 
// Check if a key exists in an object
Sydonia::Render("{{ Exists(\"Guests\") }}", Context); // "true"
Sydonia::Render("{{ Exists(\"City\") }}", Context); // "false"
Sydonia::Render("{{ ExistsIn(Time, \"Start\") }}", Context); // "true"
Sydonia::Render("{{ ExistsIn(Time, Neighbour) }}", Context); // "false"
 
// Check if a key is a specific type
Sydonia::Render("{{ IsString(Neighbour) }}", Context); // "true"
Sydonia::Render("{{ IsArray(Guests) }}", Context); // "true"
// Implemented type checks: IsArray, IsBoolean, IsFloat, IsInteger, IsNumber, IsObject, IsString,
```

You can create your own and more complex functions with callbacks. These are implemented with std::function, so you can for example use C++ lambdas. Sydonia Arguments are a vector of json pointers. 

```c++
Sydonia::Environment Environment;
/*
 * Callbacks are defined by its:
 * - name,
 * - (optional) number of arguments,
 * - callback function.
 */
Environment.AddCallback("Double", 1, [](Sydonia::Arguments& Arguments){
    int Number = Arguments.at(0)->get<int>(); // Adapt the index and type of the argument
    return 2 * Number;
});
 
// You can then use a callback like a regular function
Environment.Render("{{ Double(16) }}", Context); // "32"
```

#### Template Inheritance
Template inheritance allows you to build a base skeleton template that contains all the common elements and defines blocks that child templates can override, this functionality can be quite useful for example when developing web pages, since they will be using components and reulting a lot of code, by using blocks. Lets show an example: The base template 
```jinja
<!DOCTYPE html>
<html>
<head>
  {% Block Head %}
  <title>{% Block Title %}{% EndBlock %} - My Webpage</title>
  {% EndBlock %}
</head>
<body>
  {% Block Content %}{% EndBlock %}
</body>
</html>
```

contains three blocks that child templates can fill in. The child template 
```jinja
{% Extends "Base.html" %}
{% Block Title %}Index{% EndBlock %}
{% Block Head %}
  {{ Super() }}
  <style type='text/css'>
    body{
        background-color: #FFFFFF;
    }
  </style>
{% EndBlock %}
{% Block Content %}
  <h1>Index</h1>
{% EndBlock %}
```
calls a parent template with the extends keyword; it should be the first element in the template. It is possible to render the contents of the parent block by calling super(). In the case of multiple levels of {% extends %}, super references may be called with an argument (e.g. super(2)) to skip levels in the inheritance tree.

#### Whitespace control
In the default configuration, no whitespace is removed while rendering the file. To support a more readable template style, you can configure the environment to control whitespaces before and after a statement automatically. While enabling SetTrimBlocks removes the first newline after a statement, SetLstripBlocks strips tabs and spaces from the beginning of a line to the start of a block.
```c++
Sydonia::Environment Environment;
Environment.SetTrimBlocks(true);
Environment.SetLstripBlocks(true);
```
With both TrimBlocks and LstripBlocks enabled, you can put statements on their own lines. Furthermore, you can also strip whitespaces for both statements and expressions by hand. If you add a minus sign (-) to the start or end, the whitespaces before or after that block will be removed:
```c++
Sydonia::Render("Hello       {{- Name -}}     !", Context); // "Hello Inja!"
Sydonia::Render("{% If Neighbour In Guests -%}   I was there{% EndIf -%}   !", Context); // Renders without any whitespaces
```
#### Comments
Comments are quite important when writing code, because it is the way you can explain how something works, either so that other people can understand the code you have written or for you to remember it in the future, comments are written using the syntax '{# Hello, I'm a comment #}'.
```c++
Sydonia::Render("Hello {# I'm a comment #}!", Context); // "Hello!"
```

#### Contributions
Contributions are accepted, this software is under the MIT license, it is likely that there will be future updates to this project, as I mentioned before Sydonia is a replica of [Inja](https://github.com/pantor/inja), which is an engine for C++, if you want to make contributions I recommend that you do so to that repository, anyway the source code that is in this repository you can use it as you want, experiment and learn!

### Remember drink water baby<3
