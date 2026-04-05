# Antoria Map — Release Notes & Server Setup Guide - PRE-RELEASE DEV MODS FOR TESTING AND SERVER SETUP ONLY!!!! 
# Full release will require server owners to wipe - you may need to adjust existing editor builds - date TBC - likely the week of 27th October 2025. 

> This document consolidates changes in the latest Antoria build, inventories the mission configuration, and provides a clean setup flow for server owners. Optional modules (Panacea, Endgame Bunker) are supported but not required.

---

## 1) Changelog (Map & Mission)

**Map / World**
- Many roads **smoothed** for drivability/performance.
- **UK Model Pack** **removed** (crash source).
- **Mall** **removed** (crash source).
- **Crumpet Island** fully added (terrain, signage, landmarks).
- **Second Endgame** added (see Bunker Mod section).
- **Two new bunkers** added.
- **New Skybox** added.
- All Crumpet's signage, landmarks, and endgame builds are done **in `init.c`** only and are fully configurable.
  Server owners can replace or remove the build with their own config if desired.
- Too many other fixes to list here!

**Addons (Antoria Content Pack)**
- New Antoria‑specific **clothing**, **flags**, **weapons**, **animals**, **infected**, and **adult items**.
- **Admin Gear** pack for server admins.
- **Required** for full map functionality.

**Economy & Spawns**
- **Zombie Spawn Tool** configs for bunkers included **in the mission** (adjust to taste).
- **Panacea** removed from the base package; provided as an **optional bolt‑on** (SNAFU‑compatible).
  Use the **Antoria Weapons** config to disable if desired.
- **Loot rebalanced**: top‑tier weapons are restricted to **locked containers** and the **second endgame**.
- **Files split** by feature for easier maintenance (types/events in their own files).
  External mod configs are **optional**—install the matching Workshop mods to activate.

**Version Targeting**
- Mission files are prepared for **DayZ 1.29**.
  After 1.29 lands, apply a **single `cfgIgnore` update** (see Setup §7).

---

## 2) Required & Optional Mods

### Required
- **Antoria Main Map** 
  Steam: `https://steamcommunity.com/sharedfiles/filedetails/?id=3441239858`
  Vlad Mod Key: `https://discord.com/channels/929683428847271986/1302904280209948713`
  
- **Antoria Addons Content Pack** (Antoria clothing, flags, animals, infected, weapons, Admin Gear).  
  _This is required for the map to be fully functional._
  Steam: `https://steamcommunity.com/sharedfiles/filedetails/?id=3491502744`
  
### Optional / Recommended
- **AntoriaMap - In‑Game Map** (markers/POIs):  
  Steam: `https://steamcommunity.com/sharedfiles/filedetails/?id=3587344271`
- **AntoriaPNC - Panacea (SNAFU‑compatible “big boy” weapons)**:  
  Steam: `https://steamcommunity.com/sharedfiles/filedetails/?id=3587890336`
- **DGBunker - Endgame Bunker Mod** (Second Endgame):  
  Steam: `https://steamcommunity.com/sharedfiles/filedetails/?id=3571048133`  
  _Kept external for rapid updates. Required only if you want the second endgame active._

> **Load order hint:** Put core map + Antoria Content Pack before mission‑level customizations; optional bolt‑ons after the map/content and before mission init logic runs.

---

## 3) Mission Inventory (Auto‑detected)

**Mission root:** `/mnt/data/empty.Antoria`

**Key script:** `empty.Antoria/init.c`

**Files included via `#include` in `init.c`:**
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\BunkerHelperClasses.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\BunkerCreator.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\BaseLayout.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\BunkerAIZone.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\Preset_Atrium.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\Layout_CustomGrid20x20.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\Layout_CustomGrid20x19.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\Layout_CustomGrid11x12.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\Layout_CustomGrid22x27.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\Layout_Plus12Rooms_25x25.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\Layout_CustomGridCustom1.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\Layout_CustomGrid41x41.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\Layout_SmallBunker_1.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\Layout_SmallBunker_2.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\Layout_MediumBunker_1.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\Layout_MediumBunker_2.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\Layout_LargeBunker_1.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\Layout_LargeBunker_2.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\Layout_2FloorMazeBunker.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\Layout_OpenAreaBunker.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\Layout_11x11_3xFullGearRooms.c
- $CurrentDir:\\mpmissions\\empty.Antoria\\bunkerlayouts\\Layout_ModularTest.c  

