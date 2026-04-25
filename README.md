# Checkers Game – COP 3003 Project  

**Build Instructions:**
- This project uses SFML 3.0.2 64 bit
- Clone the repository into visual studio
- Add the appropriate SFML folder (include and lib respectively) to your Additional Include directory and your Additional Library directory
- More specifically:
- Right click the project file and click Properties. Ensure that at the top of the screen, "Configuration: " is set to All Configurations. Then, navigate to C/C++ -> General -> Additional Include Directories. In Additional Include Directories, enter the drop down menu, click <Edit...>. Then click the folder icon. Find where your SFML is downloaded to, and inside of it find and select the "include" folder. Next, navigate to Linker -> General -> Additional Library Directories. Click the folder icon, find go to your SFML folder again, and find and select the "lib" folder. Click "Apply" and close the window.
- Set the configuration to "Debug" on the top of the window.
- You can either press the "Start Without Debugging" button or right click the project file and click "Build"
- If you build from right clicking on the project file, navigate (from the .vs folder) to x64/Debug and click the .exe file

**Contributions:** 
- Elijah Rose made the bulk of the README, the PowerPoint, assets for the game and organized the files.
- Estiward Casado Antigua made the UML diagram, the Game class, the GUI, and also got everything to compile/build properly.
- Colin Pearson made the initial class structure, the logic for all the Piece classes, and the Board classes.

**Ai Usage:**
- Was used to help with debugging problems when we couldn't solve it.
- Helped make template code that we learned from and wrote our own
- It also helped us understand the SFML stuff as well as helped us learn the GUI

**Team Members:**  
- Elijah Rose  
- Estiward Casado Antigua  
- Colin Pearson  
- Leo Bustamante  

**Course:** COP 3003  
**Date:** 3/27/2026 

## Project Description & Goals
This project implements a fully interactive Checkers game that allows two players to compete on a standard 8x8 board using a graphical interface. The goal is to provide a convenient, computer‑based way to play Checkers while demonstrating Object‑Oriented Programming principles, exception handling, and event‑driven GUI programming.

The game enforces standard Checkers rules, supports piece movement, captures, king promotion, and includes visual feedback through a GUI built with SFML.

## Features & Priorities

### **Core Gameplay**
| Feature | Priority |
|--------|----------|
| Two‑player local multiplayer | H |
| Standard 8x8 board | H |
| Turn‑based movement | H |
| Legal move validation | H |
| Regular pieces move forward | H |
| King pieces move both directions | H |
| Multi‑jump / chain captures | H |
| King promotion | H |
| Mandatory capture rule | M |

### **Game Management**
| Feature | Priority |
|--------|----------|
| Win detection | H |
| Draw detection | L |
| New Game / Reset button | M |
| End‑of‑game message | L |
| Move history panel | L |
| Piece count display | L |

### **Other Features**
| Feature | Priority |
|--------|----------|
| Error messages for invalid moves | L |
| Highlight selected piece & valid moves | M |

##  Resources & Tools
- C++  
- SFML (Simple and Fast Multimedia Library)  
- GitHub for version control  
- Standard C++ libraries (`iostream`, `vector`, `stdexcept`)  

## Class Design

### **Piece (Abstract Class)**
**Variables**
- `bool isPlayer1`
- `Color color` (RED or BLACK)
- `int row, col`

**Functions**
- `virtual move() = 0`
- `virtual getValidMoves() = 0`

**Relationships**
- `RegularPiece` **is‑a** `Piece`  
- `KingPiece` **is‑a** `Piece`  
- `Board` **has‑a** `Piece`

### **RegularPiece**
**Variables**
- Inherits from `Piece`

**Functions**
- `move()` — forward diagonal movement  
- `promote()` — converts to `KingPiece`

**Relationships**
- Is‑a `Piece`  
- Board has RegularPieces  

### **KingPiece**
**Variables**
- Inherits from `Piece`

**Functions**
- `move()` — all diagonal directions  
- `getValidMoves()` — includes backward captures  

**Relationships**
- Is‑a `Piece`  
- Board has KingPieces  

### **Board**
**Variables**
- `Piece* grid[8][8]`
- `int redCount, blackCount`

**Functions**
- `initBoard()`  
- `isValidMove()`  
- `applyMove()`  
- `checkForCaptures()`  
- `displayBoard()`  

**Relationships**
- Game **has‑a** Board  
- Board **has** RegularPieces and KingPieces  

### **Game**
**Variables**
- `Board board`
- `Player player1, player2`
- `int currentTurn`
- `vector<Move> moveHistory`

**Functions**
- `startGame()`  
- `switchTurn()`  
- `checkWinCondition()`  
- `resetGame()`  
- `logMove()`  

**Relationships**
- Game **has‑a** Board  

## OOP Concepts Used

### **Encapsulation**
- Private data members accessed through public methods.

### **Inheritance**
- `Piece` → `RegularPiece`  
- `Piece` → `KingPiece`

### **Polymorphism**
- `move()` and `getValidMoves()` overridden in derived classes.  
- Base‑class pointers call derived behavior.

### **Composition**
- Game **has‑a** Board  
- Board **has‑a** 2D array of Piece pointers  
- Game **has‑a** Player array  

### **Abstract Classes**
- `Piece` contains pure virtual functions, making it abstract.

## GUI Concept
The GUI will include:

- An 8x8 checkerboard with alternating colored squares  
- Clickable pieces with highlighted valid moves  
- Player info panel (names, colors, turn indicator, piece count)  
- Move history sidebar  
- New Game and Quit buttons  
- Forfeit button  
- Win/Loss popup dialog  

##  Edge Cases & Error Handling
Handled using `try/catch`:

- Selecting an empty square  
- Moving to an invalid or occupied square  
- Skipping a mandatory capture  
- Out‑of‑bounds board access  

**Example:**
From Game::executeMove(const Move& move)
```cpp
try {
	m_board.applyMove(move);
}
catch (const std::exception& e) {
	m_statusMsg = std::string("Error: ") + e.what();
	return;
}
