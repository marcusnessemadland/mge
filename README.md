[mge](https://github.com/marcusnessemadland/mge) - Modern Graphics Engine
============================================================================

<p align="center">
    <a href="#what-is-it">What is it?</a> -
    <a href="#building">Building</a> -
    <a href="#license-apache-2">License</a>
</p>

[![License](https://img.shields.io/github/license/marcusnessemadland/mge)](https://github.com/marcusnessemadland/mge/blob/main/LICENSE)

[What is it?](https://github.com/marcusnessemadland/mge)
-------------------------------------------------------------

Experimental real-time rendering engine designed for graphics research and development. It is designed to aid in prototype development by providing simple and easy-to-use abstractions and frameworks to improve productivity of real-time research projects. It runs on [sdl](https://github.com/libsdl-org/SDL) for platform api abstraction and [bgfx](https://github.com/bkaradzic/bgfx/) for graphics api abstraction giving support for a wide range of platforms and rendering backends.

Engine Features:
* Integrated Abstraction Frameworks
* Object Oriented and Object Component Architecture
* Clean API connecting Graphics, Logic and Data
* 3D Math Library
* Live Edit Scenes with [Maya Bridge](https://github.com/marcusnessemadland/maya-bridge) integration

Graphics Features:
* Deferred pipeline (Geometry Buffer)
* Tone Mapping

[Building](https://github.com/marcusnessemadland/mge)
-------------------------------------------------------------

Submodules are used to include any essential dependencies.

When acquiring the code make sure to clone the repo with submodules included:

```bash
git clone --recurse-submodules https://github.com/marcusnessemadland/mge.git
```

If you have already cloned this repo without submodules, then use:

```bash
git submodule update --init --recursive
```

Then CMake is used for building IDE project solutions:

```bash
cd mge
mkdir build
cd build
cmake ..
```

[License (Apache 2)](https://github.com/marcusnessemadland/mge/blob/main/LICENSE)
-----------------------------------------------------------------------

<a href="http://opensource.org/license/apache-2-0" target="_blank">
<img align="right" src="https://opensource.org/wp-content/uploads/2022/10/osi-badge-dark.svg" width="100" height="137">
</a>

	Copyright 2025 Marcus Nesse Madland
    
	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