**Economy files (types/events/limits/spawns) found:** 27
- See the **“Economy files * folder added in the addons , all in **other server stuff**.

**external components (by filename heuristic):**
- mbm_gokarts (2 file(s))
- antoria_animals (1 file(s))
- antoria_events (2 file(s))
- antoria_infected_types (1 file(s))
- antoria_spawnabletypes (1 file(s))
- antoria_weapons (1 file(s))
- antoria_clothing (1 file(s))
- antoria_consumables (1 file(s))
- berzerk (1 file(s))
- bicycle (1 file(s))
- bunker_types (1 file(s))
- cfgeconomycore (1 file(s))
- horse (1 file(s))
- radzone (1 file(s))
- codelock (1 file(s))
- enhanced_flags_types (1 file(s))
- mod_events (1 file(s))

> Note: External configs are optional and require the respective Workshop mods installed server‑side. 
        Radzone is highly recommended for Antoria as this is built with that specifically in mind. 
        Json configs for the Radzone are included in addons.

---

## 4) Server Setup (Clean Install)

1. **Install required mods**
   - Antoria Map + Antoria Addons Content Pack (workshop or local server `@` folders).  
   - If using Second Endgame, also install the **Endgame Bunker Mod**.
   - Optional: **Panacea** weapons, **Antoria In‑Game Map**.

2. **Copy the mission**
   - Place the provided mission folder (e.g., `empty.Antoria`) into `mpmissions/`.
   - Keep the folder name consistent with your server start parameters.

3. **Set launch parameters**
   - Example:  
     ```
     -mission=.\mpmissions\empty.Antoria -mod=@Antoria;@AntoriaContent;@EndgameBunker;@Panacea
     ```
   - Only include optional `@` mods you actually use.

4. **Economy activation (optional modules)**
   - External mod economy files are already split out in the mission (e.g., `db/types/types_panacea.xml`, `db/events/events_bunker.xml`).  
   - If you install an optional mod, **leave the corresponding mission config file(s) in place**.  
   - If you are **not** using a mod, you can **disable** its mission file by either:
     - Removing the file from the mission `db/` folder
     - Commenting/removing its include reference in cfgeconomycore.xml).

5. **Loot & containers**
   - High‑tier loot is restricted to **locked containers** and **Second Endgame**.  
   - Ensure your chosen locking solution is configured (codelocks/lockers) to preserve the intended progression.

6. **Zombie Spawn Tool configs**
   - Bunker spawns are defined in mission JSON/XML (labels like `Sector12_*`, etc.).  
   - Tune counts, radii, and triggers per your server difficulty targets.

7. **DayZ 1.29 readiness**
   - After upgrading to **1.29**, apply the required **`cfgIgnore`** tweak (file rename path per BI 1.29 notes).  
   - This mission is otherwise 1.29‑ready.

8. **Validation pass**
   - Start the server once and inspect `server.RPT` for **“No entry”** or **“Unknown class”** errors.  
   - If you see missing classes, install the matching mod or remove its mission files (types/events/spawns).  
   - Run an **XML structure validation** on `types/*.xml`, `events/*.xml`, and `cfgspawnabletypes.xml` (critical).

9. **Backups & versioning**
   - Keep a copy of your vanilla mission + a per‑mod economy overlay.  
   - Track changes via Git (or zip-and-tag) so you can roll back a bad update quickly.

---

## 5) Endgame Bunker (Second Endgame) — Configuration

