# 🐦 Flinchy Finch: A Flappy Bird Clone with NEAT AI 🧬



<p align="center">
<img src="assets/ai-mode-demo.gif" alt="AI Mode Demo"/>
</p>

_First few generations in HARD mode_


## 🤔 What's in a Name? "Flinchy Finch"

The name "Flinchy Finch" is a playful tribute to the original "Flappy Bird" game, incorporating a nod to evolutionary science.
* **"Flinchy"** mirrors the "Flappy" part of the original title, reflecting the bird's distinct, quick movements in the game.
* **"Finch"** replaces "Bird", deliberately referencing Darwin's finches. These birds were crucial to Charles Darwin's development of the theory of evolution by natural selection.

This connection is central to the project's core AI mechanism: **NEAT (NeuroEvolution of Augmenting Topologies)**. NEAT is a genetic algorithm designed to *evolve* artificial neural networks, mimicking natural evolution for the AI to "learn" and improve over generations. Therefore, the "Flinchy Finch" isn't merely flinching its way through obstacles; it's a symbol of the project's evolutionary AI at work, constantly adapting and improving, much like Darwin's finches in nature.



## 📝 Description

### 📄 Overview
This project is a C++ implementation of a game similar to Flappy Bird, created as part of a university course on C++ Object-Oriented Programming (OOP). Beyond the game itself, it features an Artificial Intelligence (AI) mode where agents learn to navigate the obstacles using the NEAT algorithm. It showcases OOP principles through its class structure and provides both manual gameplay 🕹️ and a demonstration of evolutionary algorithms.

### 🧩 NEAT Implementation Philosophy
The development of the NEAT AI component was guided strictly by the principles outlined in the original ['NeuroEvolution of Augmenting Topologies'](https://nn.cs.utexas.edu/downloads/papers/stanley.ec02.pdf) paper by Stanley and Miikkulainen. To ensure a foundational understanding directly from the source material, external implementations were intentionally avoided, with only minimal occasional reference. For further clarity on the paper's theoretical underpinnings, resources explaining its concepts were consulted, focusing on the theory rather than specific implementation techniques.



## ♻️ Reusable NEAT Framework

The components implementing the NEAT algorithm (primarily the `Genome`, `Node`, and `Synapse` classes, along with the logic in `AIManager` for speciation, crossover, and mutation) were designed with modularity in mind. This NEAT implementation can potentially be extracted and reused as a framework in other projects requiring evolutionary algorithms or neuroevolution.



## ✨ Features

* **Gameplay:** Classic Flappy Bird mechanics where the player controls a bird, avoiding pipes.
* **Modes:**
    * **Manual Mode:** Player controls the bird directly using the spacebar.
    * **AI Mode:** Multiple AI agents learn to play simultaneously using NEAT.
* **NEAT Implementation (The 🧠 behind the AI):**
    * **Genomes:** Neural networks represented by genomes consisting of nodes (neurons) and connections (synapses).
    * **Evolution 📈:** Genomes evolve over generations through:
        * **Crossover:** Combining genomes of successful parents.
        * **Mutation:** Modifying genomes by adding nodes, adding connections, perturbing weights, or assigning random weights.
    * **Speciation 🛡️:** Grouping similar genomes into species to protect novel mutations and maintain diversity. Fitness is adjusted based on species size.
* **Game Mechanics:**
    * **Difficulty:** Easy and Hard modes, affecting the rate at which game speed increases.
    * **Physics:** Simple gravity and jump mechanics.
    * **Collision Detection:** Detects collisions with pipes and boundaries.
    * **Scoring:** Increments score as pipes are passed.
    * **Animation:** Basic bird flapping animation.



## 🔧 Dependencies

* **SFML (Simple and Fast Multimedia Library):** Used for graphics, windowing, events, and textures.
* **Eigen:** A C++ template library for linear algebra, used within the `Node` class.
* **Google Test:** Used for the unit tests.



## 📁 Project Structure

* `include/`: Contains header files (.h) defining classes, enums, and structures.
* `src/`: Contains source files (.cpp) implementing the logic defined in the headers.
* `tests/`: Contains unit tests (`genome_test.cpp`), primarily for the `Genome` class functionality.
* `assets/`: Contains game assets like textures (`.png`) and fonts (`.ttf`) + AI Mode demo.



## 🚀 Building and Running

1.  **Install Dependencies:** Ensure you have SFML and Eigen libraries installed and configured for your development environment.
2.  **Compile:** Compile the C++ source files located in the `src/` directory (e.g., `main.cpp`, `game.cpp`, `player.cpp`, `AI_player.cpp`, `genome.cpp`, etc.). You will need to link against the necessary SFML libraries (graphics, window, system).
3.  **Run:** Execute the compiled binary.

*Note: Specific compilation commands depend on your compiler (like g++) and operating system.*



## ✅ Testing

1.  **Install Google Test:** Ensure the Google Test framework is installed.
2.  **Compile Tests:** Compile the test file (`tests.txt` code) along with the necessary source files it depends on (e.g., `genome.cpp`, `node.cpp`) and link against the Google Test library.
3.  **Run Tests:** Execute the compiled test binary.



## 💡 TODO / Future Improvements

Based on comments in the source code:

- [ ] Display additional information during AI mode: number of species, count of living agents, current generation number, and maximum fitness score.
- [ ] Implement game menus (e.g., Main Menu, Restart Menu).
- [ ] Create a central configuration file for managing settings like screen dimensions, game speeds, and NEAT parameters.