# hdlc-parsing

## Quick start

This repo relies on submodules.  To checkout

```
git clone --recurse-submodules https://github.com/LucasRxTx/hdlc-parsing
```

If you did not checkout with recurse submodules, you can still get the dependencies with

```
git submodule init
git submodule update
```


## Task description

You are tasked with programming a simple game terminal that keeps track of a character on a gameboard. The game board is a 5-by-5 grid, where the character can move freely left, right, up and down. The direction to move the character is coded as integers indicating the direction (1: up, 2: down, 3: right, 4: left).
 
The directions are wrapped in HDLC frames. The instructions could have been sent over a serial port, but in this task the instructions are pre-recorded and found in transmission.bin.

Read the instructions and move the characters accordingly. Your program only has to display the final character position.


### Rules:

The character starts at (0,4), which is the bottom-left corner of the board.

Leaving the board is an illegal move (even temporarily)

If the same instruction occurs three times in a row, all three instructions should be discarded.

The solution should be written in Python or Rust with focus on readability and memory footprint.

We will recommend using yahdlc for this task. If you need third-party libraries, feel free to do so, but you have to argue for the choice.

yahdlc can be found on GitHub: https://github.com/bang-olufsen/yahdlc

HDLCis normally a request-response protocol, but transmission.bin only contains the responses.

Imagine your terminal would query for the next move over and over. You will simply see the responses from those requests. You can assume that packages are not out of order and that only one request is sent at once.