- The bunker gameplay is delivered by the **external Bunker Mod** (Workshop ID `3571048133`).  
- With the mod installed, the **Second Endgame** will activate. Without it, the rest of the map runs fine.
- Configuration levers:
  - **Access & Progression**: entry keys, raid timers, loot tiers.
  - **AI / Infected**: spawn densities per zone/phase (see mission’s bunker spawn configs).
  - **Rewards**: end‑chests/containers and lock state (sync with server’s locking mod).
- See bunker config guide below.

---

## 6) Customization Notes (Crumpet map extras)

- Crumpet's build (signage/landmarks/endgame set dressing) lives in **`init.c`**.  
- Server owners can remove or swap this build for a custom one without touching core map files.
- Search `init.c` to locate the created objects, disable at will or use your preferred placement/mod tools for edits.

---

## 7) Troubleshooting & Quality Gates

- **RPT hygiene**: zero “unknown class” or config merge errors after startup.  
- **Economy sanity**: no top‑tier weapon leaks into open world (should be **locked containers** or **Second Endgame** only).  
- **Mod mismatch**: if you see missing classes, either install the mod or remove its mission files.
- **Crash sources removed**: UK Model Pack & Mall assets are deprecated; keep them out of your load order.
- **Schema/structure checks**: validate XMLs, particularly `cfgspawnabletypes.xml` & any new `types_*.xml` you add.

---

## 8) Appendix

**Mission tree (top‑level view):**
```
📁 empty.Antoria
  📁 antoria_ce
    📄 antoria_animal_types.xml
    📄 antoria_clothing_types.xml
    📄 antoria_consumables_types.xml
    📄 antoria_event_types.xml
    📄 antoria_events.xml
    📄 antoria_infected_types.xml
    📄 antoria_spawnabletypes.xml
    📄 antoria_weapon_types.xml
    📄 bunker_types.xml
  📁 bunkerlayouts
    📄 BaseLayout.c
    📄 BunkerAIZone.c
    📄 BunkerCreator.c
    📄 BunkerHelperClasses.c
    📄 Layout_11x11_3xFullGearRooms.c
    📄 Layout_2FloorMazeBunker.c
    📄 Layout_CustomGrid11x12.c
    📄 Layout_CustomGrid17x16.c
    📄 Layout_CustomGrid20x19.c
    📄 Layout_CustomGrid20x20.c
    📄 Layout_CustomGrid22x27.c
    📄 Layout_CustomGrid41x41.c
    📄 Layout_CustomGridCustom1.c
    📄 Layout_Horseshoe_10Rooms.c
    📄 Layout_LargeBunker_1.c
    📄 Layout_LargeBunker_2.c
    📄 Layout_MediumBunker_1.c
    📄 Layout_MediumBunker_2.c
    📄 Layout_ModularTest.c
    📄 Layout_OpenAreaBunker.c
    📄 Layout_Plus12Rooms_25x25.c
    📄 Layout_SmallBunker_1.c
    📄 Layout_SmallBunker_2.c
    📄 Layout_SmallBunker_2_5x5_Rooms.c
    📄 Layout_SmallBunker_4_3x3_Rooms.c
    📄 Layout_ZigZagBolt_3x3Rooms.c
    📄 Preset_Atrium.c
  📁 db
    📄 economy.xml
    📄 events.xml
    📄 globals.xml
    📄 messages.xml
    📄 types.xml
  📁 env
    📄 bear_territories.xml
    📄 cattle_territories.xml
    📄 fox_territories.xml
    📄 hare_territories.xml
    📄 hen_territories.xml
    📄 horse_territories.xml
    📄 pig_territories.xml
    📄 polarbear_territories.xml
    📄 polarwolf_territories.xml
    📄 red_deer_territories.xml
    📄 reindeer_territories.xml
    📄 roe_deer_territories.xml
    📄 sheep_goat_territories.xml
    📄 wild_boar_territories.xml
    📄 wolf_territories.xml
    📄 zombie_territories.xml
  📁 mods_ce
    📄 berzerk_types.xml
    📄 bicycle_types.xml
    📄 codelock.xml
    📄 enhanced_flags_types.xml
    📄 horse_types.xml
    📄 mbm_gokart_spawnabletypes.xml
    📄 mbm_gokart_types.xml
    📄 mod_events.xml
    📄 radzone_types.xml
  📁 pra
    📄 warheadstorage.json
  📄 areaflags.map
  📄 cfgeconomycore.xml
  📄 cfgeffectarea.json
  📄 cfgenvironment.xml
  📄 cfgeventgroups.xml
  📄 cfgeventspawns.xml
  📄 cfggameplay.json
  📄 cfgignorelist.xml
  📄 cfgignorepos.xml
  📄 cfglimitsdefinition.xml
  📄 cfglimitsdefinitionuser.xml
  📄 cfgplayerspawnpoints.xml
  📄 cfgrandompresets.xml
  📄 cfgspawnabletypes.xml
  📄 cfgundergroundtriggers.json
  📄 cfgweather.xml
  📄 init.c
  📄 mapclusterproto.xml
  📄 mapgroupcluster.xml
  📄 mapgroupcluster01.xml
  📄 mapgroupcluster02.xml
  📄 mapgroupdirt.xml
  📄 mapgrouppos.xml
  📄 mapgroupproto.xml
```

