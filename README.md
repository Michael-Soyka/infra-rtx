<h1 align="center">INFRA RTX Remix Compatibility Mod</h1>

<div align="center" markdown="1"> 

This client modification is specifically made for nvidia's [rtx-remix](https://github.com/NVIDIAGameWorks/rtx-remix).  
How does a shader based game work with remix? By manually reimplementing fixed function rendering :) 

<div align="center" markdown="1">

<img src=".github/img/logo.png" alt="Description" width="60%">
</div>

__WIP__ & __Please Note:__  
RTX Remix was never intented to support INFRA.  
If you encounter crashes, broken things or similar, it's either due to incompatibility,  
the limits of fixed function rendering or due to bugs in the compatibility mod itself.  


### This is not trying to be a remaster:
It simply makes the game compatible with RTX Remix.  
Please keep that in mind.

</div>

<br>

<div align="center" markdown="1">

![img](.github/img/02.png)

</div>

<div align="center" markdown="1">

### __[ Remix Compatibility Features ]__   
🔹Most things are rendered using the fixed-function pipeline🔹  

🔹Remix friendly culling and the ability to manually override culling🔹  

🔹Ability to spawn and animate lights on events using a keyframe system🔹  

🔹Per map loading of remix config files to set remix variables🔹  

🔹Ability to animate remix variables on events🔹  

🔹Spawning of unique anchor meshes🔹  

🔹**DLSS 4.5 (Ray Reconstruction 2) support**🔹  

🔹**DLSS 5.0 (Neural Rendering) support**🔹  
(Integration made by [Sparkles](https://github.com/Kim2091/dxvk-remix/tree/gta4-atmos-dlss5)  

🔹... and much, much more ...🔹  

</div>


## Installation / Usage
- Download the latest [release](https://github.com/Michael-Soyka/infra-rtx/releases) and follow instructions found __there__.

<br>

#### ✳️ Info: 
- Take a look at the [Wiki](https://github.com/xoxor4d/p2-rtx/wiki/Compatibility-Mod-Feature-Guide) for in-depth guides on features that come with the compatibility mod 🍓
  
- Current releases ship with:
  - [custom build of the remix-dxvk runtime](https://github.com/xoxor4d/dxvk-remix/tree/game/p2) which includes necessary changes for Portal 2
 
- Some engine tweaks that are required to make the game compatible result in CPU bottlenecks on some maps (software skinning instead of HW skinning). This may or may not improve in future updates.

<br>

## ⚠️ Troubleshooting (click to expand):

<details><summary>Do I need to start the game via the batch file every time?</summary>
<br>

	Yes. Asiloaders / dxwrapper will not load if the game is run from steam.

<br></details>


<details><summary>How do I disable remix?</summary>
<br>

	Run `p2-rtx-disable.bat` and follow the instructions to disable or the mod.  
	The game will start afterwards to reset some required cvars.

<br></details>


<details><summary>Crashing, startup issues, not working...</summary>
<br>

- Make sure that you have no clipping software such as "medal" running in the background.  
- If your game is installed within `Program Files`:  
	  - right click `portal2.exe` and click settings - compatibility tab and enable run as admin  
	  - or install the game somewhere outside `Program Files`  
	
- Download and install [DirectX End-User Runtimes (June 2010)](https://www.microsoft.com/en-ie/download/details.aspx?id=8109)

<br></details>


<details><summary>No sound</summary>
<br>

- Either copy `_master.cache` from `root/portal2/maps/soundcache` to `root/portal2_dlc3/maps/soundcache`  
  or paste this into the in-game console and execute: `snd_rebuildaudiocache;snd_updateaudiocache;exit`

<br></details>

Other Issue?
> Look at [Closed Issues](https://github.com/Michael-Soyka/infra-rtx/issues?q=is%3Aissue+is%3Aclosed) or [Discussions](https://github.com/Michael-Soyka/infra-rtx/discussions) to see if people had similar issues

<br>

##  Credits
- [Nvidia - RTX Remix](https://github.com/NVIDIAGameWorks/rtx-remix)
- [People of the showcase discord](https://discord.gg/j6sh7JD3v9) - especially the nvidia engineers ✌️
- All early access people for testing/bug reporting and for covering my electricity bill ⚡
- [Wolƒe Strider Shoσter](https://github.com/wolfestridershooter) - for all the high quality bug reports! 
- [dear-imgui](https://github.com/ocornut/imgui)
- [imgui-blur-effect](https://github.com/3r4y/imgui-blur-effect)
- [minhook](https://github.com/TsudaKageyu/minhook)
- [toml11](https://github.com/ToruNiina/toml11)
- [dxwrapper](https://github.com/elishacloud/dxwrapper)
- [Sparkles (DLSS5 Integration)](https://github.com/Kim2091)

<br>

<div align="center" markdown="1">

![img](.github/img/01.jpg)
![img](.github/img/03.jpg)
![img](.github/img/04.jpg)

</div>
