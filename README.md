# 🚀 Virex Programming Language 🌟

Welcome to **Virex**—a lightweight, beginner-friendly programming language designed for simplicity and power! Inspired by **JavaScript** and **C++**, Virex makes coding intuitive without sacrificing robust features.

---

## ✨ Features
✅ **Explicit Typing**: Easily declare variable types for clarity.  
✅ **Dynamic Objects**: Use `let` or `var` for type inference when needed.  
✅ **Constants**: Keep values immutable with `const`.  
✅ **Functions**: Clean, readable syntax with the `function` keyword.  
✅ **Classes**: Enjoy object-oriented programming support.  
✅ **Modular Imports**: Keep your code clean with an `import` system.  

---

## 🧠 Virex Syntax Cheat Sheet

### 🔢 Variables
```Virex
int x = 10;         // Integer variable
str y = "Hello";    // String variable
bool cool = true;   // Boolean variable
let z = 42;         // Generic variable (type inferred)
const pi = 3.1415;  // Immutable constant
```

### 🎉 Functions
```Virex
function greet(str name) {
    print("Hello, " + name);
}
```

### 🛤️ Conditionals
```Virex
if (x > 5) {
    print("x is greater than 5");
} else {
    print("x is 5 or less");
}
```

### 🔁 Loops
```Virex
for (int i = 0; i < 10; i++) {
    print(i);
}
```
```Virex
while (x > 0) {
    x = x - 1;
    print(x);
}
```

### 🧍 Objects
```Virex
let person = {
    name: "DaPSL",         // string
    age: 14,                // int
    hobbies: ["coding", "gaming"] // array of strings
};
print(person.name);
```

### 🏛️ Classes
```Virex
class Animal {
    str name;

    function init(str name) {
        this.name = name;
    }

    function speak() {
        print(this.name + " makes a sound");
    }
}
```

## 🛠️ Built-in Libraries
### 📚 Standard Libraries Included
Virex has built-in tools for everyday coding tasks:
- **Math**: Handy constants like Math.PI and functions like Math.sqrt(x).
- **Strings**: Utilities like .length(), .toUpperCase(), and .toLowerCase().
- **Basic I/O**: Use print() and input() for quick input/output tasks.

### Example:
```Virex
let radius = 5;
let area = Math.PI * (radius * radius); // Built-in Math library
print("Area: " + area);
```

## 📦 Importing Modules
Take it further with external or custom modules:
```Virex
import "utils.vix"; // importing user-defined moduled

function main() {
    let result = utils.add(5, 10); // using a function from the imported module
    print(result);
}
```


## 🚀 Getting Started
1️⃣ Clone the Repository
```bash
git clone https://github.com/IAmThePSL/Virex-programming-lang
cd Virex
```
2️⃣ Build the Project
```bash

```
3️⃣ Run Virex Programs
```bash
./Virex examples/hello.vix
```

## 🤝 Contribution
We ❤️ contributors!
Found a bug? Have a cool feature idea? Feel free to open an issue or submit a pull request. Let’s make Virex even better together! ✨

## 📜 License
This project is licensed under the [MIT License](https://github.com/IAmThePSL/Virex-programming-lang/blob/main/LICENSE)

Power up your coding journey with Virex! ⚡️
---

**Why the name "Virex"?**
    The name "Virex" is a combination of the words "virtual" and "syntax". It reflects the idea of a programming language that blends virtual environments, with clear and structured syntax.


> [!NOTE]
> **Virex is currently under active development. Some features listed in this document may not yet be available. Thank you for your patience and support!**