## 9) Zombie Spawn Tool — Setup & Tuning (Antoria)

> **Important:** Copy the supplied **Zombie Spawn Tool (ZST)** config into the **ServerProfile** ZST folder.  
> **That folder is created automatically on the first boot of the map.**

### Where to put the config
1. **Start** your server once with the Antoria mission so the ZST folder is created inside your **ServerProfile** (the path you pass with `-profiles=...`).  
2. **Stop** the server.  
3. **Copy** the supplied ZST JSON (contains `"ZombieSpawnLocationData"`, labels like `"Sector12_*"`) into the newly created ZST folder.  
4. **Start** the server and check your **server.RPT** to confirm ZST loaded the profile.

### What you can tune (common fields)
```json
{
  "ModVersion": "1.2",
  "ConfigVersion": 3,
  "ZombieSpawnLocationData": [
    {
      "Label": "Sector12_1",
      "MinSpawnAmount": 3,
      "MaxSpawnAmount": 6,
      "SpawnMaxRadius": 1.0,
      "SpawnToSurface": 0,
      "SpawnDelaySeconds": 300,
      "FirstSpawnDelaySeconds": 300,
      "PlayerSafeDistance": 500.0,
      "PlayerTriggerArea": [
        { "TriggerLocation": [12247, 0, 5638], "TriggerRadius": 250 }
      ],
      "Archetypes": ["ZmbM_Soldier", "ZmbM_HeavySoldier"]
    }
  ]
}
```
- **Label** – group name for logs/troubleshooting.  
- **Min/MaxSpawnAmount** – infected per burst.  
- **SpawnMaxRadius** – scatter radius from the center.  
- **SpawnToSurface** – `1` forces to ground; `0` keeps given Y (useful underground).  
- **First/SpawnDelaySeconds** – initial and repeat delays.  
- **PlayerSafeDistance** – don’t spawn if players are too close.  
- **PlayerTriggerArea** – trigger volumes that gate spawning.  
- **Archetypes** – which infected to use (vanilla or modded).

**Antoria bunker presets (suggested):**
- **2‑floor maze** (default): 4–8 infected/burst, 300–450s delay, safe distance ≥ 400–500m, military/hard archetypes.  
- **Gas rooms**: reduce counts or disable in‑gas spawns to cut strain.  
- **Surface staging**: lighter civilian archetypes, smaller safe distances.

**RPT checks:** You should see ZST logging how many groups/archetypes loaded. `Unknown class` / `No entry` = bad classname → fix or disable that group.

**Disable option:** remove/rename the bunker groups in your ZST JSON, or set Min/Max to 0 (if supported), or move the trigger far away.

