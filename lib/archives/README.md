# Yttrius Documentations
___
**Support Server:** [Click Here](https://discord.gg/PbJQRT9zQ8)
**Available Download Types:** `TXT` `MD` `MDX*` `HTML` `MHTML` `RTXT*`

** - being worked on*
___
### Section 0 - Introduction
This is a curated list of information regarding Yttrius, the bot, and the framework(s).

**All Available Sections**<br>
> Section 0 - Introduction

> Section 1.0 - Documentation Informations

> Section 1.1 - Internal Structuring

> Section 1.2 - Yttrius Introduction

> Section 1.2.1 - Yttrius Information

> Section 1.3 - Yttrius Discord Bot Usages

> Section 1.4 - Commands Categories Explanation
___
### Section 1.0 - Documentational Informations

**Documentation Version:** `1.0 (LATEST-UNFINISHED)`

**Documentation Type:** `MARKDOWN`

**Bot Version:** `1.7 (STABLE-BRANCH@62bc834)`

**Assets Repo Version:** `3 (UNCHECKED)`

**License:** `Closed Source`

**Package Licensing:** `ISC | BSD-3 | MIT`

**Package Manager:** `snapd` `yum` `vcpkg` `conan` `cget`

**Documentation Last Revision:** `September-2-2021`
___
### Section 1.1 - Internal Structuring

**Language(s):** `en-US`

**Programming Language(s):** `C++` `JavaScript` `Elixir`

**Archival Language(s):** `JSON` `YAML` `XML` `TXT` `RTXT`

**Frameworks:** `sleepy-discord` `NodeJS` `Sugar`

**Operators / Compilation Handlers:** `NPM` `YARN` `CMAKE`

**Host Platform:** `Ubuntu-16.04LTS`
___

### Section 1.2 - Yttrius Introduction
Yttrius is written mainly in C++ using this library [here](https://github.com/yourWaifu/sleepy-discord). However, it also incorporates compatibility with JavaScript & TypeScript in order to allow more efficient packages from YARN & NPM to be installed.

___

### Section 1.2.1 - Yttrius Information
Yttrius itself is not the bot, no no. It is a framework I can use to make Discord Bots and other online automated services. However, in this case it is a Discord Bot.

**Bot Information**

User Friendliness Level (1 least 5 most): `3`

Response Time (1 Slowest, 10 Fastest): `7`

APIs Overtime (1 most, 5 least): `1`

Error Handling Depth (1 least 5 most): `4`

Self User Debugging? `y`

Unhandled Errors Expected (1 always, 3 never): `3`

Profanity Filtering Level (1 none 5 strictest): `5`

Auto-Logging? `y`

Data Collection (1 security, 5 all): `2`
___

### Section 1.3 - Yttrius Discord Bot Usages
All commands if input is required will specify when the command is read without necessary inputs. If no input is detected, a help menu of the command will be displayed along with the appropriate format for the command.

This bot requires the strictest of command formatting. If a necessary input is required and is not received within the user input, then the whole operation will cease. 

**Prefix Retrieval:** `@Yttrius`

**Prefix:** `$`

**Help Menu:** `$help`

**Command Help Seek** `$cmd`
___

### Section 1.4 - Commands Categories Explanation
This section will explain the different categories of topics of commands within the bot.

**Main**

<u>Main Commands List</u>

`bot` - Get information regarding the bot [Input NONE]

`cmd` - Get information on a specific command 
[Input commandNAME] [Format `$cmd commandNAME`]

`docs` - Gives you the list of downloadable documentations [Input type] [Format `$docs type`]

`help` - Help menu of listed commands [Input cat] [Format `$help cat`]

`invite` - Get invitation for this bot, so you can invite it into your own server(s) [Input NONE]

`support` - Get support [Input NONE]

`whatis` - Search Wikipedia [LIMITED]

**Astronomy** - Commands related to space, cosmology will be here.
<br>
<u>Astronomy Commands</u>

`iss` - Get information on the International Space Station [Inputs NONE]

`marsweather` - Get information on the weather on the planet Mars [Inputs NONE]

`apod` - NASA's picture of the day [Inputs NONE]

`moonphase` - Get the phase of the moon depending on the data [Input YYYY/MM/DD] [Format `$moonphase YYYY/MM/DD`]

`calcsun` - Get the sunrise and sunset using the longitude and latitude [Input longitude, latitude, time(optional)] [Format `$calcsun latitude longitude timeOPTIONAL `]
<br>
**Chemistry** - Commands related to the field of Chemistry & Bio-Chemistry

<u>Chemistry Commands</u>

`element` - Get information on an element within the known Periodic Table of Elements [Inputs element] [Format `$element elementNAME`]

`historyelement` - Get historical information on an element [Input's element] [Format `$historyelement element`]

`molar` - Get molar mass [Input: chemform] [Format `$molar chemform`] [Bounds 1. `Tri` & `acetyl`2. `Ac` for `acetyl` (Write as CH3CO, etc.)3. Spaces are not allowed4. Make sure each letter is capitalzed correctly]

`periodic` - Get an image of the periodic table [Input verOPTIONAL] [Format: `$periodic verOPTIONAL`]
<br>
**Other Sciences**

<u>Commands List</u>

`earth` - Get a picture of the earth using user inputted longitude & latitude [Input lat long timeYYYY-MM-DD] [Format `$earth lat long dateYYYY-MM-DD`]
<br>
**Others**

<u>Commands List</u>

`fact` - Gives you a random fact [Input NONE]
___

## WORK IN PROGRESS