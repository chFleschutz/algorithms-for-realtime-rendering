# Algorithms for Realtime Rendering

This repository contains code examples and exercises discussed in the lecture "algorithms for realtime rendering" to help understand and implement various algorithms used in real-time rendering applications.

## Overview

This repository showcases a collection of algorithms used in computer graphics. 

It is built upon the the SORS Framework, developed by Prof. Dr. Dreier. The SORS Framework serves as a robust starting point for implementing advanced rendering techniques, while also providing essential and easy-to-use functionality for basic rendering tasks. It uses OpenGL as the graphics API and Qt for creating windows and other useful functionality.

## Featured Topics

The repository implements the following techniques:

- Bumpmap / Heightmap
- Parallax Occlusion Mapping
- Deferred Shading
- Screen Space Motion Blur
- Screen Space Ambient Occlusion
- Shadowmapping
- Billboards using Geometry Shader
- Dynamic LOD using Tessellation Shader

## Repository Structure

The contents of the repository is structured as follows:

- **ARTR_Exercises/**: Contains code for advanced realtime rendering techniques.
- **CGR_Exercises/**: Contains code for basic computer graphics concepts.
- **examples/**: Contains example code for various applications.
- **SORS/**: Contains the SORS-Framework used for rendering.

## Getting Started

To get started follow these steps:

1. Make sure [Qt](https://www.qt.io/download) is installed on your System (recommended version: 6.5) and added to the `PATH` environment variable (on windows)

2. Clone this repository to your machine:
   ```bash
   git clone https://github.com/chFleschutz/algorithms-for-realtime-rendering.git
   ```

3. Open the folder in a CMake supported IDE (like Visual Studio) or generate the project files

4. Select the project to execute and build