---

## 10) Bunker Configuration (Second Endgame) — Experimental

_Status: **Experimental**. Prefer **smaller** layouts until all variants are fully tested. Larger layouts increase server CPU load._

The Second Endgame bunker uses the external **Bunker Mod** (Workshop ID `3571048133`).

### What can be configured
- **Layout variant/size** – Small/Medium/Large and themed layouts (Maze/Labs/Hub, etc.).  
  - **Recommendation:** start Small.  
- **Floors** – number of floors (Antoria default: **2**).  
- **Randomization** – reconfigure to a **different layout after every restart**; or **pin** a layout via a fixed seed.  
- **Gas** – enable/disable via **standard DayZ contaminated areas** in the mission (static or scripted).  
- **Zombies** – on/off (in ).  
- **Loot** – via **loot JSON** or your split economy (types/events/cfgspawnabletypes) dedicated to end‑reward containers.  
- **Entrances/Services** – **Landing pad** and **Recharge desk** are currently spawned in **`init.c`**; move them to any coordinates you prefer.

### Current Antoria defaults
- **Layout:** **2‑floor Maze** only.  
- **Gas:** **Enabled** using DayZ core contaminated zones configured in the **mission** but can be configured for bunker only via bunker config files.  
- **Zombies:** Optional (configurable in bunker files).  
- **Landing pad & Recharge desk:** Placed in **`bunker config`**; reposition freely.

