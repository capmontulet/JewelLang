# Jewel Language
This repository contains a C based compiler that supports the co-authored Jewel Language.
Jewel Language implements some features not found in C, such as String values, double values, print statments, etc.

# Lexeme Classes
The lexeme classes help to outline the elements that make up the language. These classes are used in the state diagram.
![LexemeClasses](https://github.com/user-attachments/assets/f8c6cad8-0edd-4b21-a69a-84017174fd08)

# State Diagram
Here the lexeme classes are used to create a deterministic state diagram, which is then used to create a transition table.

![JewelChart](https://github.com/user-attachments/assets/530f4776-0fb1-4ff5-a7a8-29ea7f455cc3)

# Transition Table
This table renders the state diagram into an easier to code format. Uses both lexeme classes and state diagram states to build the state automata.

![transitionTable](https://github.com/user-attachments/assets/6d384e0b-0b1d-4d2e-9a1d-30440ed85534)



# Instructions

Add the scripts to the specified directory, then target them in the launch.vs.json like so:
( some build path changes may be needed )
![instructions](https://github.com/user-attachments/assets/70177e93-f9bd-45e9-b58c-55a54710169f)



