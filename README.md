## Editor frosbite
old project, recoded now and uses dynamic states

## What does it do
- Edit whole VisualEnvironment + force its states
- Edit WorldRenderSettings
- Capture entities on runtime (you better load all resources for this to work correctly)
- Tries to capture original values (not perfect after all)
- Config per map

#### limits
- for sure editing lights is hard because some are only textures marked into static entity (judging by dump of worldent creation)
- textures, this engine only allows to null them or replace those which were loaded in current bundle. 
While replacing is possible, it really is only experimental rather than any useful. I myself don't know how can you load it dynamically in frosbite.

#### lights
For lights I couldnt find really easier way to detect their references
You can edit every pointed light on run

#### states
each state has a "name", but to ref the name there are some ways, I basically do:
if we can get it from refdata in entity itself -> get its blueprint from there
if not -> grab it from data and loop resource mngr comparing addr of pointer.
Could have just do all in partition loader, tho it's impossible to run this tool mid-round then

Editing state goes in several possible ways:
1. edit visual environment globally, the active one in manager (bad artifacts, or something doesnt apply as should)
2. edit each state - best, because the game adds/remove some states at runtime. Some of them are static. Which all makes sense
3. capture lerps, since this is quite okay solution because templates produce different function signatures, it's possible to capture all and edit accordingly.
Tho this was last way I wanted to do it.
4. Edit resource on load, also not bad, but not dynamic, would have to dump all possible main instances to then realisticly change them later on.
 
In conclusion getting states from resource manager is the best + editing it globally

#### Code
For now, it's extremelly messy. SDK should have some classes marked with "Captured" name, since they don't leave typeinfo.
Also editor class was not good idea, I have to recode this into more internal parts, which are never ever used anywhere else but in editor.cpp.
Probably support for fb3 going to be added in some time.

# Demo
![preview](https://github-production-user-asset-6210df.s3.amazonaws.com/69613796/542472076-7a23e338-33f5-4039-90bc-a2488acd0fc8.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAVCODYLSA53PQK4ZA%2F20260129%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20260129T225457Z&X-Amz-Expires=300&X-Amz-Signature=e2bfad01f2c344079968b7738d777c3418155dc8d20e3a0aba99685327d4dc1d&X-Amz-SignedHeaders=host)\
[![video](https://img.youtube.com/vi/LeEkiGc7Ggw/maxresdefault.jpg)](https://www.youtube.com/watch?v=LeEkiGc7Ggw)