### Config in init.c: 
	//BUNKER ***
	if (GetGame().IsServer() || !GetGame().IsMultiplayer()) {
                // CLEAN-UP LAST BUNKER SETUP
                CleanupSciFiWalls();

                // SPAWN LANDINGPAD
                SpawnDIBunkerObject("fmdpk_landingpad_1", "19726.5 71.16 933.31", "-125 0 0", 1);

                // GENERATE RANDOM BUNKER LAYOUT
                // GenerateMap("6000 3500 6000");
                GenerateMap("19547.75 25.25 782.76");

                // SPAWN RECHARGE NODES
                SpawnDIBunkerObject("fmdpk_landingpad_cardterminal", "19571.7 80.78 651.671", "115.0 0 0", 1);

                // SPAWN GAS ZONE
                // vector gasOrigin = "6000 3500 6000";
                // SpawnDIBunkerObject("ContaminatedArea_Static", gasOrigin, "0 0 0", 1.0);

                // ZOMBIE SPAWNING, TRIGGER ZONE AT THE BUNKER ENTRANCE
                GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(FP_StartBunkerZone, 1000, false);

### Gas / Contaminated Area (mission example)
```json
{
  "AreaName": "Bunker_InnerGas",
  "Type": "ContaminatedArea_Static",
  "TriggerType": "ContaminatedTrigger",
  "Data": {
    "Pos": [x, 0, z],
    "Radius": 200,
    "PosHeight": 30,
    "NegHeight": 50,
    "InnerRingCount": 2,
    "InnerPartDist": 50,
    "OuterRingToggle": 1,
    "OuterPartDist": 40,
    "OuterOffset": 0,
    "VerticalLayers": 0,
    "VerticalOffset": 0,
    "ParticleName": "graphics/particles/contaminated_area_gas_bigass"
  }
}
```
- **Pos/Radius** – center and coverage; use smaller radii for room‑scale gas.  
- **Heights/Layers** – vertical coverage (multi‑floor control).  
- **Rings** – visual density.  
- **Disable** by removing the entry or shrinking radius far from play area.

### Loot JSON (template)
```json
{
  "Version": 1,
  "LootTables": [
    {
      "ContainerClass": "Bunker_EndChest",
      "MaxItems": 8,
      "Tier": "Endgame",
      "Entries": [
        { "Classname": "NVGoggles", "Weight": 8, "MinQty": 1, "MaxQty": 1 },
        { "Classname": "Codelock", "Weight": 12, "MinQty": 1, "MaxQty": 1 },
        { "Classname": "Ammo_762x54", "Weight": 16, "MinQty": 1, "MaxQty": 4 },
        { "Classname": "Medical_Epinephrine", "Weight": 10, "MinQty": 1, "MaxQty": 2 }
      ]
    }
  ]
}
```
**Usage:** place the JSON in the server profile folder  `DI_BunkerGen`.

### Moving the Landing Pad & Recharge Desk
1. Open **`init.c`** and search for the create calls (e.g., `CreateObject("Some_Pad_Class", Vector(x,y,z), ...)`).  
2. Replace coordinates with your target world position.  
3. Restart and verify placement (editor markers/admin tools help with exact coords).

### Adding removing potential bunker layouts
Mission files have a folder called bunker layous. Open the file `BunkerCreator.c`.
layouts are at the top and can be either commented or uncommented to active each layout as a potential. if you have more than one, the bunker mod picks a layout at random each restart.

Example: 
array<ref BaseLayout> layouts = new array<ref BaseLayout>;
    // layouts.Insert( new Layout_CustomGrid11x12 );
    //layouts.Insert( new Layout_CustomGrid20x19 );
    //layouts.Insert( new Layout_CustomGrid20x20 );
    //layouts.Insert( new Layout_CustomGrid22x27 );
    //layouts.Insert( new Layout_Plus12Rooms_25x25 );
    // layouts.Insert( new Layout_CustomGridCustom1 );
	// layouts.Insert( new Layout_CustomGrid41x41 );
	// layouts.Insert( new Layout_SmallBunker_1 );
	// layouts.Insert( new Layout_SmallBunker_2 );
	// layouts.Insert( new Layout_MediumBunker_1 );
    //layouts.Insert( new Layout_MediumBunker_2 );
    // layouts.Insert( new Layout_LargeBunker_1 );
    // layouts.Insert( new Layout_LargeBunker_2 );
    layouts.Insert( new Layout_2FloorMazeBunker ); --------------- this is the only one on currently - edit at will!
    //layouts.Insert( new Layout_OpenAreaBunker );
    //layouts.Insert( new Layout_11x11_3xFullGearRooms );
    // layouts.Insert( new Layout_ModularTest );



### Recommendations & warnings
- Stick to **Small** while you test; scale up cautiously.  
- Gas + dense AI spawns can overwhelm both **players** and **server CPU** — adjust counts/delays.  
- Pin a layout for events (disable randomization) to reduce confusion.  
- Version‑control your bunker JSON & economy changes so you can roll back quickly.


### COLDZONES

ColdZones – Config Guide

File: ColdZonesConfig.json
Location: profiles/ColdZones/ColdZonesConfig.json

The file is created automatically the first time the server starts with the ColdZones mod loaded.
The profiles folder is whatever you passed with -profiles=... on your DayZ server.

Example path on a typical setup:

C:\DayZServer\ServerProfile\ColdZones\ColdZonesConfig.json

or on hosted panels: ServerProfile/ColdZones/ColdZonesConfig.json

What this mod does

ColdZones adds very cold areas on the map where players:

Lose body heat much faster.

Get hit even harder if they are wet / in water.

Can still warm up using fires and good gear – the zone does not hard-lock players into permanent freezing.

The positions and sizes of the cold zones are defined in the mod’s script and are not configurable via JSON (for now).
This config file lets you change how harsh those zones feel.

Default config

A fresh ColdZonesConfig.json will look like this:

{
  "ConfigVersion": "1",
  "TemperatureCoef": 2.5,
  "WetPenaltyCoef": 4.2,
  "DryScale": 6.0,
  "WetScale": 6.0
}

Important notes

Always stop the server before editing this file.

Use a plain text editor (e.g. Notepad++, VS Code).

Keep the JSON structure valid:

Double quotes around keys and string values.

No trailing commas.

After editing, restart the server (not just #restart in admin tools).

Setting explanations
ConfigVersion (string)

Used internally by the mod for future updates.

Do not change this manually.

When the version changes in an update, the mod will:

Backup your current config to ColdZonesConfig.json_old

Then write a new default config.

If your file gets broken, you can restore from the _old backup.

TemperatureCoef (float)

“Base cold strength in cold zones.”

This controls how hard the base cold penalty is when you’re inside a cold zone (while on land).

Higher value = stronger cold.

Lower value = softer cold.

Internally it’s multiplied by DryScale:

Base dry penalty ≈ TemperatureCoef * DryScale


Suggested ranges:

1.0 – 2.0 – Mild, more forgiving, survival-friendly.

2.5 (default) – Strong but fair. The version tuned on Antoria (~3 mins to cold in zone).

3.0 – 4.0 – Very harsh. Players will get cold very quickly.

WetPenaltyCoef (float)

“Extra cold when wet or in water.”

This controls additional cold applied when the player is in water / wet inside a cold zone.

Internally it’s multiplied by WetScale:

Extra wet penalty ≈ WetPenaltyCoef * WetScale


Suggested ranges:

2.0 – 3.0 – Noticeably worse when wet, but not insane.

4.2 (default) – Very punishing if you go swimming in a cold zone.

5.0+ – Brutal. Being wet in a zone will drop temp extremely fast.

DryScale (float)

“Multiplier for on-land cold penalty.”

This is basically a global tuning knob for how harsh the cold zones feel on dry land.

Increase DryScale to make all cold zones more lethal.

Decrease it for a softer experience.

Examples:

DryScale = 4.0

Softer overall; more casual-friendly.

DryScale = 6.0 (default)

Strong cold, tuned around ~3 mins to feel properly cold in zone vs ~5 mins outside (with typical gear).

DryScale = 9.0–10.0

Roughly ~1.5x–2x stronger drain than default. Use if you want “this area is death without proper prep”.

WetScale (float)

“Multiplier for wet / water cold penalty.”

Same idea as DryScale, but only applied when the player is wet / in water in a cold zone.

Increase WetScale if you want water in these zones to be absolutely deadly.

Keep it equal or slightly higher than DryScale for consistent behaviour.

Examples:

WetScale = 4.0

Slightly less punishing in water.

WetScale = 6.0 (default)

Matches dry scaling; water is significantly worse but not instantly lethal.

WetScale = 8.0+

Water in a cold zone becomes a “you seriously screwed up” event.

Example presets
1. Default (balanced harsh)
{
  "ConfigVersion": "1",
  "TemperatureCoef": 2.5,
  "WetPenaltyCoef": 4.2,
  "DryScale": 6.0,
  "WetScale": 6.0
}


Strong cold zones.

Still survivable with good gear, fires, and planning.

2. Softer / casual-friendly
{
  "ConfigVersion": "1",
  "TemperatureCoef": 2.0,
  "WetPenaltyCoef": 3.5,
  "DryScale": 4.0,
  "WetScale": 4.0
}


Players still feel the cold zones, but it’s less punishing.

Good for PvE-focused or higher-loot servers.

3. Hardcore survival
{
  "ConfigVersion": "1",
  "TemperatureCoef": 3.0,
  "WetPenaltyCoef": 5.0,
  "DryScale": 9.0,
  "WetScale": 7.0
}


Cold zones are extremely dangerous.

Going in wet or unprepared is pretty much a death sentence unless you act quickly.

FAQ

Q: Do I need to restart the server after editing the config?
A: Yes. The config is read when the server starts. You must do a full restart after changes.

Q: Can I move or add cold zones in this file?
A: No, not in this version. The JSON only controls how strong the cold effect is, not where the zones are. Zone positions and radii are defined in the mod scripts.

Q: I broke the JSON and now the server errors on start. What do I do?
A:

Stop the server.

Delete ColdZonesConfig.json.

Start the server again – the mod will generate a fresh default file.

Re-apply your changes carefully.

If the version ever changes in an update, you may also see a backup file like:
ColdZonesConfig.json_old – you can copy settings from that.
